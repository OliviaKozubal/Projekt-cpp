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
    void Kierowcy_z_pliku();
    void wyswietlKierowcy(const QList<QStringList> &kierowcy);
    void onKierowcaClicked(int row, int column);
    void Sortuj(int index);
    void FiltrujWyszukaj(int index);
    void Wyszukaj();
    void Filtr();
    void FiltrujFiltr(int index);

private:
    QVBoxLayout *layout;
    QTableWidget *TablicaKierowcy;
    QList<QStringList> kierowcyLista;
    QComboBox *sortComboBox;
    QComboBox *filtrComboBox;
    QLineEdit *searchLineEdit;
    QComboBox *searchByComboBox;
    QList<QStringList> przefiltrowani;
    QList<QString> mistrzowie;

private slots:
    
};

#endif // KIEROWCY_H