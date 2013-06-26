#include "command.h"

Command::Command(QObject *parent) :
	QObject(parent),
	_httpError(false)
{
}

void Command::finalize()
{
	if (!_httpError)
		_jsonParser.parse(_response);
	emit finished();
}
