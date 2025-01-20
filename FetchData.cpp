#include "FetchData.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QTimeZone>

FetchData::FetchData(QObject *parent) : QObject(parent)
{
    connect(&networkManager, &QNetworkAccessManager::finished, this, &FetchData::onNetworkReply);
}

void FetchData::Nastepny()
{
    QUrl url("https://api.jolpi.ca/ergast/f1/current/next.json");
    QNetworkRequest request(url);
    auto reply = networkManager.get(request);
    reply->setProperty("requestType", "Nastepny");
}

void FetchData::Ostatni()
{
    QUrl url(QString("https://api.jolpi.ca/ergast/f1/%1/last/results.json").arg(currentYearo));
    QNetworkRequest request(url);
    auto reply = networkManager.get(request);
    reply->setProperty("requestType", "Ostatni");
}

void FetchData::Ranking()
{
    QUrl url(QString("https://api.jolpi.ca/ergast/f1/%1/driverStandings.json").arg(currentYearr));
    QNetworkRequest request(url);
    auto reply = networkManager.get(request);
    reply->setProperty("requestType", "Ranking");
}

void FetchData::Kierowcy() // dane zapisane do pliku 
{
    const int limit = 100;
    int offset = 0;

    QUrl url(QString("https://api.jolpi.ca/ergast/f1/drivers.json?limit=%1&offset=%2").arg(limit).arg(offset));
    QNetworkRequest request(url);
    auto reply = networkManager.get(request);

    reply->setProperty("requestType", QString("Kierowcy:%1:%2").arg(limit).arg(offset));
    listaKierowcy.clear();
}

void FetchData::Mistrzowie() // dane zapisane do pliku
{
    for (int year = 1950; year <= QDate::currentDate().year(); ++year) {
        QUrl url(QString("https://api.jolpi.ca/ergast/api/f1/%1/driverStandings.json").arg(year));
        QNetworkRequest request(url);
        auto reply = networkManager.get(request);
        reply->setProperty("requestType", QString("Mistrzowie:%1").arg(year));
    }
}

void FetchData::onNetworkReply(QNetworkReply *reply)
{
    QString zadanie = reply->property("requestType").toString();

    if (reply->error() == QNetworkReply::NoError) 
    {
        QByteArray Dane = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(Dane);

        if (zadanie == "Nastepny")
            NastepnyReply(jsonDoc);
        else if (zadanie == "Ostatni")
            OstatniReply(jsonDoc);
        else if (zadanie == "Ranking")
            RankingReply(jsonDoc);
        else if (zadanie.startsWith("Kierowcy"))
            KierowcyReply(jsonDoc, zadanie);
        //else if (zadanie == "Sezony")
            //SezonyReply(jsonDoc);
       // else if (zadanie.startsWith("MistrzowieKonstruktorzy:")) {
            //QString rok = zadanie.split(":")[1];
            //MistrzowieKonstruktorzyReply(jsonDoc, rok);
        //}
        else
            qWarning() << "Błąd pobierania danych:" << reply->errorString();
    }
    reply->deleteLater();
}

void FetchData::NastepnyReply(const QJsonDocument &jsonDoc)
{
    QJsonObject rootObj = jsonDoc.object();
    QJsonObject DaneWyscig = rootObj["MRData"].toObject()["RaceTable"].toObject()["Races"].toArray().first().toObject();

    QString NazwaWyscig = DaneWyscig["raceName"].toString();
    QString DataCzas = DaneWyscig["date"].toString() + " " + DaneWyscig["time"].toString();;
    QString Lokalizacja = DaneWyscig["Circuit"].toObject()["Location"].toObject()["locality"].toString();
    QString DataQuali = DaneWyscig["Qualifying"].toObject()["date"].toString() + " " + DaneWyscig["Qualifying"].toObject()["time"].toString();
    QDateTime UTCw = QDateTime::fromString(DataCzas, Qt::ISODate);
    QDateTime UTCq = QDateTime::fromString(DataQuali, Qt::ISODate);
    UTCw.setTimeZone(QTimeZone("UTC"));
    UTCq.setTimeZone(QTimeZone("UTC"));

    QTimeZone PlTimeZone("Europe/Warsaw");
    QDateTime CzasPlw = UTCw.toTimeZone(PlTimeZone);
    QDateTime CzasPlq = UTCq.toTimeZone(PlTimeZone);

    DataCzas = CzasPlw.toString("yyyy-MM-dd HH:mm:ss");
    DataQuali = CzasPlq.toString("yyyy-MM-dd HH:mm:ss");
    emit NastepnyPobrano(NazwaWyscig, DataCzas, DataQuali, Lokalizacja);
}

void FetchData::OstatniReply(const QJsonDocument &jsonDoc)
{
    QJsonObject rootObj = jsonDoc.object();
    QJsonObject DaneWyscig = rootObj["MRData"].toObject()["RaceTable"].toObject()["Races"].toArray().first().toObject();

    QString NazwaWyscig = DaneWyscig["raceName"].toString();
    QString DataCzas = DaneWyscig["date"].toString() + " " + DaneWyscig["time"].toString();
    QString Lokalizacja = DaneWyscig["Circuit"].toObject()["Location"].toObject()["locality"].toString();
    QJsonArray Wyniki = DaneWyscig["Results"].toArray();
    
    if (Wyniki.isEmpty())
    {
        currentYearo--;
        QUrl nextUrl(QString("https://api.jolpi.ca/ergast/f1/%1/last/results.json").arg(currentYearo));
        QNetworkRequest nextRequest(nextUrl);
        auto nextReply = networkManager.get(nextRequest);
        nextReply->setProperty("requestType", "Ostatni");
    }
    else
    {
    QString Top3;
    for (int i = 0; i < 3; ++i) {
        Top3 += Wyniki[i].toObject()["Driver"].toObject()["givenName"].toString() + " " + Wyniki[i].toObject()["Driver"].toObject()["familyName"].toString() + ", ";
    }
    QString FastestLap;
    QString FastestLapDriver;

    for (const auto &result : Wyniki) 
    {
        QJsonObject resultObject = result.toObject();
        if (resultObject.contains("FastestLap")) 
        {
            FastestLap = resultObject["FastestLap"].toObject()["Time"].toObject()["time"].toString();
            QJsonObject driverObject = resultObject["Driver"].toObject();
            FastestLapDriver = driverObject["givenName"].toString() + " " + driverObject["familyName"].toString();
            break;
        }
    }

    QString DriverOfTheDay = Wyniki.first().toObject()["Driver"].toObject()["givenName"].toString() + " " + Wyniki.first().toObject()["Driver"].toObject()["familyName"].toString();

    QDateTime UTC = QDateTime::fromString(DataCzas, Qt::ISODate);
    UTC.setTimeZone(QTimeZone("UTC"));

    QTimeZone PlTimeZone("Europe/Warsaw");
    QDateTime CzasPl = UTC.toTimeZone(PlTimeZone);
        
    DataCzas = CzasPl.toString("yyyy-MM-dd HH:mm:ss");

    emit OstatniPobrano(NazwaWyscig, DataCzas, Lokalizacja, Top3, FastestLap, FastestLapDriver, DriverOfTheDay);
    }
}

void FetchData::RankingReply(const QJsonDocument &jsonDoc)
{
    QJsonObject rootObj = jsonDoc.object();
    QJsonArray standings = rootObj["MRData"].toObject()["StandingsTable"].toObject()["StandingsLists"].toArray().first().toObject()["DriverStandings"].toArray();

    if (standings.isEmpty())
    {
        currentYearr--;
        QUrl nextUrl(QString("https://api.jolpi.ca/ergast/f1/%1/driverStandings.json").arg(currentYearr));
        QNetworkRequest nextRequest(nextUrl);
        auto nextReply = networkManager.get(nextRequest);
        nextReply->setProperty("requestType", "Ranking");
    }
    else
    {
    QList<QStringList> ranking;

    for (const auto &item : standings) {
        QJsonObject driverObj = item.toObject();
        QString position = driverObj["position"].toString();
        QString driverName = driverObj["Driver"].toObject()["givenName"].toString() + " " + driverObj["Driver"].toObject()["familyName"].toString();
        QString nationality = driverObj["Driver"].toObject()["nationality"].toString();
        QString team = driverObj["Constructors"].toArray().first().toObject()["name"].toString();
        QString points = driverObj["points"].toString();

        ranking.append({position, driverName, nationality, team, points});
    }

    emit RankingPobrano(ranking);
    }
}

void FetchData::KierowcyReply(const QJsonDocument &jsonDoc, const QString &zadanie)
{
    QStringList kroki = zadanie.split(":");
    int limit = kroki[1].toInt();
    int offset = kroki[2].toInt();
    QJsonArray daneKierowcy = jsonDoc.object()["MRData"].toObject()["DriverTable"].toObject()["Drivers"].toArray();

    for (const auto &driver : daneKierowcy) {
        QJsonObject driverObj = driver.toObject();
        QString driverId = driverObj["driverId"].toString();
        QString name = driverObj["givenName"].toString() + " " + driverObj["familyName"].toString();
        QString nationality = driverObj["nationality"].toString();
        
        listaKierowcy.append({name, nationality, driverId});
    }
    if (daneKierowcy.size() == limit) {
        offset += limit;
        QUrl nextUrl(QString("https://api.jolpi.ca/ergast/f1/drivers.json?limit=%1&offset=%2").arg(limit).arg(offset));
        QNetworkRequest nextRequest(nextUrl);
        auto nextReply = networkManager.get(nextRequest);
        nextReply->setProperty("requestType", QString("Kierowcy:%1:%2").arg(limit).arg(offset));
    } else
        emit KierowcyPobrano(listaKierowcy);
}