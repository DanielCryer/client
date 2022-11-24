#include "deallistmodel.h"

#include <QJsonArray>
#include <QJsonObject>


bool checkDealJsonArray(const QJsonArray& dataArray);


DealListModel::DealListModel(QObject *parent) : QAbstractListModel(parent) {}

DealListModel::~DealListModel()
{
    qDeleteAll(_dataList);
}

int DealListModel::rowCount(const QModelIndex &parent) const
{
    return _dataList.count();
}

QVariant DealListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= _dataList.size())
        return QVariant();

    if (role == DL_ID) {
        return QVariant::fromValue(_dataList[index.row()]->id);
    } else if (role == DL_COST) {
        return QVariant::fromValue(_dataList[index.row()]->cost);
    } else if (role == DL_VALUE) {
        return QVariant::fromValue(_dataList[index.row()]->value);
    } else if (role == DL_DATE) {
        return QVariant::fromValue(_dataList[index.row()]->date);
    } else if (role == DL_NAME) {
        return QVariant::fromValue(_dataList[index.row()]->name);
    }


    return QVariant();
}

QHash<int, QByteArray> DealListModel::roleNames() const
{
    QHash<int, QByteArray> res;
    res[DL_ID] = "id";
    res[DL_COST] = "cost";
    res[DL_VALUE] = "value";
    res[DL_DATE] = "date";
    res[DL_NAME] = "name";
    return res;
}

void DealListModel::setData(const QJsonArray &data)
{
    if (!checkDealJsonArray(data))
        return;

    beginResetModel();

    qDeleteAll(_dataList);
    _dataList.clear();

    for (const auto& el : data) {
        auto r = new Deal{};
        r->id = el.toObject()["deal_id"].toInt();
        r->value = el.toObject()["value"].toInt();
        r->cost = el.toObject()["cost"].toInt();
        r->name = el.toObject()["name"].toString();
        r->setUTCDate(el.toObject()["date"].toString());

        _dataList.append(r);
    }

    endResetModel();
}



bool checkDealJsonValue(const QJsonValue& val)
{
    if (!val.isObject())
        return false;

    auto obj = val.toObject();
    return  obj.contains("deal_id") &&
            obj.contains("value") &&
            obj.contains("cost") &&
            obj.contains("name") &&
            obj.contains("date");
}

bool checkDealJsonArray(const QJsonArray& dataArray)
{
    for (const auto& el : dataArray)
        if (!checkDealJsonValue(el))
            return false;
    return true;
}
