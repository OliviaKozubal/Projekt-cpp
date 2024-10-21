#include "Glowna.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

Glowna::Glowna(QWidget *parent) : QWidget(parent) {
    QGridLayout *layout = new QGridLayout(this);

    QLabel *Nastepny = new QLabel("Widget z informacjami na temat najbliższego wyścigu", this);
    QLabel *Ostatni = new QLabel("Widget z wynikami z ostatniego wyścigu", this);
    QLabel *Ranking = new QLabel("Ranking kierowców ", this);

    layout->addWidget(Nastepny, 0, 0); 
    layout->addWidget(Ostatni, 1, 0);
    layout->addWidget(Ranking, 0, 1, 2, 1);

    setLayout(layout);
}