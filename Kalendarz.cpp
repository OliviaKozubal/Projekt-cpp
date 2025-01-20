#include "Kalendarz.h"
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QDate>
#include <QDebug>

Kalendarz::Kalendarz(QWidget *parent) : QWidget(parent), aktData(2024, 1, 1) {
    rokLabel = new QLabel(this);
    QPushButton *poprzButton = new QPushButton("<", this);
    QPushButton *nastButton = new QPushButton(">", this);

    headerLayout = new QHBoxLayout;
    headerLayout->addWidget(poprzButton);
    headerLayout->addWidget(rokLabel);
    headerLayout->addWidget(nastButton);

    mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(headerLayout);

    kalLayout = new QGridLayout();
    mainLayout->addLayout(kalLayout);

    update();

    connect(poprzButton, &QPushButton::clicked, this, &Kalendarz::poprzMiesiac);
    connect(nastButton, &QPushButton::clicked, this, &Kalendarz::nastMiesiac);
}

void Kalendarz::poprzMiesiac() {
    if (aktData.month() > 1) {
        aktData = aktData.addMonths(-1);
        update();
    }
}

void Kalendarz::nastMiesiac() {
    if (aktData.month() < 12) {
        aktData = aktData.addMonths(1);
        update();
    }
}

void Kalendarz::update() {
    QString nazwaM = aktData.toString("MMMM");
    QString rokTekst = QString("%1 - %2").arg(aktData.toString("yyyy")).arg(nazwaM);

    rokLabel->setText(rokTekst);
    rokLabel->setAlignment(Qt::AlignCenter);

    QFont font = rokLabel->font();
    font.setPointSize(18);
    font.setBold(true);
    rokLabel->setFont(font);

    if (calendarContainer) {
        delete calendarContainer;
        calendarContainer = nullptr;
    }

    tworzKalendarz();
}

void Kalendarz::tworzKalendarz() {
    calendarContainer = new QWidget(this);

    QGridLayout *gridLayout = new QGridLayout(calendarContainer);
    calendarContainer->setLayout(gridLayout);

    calendarContainer->setStyleSheet(
        "border: 4px solid red; "
        "border-radius: 15px; "
        "padding: 10px; "
    );

    int dniMiesiac = aktData.daysInMonth();
    int pierwszy = aktData.dayOfWeek();

    int licznik = 1;
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 7; ++j) {
            if (i == 0 && j < pierwszy - 1) {
                continue;
            }
            if (licznik > dniMiesiac) {
                break;
            }

            QPushButton *okno = new QPushButton(QString::number(licznik), this);

            QFont font = okno->font();
            font.setBold(true);
            font.setPointSize(12);
            okno->setFont(font);

            gridLayout->addWidget(okno, i, j);
            ++licznik;
        }
    }
    mainLayout->addWidget(calendarContainer);
}
