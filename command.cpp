#include "command.h"

Command::Command(QObject *parent) :
    QObject(parent)
{
}

void Command::emitFinished()
{
	emit finished(_response);
}
