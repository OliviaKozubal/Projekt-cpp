#include "Kierowcy.h"
#include "FetchData.h"

Kierowcy::Kierowcy(QWidget *parent) : QWidget(parent), layout(new QVBoxLayout(this)), listaKierowcy(new QTableWidget(this)) 
{
    listaKierowcy->setColumnCount(2);
    listaKierowcy->setHorizontalHeaderLabels({"Imię i Nazwisko", "Narodowość"});
    listaKierowcy->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    layout->addWidget(listaKierowcy);
    setLayout(layout);

    QComboBox *sortComboBox = new QComboBox(this);
    sortComboBox->addItem("Sortuj alfabetycznie rosnąco");
    sortComboBox->addItem("Sortuj alfabetycznie malejąco");

    QHBoxLayout *sortLayout = new QHBoxLayout();
    sortLayout->addWidget(sortComboBox);
    sortLayout->addStretch();
    
    QComboBox *filtrComboBox = new QComboBox();
    filtrComboBox->addItem("Pokaż wszystkich");
    filtrComboBox->addItem("Mistrzowie świata");

    layout->insertLayout(0, sortLayout);

    sortLayout->addWidget(filtrComboBox);

    connect(filtrComboBox, &QComboBox::currentIndexChanged, this, &Kierowcy::Filtruj);
    connect(sortComboBox, &QComboBox::currentIndexChanged, this, &Kierowcy::Sortuj);

    FetchData *fetch = new FetchData(this);
    connect(fetch, &FetchData::KierowcyPobrano, this, &Kierowcy::Przypisz);
    fetch->Kierowcy();
    connect(listaKierowcy, &QTableWidget::cellClicked, this, &Kierowcy::onKierowcaClicked);
}

void Kierowcy::onKierowcaClicked(int row, int column)
{
    QString imieNazwisko = listaKierowcy->item(row, 0)->text();

    QWidget *nowaPodstrona = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(nowaPodstrona);

    layout->addWidget(new QLabel("Szczegóły kierowcy:"));
    layout->addWidget(new QLabel(imieNazwisko));

    nowaPodstrona->setLayout(layout);
    nowaPodstrona->setWindowTitle("Szczegóły Kierowcy");
    nowaPodstrona->showMaximized();
}

void Kierowcy::Sortuj(int index)
{
    QList<QStringList> posortowani = kierowcyLista;

    auto sortujNazwisko = [](const QStringList &name1, const QStringList &name2) -> bool {
        QStringList parts1 = name1[0].split(" ");
        QStringList parts2 = name2[0].split(" ");

        if (parts1.size() > 1 && parts2.size() > 1) {
            return parts1[1].compare(parts2[1]) < 0;
        }
        return name1[0].compare(name2[0]) < 0;
    };

    if (index == 0) {  // Alfabetycznie rosnąco
        std::sort(posortowani.begin(), posortowani.end(), sortujNazwisko);
    } else if (index == 1) {  // Alfabetycznie malejąco
        std::sort(posortowani.begin(), posortowani.end(), [&](const QStringList &name1, const QStringList &name2) {
            return !sortujNazwisko(name1, name2);
        });
    }

    wyswietlKierowcy(posortowani);
}

void Kierowcy::Przypisz(const QList<QStringList> &kierowcy)
{
    kierowcyLista = kierowcy;
    wyswietlKierowcy(kierowcy);
}

void Kierowcy::wyswietlKierowcy(const QList<QStringList> &kierowcy) 
{
    listaKierowcy->clearContents();

    listaKierowcy->setRowCount(kierowcy.size());

    for (int i = 0; i < kierowcy.size()-1; ++i) {
        const auto &kierowca = kierowcy[i];
        for (int j = 0; j < kierowca.size()-1; ++j) {
            QTableWidgetItem *item = new QTableWidgetItem(kierowca[j]);
            item->setTextAlignment(Qt::AlignCenter);
            listaKierowcy->setItem(i, j, item);
        }
    }
}

void Kierowcy::Filtruj(int index)
{
    QList<QStringList> przefiltrowani;

    if (index == 0) {
        przefiltrowani = kierowcyLista;
    } else if (index == 1) {
        for (const auto &kierowca : kierowcyLista) {
            if (kierowca[2] == "Tak") {
                przefiltrowani.append(kierowca);
            }
        }
    }

    wyswietlKierowcy(przefiltrowani);
}