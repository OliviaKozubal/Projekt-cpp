// plik definiujący funkcję i zmienne używane w kierowcy.cpp
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
#include <QFile>
#include <QJsonDocument>
#include <QList>
#include <QStringList>
#include <QJsonObject>
#include <QJsonArray>
#include <QTextStream>
#include <QCoreApplication>
#include <QFormLayout>

class Kierowcy : public QWidget {
    Q_OBJECT

public:
    Kierowcy(QWidget *parent = nullptr);

public slots:
    void Kierowcy_z_pliku();
    void wyswietlKierowcy(const QList<QStringList> &kierowcy);
    void Sortuj(int index);
    void FiltrujWyszukaj(int index);
    void Wyszukaj();
    void Filtr();
    void FiltrujFiltr(int index);
    void kierowcaKlikniety(int row, int column);
    void update(const QJsonObject &dane, const QString &rok, QTableWidget *tabela);
    QStringList sezonyKierowcy(const QJsonObject &dane);
    void wyswietlSzczegoly(QVBoxLayout *layout, const QJsonObject &dane, const QStringList &lataMistrzostw);
    QJsonObject wczytajSzczegoly(const QString &driverId);
    QStringList mistrzostwa(const QString &driverId);
    
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