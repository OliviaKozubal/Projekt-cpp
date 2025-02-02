// plik tworzący kalendarz
#include "kalendarz.h"

Kalendarz::Kalendarz(QWidget *parent) : QWidget(parent), Data(QDate(2024, QDate::currentDate().month(), QDate::currentDate().day())) {
    auto *mainLayout = new QVBoxLayout(this);

    auto *headerLayout = new QHBoxLayout();
    QPushButton *poprzButton = new QPushButton("<", this);
    QPushButton *nastButton = new QPushButton(">", this);
    miesLabel = new QLabel(this);
    miesLabel->setAlignment(Qt::AlignCenter);

    headerLayout->addWidget(poprzButton);
    headerLayout->addWidget(miesLabel);
    headerLayout->addWidget(nastButton);

    kalendarzLayout = new QGridLayout();

    mainLayout->addLayout(headerLayout);
    mainLayout->addLayout(kalendarzLayout);

    connect(poprzButton, &QPushButton::clicked, this, &Kalendarz::poprzMonth);
    connect(nastButton, &QPushButton::clicked, this, &Kalendarz::nastMonth);

    QString sciezka = QCoreApplication::applicationDirPath() + "/daty_2024.json"; // sciezka do pliku daty_2024.json
    wczytajDaty(sciezka);
    update();
}

void Kalendarz::wczytajDaty(const QString &sciezka) { // funkcja wczytująca wydarzenia z pliku daty_024.json
    QFile file(sciezka);
    if (!file.open(QIODevice::ReadOnly))
        return;

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);

    QJsonObject root = doc.object();
    for (const QString &dataString : root.keys()) {
        QDate data = QDate::fromString(dataString, "yyyy-MM-dd");

        QJsonArray tablicaDaty = root.value(dataString).toArray();
        for (const QJsonValue &value : tablicaDaty) {
            if (value.isObject()) {
                QJsonObject wydarzenie = value.toObject();
                QString nazwa = wydarzenie.value("name").toString();
                QString czasLokalny = wydarzenie.value("localTime").toString();

                QString szczegoly = nazwa + " (" + QTime::fromString(czasLokalny, "HH:mm:ss").toString("HH:mm") + ")"; // stworzenie stringa który zostanie umieszczony w kalendarzu
                if (!nazwa.isEmpty()) {
                    wydarzenia[data].append(szczegoly);
                }}}}
}

void Kalendarz::poprzMonth() { // funkcja umożliwiająca przejście na poprzedni miesiąc
    if (Data.month() == 1) return;
    Data = Data.addMonths(-1);
    update();
}

void Kalendarz::nastMonth() { // funkcja umożliwiająca przejście na następny miesiąc
    if (Data.month() == 12) return;
    Data = Data.addMonths(1);
    update();
}

void Kalendarz::update() { // funkcja tworząca stronę kalendarza
    miesLabel->setText(Data.toString("MMMM yyyy"));
    miesLabel->setStyleSheet("font-size: 24px; font-weight: bold;");

    QLayoutItem *item;
    while ((item = kalendarzLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    QStringList dni = {"Poniedziałek", "Wtorek", "Środa", "Czwartek", "Piątek", "Sobota", "Niedziela"};
    for (int i = 0; i < dni.size(); ++i) {
        auto *label = new QLabel(dni[i], this);
        label->setAlignment(Qt::AlignCenter);
        label->setStyleSheet("border: 2px solid red; border-radius: 5px; padding: 5px; font-size: 18px; font-weight: bold;");
        kalendarzLayout->addWidget(label, 0, i);
    }

    QDate d(Data.year(), Data.month(), 1); // od jakiego dnia zaczyna się miesiąc
    int col1 = (d.dayOfWeek() + 5) % 7;
    int row = 1;

    for (int i = 0; i < col1; ++i) { // puste pola przed pierwszym dniem miesiąca
        auto *pustePola = new QLabel(this);
        pustePola->setStyleSheet("border: 2px solid red; border-radius: 5px; padding: 5px;");
        kalendarzLayout->addWidget(pustePola, row, i);
    }

    for (int dzien = 1; dzien <= d.daysInMonth(); ++dzien) { // tworzenie okien kalendarza z datami oraz wydarzeniami (jeśli są)
        QDate data(Data.year(), Data.month(), dzien);

        QString tekst = QString::number(dzien);
        QString wydarzenie = etykieta(data);
        if (!wydarzenie.isEmpty()) {
            tekst += "\n\n" + wydarzenie;
        }

        auto *label = new QLabel(tekst, this);
        label->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
        label->setWordWrap(true);

        label->setStyleSheet("border: 2px solid red; border-radius: 5px; padding: 5px; font-size: 16px; font-weight: bold;");

        kalendarzLayout->addWidget(label, row, col1);
        if (++col1 > 6) {
            col1 = 0;
            ++row;
        }
    }

    while (col1 <= 6) { // uzupełnianie do końca pustych okien
        auto *pustePola = new QLabel(this);
        pustePola->setStyleSheet("border: 2px solid red; border-radius: 5px; padding: 5px;");
        kalendarzLayout->addWidget(pustePola, row, col1++);
    }
}

QString Kalendarz::etykieta(const QDate &data) { // tworzenie etykiet wydarzeń
    QStringList szczegoly;
    if (wydarzenia.contains(data))
        for (const QString &wydarzenie : wydarzenia.value(data))
            szczegoly.append(wydarzenie); 
    return szczegoly.join("\n");
}