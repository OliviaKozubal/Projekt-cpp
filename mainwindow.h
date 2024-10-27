#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QStackedWidget>
#include <QPushButton>
#include <QMainWindow>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
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