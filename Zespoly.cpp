#include "Zespoly.h"
#include <QVBoxLayout>
#include <QLabel>

Zespoly::Zespoly(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *label = new QLabel("To jest podstrona z informacjami o zespołach", this);
    layout->addWidget(label);
    setLayout(layout);
}