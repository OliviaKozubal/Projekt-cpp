#ifndef KIEROWCY_H
#define KIEROWCY_H

#include <QWidget>
#include <QVBoxLayout>
#include <QListWidget>
#include <QTableWidget>
#include <QLabel>
#include <QHeaderView>
#include <QComboBox>
#include <QLineEdit>
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
    void FiltrujWyszukaj(int index);
    void Wyszukaj();

private:
    QVBoxLayout *layout;
    QTableWidget *TablicaKierowcy;
    QList<QStringList> kierowcyLista;
    QComboBox *sortComboBox;
    QComboBox *filtrComboBox;
    QLineEdit *searchLineEdit;
    QComboBox *searchByComboBox;
    QList<QStringList> przefiltrowani;
};

#endif // KIEROWCY_H