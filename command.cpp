#include "command.h"

Command::Command(QObject *parent) :
	QObject(parent),
	_error(false)
{
}

void Command::emitFinished()
{
	emit finished();
}
