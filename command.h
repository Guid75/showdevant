#ifndef COMMAND_H
#define COMMAND_H

#include <QObject>

#include "jsonparser.h"

class Command : public QObject
{
	friend class CommandManager;
	Q_OBJECT
public:
	explicit Command(QObject *parent = 0);

	const QByteArray &response() const { return _response; }
	// true if an error occured at the http level
	bool httpError() const { return _httpError; }

	const JsonParser &jsonParser() const { return _jsonParser; }
	
signals:
	void finished();

private:
	bool _httpError;
	QByteArray _response;
	JsonParser _jsonParser;

	void finalize();
};

#endif // COMMAND_H
