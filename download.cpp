#include "download.h"

Download::Download(QObject *parent) :
    QObject(parent)
{
}

void Download::emitFinished()
{
	emit finished();
}
