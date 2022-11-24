#include "clientcontroller.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDebug>


ClientController::ClientController(QObject *parent) :
    QObject{parent}, _generalRequests{new RequestListModel{this}},
    _userRequests{new RequestListModel{this}},
    _deals{new DealListModel{this}},
    _manager{new QNetworkAccessManager{this}},
    _user{nullptr},
    _creationRequestStatus{" "},
    _connectionErrorMessage{""},
    _isAllGeneralRequests{true},
    _isAllUserRequests{true}
{
    connect(this, SIGNAL(serverEnabledChanged()), this, SLOT(loadConfig()));
    connect(this, SIGNAL(serverEnabledChanged()), this, SLOT(loadGeneralRequests()));
}

bool ClientController::isServerEnabled()
{
    return _isServerEnabled;
}

RequestListModel* ClientController::getGeneralRequests()
{
    return _generalRequests;
}

RequestListModel *ClientController::getUserRequests()
{
    return _userRequests;
}

DealListModel *ClientController::getDeals()
{
    return _deals;
}

QVariant ClientController::getDealsRequest()
{
    if (_dealsRequest)
        return QVariant::fromValue(*_dealsRequest);

    return QVariant::fromValue(Request{});
}

bool ClientController::isAuthorized()
{
    return _user;
}

QVariant ClientController::getUserData()
{
    if (_user)
        return QVariant::fromValue(*_user);

    return QVariant::fromValue(UserData{});
}

void ClientController::changeServerHost(const QString& hostName, const int port)
{
    auto request = constructServerPingNetworkRequest(hostName, port);
    auto rep = _manager->get(*request);
    delete request;

    connect(rep, &QNetworkReply::finished, this, &ClientController::setServerHost);
}

void ClientController::loadGeneralRequests(bool isLoadMore)
{
    if (!_isServerEnabled) {
        emit serverDisabled();
        return;
    }

    if (isLoadMore && _isAllGeneralRequests)
        return;

    auto request = constructLoadGeneralRequestsNetworkRequest(isLoadMore);
    auto rep = _manager->get(*request);
    delete request;

    if (isLoadMore)
        connect(rep, &QNetworkReply::finished, this, &ClientController::addGeneralRequests);
    else
        connect(rep, &QNetworkReply::finished, this, &ClientController::setGeneralRequests);
}

void ClientController::loadUserRequests(bool isLoadMore)
{
    if (!_isServerEnabled) {
        emit serverDisabled();
        return;
    }

    if (!isAuthorized())
        return;

    if (isLoadMore && _isAllUserRequests)
        return;

    auto request = constructLoadUserRequestsNetworkRequest(isLoadMore);
    auto rep = _manager->get(*request);
    delete request;

    if (isLoadMore)
        connect(rep, &QNetworkReply::finished, this, &ClientController::addUserRequests);
    else
        connect(rep, &QNetworkReply::finished, this, &ClientController::setUserRequests);
}

void ClientController::getRequestDeals(int requestId)
{
    if (!_isServerEnabled) {
        emit serverDisabled();
        return;
    }

    if (!isAuthorized())
        return;

    auto request = constructGetRequestDealsNetworkRequest(requestId);
    auto rep = _manager->get(*request);
    delete request;

    connect(rep, &QNetworkReply::finished, this, &ClientController::setRequestDeals);
}

void ClientController::createRequest(const QString &direction, const int value, const int cost)
{
    if (!_isServerEnabled) {
        emit serverDisabled();
        return;
    }

    if (!isAuthorized())
        return;

    _creationRequestStatus = "";
    emit creationRequestStatusChanged();

    auto request = constructCreateRequestNetworkRequest();
    auto data = QJsonObject{{"direction", direction}, {"value", value}, {"cost", cost}};

    auto rep = _manager->post(*request, QJsonDocument{QJsonObject{{"request", data}}}.toJson());
    delete request;

    connect(rep, &QNetworkReply::finished, this, &ClientController::setCreationRequestStatus);
}

void ClientController::canselRequest(const int id)
{
    if (!_isServerEnabled) {
        emit serverDisabled();
        return;
    }

    if (!isAuthorized())
        return;

    auto request = constructCanselRequestNetworkRequest();
    auto rep = _manager->post(*request, QJsonDocument{QJsonObject{{"request_id", id}}}.toJson());
    delete request;

    connect(rep, &QNetworkReply::finished, this, &ClientController::setCanselRequestStatus);
}

void ClientController::signUp(const QString &email, const QString &password, const QString &name)
{
    if (!_isServerEnabled) {
        emit serverDisabled();
        return;
    }

    _registrationStatus.clear();
    emit registrationStatusChanged();

    if (email.isEmpty() || password.isEmpty() || name.isEmpty())
        return;

    auto request = constructSignUpNetworkRequest(UserData::createAuthHeader(email, password));
    QJsonObject reg_data{{"email", email}, {"password", password}, {"name", name}};
    auto rep = _manager->post(*request, QJsonDocument{QJsonObject{{"registration_data", reg_data}}}.toJson());
    delete request;

    connect(rep, &QNetworkReply::finished, this, &ClientController::setRegistrationStatus);
}

void ClientController::signIn(const QString &email, const QString &password)
{
    if (!_isServerEnabled) {
        emit serverDisabled();
        return;
    }

    _authorizationStatus.clear();
    emit authorizationStatusChanged();

    if (email.isEmpty() || password.isEmpty())
        return;

    signIn(UserData::createAuthHeader(email, password));
}

void ClientController::signIn(const QByteArray &authHeader)
{
    if (!_isServerEnabled) {
        emit serverDisabled();
        return;
    }

    auto request = constructSignInNetworkRequest(authHeader);
    auto rep = _manager->get(*request);
    delete request;

    connect(rep, &QNetworkReply::finished, this, &ClientController::setAuthorizationStatus);
}

void ClientController::updateUserData()
{
    if (!_isServerEnabled) {
        emit serverDisabled();
        return;
    }

    if (!isAuthorized())
        return;

    _authorizationStatus.clear();
    emit authorizationStatusChanged();

    signIn(_user->authHeader);
}

void ClientController::signOut()
{
    _authorizationStatus.clear();
    emit authorizationStatusChanged();

    delete _user;
    _user = nullptr;

    emit authorizedChanged();
    emit userDataChanged();

    saveConfig();
}

void ClientController::saveConfig() const
{
    QFile confFile{"config.conf"};
    if (!confFile.open(QIODevice::WriteOnly))
        return;

    QJsonObject confData{};

    if (_user) {
        QJsonObject userData{{"auth_header", QString::fromUtf8(_user->authHeader)}};
        confData["user_data"] = userData;
    }

    confFile.write(QJsonDocument{confData}.toJson());
}

void ClientController::loadConfig()
{
    if (!_isServerEnabled) {
        emit serverDisabled();
        return;
    }

    QFile confFile{"config.conf"};
    if (!confFile.exists() || !confFile.open(QIODevice::ReadOnly))
        return;

    auto doc = QJsonDocument::fromJson(confFile.readAll());
    if (!doc.isObject() || !doc.object().contains("user_data"))
        return;

    signIn(doc.object()["user_data"].toObject()["auth_header"].toString().toUtf8());
}

void ClientController::ignoreSslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
    reply->ignoreSslErrors();
}

void ClientController::setServerHost()
{
    auto rep = qobject_cast<QNetworkReply *>(sender());
    if (!rep) return;

    if (rep->error()) {
        _connectionErrorMessage = rep->errorString();
        emit connectionErrorMessageChanged();
        rep->deleteLater();
        return;
    }

    _serverHost = rep->url().host();
    _serverPort = rep->url().port();

    _isServerEnabled = true;
    emit serverEnabledChanged();

    rep->deleteLater();
}

void ClientController::setGeneralRequests()
{
    auto rep = qobject_cast<QNetworkReply *>(sender());
    if (!rep) return;

    if (!checkReplyError(rep)) {
        rep->deleteLater();
        return;
    }

    auto data = QJsonDocument::fromJson(rep->readAll()).object()["requests"].toArray();
    _generalRequests->setData(data);

    if (data.count() < 20)
        _isAllGeneralRequests = true;
    else
        _isAllGeneralRequests = false;

    rep->deleteLater();
}

void ClientController::setUserRequests()
{
    auto rep = qobject_cast<QNetworkReply *>(sender());
    if (!rep) return;

    if (!checkReplyError(rep)) {
        rep->deleteLater();
        return;
    }

    auto data = QJsonDocument::fromJson(rep->readAll()).object()["requests"].toArray();
    _userRequests->setData(data);

    if (data.count() < 20)
        _isAllUserRequests = true;
    else
        _isAllUserRequests = false;

    rep->deleteLater();
}

void ClientController::addGeneralRequests()
{
    auto rep = qobject_cast<QNetworkReply *>(sender());
    if (!rep) return;

    if (!checkReplyError(rep)) {
        rep->deleteLater();
        return;
    }

    auto data = QJsonDocument::fromJson(rep->readAll()).object()["requests"].toArray();
    _generalRequests->addData(data);

    if (data.count() < 20)
        _isAllGeneralRequests = true;
    else
        _isAllGeneralRequests = false;

    rep->deleteLater();
}

void ClientController::addUserRequests()
{
    auto rep = qobject_cast<QNetworkReply *>(sender());
    if (!rep) return;

    if (!checkReplyError(rep)) {
        rep->deleteLater();
        return;
    }

    auto data = QJsonDocument::fromJson(rep->readAll()).object()["requests"].toArray();
    _userRequests->addData(data);

    if (data.count() < 20)
        _isAllUserRequests = true;
    else
        _isAllUserRequests = false;

    rep->deleteLater();
}

void ClientController::setCreationRequestStatus()
{
    auto rep = qobject_cast<QNetworkReply *>(sender());
    if (!rep) return;

    if (!checkReplyError(rep)) {
        rep->deleteLater();
        return;
    } else {
        _creationRequestStatus = "Успешно";
    }

    emit creationRequestStatusChanged();
    loadUserRequests();
    loadGeneralRequests();

    rep->deleteLater();
}

void ClientController::setCanselRequestStatus()
{
    auto rep = qobject_cast<QNetworkReply *>(sender());
    if (!rep) return;

    if (!checkReplyError(rep)) {
        rep->deleteLater();
        return;
    } else {
        _canselRequestStatus = "Успешно";
        loadUserRequests();
    }

    emit canselRequestStatusChanged();
    loadGeneralRequests();

    rep->deleteLater();
}

void ClientController::setRequestDeals()
{
    auto rep = qobject_cast<QNetworkReply *>(sender());
    if (!rep) return;

    if (!checkReplyError(rep)) {
        rep->deleteLater();
        return;
    }

    auto data = QJsonDocument::fromJson(rep->readAll()).object();
    auto reqData = data["request"].toObject();

    _deals->setData(data["deals"].toArray());

    delete _dealsRequest;
    _dealsRequest = new Request{};

    _dealsRequest->id = reqData["request_id"].toInt();
    _dealsRequest->status = reqData["status"].toString();
    _dealsRequest->date = reqData["open_date"].toString();
    _dealsRequest->user_id = reqData["user_id"].toInt();
    _dealsRequest->current_value = reqData["current_value"].toInt();
    _dealsRequest->value = reqData["value"].toInt();
    _dealsRequest->direction = reqData["direction"].toString();
    _dealsRequest->cost = reqData["cost"].toInt();
    _dealsRequest->setUTCDate(reqData["open_date"].toString());

    emit dealsRequestChanged();

    rep->deleteLater();
}

void ClientController::setRegistrationStatus()
{
    auto rep = qobject_cast<QNetworkReply *>(sender());
    if (!rep) return;

    if (!checkReplyError(rep)) {
        rep->deleteLater();
        return;
    }

    auto content = QJsonDocument::fromJson(rep->readAll()).object();

    _registrationStatus = content["message"].toString();
    emit registrationStatusChanged();

    if (content["message"].toString() == "Successful") {
        _user = new UserData{content["user"].toObject(), rep->request().rawHeader("Authorization")};

        emit authorizedChanged();
        emit userDataChanged();

        saveConfig();
    }

    rep->deleteLater();
}

void ClientController::setAuthorizationStatus()
{
    auto rep = qobject_cast<QNetworkReply *>(sender());
    if (!rep) return;

    if (!checkReplyError(rep)) {
        rep->deleteLater();
        return;
    }

    auto content = QJsonDocument::fromJson(rep->readAll()).object();

    _authorizationStatus = content["message"].toString();
    emit authorizationStatusChanged();

    if (content["message"].toString() == "Successful") {
        if (_user)
            delete _user;

        _user = new UserData{content["user"].toObject(), rep->request().rawHeader("Authorization")};

        emit authorizedChanged();
        emit userDataChanged();

        saveConfig();
    }

    rep->deleteLater();
}





QNetworkRequest *ClientController::constructServerPingNetworkRequest(const QString &hostName, const int port) const
{
    QUrl url{QString{"http://%1:%2/ping"}.arg(hostName, QString::number(port))};
    return new QNetworkRequest{url};
}

QNetworkRequest *ClientController::constructLoadGeneralRequestsNetworkRequest(bool isLoadMore) const
{
    QUrlQuery params{{"order", _generalFilters.isOrderByDate ? "date" : "cost"},
                     {"direction", _generalFilters.isBuy ? "buy" : "sale"},
                     {"reverse_sort", _generalFilters.isReverseSort ? "true" : "false"},
                     {"limit_start", isLoadMore ? QString::number(_generalRequests->rowCount()) : "0"},
                     {"limit", "20"}};
    QUrl url{QString{"http://%1:%2"}.arg(_serverHost, QString::number(_serverPort))};
    url.setQuery(params);

    return new QNetworkRequest{url};
}

QNetworkRequest *ClientController::constructLoadUserRequestsNetworkRequest(bool isLoadMore) const
{
    QUrlQuery params{{"order", _userFilters.isOrderByDate ? "date" : "cost"},
                     {"direction", _userFilters.isBuy ? "buy" : "sale"},
                     {"reverse_sort", _userFilters.isReverseSort ? "true" : "false"},
                     {"limit_start", isLoadMore ? QString::number(_userRequests->rowCount()) : "0"},
                     {"limit", "20"}};
    QUrl url{QString{"http://%1:%2/user/requests"}.arg(_serverHost, QString::number(_serverPort))};
    url.setQuery(params);
    auto request = new QNetworkRequest{url};
    request->setRawHeader("Authorization", _user->authHeader);

    return request;
}

QNetworkRequest *ClientController::constructGetRequestDealsNetworkRequest(int requestId) const
{
    QUrlQuery params{{"request_id", QString::number(requestId)}};
    QUrl url{QString{"http://%1:%2/user/requests/info"}.arg(_serverHost, QString::number(_serverPort))};
    url.setQuery(params);
    auto request = new QNetworkRequest{url};
    request->setRawHeader("Authorization", _user->authHeader);

    return request;
}

QNetworkRequest *ClientController::constructCreateRequestNetworkRequest() const
{
    auto request = new QNetworkRequest{QUrl{QString{"http://%1:%2/user/requests/new_request"}
                                               .arg(_serverHost, QString::number(_serverPort))}};
    request->setRawHeader("Content-type", "application/json");
    request->setRawHeader("Authorization", _user->authHeader);

    return request;
}

QNetworkRequest *ClientController::constructCanselRequestNetworkRequest() const
{
    auto request = new QNetworkRequest{QUrl{QString{"http://%1:%2/user/requests/cansel_request"}
                                 .arg(_serverHost, QString::number(_serverPort))}};
    request->setRawHeader("Content-type", "application/json");
    request->setRawHeader("Authorization", _user->authHeader);

    return request;
}

QNetworkRequest *ClientController::constructSignUpNetworkRequest(const QByteArray &authHeader) const
{
    auto request = new QNetworkRequest{QUrl{QString{"http://%1:%2/new_user"}.arg(_serverHost, QString::number(_serverPort))}};
    request->setRawHeader("Content-type", "application/json");
    request->setRawHeader("Authorization", authHeader);

    return request;
}

QNetworkRequest *ClientController::constructSignInNetworkRequest(const QByteArray &authHeader) const
{
    auto request = new QNetworkRequest{QUrl{QString{"http://%1:%2/user"}.arg(_serverHost, QString::number(_serverPort))}};
    request->setRawHeader("Authorization", authHeader);

    return request;
}



bool ClientController::checkReplyError(const QNetworkReply *rep)
{
    if (rep->error()) {
        switch (rep->error()) {
        case QNetworkReply::ConnectionRefusedError:
            _connectionErrorMessage = "Сервер недоступен";
            emit connectionErrorMessageChanged();
            _isServerEnabled = false;
            emit serverEnabledChanged();
            break;
        case QNetworkReply::AuthenticationRequiredError:
            signOut();
            break;
        default:
            qDebug() << rep->errorString();
            break;
        }

        return false;
    }

    return true;
}
