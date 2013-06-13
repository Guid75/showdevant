//  Copyright 2013 Guillaume Denry (guillaume.denry@gmail.com)
//  This file is part of BetaSeeker.
//
//  BetaSeeker is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  BetaSeeker is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with BetaSeeker.  If not, see <http://www.gnu.org/licenses/>.

#include "jsonparser.h"

JsonParser::JsonParser(const QByteArray &content)
	: valid(false)
{
	if (content.isEmpty())
		return;

	parse(content);
}

bool JsonParser::parse(const QByteArray &content)
{
	valid = false;
	QJsonParseError error;
	doc = QJsonDocument::fromJson(content, &error);
	if (doc.isNull()) {
		qDebug("error while parsing json");
		qDebug("%s: %d", qPrintable(error.errorString()), error.offset);
		return false;
	}

	if (!doc.isObject()) {
		// we except an object for the doc
		return false;
	}

	QJsonObject obj = doc.object();
	QJsonValue val = obj.value("root");
	if (!val.isObject()) {
		// response root child must be an object
		return false;
	}

	_root = val.toObject();
	_code = (int) _root.value("code").toDouble();
	valid = true;
	return true;
}
