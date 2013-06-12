#ifndef AUTHENTICATOR_H
#define AUTHENTICATOR_H

#include <QObject>

class Command;

class Authenticator : public QObject
{
	Q_OBJECT

	Q_PROPERTY(QString user READ user() NOTIFY userChanged())
	Q_PROPERTY(LogState logState READ logState() NOTIFY logStateChanged())
	Q_PROPERTY(QString token READ token()) // no needs to have a notify for this value for now
	Q_ENUMS(AuthenticationError)
	Q_ENUMS(LogState)

public:
	enum AuthenticationError {
		NoError = -1,
		UnknownUser = 0,
		BadPassword,
		UnknownError
	};

	enum LogState {
		NotLogged,
		Logging,
		Logged
	};

	explicit Authenticator(QObject *parent = 0);

	Q_INVOKABLE void login(const QString &user, const QString &password, bool rememberMe);

	Q_INVOKABLE void logout();

	// launch login automatically if settings contain user & password
	Q_INVOKABLE void autoLogin();

	Q_INVOKABLE void forgetMe();

	LogState logState() const { return _logState; }

	const QString &user() const { return _user; }
	const QString &token() const { return _token; }

	Q_INVOKABLE AuthenticationError lastError() const { return _lastError; }
	
signals:
	void error(AuthenticationError error);
	void userChanged();
	void logStateChanged();
	
public slots:

private:
	LogState _logState;
	QString _user;
	QString _token;
	bool _tmpRememberMe;
	QString _tmpPasswordMd5;
	Command *currentCommand;
	AuthenticationError _lastError;

	void setUser(const QString &user);
	void setLogState(LogState logState);

	void loginMd5(const QString &user, const QString &passwordMd5, bool rememberMe);

private slots:
	void commandFinished();
	
};

#endif // AUTHENTICATOR_H
