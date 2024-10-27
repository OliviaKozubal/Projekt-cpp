#include "Glowna.h"
#include "FetchData.h"
#include <QTimer>
#include <QDateTime>

Glowna::Glowna(QWidget *parent) : QWidget(parent) {
    QGridLayout *layout = new QGridLayout(this);

    QLabel *NazwaWyscigLabel = new QLabel(this);
    QLabel *DataWyscigLabel = new QLabel(this);
    QLabel *LokalizacjaLabel = new QLabel(this);
    QLabel *LicznikLabel = new QLabel(this);

    QVBoxLayout *infoLayout = new QVBoxLayout();
    infoLayout->addWidget(NazwaWyscigLabel);
    infoLayout->addWidget(DataWyscigLabel);
    infoLayout->addWidget(LokalizacjaLabel);
    infoLayout->addWidget(LicznikLabel);

    QWidget *Nastepnyw = new QWidget(this);
    Nastepnyw->setLayout(infoLayout);
    layout->addWidget(Nastepnyw);

    FetchData *fetch = new FetchData(this);

    connect(fetch, &FetchData::Pobrano, this, [=](const QString &NazwaWyscig, const QString &DataCzas, const QString &Lokalizacja) {
        NazwaWyscigLabel->setText("Nazwa wyścigu: " + NazwaWyscig);
        DataWyscigLabel->setText("Data i czas wyścigu: " + DataCzas);
        LokalizacjaLabel->setText("Lokalizacja: " + Lokalizacja);

        QDateTime Czas = QDateTime::fromString(DataCzas, "yyyy-MM-dd HH:mm:ss");

        QTimer *timer = new QTimer(this);
        
        connect(timer, &QTimer::timeout, [LicznikLabel, Czas, timer]() { // problem z licznikiem
            QDateTime aktualny = QDateTime::currentDateTime();
            int secondsLeft = aktualny.secsTo(Czas);
            if (secondsLeft >= 0) {
                int hours = secondsLeft / 3600;
                int minutes = (secondsLeft % 3600) / 60;
                int seconds = secondsLeft % 60;
                LicznikLabel->setText(QString("Czas do wyścigu: %1:%2:%3")
                    .arg(hours, 2, 10, QLatin1Char('0'))
                    .arg(minutes, 2, 10, QLatin1Char('0'))
                    .arg(seconds, 2, 10, QLatin1Char('0')));
            } else {
                LicznikLabel->setText("Wyścig już się odbył!");
                timer->stop();
            }
        });

        timer->start(1000);
    });


    fetch->Nastepny();

    QLabel *Ostatni = new QLabel("Widget z wynikami z ostatniego wyścigu", this);
    QLabel *Ranking = new QLabel("Ranking kierowców ", this);

    layout->addWidget(Nastepnyw, 0, 0); 
    layout->addWidget(Ostatni, 1, 0);
    layout->addWidget(Ranking, 0, 1, 2, 1);

    setLayout(layout);
}