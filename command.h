#ifndef COMMAND_H
#define COMMAND_H

#include <QObject>

class Command : public QObject
{
	friend class CommandManager;
	Q_OBJECT
public:
	explicit Command(QObject *parent = 0);

	const QByteArray &response() const { return _response; }
	bool error() const { return _error; }
	
signals:
	void finished();

private:
	bool _error;
	QByteArray _response;

	void emitFinished();
};

#endif // COMMAND_H
