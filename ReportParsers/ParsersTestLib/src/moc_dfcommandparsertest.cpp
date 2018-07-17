/****************************************************************************
** Meta object code from reading C++ file 'dfcommandparsertest.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "dfcommandparsertest.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dfcommandparsertest.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_DfCommandParserTest_t {
    QByteArrayData data[8];
    char stringdata[103];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_DfCommandParserTest_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_DfCommandParserTest_t qt_meta_stringdata_DfCommandParserTest = {
    {
QT_MOC_LITERAL(0, 0, 19),
QT_MOC_LITERAL(1, 20, 4),
QT_MOC_LITERAL(2, 25, 0),
QT_MOC_LITERAL(3, 26, 7),
QT_MOC_LITERAL(4, 34, 16),
QT_MOC_LITERAL(5, 51, 11),
QT_MOC_LITERAL(6, 63, 21),
QT_MOC_LITERAL(7, 85, 16)
    },
    "DfCommandParserTest\0init\0\0cleanup\0"
    "testRawData_data\0testRawData\0"
    "testDescriptions_data\0testDescriptions\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DfCommandParserTest[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x08,
       3,    0,   45,    2, 0x08,
       4,    0,   46,    2, 0x08,
       5,    0,   47,    2, 0x08,
       6,    0,   48,    2, 0x08,
       7,    0,   49,    2, 0x08,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void DfCommandParserTest::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DfCommandParserTest *_t = static_cast<DfCommandParserTest *>(_o);
        switch (_id) {
        case 0: _t->init(); break;
        case 1: _t->cleanup(); break;
        case 2: _t->testRawData_data(); break;
        case 3: _t->testRawData(); break;
        case 4: _t->testDescriptions_data(); break;
        case 5: _t->testDescriptions(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject DfCommandParserTest::staticMetaObject = {
    { &QtTestSuite::staticMetaObject, qt_meta_stringdata_DfCommandParserTest.data,
      qt_meta_data_DfCommandParserTest,  qt_static_metacall, 0, 0}
};


const QMetaObject *DfCommandParserTest::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DfCommandParserTest::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DfCommandParserTest.stringdata))
        return static_cast<void*>(const_cast< DfCommandParserTest*>(this));
    return QtTestSuite::qt_metacast(_clname);
}

int DfCommandParserTest::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QtTestSuite::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
