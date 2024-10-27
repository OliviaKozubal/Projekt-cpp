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

signals:
    void Pobrano(const QString &NazwaWyscig, const QString &DataCzas, const QString &Lokalizacja);

private slots:
    void onNetworkReply(QNetworkReply *reply);

private:
    QNetworkAccessManager networkManager;
};

#endif // FETCHDATA_H