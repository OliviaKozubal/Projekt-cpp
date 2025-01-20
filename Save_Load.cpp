#include "Save_Load.h"

void Save(const QList<QStringList> &mistrzowie) {
    QJsonArray jsonArray;

    for (const auto &mistrz : mistrzowie) {
        QJsonObject jsonObject;
        jsonObject["driverid"] = mistrz[0];
        jsonArray.append(jsonObject);
    }

    QJsonDocument doc(jsonArray);

    QFile file("C:/Users/Holin/Desktop/Studia/StudiaRok2/Semestr3/ProjektC++/Projekt-cpp/test.json");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    } else {
        qWarning() << "Nie udało się otworzyć pliku do zapisu!";
    }
}
