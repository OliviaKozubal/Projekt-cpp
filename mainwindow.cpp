#include "mainwindow.h"
#include "Glowna.h"
#include "Kalendarz.h"
#include "Wyniki.h"
#include "Kierowcy.h"
#include "FetchData.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) 
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    QIcon appIcon(":/zdjecia/logo.png");
    this->setWindowIcon(appIcon);

    stackedWidget = new QStackedWidget(this);

    Glowna *glownaStrona = new Glowna(this);
    stackedWidget->addWidget(glownaStrona); 
    stackedWidget->addWidget(new Kalendarz(this));
    stackedWidget->addWidget(new Wyniki(this));
    stackedWidget->addWidget(new Kierowcy(this));

    QHBoxLayout *poziomo = new QHBoxLayout();
    QStringList etykiety = { "Główna", "Kalendarz", "Wyniki", "Kierowcy"};
    
    for (int i = 0; i < etykiety.size(); ++i) {
        QPushButton *button = new QPushButton(etykiety[i], this);
        button->setStyleSheet("QPushButton { border: 2px solid red; border-radius: 10px; font-size: 16px; font-weight: bold;}");
        button->setMinimumHeight(50);
        poziomo->addWidget(button);
        connect(button, &QPushButton::clicked, this, [=]() { NaStrone(i); });
    }

    mainLayout->addLayout(poziomo);
    mainLayout->addWidget(stackedWidget); 
    showMaximized();
}

void MainWindow::NaStrone(int index) {
    stackedWidget->setCurrentIndex(index);
}
