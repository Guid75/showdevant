#include "jsonparser.h"
#include "showsearchmodel.h"

ShowSearchModel::ShowSearchModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

int ShowSearchModel::rowCount(const QModelIndex &parent) const
{
	if (!parent.isValid())
		return items.count();
	return 0;
}

QVariant ShowSearchModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	const QPair<QString,QString> &pair = items[index.row()];
	if (role == Qt::DisplayRole)
		return pair.second;
	return QVariant();
}

QVariant ShowSearchModel::get(int row)
{
	const QPair<QString,QString> &pair = items[row];

	QMap<QString, QVariant> itemData;
	itemData.insert("showId", pair.first);
	itemData.insert("title", pair.second);
	return QVariant(itemData);
}

void ShowSearchModel::parseJson(const QByteArray &data)
{
	JsonParser parser(data);

	if (!parser.isValid()) {
		// TODO manage error
		return;
	}

	beginResetModel();

	items.clear();

	QJsonObject shows = parser.root().value("shows").toObject();
	if (shows.isEmpty()) {
		// TODO manage error
		endResetModel();
		return;
	}

	foreach (const QString &key, shows.keys()) {
		QJsonObject show = shows.value(key).toObject();
		if (show.isEmpty()) {
			// TODO manage error
			continue;
		}

		QString title = show.value("title").toString();
		if (title.isNull()) {
			// TODO manage error
			continue;
		}
		QString url = show.value("url").toString();
		if (url.isNull()) {
			// TODO manage error
			continue;
		}
		items << QPair<QString,QString>(url, title);
	}
	endResetModel();
}
