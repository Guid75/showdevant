#ifndef COMMAND_H
#define COMMAND_H

#include <QObject>

class Command : public QObject
{
	friend class CommandManager;
	Q_OBJECT
public:
	explicit Command(QObject *parent = 0);
	
signals:
	void finished(const QByteArray &response);

public slots:

private:
	QByteArray _response;

	void emitFinished();
};

#endif // COMMAND_H
