.pragma library

Qt.include("commands.js");

//var queryModel = Qt.createQmlObject("import com.guid75 1.0; SqlQueryModel { }", null);

function __parseShowEpisodes(showId, queryModel, error, root) {
	var seasonNum;
	var episodeNum;
	var season;
	var episode;
	var str;
	if (error || root.code !== 1) {
		console.log("Parsing error");
		return;
	}
	for (seasonNum in root.seasons) {
		if (root.seasons.hasOwnProperty(seasonNum)) {
			season = root.seasons[seasonNum];

			// store the season into db
			queryModel.query = "INSERT INTO season (show_id, number) VALUES ('%1', %2)".arg(showId).arg(season.number);
			//				tx.executeSql("INSERT INTO season (show_id, number) VALUES (?, ?)", [showId, season.number]);

			for (episodeNum in season.episodes) {
				if (season.episodes.hasOwnProperty(episodeNum)) {
					episode = season.episodes[episodeNum];
					// store the episode into db
					str = "INSERT INTO episode (show_id, season, episode, title, number, global, date, comments) " +
							"VALUES ('%1', %2, %3, '%4', '%5', %6, %7, %8)".arg(showId).
					arg(season.number).
					arg(episode.episode).
					arg(episode.title).
					arg(episode.number).
					arg(episode.global).arg(episode.date).arg(episode.comments);
					console.log(str);
//					queryModel.query = str;

/*					tx.executeSql("INSERT INTO episode (show_id, season, episode, title, number, global, date, comments) " +
								  "VALUES (?, ?, ?, ?, ?, ?, ?, ?)", [showId, season.number, episode.episode, episode.title, episode.number, episode.global, episode.date, episode.comments]);*/
				}
			}
		}
	}

	// update the episodes last check date of the show
/*	var now = new Date();
	tx.executeSql("UPDATE show SET episodes_last_check_date=%1 WHERE show_id='%2'".arg(now.getTime() / 1000).arg(showId));*/
}

function __fetchShow(showId) {
	showsEpisodes({ showId: showId }, function(error, root) {
		__parseShowEpisodes(showId, error, root);
	});

	//showsEpisodes({ showId: showId }, __parseShowEpisodes.bind(Qt, tx));
}

function fetchAllEpisodes(showId, queryModel, callback) {
	showsEpisodes({ showId: showId }, function(error, root) {
		__parseShowEpisodes(showId, queryModel, error, root);
		if (callback) {
			callback(!error);
		}
	});
}

// Load a show seasons and episodes
function load(showId, callback) {
	var lastCheckEpoch = 0;
	var expiration = 24 * 64 * 64 * 1000; // TODO make this value customizable
	var now;

	db.readTransaction(function(tx) {
		var expired = false;
		var rs = tx.executeSql('SELECT episodes_last_check_date FROM show WHERE show_id="%1"'.arg(showId));

		if (rs.rows.length > 0) {
			lastCheckEpoch = rs.rows.item(0).episodes_last_check_date * 1000;
		}

		now = new Date();
		if (now.getTime() - lastCheckEpoch > expiration) {
			expired = true;
			// expired data, we need to launch the request
			__fetchShow(showId);
		}
		callback(showId, expired);
	});
}

function addShow(showId, title, callback) {
	db.transaction(function(tx) {
		var ts = tx.executeSql("INSERT INTO show (show_id, title) VALUES (?, ?)", [showId, title]);
		var success = ts.rowsAffected > 0;
		if (success) {
			// we've got to pass a select to get the last inserted row
			// maybe there is a better way?
			ts = tx.executeSql("SELECT * from show WHERE rowid=?", [ts.insertId]);
			success = ts.rows.length === 1;
			if (success) {
				callback(success, ts.rows.item(0));
				return;
			}
		}
		callback(false);
	});
}
