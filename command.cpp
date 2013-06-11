#include "command.h"

Command::Command(QObject *parent) :
	QObject(parent),
	_error(false)
{
}

void Command::finalize()
{
	_jsonParser.parse(_response);
	emit finished();
}
