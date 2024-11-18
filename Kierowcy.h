#ifndef KIEROWCY_H
#define KIEROWCY_H

#include <QWidget>
#include <QVBoxLayout>
#include <QListWidget>
#include <QTableWidget>
#include <QLabel>
#include <QHeaderView>
#include <QComboBox>
#include <QHBoxLayout>

class Kierowcy : public QWidget {
    Q_OBJECT

public:
    Kierowcy(QWidget *parent = nullptr);

public slots:
    void Przypisz(const QList<QStringList> &kierowcy);
    void wyswietlKierowcy(const QList<QStringList> &kierowcy);
    void onKierowcaClicked(int row, int column);
    void Sortuj(int index);
    void Filtruj(int filtrIndex);

private:
    QVBoxLayout *layout;
    QTableWidget *listaKierowcy;
    QList<QStringList> kierowcyLista;
};

#endif // KIEROWCY_H