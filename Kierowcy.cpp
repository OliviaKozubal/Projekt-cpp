// plik generujący tabelę z kierowcami, możliwość wyszukiwania i filtracji, okno ze szczegółami kierowcy
#include "Kierowcy.h"
#include "FetchData.h"

Kierowcy::Kierowcy(QWidget *parent) : QWidget(parent), layout(new QVBoxLayout(this)), TablicaKierowcy(new QTableWidget(this)), sortComboBox(new QComboBox(this)), filtrComboBox(new QComboBox(this)), searchLineEdit(new QLineEdit(this)), searchByComboBox(new QComboBox(this))
{
    searchLineEdit->setPlaceholderText("Wyszukaj... "); // wyszukiwarka

    TablicaKierowcy->setColumnCount(2); // tablica z kierowcami
    TablicaKierowcy->setHorizontalHeaderLabels({"Imię i Nazwisko", "Narodowość"});
    TablicaKierowcy->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    
    searchByComboBox->addItem("Imię i Nazwisko"); // wyszukiwanie po imieniu i nazwisku, narodowości
    searchByComboBox->addItem("Narodowość");

    sortComboBox->addItem("Sortuj alfabetycznie rosnąco"); // sortowanie
    sortComboBox->addItem("Sortuj alfabetycznie malejąco");

    filtrComboBox->addItem("Pokaż wszystkich"); // filtracja - pokaż wszytskich lub pokaż mistrzów świata
    filtrComboBox->addItem("Mistrzowie świata");

    QHBoxLayout *searchLayout = new QHBoxLayout(); // layout wyszukiwarki - linia wyszukiwania + opcje wyszukiwania
    searchLayout->addWidget(searchByComboBox);
    searchLayout->addWidget(searchLineEdit);
    searchLayout->addStretch();

    QHBoxLayout *sortLayout = new QHBoxLayout(); // layout sortowanie + filtracja
    sortLayout->addWidget(sortComboBox);
    sortLayout->addWidget(filtrComboBox);
    sortLayout->addStretch();
    
    layout->addLayout(searchLayout); // layout główny
    layout->addLayout(sortLayout);
    layout->addWidget(TablicaKierowcy);

    setLayout(layout);

    connect(filtrComboBox, &QComboBox::currentIndexChanged, this, &Kierowcy::Filtr);
    connect(sortComboBox, &QComboBox::currentIndexChanged, this, &Kierowcy::Sortuj);
    connect(TablicaKierowcy, &QTableWidget::cellClicked, this, &Kierowcy::kierowcaKlikniety);
    connect(searchLineEdit, &QLineEdit::textChanged, this, &Kierowcy::Wyszukaj);
    connect(searchByComboBox, &QComboBox::currentIndexChanged, this, &Kierowcy::Wyszukaj);

    Kierowcy_z_pliku();
}

void Kierowcy::Kierowcy_z_pliku() // wczytywanie listy kierowców z pliku
{
    QString sciezka = QCoreApplication::applicationDirPath() + "/kierowcy.json";
    QFile file(sciezka);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);

    QJsonArray jsonArray = doc.array();
    QList<QStringList> kierowcy;
    
    for (const QJsonValue &value : jsonArray) { // z pliku odczytujemy imie i nazwisko, narodowość i driverId
        if (value.isObject()) {
            QJsonObject obj = value.toObject();
            QString imieNazwisko = obj["imie_nazwisko"].toString();
            QString narodowosc = obj["narodowosc"].toString();
            QString driverId = obj["driverid"].toString();
            kierowcy.append({imieNazwisko, narodowosc, driverId});
        }
    }

    kierowcyLista = kierowcy;
    przefiltrowani = kierowcy;
    wyswietlKierowcy(kierowcy);
}

void Kierowcy::Sortuj(int index) // mechanika sortowania
{
    auto sortujNazwisko = [](const QStringList &name1, const QStringList &name2) -> bool {
        QStringList p1 = name1[0].split(" ");
        QStringList p2 = name2[0].split(" ");
        return p1.last().compare(p2.last()) < 0;
    };

    if (index == 0) {  // Alfabetycznie rosnąco
        std::sort(przefiltrowani.begin(), przefiltrowani.end(), sortujNazwisko);
    } else if (index == 1) {  // Alfabetycznie malejąco
        std::sort(przefiltrowani.begin(), przefiltrowani.end(), [&](const QStringList &name1, const QStringList &name2) { return !sortujNazwisko(name1, name2); });
    }
    wyswietlKierowcy(przefiltrowani); // wyświetlamy kierowców posortowanych (z uwzględnieniem filtracji)
}

void Kierowcy::wyswietlKierowcy(const QList<QStringList> &kierowcy) // wyświetlanie kierowców w tabeli
{
    TablicaKierowcy->clearContents();

    TablicaKierowcy->setRowCount(kierowcy.size());

    for (int i = 0; i < kierowcy.size(); ++i) {
        const auto &kierowca = kierowcy[i];
        for (int j = 0; j < kierowca.size()-1; ++j) {
            QTableWidgetItem *item = new QTableWidgetItem(kierowca[j]);
            item->setTextAlignment(Qt::AlignCenter);
            TablicaKierowcy->setItem(i, j, item);
        }
    }
}

void Kierowcy::Wyszukaj() // przekaż metodę wyszukiwania
{
    FiltrujWyszukaj(searchByComboBox->currentIndex());
}

void Kierowcy::FiltrujWyszukaj(int index) // wyszukuj po wybranej metodzie
{
    QString tekst = searchLineEdit->text();
    przefiltrowani.clear();

    if (tekst.isEmpty()) {
        przefiltrowani = kierowcyLista;
    } else {
        auto warunek = [index, tekst](const QStringList& kierowca) {
            return index == 0 ? kierowca[0].toLower().contains(tekst.toLower()) : kierowca[1].toLower().contains(tekst.toLower());
        };
        for (const auto &kierowca : kierowcyLista)
            if (warunek(kierowca))
                przefiltrowani.append(kierowca);
    }

    wyswietlKierowcy(przefiltrowani);
}

void Kierowcy::Filtr() // przekaż metodę filtracji
{
    FiltrujFiltr(filtrComboBox->currentIndex());
}

void Kierowcy::FiltrujFiltr(int index) // filtruj po wybranej opcji
{
    przefiltrowani.clear();

    if (index == 0)
        przefiltrowani = kierowcyLista;
     else if (index == 1) {
        QList<QString> mistrzID;
        QString sciezka = QCoreApplication::applicationDirPath() + "/mistrzowie.json";
        QFile file(sciezka);
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray data = file.readAll();
            file.close();

            QJsonDocument doc = QJsonDocument::fromJson(data);
            if (doc.isArray()) {
                QJsonArray jsonArray = doc.array();
                for (const QJsonValue &value : jsonArray) {
                    if (value.isObject()) {
                        QJsonObject obj = value.toObject();
                        if (obj.contains("driverid") && obj["driverid"].isString()) {
                            mistrzID.append(obj["driverid"].toString());
        }   }   }   }   }

        for (const auto &kierowca : kierowcyLista) {
            QString driverIdKierowcy = kierowca[2];
            if (mistrzID.contains(driverIdKierowcy)) {
                przefiltrowani.append(kierowca);
            }
        }
    }

    wyswietlKierowcy(przefiltrowani);
}

void Kierowcy::kierowcaKlikniety(int row, int column) // mechanika wyświetlania okna ze szczegółami kierowców
{
    QString driverId = przefiltrowani[row][2];
    if (driverId.isEmpty()) return;

    QJsonObject daneKierowcy = wczytajSzczegoly(driverId);
    QStringList lataMistrzostw = mistrzostwa(driverId);

    QWidget *szczegolyOkno = new QWidget; // okno szczegółów
    QVBoxLayout *mainLayout = new QVBoxLayout;
    szczegolyOkno->setLayout(mainLayout);
    szczegolyOkno->setWindowTitle("Szczegóły Kierowcy");

    szczegolyOkno->setStyleSheet("QWidget#SzczegolyOkno { border: 5px solid red; border-radius: 15px; padding: 15px; }");
    szczegolyOkno->setObjectName("SzczegolyOkno"); //  czerowna ramka dookoła okna

    wyswietlSzczegoly(mainLayout, daneKierowcy, lataMistrzostw);

    QComboBox *sezonyLista = new QComboBox; // rozwijana lista z latami aktywności kierowców
    sezonyLista->addItem("Wybierz sezon");
    QStringList sezony = sezonyKierowcy(daneKierowcy);
    sezonyLista->addItems(sezony);
    mainLayout->addWidget(sezonyLista);
    sezonyLista->setStyleSheet("QComboBox { font-size: 16px; }");

    QTableWidget *tabelaWyscigow = new QTableWidget; // stworzenie tabeli z wyścigami sezonu
    mainLayout->addWidget(tabelaWyscigow);

    connect(sezonyLista, &QComboBox::currentTextChanged, [=](const QString &rok) { update(daneKierowcy, rok, tabelaWyscigow);});
    szczegolyOkno->showMaximized();
}

QStringList Kierowcy::mistrzostwa(const QString &driverId) // wczytuje informacje na temat mistrzostw kierowcy
{
    QStringList lataMistrzostw;
    QString sciezkaMistrzowie = QCoreApplication::applicationDirPath() + "/mistrzowie.json";
    QFile fileMistrzowie(sciezkaMistrzowie);
    
    if (fileMistrzowie.open(QIODevice::ReadOnly)) {
        QByteArray data = fileMistrzowie.readAll();
        fileMistrzowie.close();

        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonArray jsonArray = doc.array();
        for (const QJsonValue &value : jsonArray) {
            if (value.isObject()) {
                QJsonObject obj = value.toObject();
                if (obj["driverid"].toString() == driverId) {
                    lataMistrzostw.append(obj["rok"].toString());
    }}}}
    return lataMistrzostw;
}

QJsonObject Kierowcy::wczytajSzczegoly(const QString &driverId) // wczytuje cały plik z danymi na temat kierowcy
{
    QString sciezka = QCoreApplication::applicationDirPath() + "/kierowcy/" + driverId + "_dane.json";
    QFile file(sciezka);
    
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        file.close();
        
        QJsonDocument doc = QJsonDocument::fromJson(data);
        return doc.object();
    }
    return QJsonObject();
}

void Kierowcy::wyswietlSzczegoly(QVBoxLayout *layout, const QJsonObject &dane, const QStringList &lataMistrzostw) // wyświetlenie szczegółów na temat danego kierowcy
{
    QJsonObject raceTable = dane["MRData"].toObject()["RaceTable"].toObject();
    QJsonArray races = raceTable["Races"].toArray();
    QJsonObject daneKierowcy = races[0].toObject()["Results"].toArray()[0].toObject()["Driver"].toObject();

    QString imie = daneKierowcy["givenName"].toString();
    QString nazwisko = daneKierowcy["familyName"].toString();
    QString dataUrodzenia = daneKierowcy["dateOfBirth"].toString();
    QString narodowosc = daneKierowcy["nationality"].toString();
    int liczbaStartow = races.size();
    int najwyzszaPozycja = 99;
    int liczbaWygranych = 0;

    for (const QJsonValue &race : races) {
        QJsonObject obj = race.toObject();
        QJsonArray results = obj["Results"].toArray();
        for (const QJsonValue &r : results) {
            QJsonObject rObj = r.toObject();
            int position = rObj["position"].toString().toInt();
            if (position > 0 && position < najwyzszaPozycja) {
                najwyzszaPozycja = position;
            }
            if (rObj["position"] == "1") {
                liczbaWygranych++;
            }
        }
    }

    QString mistrzostwa = lataMistrzostw.isEmpty() ? "Brak" : lataMistrzostw.join(", ");

    QGridLayout *gridLayout = new QGridLayout;

    QFont labelFont("Arial", 14, QFont::Bold);
    QFont valueFont("Arial", 14);

    // Funkcja pomocnicza do dodawania etykiet i wartości do layoutu
    auto dodajPole = [&](int row, int col, const QString &etykieta, const QString &wartosc) {
        QLabel *label = new QLabel(etykieta);
        label->setFont(labelFont);
        //label->setStyleSheet("color: #222222;"); 

        QLabel *value = new QLabel(wartosc);
        value->setFont(valueFont);
        //value->setStyleSheet("color: #444444; padding: 5px;");

        gridLayout->addWidget(label, row, col);
        gridLayout->addWidget(value, row, col + 1);
    };

    // Pierwsza kolumna: Dane osobowe
    dodajPole(0, 0, "Imię:", imie);
    dodajPole(1, 0, "Nazwisko:", nazwisko);
    dodajPole(2, 0, "Data urodzenia:", dataUrodzenia);
    dodajPole(3, 0, "Narodowość:", narodowosc);

    // Druga kolumna: Statystyki
    dodajPole(0, 2, "Liczba startów:", QString::number(liczbaStartow));
    dodajPole(1, 2, "Najwyższa pozycja:", QString::number(najwyzszaPozycja));
    dodajPole(2, 2, "Liczba wygranych:", QString::number(liczbaWygranych));
    dodajPole(3, 2, "Mistrzostwa:", mistrzostwa);

    layout->addLayout(gridLayout);
}

QStringList Kierowcy::sezonyKierowcy(const QJsonObject &dane) // funkcja zwraca sezony aktywności danego kierowcy
{
    QStringList sezony;
    if (!dane.contains("MRData")) return sezony;

    QJsonObject raceTable = dane["MRData"].toObject()["RaceTable"].toObject();
    QJsonArray races = raceTable["Races"].toArray();
    
    for (const QJsonValue &race : races) {
        QString sezon = race.toObject()["season"].toString();
        if (!sezony.contains(sezon)) {
            sezony.append(sezon);
        }
    }
    return sezony;
}

void Kierowcy::update(const QJsonObject &dane, const QString &rok, QTableWidget *tabela) // tworzenie tabeli szczegółów kierowcy
{
    QJsonObject raceTable = dane["MRData"].toObject()["RaceTable"].toObject();
    QJsonArray races = raceTable["Races"].toArray();

    tabela->clear();
    tabela->setRowCount(0);
    tabela->setColumnCount(8);

    QStringList naglowki = {"Nazwa wyścigu", "Data", "Numer kierowcy", "Pozycja", "Najszybsze okrążenie", "Punkty", "Status", "Konstruktor"}; // nagłówki tabeli
    tabela->setHorizontalHeaderLabels(naglowki);
    tabela->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    int row = 0;
    for (const QJsonValue &r : races) {
        QJsonObject obj = r.toObject();
        if (obj["season"].toString() == rok) {
            QJsonArray results = obj["Results"].toArray();
            for (const QJsonValue &r : results) {
                QJsonObject rObj = r.toObject();
                QString pozycja = rObj["positionText"].toString();
                QString status = "";

                bool czyLiczbowa;
                pozycja.toInt(&czyLiczbowa);
                if (!czyLiczbowa) {
                    status = rObj["status"].toString();
                }

                QString konstruktor = rObj["Constructor"].toObject()["name"].toString();

                tabela->insertRow(row);
                tabela->setItem(row, 0, new QTableWidgetItem(obj["raceName"].toString()));
                tabela->setItem(row, 1, new QTableWidgetItem(obj["date"].toString()));
                tabela->setItem(row, 2, new QTableWidgetItem(rObj["number"].toString()));
                tabela->setItem(row, 3, new QTableWidgetItem(pozycja));
                tabela->setItem(row, 4, new QTableWidgetItem(rObj["FastestLap"].toObject()["Time"].toObject()["time"].toString()));
                tabela->setItem(row, 5, new QTableWidgetItem(rObj["points"].toString()));
                tabela->setItem(row, 6, new QTableWidgetItem(status));
                tabela->setItem(row, 7, new QTableWidgetItem(konstruktor));

                for (int col = 0; col < tabela->columnCount(); ++col)
                    tabela->item(row, col)->setTextAlignment(Qt::AlignCenter);
                row++;
            }
        }
    }
}