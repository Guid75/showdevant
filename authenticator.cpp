#include <QCryptographicHash>
#include <QJsonDocument>
#include <QJsonObject>

#include "commandmanager.h"
#include "command.h"
#include "settings.h"

#include "authenticator.h"

Authenticator::Authenticator(QObject *parent) :
	QObject(parent),
	_logState(NotLogged),
	currentCommand(0),
	_lastError(NoError)
{
}

void Authenticator::login(const QString &user, const QString &password, bool rememberMe)
{
	QString passwordMd5(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Md5).toHex());
	loginMd5(user, passwordMd5, rememberMe);
}

void Authenticator::logout()
{
	// TODO destroy the token

	setLogState(NotLogged);
}

void Authenticator::autoLogin()
{
	Settings settings;
	QString user = settings.getValue("account/user", "").toString();
	QString passwordMd5 = settings.getValue("account/password", "").toString();

	if (!user.isEmpty() && !passwordMd5.isEmpty())
		loginMd5(user, passwordMd5, false);
}

void Authenticator::forgetMe()
{
	Settings settings;
	settings.remove("account/user");
	settings.remove("account/password");

	logout();
}

void Authenticator::commandFinished()
{
	Command *command = qobject_cast<Command*>(sender());
	Q_ASSERT(command);

	if (!command->jsonParser().isValid()) {
		qCritical("Invalid response format");
		return;
	}

	const QJsonObject &root = command->jsonParser().root();
	QString user;
	QJsonObject obj;
	int errorCode = (int) root.value("code").toDouble(-1);
	Q_ASSERT(errorCode >= 0 && errorCode <= 1);

	if (errorCode == 0) {
		// manage error
		obj = root.value("errors").toObject().value("error").toObject();
		int code = (int) obj.value("code").toDouble(-1);
		switch (code) {
		case 4002:
			_lastError = UnknownUser;
			break;
		case 4003:
			_lastError = BadPassword;
			break;
		default:
			// a code we do not know yet
			_lastError = UnknownError;
			break;
		}
		setLogState(NotLogged);
		emit error(_lastError);
	} else {
		// we are authenticated!
		obj = root.value("member").toObject();
		user = obj.value("login").toString("");
		Q_ASSERT(!user.isEmpty());
		setUser(user);
		_token = obj.value("token").toString("");
		Q_ASSERT(!_token.isEmpty());

		// if we must remember, we have to apply data on settings
		if (_tmpRememberMe) {
			Settings settings;
			settings.setValue("account/user", user);
			settings.setValue("account/password", _tmpPasswordMd5);
		}

		CommandManager::instance().recordAuthToken(_token);

		setLogState(Logged);
	}
	currentCommand->deleteLater();
	currentCommand = 0;
}

void Authenticator::setUser(const QString &user)
{
	if (user == _user)
		return;

	_user = user;
	emit userChanged();
}

void Authenticator::setLogState(Authenticator::LogState logState)
{
	if (_logState == logState)
		return;

	_logState = logState;
	emit logStateChanged();
}

void Authenticator::loginMd5(const QString &user, const QString &passwordMd5, bool rememberMe)
{
	if (currentCommand != 0) {
		qCritical("A login operation is already running");
		return;
	}

	// TODO deal with an already logged user
	currentCommand = CommandManager::instance().membersAuth(user, passwordMd5);
	connect(currentCommand, &Command::finished, this, &Authenticator::commandFinished);
	_tmpRememberMe = rememberMe;
	_tmpPasswordMd5 = passwordMd5;
	setLogState(Logging);
}
