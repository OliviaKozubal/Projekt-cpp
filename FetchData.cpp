#include "FetchData.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QDebug>

FetchData::FetchData(QObject *parent) : QObject(parent)
{
    connect(&networkManager, &QNetworkAccessManager::finished, this, &FetchData::onNetworkReply);
}

void FetchData::Nastepny()
{
    QUrl url("https://ergast.com/api/f1/current/next.json");
    QNetworkRequest request(url);
    networkManager.get(request);
}

void FetchData::onNetworkReply(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray Dane = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(Dane);
        QJsonObject rootObj = jsonDoc.object();
        
        QJsonObject DaneWyscig = rootObj["MRData"].toObject()
                                 ["RaceTable"].toObject()
                                 ["Races"].toArray().first().toObject();
        
        QString NazwaWyscig = DaneWyscig["raceName"].toString();
        QString DataWyscig = DaneWyscig["date"].toString();  
        QString CzasWyscig = DaneWyscig["time"].toString(); 
        QString Lokalizacja = DaneWyscig["Circuit"].toObject()["Location"].toObject()["locality"].toString();

        if (CzasWyscig.isEmpty()) {
            CzasWyscig = "22:00:00";
        }

         QString DataCzas = DataWyscig + " " + CzasWyscig;

        emit Pobrano(NazwaWyscig, DataCzas, Lokalizacja);
    } else {
        qWarning() << "Błąd pobierania danych:" << reply->errorString();
    }

    reply->deleteLater();
}