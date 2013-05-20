.import QtQuick.LocalStorage 2.0 as SQL

var db = SQL.LocalStorage.openDatabaseSync("ShowDevant", "1.0", "shows");

function init() {
	db.transaction(function(tx) {
		tx.executeSql('PRAGMA foreign_keys = ON;');

		// shows
		tx.executeSql('CREATE TABLE IF NOT EXISTS show (show_id text primary key, title text, episodes_last_check_date integer);');

		// seasons
		tx.executeSql('CREATE TABLE IF NOT EXISTS season (show_id text, number integer, PRIMARY KEY (show_id, number));');

		// episodes
		tx.executeSql('CREATE TABLE IF NOT EXISTS episode ' +
					  '(show_id text, season integer, episode integer, title text, ' +
					  'number text, global integer, date integer, ' +
					  'comments integer, subtitles_last_check_date integer, PRIMARY KEY (show_id, season, episode));');

		// subtitles
		tx.executeSql('CREATE TABLE IF NOT EXISTS subtitle ' +
					  '(id integer primary key, show_id text, season integer, episode integer, ' +
					  'language text, source text, file text, ' +
					  'url text, quality integer, ' +
					  'UNIQUE (show_id, season, episode, url) ON CONFLICT REPLACE);');

		// subtitles content
		tx.executeSql('CREATE TABLE IF NOT EXISTS subtitle_content ' +
					  '(subtitle_id integer, file text, ' +
					  'FOREIGN KEY (subtitle_id) REFERENCES subtitle(id) ON DELETE CASCADE, ' +
					  'UNIQUE (subtitle_id, file) ON CONFLICT REPLACE);');
	});
}
