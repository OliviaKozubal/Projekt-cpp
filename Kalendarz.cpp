#include "kalendarz.h"

Kalendarz::Kalendarz(QWidget *parent) : QWidget(parent), Data(QDate(2024, QDate::currentDate().month(), 1)) {
    auto *mainLayout = new QVBoxLayout(this);

    auto *headerLayout = new QHBoxLayout();
    QPushButton *poprzButton = new QPushButton("<", this);
    QPushButton *nastButton = new QPushButton(">", this);
    miesLabel = new QLabel(this);
    miesLabel->setAlignment(Qt::AlignCenter);

    headerLayout->addWidget(poprzButton);
    headerLayout->addWidget(miesLabel);
    headerLayout->addWidget(nastButton);
    mainLayout->addLayout(headerLayout);

    kalendarzLayout = new QGridLayout();
    mainLayout->addLayout(kalendarzLayout);

    connect(poprzButton, &QPushButton::clicked, this, &Kalendarz::poprzMonth);
    connect(nastButton, &QPushButton::clicked, this, &Kalendarz::nastMonth);

    QString sciezka = QCoreApplication::applicationDirPath() + "/daty_2024.json";
    wczytajDaty(sciezka);
    update();
}

void Kalendarz::wczytajDaty(const QString &sciezka) {
    QFile file(sciezka);
    if (!file.open(QIODevice::ReadOnly))
        return;

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);

    QJsonObject root = doc.object();
    for (const QString &dateString : root.keys()) {
        QDate date = QDate::fromString(dateString, "yyyy-MM-dd");

        QJsonArray eventArray = root.value(dateString).toArray();
        for (const QJsonValue &value : eventArray) {
            if (value.isObject()) {
                QJsonObject eventObject = value.toObject();
                QString nazwa = eventObject.value("name").toString();
                QString localTime = eventObject.value("localTime").toString();

                QTime czas = QTime::fromString(localTime, "HH:mm:ss");
                QString kCzas = czas.toString("HH:mm");

                QString szczegoly = nazwa + " (" + kCzas + ")";
                if (!nazwa.isEmpty()) {
                    wydarzenia[date].append(szczegoly);
                }}}}
}

void Kalendarz::poprzMonth() {
    if (Data.month() == 1) return;
    Data = Data.addMonths(-1);
    update();
}

void Kalendarz::nastMonth() {
    if (Data.month() == 12) return;
    Data = Data.addMonths(1);
    update();
}

void Kalendarz::update() {
    miesLabel->setText(Data.toString("MMMM yyyy"));
    miesLabel->setStyleSheet("font-size: 24px; font-weight: bold;");

    QLayoutItem *item;
    while ((item = kalendarzLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    QStringList dni = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
    for (int i = 0; i < dni.size(); ++i) {
        auto *label = new QLabel(dni[i], this);
        label->setAlignment(Qt::AlignCenter);
        label->setStyleSheet("border: 2px solid red; border-radius: 5px; padding: 5px; font-size: 18px; font-weight: bold;");
        kalendarzLayout->addWidget(label, 0, i);
    }

    QDate pierwszyDzien(Data.year(), Data.month(), 1);
    int col1 = (pierwszyDzien.dayOfWeek() + 5) % 7;
    int row = 1;

    for (int i = 0; i < col1; ++i) {
        auto *emptyLabel = new QLabel(this);
        emptyLabel->setStyleSheet("border: 2px solid red; border-radius: 5px; padding: 5px;");
        kalendarzLayout->addWidget(emptyLabel, row, i);
    }

    for (int day = 1; day <= pierwszyDzien.daysInMonth(); ++day) {
        QDate date(Data.year(), Data.month(), day);

        QString text = QString::number(day);
        QString eventText = etykieta(date);
        if (!eventText.isEmpty()) {
            text += "\n\n" + eventText;
        }

        auto *label = new QLabel(text, this);
        label->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
        label->setWordWrap(true);

        label->setStyleSheet("border: 2px solid red; border-radius: 5px; padding: 5px; font-size: 16px; font-weight: bold;");

        kalendarzLayout->addWidget(label, row, col1);
        if (++col1 > 6) {
            col1 = 0;
            ++row;
        }
    }

    while (col1 <= 6) {
        auto *emptyLabel = new QLabel(this);
        emptyLabel->setStyleSheet("border: 2px solid red; border-radius: 5px; padding: 5px;");
        kalendarzLayout->addWidget(emptyLabel, row, col1++);
    }
}

QString Kalendarz::etykieta(const QDate &date) {
    QStringList szczegoly;
    if (wydarzenia.contains(date))
        for (const QString &event : wydarzenia.value(date))
            szczegoly.append(event); 
    return szczegoly.join("\n");
}