#include "showsearchmodel.h"

ShowSearchModel::ShowSearchModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

int ShowSearchModel::rowCount(const QModelIndex &parent) const
{
	if (!parent.isValid())
		return 4;
	return 0;
}

QVariant ShowSearchModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (role == Qt::DisplayRole)
		return "toto";
	return QVariant();
}

void ShowSearchModel::parse(const QByteArray &data)
{
}
