#ifndef REQUESTLISTMODEL_H
#define REQUESTLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>

#include "request.h"

class RequestListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum eRLRoleTypes
    {
        RL_ID = Qt::UserRole + 1,
        RL_USER_ID,
        RL_COST,
        RL_VALUE,
        RL_CUR_VAL,
        RL_STATUS,
        RL_DATE,
        RL_DIRECT
    };

    explicit RequestListModel(QObject *parent = nullptr);
    ~RequestListModel();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int,QByteArray> roleNames() const;

    void setData(const QJsonArray& data);
    void addData(const QJsonArray& data);

private:
    QList<Request*> _dataList;
};

#endif // REQUESTLISTMODEL_H
