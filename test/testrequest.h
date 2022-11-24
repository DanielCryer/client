#ifndef TESTREQUEST_H
#define TESTREQUEST_H

#include <QtTest>

class TestRequest : public QObject
{
    Q_OBJECT

public:
    TestRequest();
    ~TestRequest();

private slots:
    void defaultValues();
    void setUTCDate();
};

#endif // TESTREQUEST_H
