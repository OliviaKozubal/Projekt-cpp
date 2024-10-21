#include "Wyniki.h"
#include <QVBoxLayout>
#include <QLabel>

Wyniki::Wyniki(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *label = new QLabel("To jest podstrona z informacjami o wynikach", this);
    layout->addWidget(label);
    setLayout(layout);
}