#ifndef TESTUSERDATA_H
#define TESTUSERDATA_H

#include <QtTest>

class TestUserData : public QObject
{
    Q_OBJECT
public:
    TestUserData();

private slots:
    void createAuthHeader_data();
    void createAuthHeader();
};

#endif // TESTUSERDATA_H
