#ifndef SQLTABLEMODEL_H
#define SQLTABLEMODEL_H

#include <QSqlTableModel>

class SqlTableModel : public QSqlTableModel
{
	Q_OBJECT
	Q_PROPERTY(int count READ getCount() NOTIFY countChanged())
	Q_PROPERTY(QString table READ tableName() WRITE setTable(tableName))
	Q_PROPERTY(Qt::SortOrder sortOrder READ getSortOrder() WRITE setSortOrder(order))
	Q_PROPERTY(QString sortField READ getSortField() WRITE setSortField(field))
	Q_PROPERTY(QString currentFilter READ filter() WRITE setFilter(filter))

public:
	SqlTableModel(QObject *parent = 0);
	Q_INVOKABLE QVariant data(const QModelIndex &index, int role = Qt::DisplayRole ) const;
	void generateRoleNames();
	virtual QHash<int, QByteArray> roleNames() const{return roles;}

	void setTable(const QString & tableName);

	int getCount() { this->count = this->rowCount(); return count; }

	Qt::SortOrder getSortOrder() const;
	void setSortOrder(Qt::SortOrder order);

	const QString &getSortField() const;
	void setSortField(const QString &field);

signals:
	void countChanged();

private:
	int count;
	QHash<int, QByteArray> roles;

	Qt::SortOrder _sortOrder;
	QString _sortField;
};

#endif // SQLTABLEMODEL_H
