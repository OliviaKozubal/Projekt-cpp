#include "mainwindow.h"
#include "Glowna.h"
#include "Kalendarz.h"
#include "Wyniki.h"
#include "Kierowcy.h"
#include "Zespoly.h"
#include "FetchData.h"

#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) 
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    QIcon appIcon(":/zdjecia/logo.png");
    this->setWindowIcon(appIcon);

    stackedWidget = new QStackedWidget(this);

    Glowna *glownaPage = new Glowna(this);
    stackedWidget->addWidget(glownaPage); 
    stackedWidget->addWidget(new Kalendarz(this));
    stackedWidget->addWidget(new Wyniki(this));
    stackedWidget->addWidget(new Kierowcy(this));
    stackedWidget->addWidget(new Zespoly(this));

    QHBoxLayout *poziomo = new QHBoxLayout();
    QStringList etykiety = { "Główna", "Kalendarz", "Wyniki", "Kierowcy", "Zespoły"};
    QList<void(MainWindow::*)()> funkcje = { &MainWindow::goToGlowna, &MainWindow::goToKalendarz, &MainWindow::goToWyniki, &MainWindow::goToKierowcy, &MainWindow::goToZespoly};
    
    for (int i = 0; i < etykiety.size(); ++i) {
        QPushButton *button = new QPushButton(etykiety[i], this);
        button->setMinimumHeight(50);
        poziomo->addWidget(button);
        connect(button, &QPushButton::clicked, this, funkcje[i]);
    }

    mainLayout->addLayout(poziomo);
    mainLayout->addWidget(stackedWidget); 
    showMaximized();
}

void MainWindow::goToGlowna() {
    stackedWidget->setCurrentIndex(0);
}

void MainWindow::goToKalendarz() {
    stackedWidget->setCurrentIndex(1);
}

void MainWindow::goToWyniki() {
    stackedWidget->setCurrentIndex(2);
}

void MainWindow::goToKierowcy() {
    stackedWidget->setCurrentIndex(3);
}

void MainWindow::goToZespoly() {
    stackedWidget->setCurrentIndex(4);


}