// Plik zawierający definicje funkcji oraz stałych wykorzystywanych do pobierania danych z bazy api.jolpi.ca/ergast
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

signals:
    void NastepnyPobrano(const QString &NazwaWyscig, const QString &DataCzas, const QString &DataQuali, const QString &Lokalizacja);
    void OstatniPobrano(const QString &NazwaWyscigo, const QString &DataCzaso, const QString &Lokalizacjao, const QString &Top3, const QString &FastestLap, const QString &FastestLapDriver);
    void RankingPobrano(const QList<QStringList> &ranking);

private slots:
    void onNetworkReply(QNetworkReply *reply);

private:
    QNetworkAccessManager networkManager;
    int aktualnyRok = QDate::currentDate().year();
    QTimeZone PL{"Europe/Warsaw"};
    void NastepnyReply(const QJsonDocument &jsonDoc);
    void OstatniReply(const QJsonDocument &jsonDoc);
    void RankingReply(const QJsonDocument &jsonDoc);
    QString czasPolski(const QString &dataUTC, const QString &czasUTC);
};

#endif // FETCHDATA_H