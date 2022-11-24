#ifndef DEALLISTMODEL_H
#define DEALLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>

#include "deal.h"

class DealListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum eDLRoleTypes
    {
        DL_ID = Qt::UserRole + 1,
        DL_COST,
        DL_VALUE,
        DL_DATE,
        DL_NAME
    };

    explicit DealListModel(QObject *parent = nullptr);
    ~DealListModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int,QByteArray> roleNames() const;

    void setData(const QJsonArray& data);

private:
    QList<Deal*> _dataList;
};

#endif // DEALLISTMODEL_H
