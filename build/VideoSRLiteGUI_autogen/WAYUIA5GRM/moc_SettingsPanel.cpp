/****************************************************************************
** Meta object code from reading C++ file 'SettingsPanel.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "UI/SettingsPanel.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SettingsPanel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SettingsPanel_t {
    QByteArrayData data[20];
    char stringdata0[301];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SettingsPanel_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SettingsPanel_t qt_meta_stringdata_SettingsPanel = {
    {
QT_MOC_LITERAL(0, 0, 13), // "SettingsPanel"
QT_MOC_LITERAL(1, 14, 15), // "settingsChanged"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 18), // "ProcessingSettings"
QT_MOC_LITERAL(4, 50, 8), // "settings"
QT_MOC_LITERAL(5, 59, 12), // "presetLoaded"
QT_MOC_LITERAL(6, 72, 10), // "presetName"
QT_MOC_LITERAL(7, 83, 11), // "presetSaved"
QT_MOC_LITERAL(8, 95, 20), // "onScaleFactorChanged"
QT_MOC_LITERAL(9, 116, 6), // "factor"
QT_MOC_LITERAL(10, 123, 24), // "onDenoiseStrengthChanged"
QT_MOC_LITERAL(11, 148, 8), // "strength"
QT_MOC_LITERAL(12, 157, 24), // "onSharpenStrengthChanged"
QT_MOC_LITERAL(13, 182, 19), // "onOutputPathChanged"
QT_MOC_LITERAL(14, 202, 21), // "onOutputFormatChanged"
QT_MOC_LITERAL(15, 224, 6), // "format"
QT_MOC_LITERAL(16, 231, 14), // "onApplyClicked"
QT_MOC_LITERAL(17, 246, 14), // "onResetClicked"
QT_MOC_LITERAL(18, 261, 19), // "onSavePresetClicked"
QT_MOC_LITERAL(19, 281, 19) // "onLoadPresetClicked"

    },
    "SettingsPanel\0settingsChanged\0\0"
    "ProcessingSettings\0settings\0presetLoaded\0"
    "presetName\0presetSaved\0onScaleFactorChanged\0"
    "factor\0onDenoiseStrengthChanged\0"
    "strength\0onSharpenStrengthChanged\0"
    "onOutputPathChanged\0onOutputFormatChanged\0"
    "format\0onApplyClicked\0onResetClicked\0"
    "onSavePresetClicked\0onLoadPresetClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SettingsPanel[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x06 /* Public */,
       5,    1,   77,    2, 0x06 /* Public */,
       7,    1,   80,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    1,   83,    2, 0x0a /* Public */,
      10,    1,   86,    2, 0x0a /* Public */,
      12,    1,   89,    2, 0x0a /* Public */,
      13,    0,   92,    2, 0x0a /* Public */,
      14,    1,   93,    2, 0x0a /* Public */,
      16,    0,   96,    2, 0x0a /* Public */,
      17,    0,   97,    2, 0x0a /* Public */,
      18,    0,   98,    2, 0x0a /* Public */,
      19,    0,   99,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::QString,    6,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void, QMetaType::Double,   11,
    QMetaType::Void, QMetaType::Double,   11,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   15,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void SettingsPanel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SettingsPanel *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->settingsChanged((*reinterpret_cast< const ProcessingSettings(*)>(_a[1]))); break;
        case 1: _t->presetLoaded((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->presetSaved((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->onScaleFactorChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->onDenoiseStrengthChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 5: _t->onSharpenStrengthChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 6: _t->onOutputPathChanged(); break;
        case 7: _t->onOutputFormatChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->onApplyClicked(); break;
        case 9: _t->onResetClicked(); break;
        case 10: _t->onSavePresetClicked(); break;
        case 11: _t->onLoadPresetClicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SettingsPanel::*)(const ProcessingSettings & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SettingsPanel::settingsChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SettingsPanel::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SettingsPanel::presetLoaded)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SettingsPanel::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SettingsPanel::presetSaved)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SettingsPanel::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_SettingsPanel.data,
    qt_meta_data_SettingsPanel,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SettingsPanel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SettingsPanel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SettingsPanel.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int SettingsPanel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void SettingsPanel::settingsChanged(const ProcessingSettings & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SettingsPanel::presetLoaded(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SettingsPanel::presetSaved(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
