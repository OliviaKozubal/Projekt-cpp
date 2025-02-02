#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QStackedWidget>
#include <QPushButton>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QApplication>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
private:
    QStackedWidget *stackedWidget;

private slots:
    void NaStrone(int index);
};

#endif // MAINWINDOW_H