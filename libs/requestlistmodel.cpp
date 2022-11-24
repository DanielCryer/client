#include "requestlistmodel.h"

#include <QJsonArray>
#include <QJsonObject>


bool checkRequestJsonValue(const QJsonValue& val);
bool checkRequestJsonArray(const QJsonArray& array);


RequestListModel::RequestListModel(QObject *parent) : QAbstractListModel(parent) {}

RequestListModel::~RequestListModel()
{
    qDeleteAll(_dataList);
}

int RequestListModel::rowCount(const QModelIndex &parent) const
{
    return _dataList.count();
}

QVariant RequestListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= _dataList.size())
        return QVariant();

    if (role == RL_ID) {
        return QVariant::fromValue(_dataList[index.row()]->id);
    } else if (role == RL_USER_ID) {
        return QVariant::fromValue(_dataList[index.row()]->user_id);
    } else if (role == RL_COST) {
        return QVariant::fromValue(_dataList[index.row()]->cost);
    } else if (role == RL_VALUE) {
        return QVariant::fromValue(_dataList[index.row()]->value);
    } else if (role == RL_CUR_VAL) {
        return QVariant::fromValue(_dataList[index.row()]->current_value);
    } else if (role == RL_STATUS) {
        return QVariant::fromValue(_dataList[index.row()]->status);
    } else if (role == RL_DATE) {
        return QVariant::fromValue(_dataList[index.row()]->date);
    } else if (role == RL_DIRECT) {
        return QVariant::fromValue(_dataList[index.row()]->direction);
    }


    return QVariant();
}

QHash<int, QByteArray> RequestListModel::roleNames() const
{
    QHash<int, QByteArray> res;
    res[RL_ID] = "id";
    res[RL_USER_ID] = "user_id";
    res[RL_COST] = "cost";
    res[RL_VALUE] = "value";
    res[RL_CUR_VAL] = "current_value";
    res[RL_STATUS] = "status";
    res[RL_DATE] = "date";
    res[RL_DIRECT] = "direction";
    return res;
}

void RequestListModel::setData(const QJsonArray &data)
{
    if (!checkRequestJsonArray(data))
        return;

    beginResetModel();

    qDeleteAll(_dataList);
    _dataList.clear();

    Request *r;
    QJsonObject obj;

    for (const auto& el : data) {
        r = new Request{};
        obj = el.toObject();

        r->id = obj["request_id"].toInt();
        r->status = obj["status"].toString();
        r->user_id = obj["user_id"].toInt();
        r->current_value = obj["current_value"].toInt();
        r->value = obj["value"].toInt();
        r->direction = obj["direction"].toString();
        r->cost = obj["cost"].toInt();
        r->setUTCDate(obj["open_date"].toString());

        _dataList.append(r);
    }

    endResetModel();
}

void RequestListModel::addData(const QJsonArray &data)
{
    if (!data.count() || !checkRequestJsonArray(data))
        return;

    beginInsertRows(QModelIndex{}, _dataList.count(), _dataList.count() + data.count() - 1);

    for (const auto& el : data) {
        auto r = new Request{};
        r->id = el.toObject()["request_id"].toInt();
        r->status = el.toObject()["status"].toString();
        r->user_id = el.toObject()["user_id"].toInt();
        r->current_value = el.toObject()["current_value"].toInt();
        r->value = el.toObject()["value"].toInt();
        r->direction = el.toObject()["direction"].toString();
        r->cost = el.toObject()["cost"].toInt();
        r->setUTCDate(el.toObject()["open_date"].toString());

        _dataList.append(r);
    }

    endInsertRows();
}


bool checkRequestJsonValue(const QJsonValue& val)
{
    if (!val.isObject())
        return false;

    auto obj = val.toObject();
    return  obj.contains("request_id") &&
            obj.contains("status") &&
            obj.contains("user_id") &&
            obj.contains("current_value") &&
            obj.contains("value") &&
            obj.contains("direction") &&
            obj.contains("cost") &&
            obj.contains("open_date");
}

bool checkRequestJsonArray(const QJsonArray& dataArray)
{
    for (const auto& el : dataArray)
        if (!checkRequestJsonValue(el))
            return false;
    return true;
}
