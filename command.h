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
	bool error() const { return _error; }

	const JsonParser &jsonParser() const { return _jsonParser; }
	
signals:
	void finished();

private:
	bool _error;
	QByteArray _response;
	JsonParser _jsonParser;

	void finalize();
};

#endif // COMMAND_H
