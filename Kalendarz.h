// plik inicjalizujący funkcje i zmienne wykorzystywane w kalendarzu
#ifndef KALENDARZ_H
#define KALENDARZ_H

#include <QApplication>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QDate>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QMap>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>

class Kalendarz : public QWidget {
    Q_OBJECT

public:
    explicit Kalendarz(QWidget *parent = nullptr);
    void wczytajDaty(const QString &filePath);

private slots:
    void poprzMonth();
    void nastMonth();

private:
    void update();
    QString etykieta(const QDate &date);

    QLabel *miesLabel;
    QGridLayout *kalendarzLayout;
    QDate Data;
    QMap<QDate, QStringList> wydarzenia;
};

#endif // KALENDARZ_H