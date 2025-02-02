// plik tworzący stronę główną, składa się z rankingu i danych na temat następnego i ostatniego wyścigu
#include "Glowna.h"
#include "FetchData.h"

Glowna::Glowna(QWidget *parent) : QWidget(parent) {
    
    // inicjalizacja potrzebnych etykiet, tabeli oraz timera
    NazwaWyscigLabel = new QLabel(this);
    DataWyscigLabel = new QLabel(this);
    DataQualiLabel = new QLabel(this);
    LokalizacjaLabel = new QLabel(this);
    LicznikLabel = new QLabel(this);
    NazwaOstatniLabel = new QLabel(this);
    DataOstatniLabel = new QLabel(this);
    LokalizacjaOstatniLabel = new QLabel(this);
    Top3Label = new QLabel(this);
    FastestLapLabel = new QLabel(this);
    tablicaRanking = new QTableWidget(this);
    timer = new QTimer(this);

    // layout główny
    QGridLayout *layout = new QGridLayout(this);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(10);
    layout->setColumnStretch(0, 1);
    layout->setColumnStretch(1, 1);
    this->setStyleSheet("QLabel { font-size: 14px; }");

    // layout następny wyścig
    QVBoxLayout *nastepnyLayout = new QVBoxLayout();
    nastepnyLayout->setContentsMargins(10, 10, 10, 10);
    nastepnyLayout->setSpacing(5);
    nastepnyLayout->addWidget(NazwaWyscigLabel);
    nastepnyLayout->addWidget(DataQualiLabel);
    nastepnyLayout->addWidget(DataWyscigLabel);
    nastepnyLayout->addWidget(LokalizacjaLabel);
    nastepnyLayout->addWidget(LicznikLabel);

    // tworzymy widget następnego wyścigu
    QWidget *Nastepnyw = new QWidget(this);
    Nastepnyw->setLayout(nastepnyLayout);
    Nastepnyw->setStyleSheet("border: 1px solid gray; padding: 10px; border-radius: 5px;");

    // layout ostatni wyścig
    QVBoxLayout *wynikiLayout = new QVBoxLayout();
    wynikiLayout->setContentsMargins(10, 10, 10, 10);
    wynikiLayout->setSpacing(5);
    wynikiLayout->addWidget(NazwaOstatniLabel);
    wynikiLayout->addWidget(DataOstatniLabel);
    wynikiLayout->addWidget(LokalizacjaOstatniLabel);
    wynikiLayout->addWidget(Top3Label);
    wynikiLayout->addWidget(FastestLapLabel);

    // tworzymy widget ostatniego wyścigu
    QWidget *Ostatniw = new QWidget(this);
    Ostatniw->setLayout(wynikiLayout);
    Ostatniw->setStyleSheet("border: 1px solid gray; padding: 10px; border-radius: 5px; ");

    // edycja tablicy z rankingiem
    tablicaRanking->setColumnCount(5);
    tablicaRanking->setHorizontalHeaderLabels({"Pozycja", "Imię i nazwisko", "Narodowość", "Zespół", "Punkty"});
    tablicaRanking->setStyleSheet("QTableWidget { font-size: 16px; }");
    tablicaRanking->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tablicaRanking->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QFont headerFont = tablicaRanking->font();
    headerFont.setPointSize(12);
    tablicaRanking->horizontalHeader()->setFont(headerFont);
    tablicaRanking->verticalHeader()->setFont(headerFont);

    FetchData *fetch = new FetchData(this);

    // pobranie danych o nastęnym wyścigu i włączenie timera
    connect(fetch, &FetchData::NastepnyPobrano, this, [=](const QString &NazwaWyscig, const QString &DataCzas, const QString &DataQuali, const QString &Lokalizacja) {
        NazwaWyscigLabel->setText("Nazwa wyścigu: <b>" + NazwaWyscig + "</b>");
        DataWyscigLabel->setText("Data i czas wyścigu: <b>" + DataCzas + "</b>");
        DataQualiLabel->setText("Data i czas kwalifikacji: <b>" + DataQuali + "</b>");
        LokalizacjaLabel->setText("Lokalizacja: <b>" + Lokalizacja + "</b>");

        QDateTime Czas = QDateTime::fromString(DataCzas, "yyyy-MM-dd HH:mm:ss");

        odswierzTimer(LicznikLabel, Czas);

        connect(timer, &QTimer::timeout, [this, Czas]() {
            odswierzTimer(LicznikLabel, Czas);
        });

        timer->start(1000);
    });
    
    // pobieranie danych o ostatnim wyścigu
    connect(fetch, &FetchData::OstatniPobrano, this, [=](const QString &NazwaWyscigo, const QString &DataCzaso, const QString &Lokalizacjao, const QString &Top3, const QString &FastestLap, const QString &FastestLapDriver) {
        NazwaOstatniLabel->setText("Nazwa wyścigu: <b>" + NazwaWyscigo + "</b>");
        DataOstatniLabel->setText("Data i czas ostatniego wyścigu: <b>" + DataCzaso + "</b>");
        LokalizacjaOstatniLabel->setText("Lokalizacja: <b>" + Lokalizacjao + "</b>");
        Top3Label->setText("Top 3: <b>" + Top3 + "</b>");
        FastestLapLabel->setText("Najszybsze okrążenie: <b>" + FastestLap + "</b> wykonane przez: <b>" + FastestLapDriver + "</b>");
    });

    // pobieranie danych o aktualnym rankingu
    connect(fetch, &FetchData::RankingPobrano, this, [=](const QList<QStringList> &rankings) {
        tablicaRanking->setRowCount(rankings.size());
        for (int i = 0; i < rankings.size(); ++i) {
            for (int j = 0; j < rankings[i].size(); ++j) {
                QTableWidgetItem *item = new QTableWidgetItem(rankings[i][j]);
                item->setTextAlignment(Qt::AlignCenter);
                tablicaRanking->setItem(i, j, item);
            }
        }
    });

    fetch->Nastepny();
    fetch->Ostatni();
    fetch->Ranking();

    // dodajemy do layoutu głównego widgetów
    layout->addWidget(Nastepnyw, 0, 0); 
    layout->addWidget(Ostatniw, 1, 0);
    layout->addWidget(tablicaRanking, 0, 1, 2, 1);

    setLayout(layout);
}

void Glowna::odswierzTimer(QLabel *LicznikLabel, const QDateTime &Czas) { // funkcja aktualizująca timer i etykietę LicznikLabel
    QDateTime aktualny = QDateTime::currentDateTime();
    int CzasS = aktualny.secsTo(Czas);
    if (CzasS > 0) { // sprawdzenie czy wyścig już się nie odbył/zaczął
        int dni = CzasS / 86400;
        int h = (CzasS % 86400) / 3600;
        int m = (CzasS % 3600) / 60;
        int s = CzasS % 60;

        // odpowiednia odmiana słowa dzień lub jego usunięcie
        if (dni > 1)
            LicznikLabel->setText(QString("Czas do wyścigu: <b>%1 dni %2:%3:%4</b>").arg(dni).arg(h, 2, 10, QLatin1Char('0')).arg(m, 2, 10, QLatin1Char('0')).arg(s, 2, 10, QLatin1Char('0')));

        else if (dni == 1)
            LicznikLabel->setText(QString("Czas do wyścigu: <b>%1 dzień %2:%3:%4</b>").arg(dni).arg(h, 2, 10, QLatin1Char('0')).arg(m, 2, 10, QLatin1Char('0')).arg(s, 2, 10, QLatin1Char('0')));

        else
            LicznikLabel->setText(QString("Czas do wyścigu: <b>%1:%2:%3</b>").arg(h, 2, 10, QLatin1Char('0')).arg(m, 2, 10, QLatin1Char('0')).arg(s, 2, 10, QLatin1Char('0')));

    } else {
        LicznikLabel->setText("Wyścig już się odbył!");
        timer->stop();
    }
}