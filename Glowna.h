// plik zawierający definicje funkcji i zmiennych używanych na stronie głównej
#ifndef GLOWNA_H
#define GLOWNA_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QTimer>
#include <QDateTime>
#include <QHeaderView>

class Glowna : public QWidget {
    Q_OBJECT

public:
    Glowna(QWidget *parent = nullptr);

private:
    QLabel *NazwaWyscigLabel;
    QLabel *DataWyscigLabel;
    QLabel *DataQualiLabel;
    QLabel *LokalizacjaLabel;
    QLabel *LicznikLabel;
    QLabel *NazwaOstatniLabel;
    QLabel *DataOstatniLabel;
    QLabel *LokalizacjaOstatniLabel;
    QLabel *Top3Label;
    QLabel *FastestLapLabel;
    QTableWidget *tablicaRanking;
    QTimer *timer;
    void odswierzTimer(QLabel *LicznikLabel, const QDateTime &Czas);
};

#endif // GLOWNA_H