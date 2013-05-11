#ifndef SHOWSEARCHMODEL_H
#define SHOWSEARCHMODEL_H

#include <QAbstractListModel>
#include <QtQml>

class ShowSearchModel : public QAbstractListModel
{
	Q_OBJECT

	Q_PROPERTY(int count READ getCount() NOTIFY countChanged())
public:
	explicit ShowSearchModel(QObject *parent = 0);

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

	int getCount() { this->count = this->rowCount(); return count; }

	Q_INVOKABLE QVariant get(int row);
	Q_INVOKABLE void parseJson(const QByteArray &data);

signals:
	void countChanged();
	
public slots:

private:
	QList<QPair<QString,QString> > items;
	int count;
	
};

#endif // SHOWSEARCHMODEL_H
