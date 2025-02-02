/****************************************************************************
** Meta object code from reading C++ file 'Kierowcy.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../Kierowcy.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Kierowcy.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSKierowcyENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSKierowcyENDCLASS = QtMocHelpers::stringData(
    "Kierowcy",
    "Kierowcy_z_pliku",
    "",
    "wyswietlKierowcy",
    "QList<QStringList>",
    "kierowcy",
    "Sortuj",
    "index",
    "FiltrujWyszukaj",
    "Wyszukaj",
    "Filtr",
    "FiltrujFiltr",
    "kierowcaKlikniety",
    "row",
    "column",
    "update",
    "dane",
    "rok",
    "QTableWidget*",
    "tabela",
    "sezonyKierowcy",
    "wyswietlSzczegoly",
    "QVBoxLayout*",
    "layout",
    "lataMistrzostw",
    "wczytajSzczegoly",
    "driverId",
    "mistrzostwa"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSKierowcyENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   92,    2, 0x0a,    1 /* Public */,
       3,    1,   93,    2, 0x0a,    2 /* Public */,
       6,    1,   96,    2, 0x0a,    4 /* Public */,
       8,    1,   99,    2, 0x0a,    6 /* Public */,
       9,    0,  102,    2, 0x0a,    8 /* Public */,
      10,    0,  103,    2, 0x0a,    9 /* Public */,
      11,    1,  104,    2, 0x0a,   10 /* Public */,
      12,    2,  107,    2, 0x0a,   12 /* Public */,
      15,    3,  112,    2, 0x0a,   15 /* Public */,
      20,    1,  119,    2, 0x0a,   19 /* Public */,
      21,    3,  122,    2, 0x0a,   21 /* Public */,
      25,    1,  129,    2, 0x0a,   25 /* Public */,
      27,    1,  132,    2, 0x0a,   27 /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   13,   14,
    QMetaType::Void, QMetaType::QJsonObject, QMetaType::QString, 0x80000000 | 18,   16,   17,   19,
    QMetaType::QStringList, QMetaType::QJsonObject,   16,
    QMetaType::Void, 0x80000000 | 22, QMetaType::QJsonObject, QMetaType::QStringList,   23,   16,   24,
    QMetaType::QJsonObject, QMetaType::QString,   26,
    QMetaType::QStringList, QMetaType::QString,   26,

       0        // eod
};

Q_CONSTINIT const QMetaObject Kierowcy::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_CLASSKierowcyENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSKierowcyENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSKierowcyENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<Kierowcy, std::true_type>,
        // method 'Kierowcy_z_pliku'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'wyswietlKierowcy'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QList<QStringList> &, std::false_type>,
        // method 'Sortuj'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'FiltrujWyszukaj'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'Wyszukaj'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'Filtr'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'FiltrujFiltr'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'kierowcaKlikniety'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'update'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QJsonObject &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        QtPrivate::TypeAndForceComplete<QTableWidget *, std::false_type>,
        // method 'sezonyKierowcy'
        QtPrivate::TypeAndForceComplete<QStringList, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QJsonObject &, std::false_type>,
        // method 'wyswietlSzczegoly'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QVBoxLayout *, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QJsonObject &, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QStringList &, std::false_type>,
        // method 'wczytajSzczegoly'
        QtPrivate::TypeAndForceComplete<QJsonObject, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'mistrzostwa'
        QtPrivate::TypeAndForceComplete<QStringList, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>
    >,
    nullptr
} };

void Kierowcy::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Kierowcy *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->Kierowcy_z_pliku(); break;
        case 1: _t->wyswietlKierowcy((*reinterpret_cast< std::add_pointer_t<QList<QStringList>>>(_a[1]))); break;
        case 2: _t->Sortuj((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->FiltrujWyszukaj((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->Wyszukaj(); break;
        case 5: _t->Filtr(); break;
        case 6: _t->FiltrujFiltr((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 7: _t->kierowcaKlikniety((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 8: _t->update((*reinterpret_cast< std::add_pointer_t<QJsonObject>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QTableWidget*>>(_a[3]))); break;
        case 9: { QStringList _r = _t->sezonyKierowcy((*reinterpret_cast< std::add_pointer_t<QJsonObject>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = std::move(_r); }  break;
        case 10: _t->wyswietlSzczegoly((*reinterpret_cast< std::add_pointer_t<QVBoxLayout*>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<QJsonObject>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<QStringList>>(_a[3]))); break;
        case 11: { QJsonObject _r = _t->wczytajSzczegoly((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QJsonObject*>(_a[0]) = std::move(_r); }  break;
        case 12: { QStringList _r = _t->mistrzostwa((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])));
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<QStringList> >(); break;
            }
            break;
        case 8:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 2:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QTableWidget* >(); break;
            }
            break;
        case 10:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QVBoxLayout* >(); break;
            }
            break;
        }
    }
}

const QMetaObject *Kierowcy::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Kierowcy::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSKierowcyENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Kierowcy::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    return _id;
}
QT_WARNING_POP
