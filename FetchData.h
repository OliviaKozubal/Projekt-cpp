#ifndef FETCHDATA_H
#define FETCHDATA_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QTimeZone>

class FetchData : public QObject
{
    Q_OBJECT
public:
    explicit FetchData(QObject *parent = nullptr);
    void Nastepny();
    void Ostatni();
    void Ranking();
    void Kierowcy();
    void Mistrzowie();

signals:
    void NastepnyPobrano(const QString &NazwaWyscig, const QString &DataCzas, const QString &DataQuali, const QString &Lokalizacja);
    void OstatniPobrano(const QString &NazwaWyscigo, const QString &DataCzaso, const QString &Lokalizacjao, const QString &Top3, const QString &FastestLap, const QString &FastestLapDriver, const QString &DriverOfTheDay);
    void RankingPobrano(const QList<QStringList> &rankings);
    void KierowcyPobrano(const QList<QStringList> &kierowcy);

private slots:
    void onNetworkReply(QNetworkReply *reply);

private:
    QNetworkAccessManager networkManager;
    QList<QStringList> listaKierowcy;
    QList<QStringList> listaMistrzowie;
    int currentYear = QDate::currentDate().year();
    int currentYearo = QDate::currentDate().year();
    int currentYearr = QDate::currentDate().year();
    void NastepnyReply(const QJsonDocument &jsonDoc);
    void OstatniReply(const QJsonDocument &jsonDoc);
    void RankingReply(const QJsonDocument &jsonDoc);
    void KierowcyReply(const QJsonDocument &jsonDoc, const QString &zadanie);
};

#endif // FETCHDATA_H