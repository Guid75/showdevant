#include "settings.h"

#include <QDesktopServices>
#include <QCoreApplication>


#define Q_SETTINGS QSettings settings("Guid75", "ShowDevant")

Settings::Settings(QObject *parent) : QObject(parent)
{
	// Initialize the settings path
//	m_confFile = QDesktopServices::storageLocation( QDesktopServices::DataLocation ) + "/" + appName + "/" + fileName;
}

void Settings::setValue(const QString & key, const QVariant & value)
{
	Q_SETTINGS;

	settings.setValue(key, value);
}

QVariant Settings::getValue( const QString & key, const QVariant & defaultValue) const
{
	Q_SETTINGS;

	return settings.value(key, defaultValue);
}

// first index=0
void Settings::removeArrayEntry( const QString & key , int index)
{
	QList< QVariantMap > array;
	array = getArray(key);
	array.removeAt(index);

	removeArray(key);

	foreach (QVariantMap entry, array)
		appendToArray(key, entry);
}

void Settings::removeArray( const QString & key )
{
	Q_SETTINGS;

	settings.remove(key);
}

void Settings::appendToArray( const QString & key, QMap<QString, QVariant> values)
{
	Q_SETTINGS;
	QMapIterator<QString, QVariant> i(values);

	// get the current size of this array
	int size = settings.beginReadArray(key);
	settings.endArray();

	settings.beginWriteArray(key);
	settings.setArrayIndex(size);
	while (i.hasNext()) {
		i.next();
		settings.setValue(i.key(), i.value() );
	}
	settings.endArray();
}


QString Settings::getArrayJson( const QString & key)
{
	Q_SETTINGS;
	QString list;

	list = "[ { ";
	int size = settings.beginReadArray(key);

	for (int i = 0; i < size; ++i) {
		settings.setArrayIndex(i);
		QVariantMap entry;
		QStringList keys;
		keys = settings.allKeys();
		foreach (QString key, keys) {
			entry.insert( key, settings.value(key) );
			list += "\""+key+"\" : \""+settings.value(key).toString()+"\", ";
		}

		list.remove( list.length()-2, 2 );
		list += "},{";
	}
	list.remove( list.length()-2, 2 );
	list += " ]";
	list = list.replace("\n","<br>").replace("\r","").replace("\t"," ");
	//     qDebug() << "LIST: " << list;

	settings.endArray();
	return list;
}

QString Settings::getArrayXml( const QString & key)
{
	Q_SETTINGS;
	QString list;

	int size = settings.beginReadArray(key);

	list = "<xml>";
	for (int i = 0; i < size; ++i) {
		list += "<item>";
		settings.setArrayIndex(i);
		QVariantMap entry;
		QStringList keys;
		keys = settings.allKeys();
		foreach (QString key, keys) {
			entry.insert( key, settings.value(key) );
			list += "<"+key+">"+settings.value(key).toString()+"</"+key+">";
		}
		list += "</item>";
	}
	list = list.replace("\n"," ").replace("\r"," ").replace("\t"," ").replace("<br>"," ");
	list += "</xml>";
	//     qDebug() << "LIST: " << list;

	settings.endArray();
	return list;
}

bool Settings::checkValueArray( const QString & key, const QString & arrayKey , const QString & value )
{
	Q_SETTINGS;

	int size = settings.beginReadArray(key);

	for (int i = 0; i < size; ++i) {
		settings.setArrayIndex(i);
		if ( settings.value(arrayKey).toString().compare(value) == 0 ) return true;
	}

	return false;
}

int Settings::getIndexOfValueInArray( const QString & key, const QString & arrayKey , const QString & value )
{
	Q_SETTINGS;

	int size = settings.beginReadArray(key);

	for (int i = 0; i < size; ++i) {
		settings.setArrayIndex(i);
		if ( settings.value(arrayKey).toString().compare(value) == 0 ) return i;
	}
	return -1;
}

QList< QVariantMap > Settings::getArray( const QString & key)
{
	Q_SETTINGS;
	QList< QVariantMap > list;

	int size = settings.beginReadArray(key);

	for (int i = 0; i < size; ++i) {
		settings.setArrayIndex(i);
		QVariantMap entry;
		QStringList keys;
		keys = settings.allKeys();
		foreach (QString key, keys) {
			entry.insert( key, settings.value(key) );
		}

		list.append(entry);
	}

	settings.endArray();
	return list;
}



// https://bugreports.qt-project.org/browse/QTBUG-71
int Settings::getTimeZone()
{
	// determine how far off of UTC we are
	QDateTime now = QDateTime::currentDateTime();

	// check if we need to account for UTC being tomorrow relative to us
	int dayOffset = 0;
	if( now.date() < now.toUTC().date() )
	{
		dayOffset = 24;
	}

	int timezone = now.time().hour() - ( now.toUTC().time().hour() + dayOffset );
	//    qDebug() << "TIMEZONE" << timezone;
	return timezone;
}


// convert a string with format "2012-02-06T06:00:00" ( the java Date() )to QDateTime
QDateTime Settings::stringToDate(QString s)
{
	QDateTime t;

	t = QDateTime::fromString(s, "yyyy-MM-ddThh:mm:ss");

	return t;
}
