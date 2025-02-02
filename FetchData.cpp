// plik z funkcjami pobierania i zwracania w odpowiedniej formie danych z api.jolpi.ca/ergast + funkcja konwertująca czas z api na polski
#include "FetchData.h"

FetchData::FetchData(QObject *parent) : QObject(parent)
{
    connect(&networkManager, &QNetworkAccessManager::finished, this, &FetchData::onNetworkReply);
}

void FetchData::Nastepny() // wczytaj dane na temat następnego wyścigu 
{
    QUrl url("https://api.jolpi.ca/ergast/f1/current/next.json"); // endpoint 
    QNetworkRequest request(url);
    auto reply = networkManager.get(request);
    reply->setProperty("zadanie", "Nastepny"); // precyzujemy typ zapytania
}

void FetchData::Ostatni() // wczytaj dane na temat ostatniego wyścigu
{
    QUrl url(QString("https://api.jolpi.ca/ergast/f1/%1/last/results.json").arg(aktualnyRok)); // argumentem jest aktualny/poprzedni rok
    QNetworkRequest request(url);
    auto reply = networkManager.get(request);
    reply->setProperty("zadanie", "Ostatni");
}

void FetchData::Ranking() // wczytuje ranking aktualnego lub poprzedniego sezonu (w przypadku gdy baza nie zawiera danych na aktualny rok)
{
    QUrl url(QString("https://api.jolpi.ca/ergast/f1/%1/driverStandings.json").arg(aktualnyRok)); 
    QNetworkRequest request(url);
    auto reply = networkManager.get(request);
    reply->setProperty("zadanie", "Ranking");
}

void FetchData::onNetworkReply(QNetworkReply *reply)
{
    QString zadanie = reply->property("zadanie").toString();

    if (reply->error() == QNetworkReply::NoError) 
    {
        QByteArray Dane = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(Dane);

        if (zadanie == "Nastepny") // sprawdzenie jakie zapytanie zostało przekazane, wywołanie odpowiedniej funkcji
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

void FetchData::NastepnyReply(const QJsonDocument &jsonDoc) // funkcja zwracająca nazwę, lokalizację, datę oraz czas wyścigu oraz kwalifikacji, następnego race weekend
{
    QJsonObject rootObj = jsonDoc.object();
    QJsonObject DaneWyscig = rootObj["MRData"].toObject()["RaceTable"].toObject()["Races"].toArray().first().toObject(); // po analizie zawartości endpointa, ustalono ścieżkę do potrzebnych danych

    QString NazwaWyscig = DaneWyscig["raceName"].toString();
    QString Lokalizacja = DaneWyscig["Circuit"].toObject()["Location"].toObject()["locality"].toString();
    QString DataCzas = czasPolski(DaneWyscig["date"].toString(), DaneWyscig["time"].toString());
    QString DataQuali = czasPolski(DaneWyscig["Qualifying"].toObject()["date"].toString(), DaneWyscig["Qualifying"].toObject()["time"].toString());

    // emitujemy sygnał oznaczający zakończenie pobierania potrzebnych danych
    emit NastepnyPobrano(NazwaWyscig, DataCzas, DataQuali, Lokalizacja);
}

void FetchData::OstatniReply(const QJsonDocument &jsonDoc) // funkcja zwraca nazwę, datę i czas, lokalizację, top 3, czas najszybszego okrążenia oraz jego wykonawcę i kierowcę dnia z ostatniego wyścigu
{
    QJsonObject rootObj = jsonDoc.object();
    QJsonObject DaneWyscig = rootObj["MRData"].toObject()["RaceTable"].toObject()["Races"].toArray().first().toObject();
    QJsonArray Wyniki = DaneWyscig["Results"].toArray();
    
    if (Wyniki.isEmpty()) // jeżeli pobrane dane z ostaniego wyścigu aktualnego roku są puste, pobiera dane z ostatniego wyścigu poprzedniego roku
    {
        int poprzedni = aktualnyRok - 1;
        QUrl nextUrl(QString("https://api.jolpi.ca/ergast/f1/%1/last/results.json").arg(poprzedni));
        QNetworkRequest nextRequest(nextUrl);
        auto nextReply = networkManager.get(nextRequest);
        nextReply->setProperty("zadanie", "Ostatni");
    }
    else
    {
        QString NazwaWyscig = DaneWyscig["raceName"].toString();
        QString DataCzas = czasPolski(DaneWyscig["date"].toString(), DaneWyscig["time"].toString());
        QString Lokalizacja = DaneWyscig["Circuit"].toObject()["Location"].toObject()["locality"].toString();
        
        QString Top3;
        for (int i = 0; i < 3; ++i) // zwraca imiona i nazwiska kierowców którzy stanęli na podium
            Top3 += Wyniki[i].toObject()["Driver"].toObject()["givenName"].toString() + " " + Wyniki[i].toObject()["Driver"].toObject()["familyName"].toString() + ", ";

    QString FastestLap;
    QString FastestLapDriver;

    for (const auto &w : Wyniki) // przeszukujemy wyniki w poszukiwaniu czasu najszybszego okrążenia
    {
        QJsonObject dane = w.toObject();
        QJsonObject najszybsze = dane["FastestLap"].toObject();

        if (najszybsze["rank"].toString() == "1") // jeżeli rank == 1 oznacza to, że ten kierowca wykonał najszybsze okrążenie
        {
            FastestLap = najszybsze["Time"].toObject()["time"].toString();
            FastestLapDriver = dane["Driver"].toObject()["givenName"].toString() + " " + dane["Driver"].toObject()["familyName"].toString();
            break;
        }
    }

    emit OstatniPobrano(NazwaWyscig, DataCzas, Lokalizacja, Top3, FastestLap, FastestLapDriver);
    }
}

void FetchData::RankingReply(const QJsonDocument &jsonDoc) // funkcja zwraca aktualny/zeszłoroczny ranking (zwracane dane dla kierowcy: pozycja, imię i nazwisko, narodowość, zespół i punkty)
{
    QJsonObject rootObj = jsonDoc.object();
    QJsonArray pozycje = rootObj["MRData"].toObject()["StandingsTable"].toObject()["StandingsLists"].toArray().first().toObject()["DriverStandings"].toArray();

    if (pozycje.isEmpty()) // jeżeli ranking aktualnego sezonu jest pusty, bierze finalny ranking poprzedniego sezonu
    {
        int poprzedni = aktualnyRok - 1;
        QUrl nextUrl(QString("https://api.jolpi.ca/ergast/f1/%1/driverStandings.json").arg(poprzedni));
        QNetworkRequest nextRequest(nextUrl);
        auto nextReply = networkManager.get(nextRequest);
        nextReply->setProperty("zadanie", "Ranking");
    }
    else
    {
    QList<QStringList> ranking;

    for (const auto &item : pozycje) {
        QJsonObject driverObj = item.toObject();
        QString pozycja = driverObj["position"].toString();
        QString imieNazwisko = driverObj["Driver"].toObject()["givenName"].toString() + " " + driverObj["Driver"].toObject()["familyName"].toString();
        QString narodowosc = driverObj["Driver"].toObject()["nationality"].toString();
        QString zespol = driverObj["Constructors"].toArray().first().toObject()["name"].toString();
        QString punkty = driverObj["points"].toString();

        ranking.append({pozycja, imieNazwisko, narodowosc, zespol, punkty});
    }

    emit RankingPobrano(ranking);
    }
}

QString FetchData::czasPolski(const QString &dataUTC, const QString &czasUTC) // funkcja konwertująca czas z api na czas polski
{
    QDateTime plCzas = QDateTime::fromString(dataUTC + "T" + czasUTC, Qt::ISODate);
    plCzas.setTimeZone(QTimeZone("UTC"));
    return plCzas.toTimeZone(PL).toString("yyyy-MM-dd HH:mm:ss");
}