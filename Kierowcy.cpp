#include "Kierowcy.h"
#include <QVBoxLayout>
#include <QLabel>

Kierowcy::Kierowcy(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *label = new QLabel("To jest podstrona z informacjami o kierowcach", this);
    layout->addWidget(label);
    setLayout(layout);
}