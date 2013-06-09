.pragma library

var apiKey = "9adb4ab628c6";
var websiteUrl = "http://api.betaseries.com";
var tokenKey;

function __createReadyStateChangeCallback(doc, callback) {
	return function() {
		if (doc.readyState === XMLHttpRequest.DONE) {
			callback();
		}
	}
}

function showsEpisodes(config, callback)
{
	var ar = [];
	var str;
	var doc;

	config = config || {};

	ar.push("%1/shows/episodes/%2.json?key=%3".arg(websiteUrl).arg(config.showId).arg(apiKey));

	if (config.season >= 0)
		ar.push("&season=%1".arg(config.season));
	if (config.episode >= 0)
		ar.push("&episode=%1".arg(config.episode));
	if (config.summary !== false)
		ar.push("&summary=1");
	if (config.hide_notes !== false)
		ar.push("&hide_notes=1");

	str = ar.join("");

	doc = new XMLHttpRequest();
	doc.onreadystatechange = __createReadyStateChangeCallback(doc, function() {
		var obj = JSON.parse(doc.responseText);
		if (obj && obj.root)
			callback(false, obj.root);
		else
			callback(true);
	});

	doc.open("GET", str, true);
	doc.send();
}

function showsSearch(config, callback)
{
	var str;
	var doc;

	config = config || {};

	str = "%1/shows/search.json?title=%2&key=%3".arg(websiteUrl).arg(config.expression).arg(apiKey);

	doc = new XMLHttpRequest();
	doc.onreadystatechange = __createReadyStateChangeCallback(doc, function() {
		var obj = JSON.parse(doc.responseText);
		if (obj && obj.root)
			callback(false, obj.root);
		else
			callback(true);
	});

	doc.open("GET", str, true);
	doc.send();
}

function recordAuthToken(token)
{
	tokenKey = token;
}

function membersAuth(login, passwordHash, callback)
{
	var str;
	var doc;

	str = "%1/members/auth.json?login=%2&password=%3&key=%4".arg(websiteUrl).arg(login).arg(passwordHash).arg(apiKey);

	doc = new XMLHttpRequest();
	doc.onreadystatechange = __createReadyStateChangeCallback(doc, function() {
		console.log(doc.responseText);
		var obj = JSON.parse(doc.responseText);
		if (obj && obj.root)
			callback(false, obj.root);
		else
			callback(true);
	});

	doc.open("GET", str, true);
	doc.send();
}
