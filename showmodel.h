#ifndef SHOWMODEL_H
#define SHOWMODEL_H

#include <QAbstractListModel>
#include <QSqlRecord>

class ShowModel : public QAbstractListModel
{
	Q_OBJECT

	Q_PROPERTY(int count READ getCount NOTIFY countChanged)
	Q_PROPERTY(QString table READ getTableName WRITE setTable(QString))

public:
	explicit ShowModel(QObject *parent = 0);

	// inherited
	int rowCount(const QModelIndex & parent = QModelIndex()) const;
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
	QHash<int, QByteArray> roleNames() const{ return roles; }

	// custom
	int getCount() { this->count = this->rowCount(); return count; }

	Q_INVOKABLE bool addShow(const QString &showId, const QString &title);

	Q_INVOKABLE void setTable(const QString &tableName);
	const QString &getTableName() const { return _tableName; }
	Q_INVOKABLE void select();
	
signals:
	void countChanged();

public slots:

private:
	int count;
	QList<QSqlRecord> shows;
	QString _tableName;
	QHash<int, QByteArray> roles;

	void generateRoles();
	int getInsertionIndex(const QSqlRecord &record) const;
};

#endif // SHOWMODEL_H
