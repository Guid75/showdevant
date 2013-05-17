#ifndef SQLTABLEMODEL_H
#define SQLTABLEMODEL_H

#include <QSqlTableModel>

class SqlTableModel : public QSqlTableModel
{
	Q_OBJECT
	Q_PROPERTY(QString table READ tableName() WRITE setTable(tableName))
	Q_PROPERTY(int count READ getCount() NOTIFY countChanged())

public:
	SqlTableModel(QObject *parent = 0);
	Q_INVOKABLE QVariant data(const QModelIndex &index, int role=Qt::DisplayRole ) const;
	void generateRoleNames();
	virtual QHash<int, QByteArray> roleNames() const{return roles;}

	void setTable(const QString & tableName);

	int getCount() { this->count = this->rowCount(); return count; }

signals:
	void countChanged();

private:
	int count;
	QHash<int, QByteArray> roles;

};

#endif // SQLTABLEMODEL_H
