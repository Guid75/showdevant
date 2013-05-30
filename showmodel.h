#ifndef SHOWMODEL_H
#define SHOWMODEL_H

#include <QAbstractListModel>
#include <QSqlRecord>

#include "sqltablemodel.h"

class ShowModel : public SqlTableModel
{
	Q_OBJECT

public:
	explicit ShowModel(QObject *parent = 0);

	// custom
	Q_INVOKABLE bool addShow(const QString &showId, const QString &title);
};

#endif // SHOWMODEL_H
