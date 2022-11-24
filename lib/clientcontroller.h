#ifndef CLIENTCONTROLLER_H
#define CLIENTCONTROLLER_H

#include <QObject>
#include <QList>
#include <QNetworkAccessManager>

#include "requestlistmodel.h"
#include "deallistmodel.h"
#include "filters.h"
#include "userdata.h"

class ClientController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool isServerEnabled READ isServerEnabled NOTIFY serverEnabledChanged);
    Q_PROPERTY(QString connectionErrorMessage MEMBER _connectionErrorMessage NOTIFY connectionErrorMessageChanged);

    Q_PROPERTY(RequestListModel *generalRequests READ getGeneralRequests CONSTANT);
    Q_PROPERTY(RequestListModel *userRequests READ getUserRequests CONSTANT);

    Q_PROPERTY(Filters generalFilters MEMBER _generalFilters NOTIFY generalFiltersChanged);
    Q_PROPERTY(Filters userFilters MEMBER _userFilters NOTIFY userFiltersChanged);

    Q_PROPERTY(DealListModel *deals READ getDeals CONSTANT);
    Q_PROPERTY(QVariant dealsRequest READ getDealsRequest NOTIFY dealsRequestChanged);

    Q_PROPERTY(QString creationRequestStatus MEMBER _creationRequestStatus NOTIFY creationRequestStatusChanged);
    Q_PROPERTY(QString canselRequestStatus MEMBER _canselRequestStatus NOTIFY canselRequestStatusChanged);

    Q_PROPERTY(bool isAuthorized READ isAuthorized NOTIFY authorizedChanged);
    Q_PROPERTY(QString registrationStatus MEMBER _registrationStatus NOTIFY registrationStatusChanged);
    Q_PROPERTY(QString authorizationStatus MEMBER _authorizationStatus NOTIFY authorizationStatusChanged);
    Q_PROPERTY(QVariant user READ getUserData NOTIFY userDataChanged);

public:
    explicit ClientController(QObject *parent = nullptr);

    bool isServerEnabled();

    RequestListModel *getGeneralRequests();
    RequestListModel *getUserRequests();

    DealListModel *getDeals();
    QVariant getDealsRequest();

    bool isAuthorized();
    QVariant getUserData();

signals:
    void connectionErrorMessageChanged();
    void serverEnabledChanged();
    void serverDisabled();

    void generalFiltersChanged();
    void userFiltersChanged();
    void dealsRequestChanged();

    void creationRequestStatusChanged();
    void canselRequestStatusChanged();

    void authorizedChanged();
    void registrationStatusChanged();
    void authorizationStatusChanged();
    void userDataChanged();

public slots:
    void changeServerHost(const QString& hostName, const int port);

    void loadGeneralRequests(bool isLoadMore = false);
    void loadUserRequests(bool isLoadMore = false);
    void getRequestDeals(int requestId);

    void createRequest(const QString& direction, const int value, const int cost);
    void canselRequest(const int id);

    void signUp(const QString& email, const QString& password, const QString& name);
    void signIn(const QString& email, const QString& password);
    void updateUserData();
    void signOut();

    void saveConfig() const;
    void loadConfig();

private slots:
    void ignoreSslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    void setServerHost();

    void setGeneralRequests();
    void setUserRequests();
    void setRequestDeals();

    void addGeneralRequests();
    void addUserRequests();

    void setCreationRequestStatus();
    void setCanselRequestStatus();

    void setRegistrationStatus();
    void setAuthorizationStatus();
    void signIn(const QByteArray& authHeader);

private:
    QNetworkRequest *constructServerPingNetworkRequest(const QString& hostName, const int port) const;

    QNetworkRequest *constructLoadGeneralRequestsNetworkRequest(bool isLoadMore) const;
    QNetworkRequest *constructLoadUserRequestsNetworkRequest(bool isLoadMore) const;
    QNetworkRequest *constructGetRequestDealsNetworkRequest(int requestId) const;

    QNetworkRequest *constructCreateRequestNetworkRequest() const;
    QNetworkRequest *constructCanselRequestNetworkRequest() const;

    QNetworkRequest *constructSignUpNetworkRequest(const QByteArray &authHeader) const;
    QNetworkRequest *constructSignInNetworkRequest(const QByteArray &authHeader) const;

    bool checkReplyError(const QNetworkReply *rep);


    RequestListModel *_generalRequests, *_userRequests;
    Filters _generalFilters, _userFilters;
    DealListModel *_deals;
    Request *_dealsRequest;
    QNetworkAccessManager *_manager;
    UserData *_user;
    QString _registrationStatus, _authorizationStatus,
            _creationRequestStatus, _canselRequestStatus,
            _serverHost, _connectionErrorMessage;
    int _serverPort;
    bool _isAllGeneralRequests, _isAllUserRequests,
         _isServerEnabled;
};

#endif // CLIENTCONTROLLER_H
