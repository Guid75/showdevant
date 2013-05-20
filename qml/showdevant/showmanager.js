.pragma library
.import QtQuick.LocalStorage 2.0 as SQL

Qt.include("commands.js");

var db = SQL.LocalStorage.openDatabaseSync("ShowDevant", "1.0", "shows");

function __parseShowEpisodes(showId, error, root) {
	var seasonNum;
	var episodeNum;
	var season;
	var episode;
	if (error || root.code !== 1) {
		console.log("Parsing error");
		return;
	}
	console.log("pouet");
	db.transaction(function(tx) {
		for (seasonNum in root.seasons) {
			if (root.seasons.hasOwnProperty(seasonNum)) {
				season = root.seasons[seasonNum];

				// store the season into db
				tx.executeSql("INSERT INTO season (show_id, number) VALUES (?, ?)", [showId, season.number]);

				for (episodeNum in season.episodes) {
					if (season.episodes.hasOwnProperty(episodeNum)) {
						episode = season.episodes[episodeNum];
						// store the episode into db
						tx.executeSql("INSERT INTO episode (show_id, season, episode, title, number, global, date, comments) " +
									  "VALUES (?, ?, ?, ?, ?, ?, ?, ?)", [showId, season.number, episode.episode, episode.title, episode.number, episode.global, episode.date, episode.comments]);
					}
				}
			}
		}

		// update the episodes last check date of the show
		var now = new Date();
		tx.executeSql("UPDATE show SET episodes_last_check_date=%1 WHERE show_id='%2'".arg(now.getTime() / 1000).arg(showId));
	});
}

function __fetchShow(showId) {
	showsEpisodes({ showId: showId }, function(error, root) {
		__parseShowEpisodes(showId, error, root);
	});

	//showsEpisodes({ showId: showId }, __parseShowEpisodes.bind(Qt, tx));
}

// Load a show seasons and episodes
function load(showId, callback) {
	var lastCheckEpoch = 0;
	var expiration = 24 * 64 * 64 * 1000; // TODO make this value customizable
	var doc;
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

function addShow(showId, title) {
	db.transaction(function(tx) {
		tx.executeSql("INSERT INTO show (show_id, title) VALUES (?, ?)", [showId, title]);
	});
}
