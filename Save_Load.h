#ifndef SAVE_LOAD_H
#define SAVE_LOAD_H

#include <QList>
#include <QStringList>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTextStream>

void Save(const QList<QStringList> &mistrzowie);
QList<QStringList> Load();
void initdata();

#endif // SAVE_LOAD_H