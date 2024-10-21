#include "Kalendarz.h"
#include <QVBoxLayout>
#include <QLabel>

Kalendarz::Kalendarz(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *label = new QLabel("To jest podstrona z kalendarzem", this);
    layout->addWidget(label);
    setLayout(layout);
}