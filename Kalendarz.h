#ifndef KALENDARZ_H
#define KALENDARZ_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QDate>

class Kalendarz : public QWidget {
    Q_OBJECT

public:
    Kalendarz(QWidget *parent = nullptr);

private slots:
    void poprzMiesiac();
    void nastMiesiac();

private:
    QLabel *rokLabel;
    QVBoxLayout *mainLayout;
    QHBoxLayout *headerLayout;
    QGridLayout *kalLayout;
    QDate aktData;
    QWidget *calendarContainer;

    void update();
    void tworzKalendarz();
};

#endif // KALENDARZ_H