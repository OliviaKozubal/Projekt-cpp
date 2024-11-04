#include "Glowna.h"
#include "FetchData.h"
#include <QTimer>
#include <QDateTime>
#include <QHeaderView>

Glowna::Glowna(QWidget *parent) : QWidget(parent) {
    
    QGridLayout *layout = new QGridLayout(this);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(10);
    layout->setColumnStretch(0, 1);
    layout->setColumnStretch(1, 1);
    this->setStyleSheet("QLabel { font-size: 14px; }");
    QLabel *NazwaWyscigLabel = new QLabel(this);
    QLabel *DataWyscigLabel = new QLabel(this);
    QLabel *LokalizacjaLabel = new QLabel(this);
    QLabel *LicznikLabel = new QLabel(this);

    QVBoxLayout *infoLayout = new QVBoxLayout();
    infoLayout->setContentsMargins(10, 10, 10, 10);
    infoLayout->setSpacing(5);
    infoLayout->addWidget(NazwaWyscigLabel);
    infoLayout->addWidget(DataWyscigLabel);
    infoLayout->addWidget(LokalizacjaLabel);
    infoLayout->addWidget(LicznikLabel);

    QWidget *Nastepnyw = new QWidget(this);
    Nastepnyw->setLayout(infoLayout);
    Nastepnyw->setStyleSheet("border: 1px solid gray; padding: 10px; border-radius: 5px;");

    layout->addWidget(Nastepnyw);

    // Ustawienia dla ostatniego wyścigu
    QLabel *NazwaOstatniLabel = new QLabel(this);
    QLabel *DataOstatniLabel = new QLabel(this);
    QLabel *LokalizacjaOstatniLabel = new QLabel(this);
    QLabel *Top3Label = new QLabel(this);
    QLabel *FastestLapLabel = new QLabel(this);
    QLabel *DriverOfTheDayLabel = new QLabel(this);

    QVBoxLayout *wynikiLayout = new QVBoxLayout();
    wynikiLayout->setContentsMargins(10, 10, 10, 10);
    wynikiLayout->setSpacing(5);
    wynikiLayout->addWidget(NazwaOstatniLabel);
    wynikiLayout->addWidget(DataOstatniLabel);
    wynikiLayout->addWidget(LokalizacjaOstatniLabel);
    wynikiLayout->addWidget(Top3Label);
    wynikiLayout->addWidget(FastestLapLabel);
    wynikiLayout->addWidget(DriverOfTheDayLabel);

    QWidget *Ostatniw = new QWidget(this);
    Ostatniw->setLayout(wynikiLayout);
    Ostatniw->setStyleSheet("border: 1px solid gray; padding: 10px; border-radius: 5px; ");

    FetchData *fetch = new FetchData(this);

    connect(fetch, &FetchData::Pobrano, this, [=](const QString &NazwaWyscig, const QString &DataCzas, const QString &Lokalizacja) {
        NazwaWyscigLabel->setText("Nazwa wyścigu: <b>" + NazwaWyscig + "</b>");
        DataWyscigLabel->setText("Data i czas wyścigu: <b>" + DataCzas + "</b>");
        LokalizacjaLabel->setText("Lokalizacja: <b>" + Lokalizacja + "</b>");

        // Ustawienie daty i godziny wyścigu
        QDateTime Czas = QDateTime::fromString(DataCzas, "yyyy-MM-dd HH:mm:ss");
        QTimer *timer = new QTimer(this);

        auto OdswierzTimer = [LicznikLabel, Czas, timer]() {
            QDateTime aktualny = QDateTime::currentDateTime();
            int CzasS = aktualny.secsTo(Czas);
            if (CzasS >= 0) {
                int h = CzasS / 3600;
                int m = (CzasS % 3600) / 60;
                int s = CzasS % 60;
                LicznikLabel->setText(QString("Czas do wyścigu: <b>%1:%2:%3</b>")
                    .arg(h, 2, 10, QLatin1Char('0'))
                    .arg(m, 2, 10, QLatin1Char('0'))
                    .arg(s, 2, 10, QLatin1Char('0')));
            } else {
                LicznikLabel->setText("Wyścig już się odbył!");
                timer->stop();
            }
        };

        OdswierzTimer();
        connect(timer, &QTimer::timeout, OdswierzTimer);

        timer->start(1000);
    });
    
    connect(fetch, &FetchData::OstatniPobrano, this, [=](const QString &NazwaWyscigo, const QString &DataCzaso, const QString &Lokalizacjao, const QString &Top3, const QString &FastestLap, const QString &FastestLapDriver, const QString &DriverOfTheDay) {
        NazwaOstatniLabel->setText("Nazwa wyścigu: <b>" + NazwaWyscigo + "</b>");
        DataOstatniLabel->setText("Data i czas ostatniego wyścigu: <b>" + DataCzaso + "</b>");
        LokalizacjaOstatniLabel->setText("Lokalizacja: <b>" + Lokalizacjao + "</b>");
        Top3Label->setText("Top 3: <b>" + Top3 + "</b>");
        FastestLapLabel->setText("Najszybsze okrążenie: <b>" + FastestLap + "</b> wykonane przez: <b>" + FastestLapDriver + "</b>");
        DriverOfTheDayLabel->setText("Driver of the day: <b>" + DriverOfTheDay + "</b>");
    });

    QTableWidget *rankingTable = new QTableWidget(this);
   
    rankingTable->setColumnCount(5);
    rankingTable->setHorizontalHeaderLabels({"Pozycja", "Imię i nazwisko", "Narodowość", "Zespół", "Punkty"});
    rankingTable->setStyleSheet("QTableWidget { font-size: 16px; }");
    rankingTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    rankingTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QFont headerFont = rankingTable->horizontalHeader()->font();
    headerFont.setPointSize(12);
    rankingTable->horizontalHeader()->setFont(headerFont);

    headerFont = rankingTable->verticalHeader()->font();
    headerFont.setPointSize(12);
    rankingTable->verticalHeader()->setFont(headerFont);

    connect(fetch, &FetchData::RankingPobrano, this, [=](const QList<QStringList> &rankings) {
        rankingTable->setRowCount(rankings.size());
        for (int i = 0; i < rankings.size(); ++i) {
            for (int j = 0; j < rankings[i].size(); ++j) {
                QTableWidgetItem *item = new QTableWidgetItem(rankings[i][j]);
                item->setTextAlignment(Qt::AlignCenter);
                rankingTable->setItem(i, j, item);
            }
        }
    });

    fetch->Nastepny();
    fetch->Ostatni();
    fetch->Ranking();

    layout->addWidget(Nastepnyw, 0, 0); 
    layout->addWidget(Ostatniw, 1, 0);
    layout->addWidget(rankingTable, 0, 1, 2, 1);

    setLayout(layout);
}
