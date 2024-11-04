#include "FetchData.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QTimeZone>
#include <QDebug>

FetchData::FetchData(QObject *parent) : QObject(parent)
{
    connect(&networkManager, &QNetworkAccessManager::finished, this, &FetchData::onNetworkReply);
}

void FetchData::Nastepny()
{
    QUrl url("https://ergast.com/api/f1/current/next.json");
    QNetworkRequest request(url);
    auto reply = networkManager.get(request);
    reply->setProperty("requestType", "Nastepny");
}

void FetchData::Ostatni()
{
    QUrl url("https://ergast.com/api/f1/current/last/results.json");
    QNetworkRequest request(url);
    auto reply = networkManager.get(request);
    reply->setProperty("requestType", "Ostatni");
}

void FetchData::Ranking()
{
    QUrl url("https://ergast.com/api/f1/current/driverStandings.json");
    QNetworkRequest request(url);
    auto reply = networkManager.get(request);
    reply->setProperty("requestType", "Ranking");
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
        else
            qWarning() << "Błąd pobierania danych:" << reply->errorString();
    }
    reply->deleteLater();
}

void FetchData::NastepnyReply(const QJsonDocument &jsonDoc)
{
    QJsonObject rootObj = jsonDoc.object();
    QJsonObject DaneWyscig = rootObj["MRData"].toObject()
                             ["RaceTable"].toObject()
                             ["Races"].toArray().first().toObject();

    QString NazwaWyscig = DaneWyscig["raceName"].toString();
    QString DataWyscig = DaneWyscig["date"].toString();
    QString CzasWyscig = DaneWyscig["time"].toString();
    QString Lokalizacja = DaneWyscig["Circuit"].toObject()["Location"].toObject()["locality"].toString();

    QString DataCzas = DataWyscig + " " + CzasWyscig;
    QDateTime UTC = QDateTime::fromString(DataCzas, Qt::ISODate);
    UTC.setTimeZone(QTimeZone("UTC"));

    QTimeZone PlTimeZone("Europe/Warsaw");
    QDateTime CzasPl = UTC.toTimeZone(PlTimeZone);
        
    DataCzas = CzasPl.toString("yyyy-MM-dd HH:mm:ss");

    emit Pobrano(NazwaWyscig, DataCzas, Lokalizacja);
}

void FetchData::OstatniReply(const QJsonDocument &jsonDoc)
{
    QJsonObject rootObj = jsonDoc.object();
    QJsonObject DaneWyscig = rootObj["MRData"].toObject()
                             ["RaceTable"].toObject()
                             ["Races"].toArray().first().toObject();

    QString NazwaWyscig = DaneWyscig["raceName"].toString();
    QString DataWyscig = DaneWyscig["date"].toString();
    QString CzasWyscig = DaneWyscig["time"].toString();
    QString Lokalizacja = DaneWyscig["Circuit"].toObject()["Location"].toObject()["locality"].toString();

    QJsonArray Wyniki = DaneWyscig["Results"].toArray();
    QString Top3;
    for (int i = 0; i < 3; ++i) {
        Top3 += Wyniki[i].toObject()["Driver"].toObject()["givenName"].toString() + " " + Wyniki[i].toObject()["Driver"].toObject()["familyName"].toString() + ", ";
    }

    QString FastestLap = Wyniki.first().toObject()["FastestLap"].toObject()["Time"].toObject()["time"].toString();
    QString FastestLapDriver = Wyniki.first().toObject()["FastestLap"].toObject()["Driver"].toObject()["familyName"].toString();
    QString DriverOfTheDay = Wyniki.first().toObject()["Driver"].toObject()["familyName"].toString();

    QString DataCzas = DataWyscig + " " + CzasWyscig;
    QDateTime UTC = QDateTime::fromString(DataCzas, Qt::ISODate);
    UTC.setTimeZone(QTimeZone("UTC"));

    QTimeZone PlTimeZone("Europe/Warsaw");
    QDateTime CzasPl = UTC.toTimeZone(PlTimeZone);
        
    DataCzas = CzasPl.toString("yyyy-MM-dd HH:mm:ss");

    emit OstatniPobrano(NazwaWyscig, DataCzas, Lokalizacja, Top3, FastestLap, FastestLapDriver, DriverOfTheDay);
}

void FetchData::RankingReply(const QJsonDocument &jsonDoc)
{
    QJsonObject rootObj = jsonDoc.object();
    QJsonArray standings = rootObj["MRData"].toObject()["StandingsTable"].toObject()["StandingsLists"].toArray().first().toObject()["DriverStandings"].toArray();

    QList<QStringList> rankings;

    for (const auto &item : standings) {
        QJsonObject driverObj = item.toObject();
        QString position = driverObj["position"].toString();
        QString driverName = driverObj["Driver"].toObject()["givenName"].toString() + " " + driverObj["Driver"].toObject()["familyName"].toString();
        QString nationality = driverObj["Driver"].toObject()["nationality"].toString();
        QString team = driverObj["Constructors"].toArray().first().toObject()["name"].toString();
        QString points = driverObj["points"].toString();

        rankings.append(QStringList() << position << driverName << nationality << team << points);
    }

    emit RankingPobrano(rankings);
}