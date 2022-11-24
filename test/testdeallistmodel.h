#ifndef TESTDEALLISTMODEL_H
#define TESTDEALLISTMODEL_H

#include <QtTest>
#include <QJsonArray>

class TestDealListModel : public QObject
{
    Q_OBJECT
public:
    TestDealListModel();

private slots:
    void setData_data();
    void setData();

private:
    QJsonArray _validArray, _invalidArray;
};

#endif // TESTDEALLISTMODEL_H
