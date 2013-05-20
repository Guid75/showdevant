#ifndef SQLQUERYMODEL_H
#define SQLQUERYMODEL_H

#include <QSqlQueryModel>

class SqlQueryModel : public QSqlQueryModel
{
	Q_OBJECT
	Q_PROPERTY(QString query READ getQuery WRITE setQuery)
	Q_PROPERTY(int count READ getCount() NOTIFY countChanged())

public:
	explicit SqlQueryModel(QObject *parent = 0);

	const QString &getQuery() const { return _query; }
	void setQuery(const QString &query);
	void setQuery(const QSqlQuery &query);
	QVariant data(const QModelIndex &index, int role) const;
	virtual QHash<int, QByteArray> roleNames() const{return roles;}
	int getCount() { this->count = this->rowCount(); return count; }
	Q_INVOKABLE QVariant get(int row);

signals:
	void countChanged();

public slots:

private:
	int count;
	QString _query;
	QHash<int, QByteArray> roles;

	void generateRoleNames();
};

#endif // SQLQUERYMODEL_H
