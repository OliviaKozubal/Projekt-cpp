#ifndef WYNIKI_H
#define WYNIKI_H

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QJsonObject>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QHeaderView>
#include <QMessageBox>
#include <QDebug>
#include <QCoreApplication>

class Wyniki : public QWidget {
    Q_OBJECT

public:
    explicit Wyniki(QWidget *parent = nullptr);
    void wczytaj(const QString &sciezka);

private slots:
    void filtruj();
    void wyswietl(int r, int c);

private:
    QTableWidget *tablica;
    QLineEdit *wyszukiwarka;
    QJsonObject dane;

    void wypelnij();
    void wyswietlInfo(const QString &year, const QJsonArray &ranking);
};

#endif // WYNIKI_H
