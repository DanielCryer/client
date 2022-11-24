#ifndef TESTREQUESTLISTMODEL_H
#define TESTREQUESTLISTMODEL_H

#include <QtTest>
#include <QJsonArray>

class TestRequestListModel : public QObject
{
    Q_OBJECT
public:
    TestRequestListModel();

private slots:
    void setData_data();
    void setData();
    void addData_data();
    void addData();

private:
    QJsonArray _validArray, _invalidArray;
};

#endif // TESTREQUESTLISTMODEL_H
