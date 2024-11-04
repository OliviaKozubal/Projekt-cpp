#ifndef FETCHDATA_H
#define FETCHDATA_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class FetchData : public QObject
{
    Q_OBJECT
public:
    explicit FetchData(QObject *parent = nullptr);
    void Nastepny();
    void Ostatni();
    void Ranking();

signals:
    void Pobrano(const QString &NazwaWyscig, const QString &DataCzas, const QString &Lokalizacja);
    void OstatniPobrano(const QString &NazwaWyscigo, const QString &DataCzaso, const QString &Lokalizacjao,
                        const QString &Top3, const QString &FastestLap, const QString &FastestLapDriver, const QString &DriverOfTheDay);
    void RankingPobrano(const QList<QStringList> &rankings);

private slots:
    void onNetworkReply(QNetworkReply *reply);

private:
    QNetworkAccessManager networkManager;
    void NastepnyReply(const QJsonDocument &jsonDoc);
    void OstatniReply(const QJsonDocument &jsonDoc);
    void RankingReply(const QJsonDocument &jsonDoc);
};

#endif // FETCHDATA_H