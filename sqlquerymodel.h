#ifndef SQLQUERYMODEL_H
#define SQLQUERYMODEL_H

#include <QSqlQueryModel>

class SqlQueryModel : public QSqlQueryModel
{
	Q_OBJECT
	Q_PROPERTY(QString query READ getQuery WRITE setQuery)
	Q_PROPERTY(int count READ getCount() NOTIFY countChanged())

	Q_PROPERTY(bool synchronized READ getSynchronized() NOTIFY synchronizedChanged())
	Q_PROPERTY(bool synchronizing READ getSynchronizing() NOTIFY synchronizingChanged())

public:
	explicit SqlQueryModel(QObject *parent = 0);

	const QString &getQuery() const { return _query; }
	Q_INVOKABLE void setQuery(const QString &query);
	Q_INVOKABLE void setQuery(const QSqlQuery &query);
	QVariant data(const QModelIndex &index, int role) const;
	virtual QHash<int, QByteArray> roleNames() const{return roles;}
	int getCount() { this->count = this->rowCount(); return count; }
	Q_INVOKABLE QVariant get(int row);

	bool getSynchronized() const { return _synchronized; }
	bool getSynchronizing() const { return _synchronizing; }

signals:
	void countChanged();
	void synchronizedChanged();
	void synchronizingChanged();

protected:
	void setSynchronized(bool value);
	void setSynchronizing(bool value);

private:
	int count;
	QString _query;
	QHash<int, QByteArray> roles;

	bool _synchronized;
	bool _synchronizing;

	void generateRoleNames();
};

#endif // SQLQUERYMODEL_H
