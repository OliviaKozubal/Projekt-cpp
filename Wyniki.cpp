#include "Wyniki.h"

Wyniki::Wyniki(QWidget *parent) : QWidget(parent) {
    wyszukiwarka = new QLineEdit(this);
    wyszukiwarka->setPlaceholderText("Wyszukaj... ");
    tablica = new QTableWidget(this);
    tablica->setColumnCount(2);
    tablica->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(wyszukiwarka);
    layout->addWidget(tablica);
    setLayout(layout);

    connect(wyszukiwarka, &QLineEdit::textChanged, this, &Wyniki::filtruj);
    connect(tablica, &QTableWidget::cellClicked, this, &Wyniki::wyswietl);

    QString sciezka = QCoreApplication::applicationDirPath() + "/rankingi.json";
    wczytaj(sciezka);
}

void Wyniki::wczytaj(const QString &sciezka) {
    QFile file(sciezka);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

    dane = jsonDoc.object();
    wypelnij();
}

void Wyniki::wypelnij() {
    tablica->setRowCount(0);
    tablica->setColumnCount(2);

    QList<int> lata;
    for (const QString &key : dane.keys()) {
        lata.append(key.toInt());
    }
    std::sort(lata.begin(), lata.end());

    int c = 2;
    int r = (lata.size() + c - 1) / c;

    tablica->setRowCount(r);

    QFont font = tablica->font();
    font.setPointSize(12);
    tablica->setFont(font);
    tablica->setStyleSheet("QTableWidget { text-align: center; }");

    for (int i = 0; i < lata.size(); ++i) {
        int r = i / c;
        int col = i % c;
        QTableWidgetItem *item = new QTableWidgetItem(QString::number(lata[i]));
        item->setTextAlignment(Qt::AlignCenter);
        tablica->setItem(r, col, item);
    }

    QFont headerFont = tablica->horizontalHeader()->font();
    headerFont.setBold(true);
    tablica->horizontalHeader()->setFont(headerFont);

    tablica->horizontalHeader()->hide();
    tablica->verticalHeader()->setVisible(false);
}

void Wyniki::filtruj() {
    QString filter = wyszukiwarka->text();
    for (int r = 0; r < tablica->rowCount(); ++r) {
        bool dopasowanie = false;
        for (int col = 0; col < tablica->columnCount(); ++col) {
            QTableWidgetItem *item = tablica->item(r, col);
            if (item && item->text().contains(filter, Qt::CaseInsensitive)) {
                dopasowanie = true;
                break;
            }
        }
        tablica->setRowHidden(r, !dopasowanie && !filter.isEmpty());
    }
}

void Wyniki::wyswietl(int r, int c) {
    QTableWidgetItem *item = tablica->item(r, c);
    if (!item) return;

    QString rok = item->text();

    QJsonArray ranking = dane.value(rok).toArray();
    wyswietlInfo(rok, ranking);
}

void Wyniki::wyswietlInfo(const QString &rok, const QJsonArray &ranking) {
    QWidget *szczegolyOkno = new QWidget;
    szczegolyOkno->setWindowTitle("Ranking dla roku " + rok);
    szczegolyOkno->resize(800, 600);

    QTableWidget *szczegolyTablica = new QTableWidget(szczegolyOkno);
    szczegolyTablica->setColumnCount(4);
    szczegolyTablica->setHorizontalHeaderLabels({"Pozycja", "Imię i nazwisko", "Punkty", "Zespół"});
    szczegolyTablica->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    szczegolyTablica->verticalHeader()->setVisible(false);
    szczegolyTablica->setRowCount(ranking.size());

    QFont szczegolyFont = szczegolyTablica->font();
    szczegolyFont.setPointSize(12);
    szczegolyTablica->setFont(szczegolyFont);
    szczegolyTablica->setStyleSheet("QTableWidget { text-align: center; }");

    QFont grubyFont = szczegolyTablica->horizontalHeader()->font();
    grubyFont.setBold(true);
    szczegolyTablica->horizontalHeader()->setFont(grubyFont);

    for (int i = 0; i < ranking.size(); ++i) {
        QJsonObject wpis = ranking[i].toObject();
        szczegolyTablica->setItem(i, 0, new QTableWidgetItem(wpis["position"].toString()));
        szczegolyTablica->setItem(i, 1, new QTableWidgetItem(wpis["driver"].toString()));
        szczegolyTablica->setItem(i, 2, new QTableWidgetItem(wpis["points"].toString()));
        szczegolyTablica->setItem(i, 3, new QTableWidgetItem(wpis["team"].toString()));
    }

    for (int r = 0; r < szczegolyTablica->rowCount(); ++r) {
        for (int col = 0; col < szczegolyTablica->columnCount(); ++col) {
            QTableWidgetItem *item = szczegolyTablica->item(r, col);
            if (item)
                item->setTextAlignment(Qt::AlignCenter);
        }
    }

    QVBoxLayout *layout = new QVBoxLayout(szczegolyOkno);
    layout->addWidget(szczegolyTablica);
    szczegolyOkno->setLayout(layout);

    szczegolyOkno->showMaximized();
}