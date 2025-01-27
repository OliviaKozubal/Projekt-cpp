#include "Kierowcy.h"
#include "FetchData.h"

Kierowcy::Kierowcy(QWidget *parent) : QWidget(parent), layout(new QVBoxLayout(this)), TablicaKierowcy(new QTableWidget(this)), sortComboBox(new QComboBox(this)), filtrComboBox(new QComboBox(this)), searchLineEdit(new QLineEdit(this)), searchByComboBox(new QComboBox(this))
{
    searchLineEdit->setPlaceholderText("Wyszukaj... ");

    TablicaKierowcy->setColumnCount(2);
    TablicaKierowcy->setHorizontalHeaderLabels({"Imię i Nazwisko", "Narodowość"});
    TablicaKierowcy->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    
    searchByComboBox->addItem("Imię i Nazwisko");
    searchByComboBox->addItem("Narodowość");

    sortComboBox->addItem("Sortuj alfabetycznie rosnąco");
    sortComboBox->addItem("Sortuj alfabetycznie malejąco");

    filtrComboBox->addItem("Pokaż wszystkich");
    filtrComboBox->addItem("Mistrzowie świata");

    QHBoxLayout *searchLayout = new QHBoxLayout();
    searchLayout->addWidget(searchByComboBox);
    searchLayout->addWidget(searchLineEdit);
    searchLayout->addStretch();

    QHBoxLayout *sortLayout = new QHBoxLayout();
    sortLayout->addWidget(sortComboBox);
    sortLayout->addWidget(filtrComboBox);
    sortLayout->addStretch();
    
    layout->addLayout(searchLayout);
    layout->addLayout(sortLayout);
    layout->addWidget(TablicaKierowcy);

    setLayout(layout);

    connect(filtrComboBox, &QComboBox::currentIndexChanged, this, &Kierowcy::Filtr);
    connect(sortComboBox, &QComboBox::currentIndexChanged, this, &Kierowcy::Sortuj);
    connect(TablicaKierowcy, &QTableWidget::cellClicked, this, &Kierowcy::onKierowcaClicked);
    connect(searchLineEdit, &QLineEdit::textChanged, this, &Kierowcy::Wyszukaj);
    connect(searchByComboBox, &QComboBox::currentIndexChanged, this, &Kierowcy::Wyszukaj);

    Kierowcy_z_pliku();
}

void Kierowcy::Kierowcy_z_pliku()
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
    
    for (const QJsonValue &value : jsonArray) {
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

void Kierowcy::onKierowcaClicked(int row, int column)
{
    QString driverId = przefiltrowani[row][2];

    if (driverId.isEmpty()) {
        return;
    }

    QString imieNazwisko, narodowosc;
    int wygrane = 0, maxPoz = -1;
    QList<QString> lataMistrzostw;

    for (const auto& kierowca : kierowcyLista) {
        if (kierowca[2] == driverId) {
            imieNazwisko = kierowca[0];
            narodowosc = kierowca[1];
            break;
        }
    }

    QString sciezkaWygPoz = QCoreApplication::applicationDirPath() + "/wyg_poz.json";
    QFile fileWygPoz(sciezkaWygPoz);
    if (fileWygPoz.open(QIODevice::ReadOnly)) {
        QByteArray data = fileWygPoz.readAll();
        fileWygPoz.close();

        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonArray jsonArray = doc.array();
        for (const QJsonValue &value : jsonArray) {
            if (value.isObject()) {
                QJsonObject obj = value.toObject();
                if (obj["driverId"].toString() == driverId) {
                    wygrane = obj["wygrane"].toInt();
                    maxPoz = obj["max_poz"].isNull() ? -1 : obj["max_poz"].toInt();
                    break;
                }
            }
        }
    }

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
                }
            }
        }
    }

    QWidget *szczegolyOkno = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(szczegolyOkno);

    szczegolyOkno->setStyleSheet("border: 5px solid red");

    QLabel *imieNazwiskoLabel = new QLabel(imieNazwisko);
    imieNazwiskoLabel->setAlignment(Qt::AlignCenter);
    imieNazwiskoLabel->setStyleSheet("font-size: 30px; font-weight: bold");

    QLabel *narodowoscLabel = new QLabel("<b>Narodowość:</b> " + narodowosc);
    narodowoscLabel->setStyleSheet("font-size: 20px;");
    narodowoscLabel->setAlignment(Qt::AlignCenter);

    QLabel *wygraneLabel = new QLabel("<b>Liczba wygranych:</b> " + QString::number(wygrane));
    wygraneLabel->setStyleSheet("font-size: 20px");
    wygraneLabel->setAlignment(Qt::AlignCenter);

    QLabel *maxPozLabel = new QLabel("<b>Najwyższa osiągnieta pozycja:</b> " + (maxPoz == -1 ? "Brak danych" : QString::number(maxPoz)));
    maxPozLabel->setStyleSheet("font-size: 20px");
    maxPozLabel->setAlignment(Qt::AlignCenter);

    layout->addWidget(imieNazwiskoLabel);
    layout->addWidget(narodowoscLabel);
    layout->addWidget(wygraneLabel);
    layout->addWidget(maxPozLabel);

    if (!lataMistrzostw.isEmpty()) {
        QLabel *mistrzostwaLabel = new QLabel("<b>Lata mistrzostw:</b> " + lataMistrzostw.join(", "));
        mistrzostwaLabel->setStyleSheet("font-size: 20px");
        mistrzostwaLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(mistrzostwaLabel);
    }

    szczegolyOkno->setLayout(layout);
    szczegolyOkno->setWindowTitle("Szczegóły Kierowcy");
    szczegolyOkno->showMaximized();
}

void Kierowcy::Sortuj(int index)
{
    auto sortujNazwisko = [](const QStringList &name1, const QStringList &name2) -> bool {
        QStringList p1 = name1[0].split(" ");
        QStringList p2 = name2[0].split(" ");
        return p1.last().compare(p2.last()) < 0;
    };

    if (index == 0) {  // Alfabetycznie rosnąco
        std::sort(przefiltrowani.begin(), przefiltrowani.end(), sortujNazwisko);
    } else if (index == 1) {  // Alfabetycznie malejąco
        std::sort(przefiltrowani.begin(), przefiltrowani.end(), 
                  [&](const QStringList &name1, const QStringList &name2) { return !sortujNazwisko(name1, name2); });
    }

    wyswietlKierowcy(przefiltrowani);
}

void Kierowcy::wyswietlKierowcy(const QList<QStringList> &kierowcy) 
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

void Kierowcy::Wyszukaj()
{
    FiltrujWyszukaj(searchByComboBox->currentIndex());
}

void Kierowcy::FiltrujWyszukaj(int index)
{
    QString tekst = searchLineEdit->text();
    przefiltrowani.clear();

    if (tekst.isEmpty())
        przefiltrowani = kierowcyLista;
    else
        if (index == 0)
        {
            for (const auto &kierowca : kierowcyLista)
                if (kierowca[0].toLower().contains(tekst.toLower()))
                    przefiltrowani.append(kierowca);
        }
        else if (index == 1)
        {
            for (const auto &kierowca : kierowcyLista)
                if (kierowca[1].toLower().contains(tekst.toLower()))
                    przefiltrowani.append(kierowca);
        }
    wyswietlKierowcy(przefiltrowani);
}

void Kierowcy::Filtr()
{
    FiltrujFiltr(filtrComboBox->currentIndex());
}

void Kierowcy::FiltrujFiltr(int index)
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


