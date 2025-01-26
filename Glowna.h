#ifndef GLOWNA_H
#define GLOWNA_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QTimer>
#include <QDateTime>
#include <QHeaderView>

class Glowna : public QWidget {
    Q_OBJECT

public:
    Glowna(QWidget *parent = nullptr);

};
#endif // GLOWNA_H