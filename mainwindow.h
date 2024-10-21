#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QStackedWidget>
#include <QPushButton>

class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    QStackedWidget *stackedWidget;

private slots:
    void goToGlowna();
    void goToKalendarz();
    void goToWyniki();
    void goToKierowcy();
    void goToZespoly();
};

#endif // MAINWINDOW_H