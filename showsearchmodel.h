#ifndef SHOWSEARCHMODEL_H
#define SHOWSEARCHMODEL_H

#include <QAbstractListModel>
#include <QtQml>

class ShowSearchModel : public QAbstractListModel
{
	Q_OBJECT
public:
	explicit ShowSearchModel(QObject *parent = 0);

	int rowCount(const QModelIndex &parent) const;
	QVariant data(const QModelIndex &index, int role) const;


	Q_INVOKABLE void parse(const QByteArray &data);

signals:
	
public slots:
	
};

#endif // SHOWSEARCHMODEL_H
