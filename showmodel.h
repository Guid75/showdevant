#ifndef SHOWMODEL_H
#define SHOWMODEL_H

#include <QAbstractListModel>
#include <QSqlRecord>

#include "sqltablemodel.h"

//class ShowModel : public QAbstractListModel
class ShowModel : public SqlTableModel
{
	Q_OBJECT

public:
	explicit ShowModel(QObject *parent = 0);

	// inherited
	//int rowCount(const QModelIndex & parent = QModelIndex()) const;
//	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

	// custom
	Q_INVOKABLE bool addShow(const QString &showId, const QString &title);

private:
	QList<QSqlRecord> shows;
//	int getInsertionIndex(const QSqlRecord &record) const;
};

#endif // SHOWMODEL_H
