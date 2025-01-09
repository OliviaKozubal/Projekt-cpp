#include "Kierowcy.h"
#include "FetchData.h"

Kierowcy::Kierowcy(QWidget *parent) : QWidget(parent), layout(new QVBoxLayout(this)), TablicaKierowcy(new QTableWidget(this)), sortComboBox(new QComboBox(this)), filtrComboBox(new QComboBox(this)), searchLineEdit(new QLineEdit(this)), searchByComboBox(new QComboBox(this))
{
    TablicaKierowcy->setColumnCount(2);
    TablicaKierowcy->setHorizontalHeaderLabels({"Imię i Nazwisko", "Narodowość"});
    TablicaKierowcy->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    
    searchByComboBox->addItem("Imię i Nazwisko");
    searchByComboBox->addItem("Narodowość");

    sortComboBox->addItem("Sortuj alfabetycznie rosnąco");
    sortComboBox->addItem("Sortuj alfabetycznie malejąco");
    filtrComboBox->addItem("Pokaż wszystkich");
    filtrComboBox->addItem("Mistrzowie świata");

    QHBoxLayout *searchLayout = new QHBoxLayout();
    searchLayout->addWidget(searchByComboBox);
    searchLayout->addWidget(searchLineEdit);
    searchLayout->addStretch();

    QHBoxLayout *sortLayout = new QHBoxLayout();
    sortLayout->addWidget(sortComboBox);
    sortLayout->addWidget(filtrComboBox);
    sortLayout->addStretch();
    
    layout->addLayout(searchLayout);
    layout->addLayout(sortLayout);
    layout->addWidget(TablicaKierowcy);

    setLayout(layout);

    //connect(filtrComboBox, &QComboBox::currentIndexChanged, this, &Kierowcy::Filtruj);
    connect(sortComboBox, &QComboBox::currentIndexChanged, this, &Kierowcy::Sortuj);
    connect(TablicaKierowcy, &QTableWidget::cellClicked, this, &Kierowcy::onKierowcaClicked);
    connect(searchLineEdit, &QLineEdit::textChanged, this, &Kierowcy::Wyszukaj);
    connect(searchByComboBox, &QComboBox::currentIndexChanged, this, &Kierowcy::Wyszukaj);

    FetchData *fetch = new FetchData(this);
    connect(fetch, &FetchData::KierowcyPobrano, this, &Kierowcy::Przypisz);
    fetch->Kierowcy();
    
}

void Kierowcy::onKierowcaClicked(int row, int column)
{
    QString imieNazwisko = TablicaKierowcy->item(row, 0)->text();

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
    auto sortujNazwisko = [](const QStringList &name1, const QStringList &name2) -> bool {
        QStringList parts1 = name1[0].split(" ");
        QStringList parts2 = name2[0].split(" ");
        return parts1.last().compare(parts2.last()) < 0;
    };

    if (index == 0) {  // Alfabetycznie rosnąco
        std::sort(przefiltrowani.begin(), przefiltrowani.end(), sortujNazwisko);
    } else if (index == 1) {  // Alfabetycznie malejąco
        std::sort(przefiltrowani.begin(), przefiltrowani.end(), 
                  [&](const QStringList &name1, const QStringList &name2) { return !sortujNazwisko(name1, name2); });
    }

    wyswietlKierowcy(przefiltrowani);
}

void Kierowcy::Przypisz(const QList<QStringList> &kierowcy)
{
    kierowcyLista = kierowcy;
    przefiltrowani = kierowcy;
    wyswietlKierowcy(kierowcy);
}

void Kierowcy::wyswietlKierowcy(const QList<QStringList> &kierowcy) 
{
    TablicaKierowcy->clearContents();

    TablicaKierowcy->setRowCount(kierowcy.size());

    for (int i = 0; i < kierowcy.size(); ++i) {
        const auto &kierowca = kierowcy[i];
        for (int j = 0; j < kierowca.size()-1; ++j) {
            QTableWidgetItem *item = new QTableWidgetItem(kierowca[j]);
            item->setTextAlignment(Qt::AlignCenter);
            TablicaKierowcy->setItem(i, j, item);
        }
    }
}

void Kierowcy::Wyszukaj()
{
    FiltrujWyszukaj(searchByComboBox->currentIndex());
}

void Kierowcy::FiltrujWyszukaj(int index)
{
    QString tekst = searchLineEdit->text();
    przefiltrowani.clear();

    if (tekst.isEmpty())
        przefiltrowani = kierowcyLista;
    else
        if (index == 0)
        {
            for (const auto &kierowca : kierowcyLista)
                if (kierowca[0].toLower().contains(tekst.toLower()))
                    przefiltrowani.append(kierowca);
        }
        else if (index == 1)
        {
            for (const auto &kierowca : kierowcyLista)
                if (kierowca[1].toLower().contains(tekst.toLower()))
                    przefiltrowani.append(kierowca);
        }
    wyswietlKierowcy(przefiltrowani);
}