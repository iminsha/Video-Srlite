/****************************************************************************
** Meta object code from reading C++ file 'ImagePreviewWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "UI/ImagePreviewWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ImagePreviewWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ImagePreviewWidget_t {
    QByteArrayData data[13];
    char stringdata0[158];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ImagePreviewWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ImagePreviewWidget_t qt_meta_stringdata_ImagePreviewWidget = {
    {
QT_MOC_LITERAL(0, 0, 18), // "ImagePreviewWidget"
QT_MOC_LITERAL(1, 19, 12), // "previewSaved"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 4), // "path"
QT_MOC_LITERAL(4, 38, 16), // "imageInfoUpdated"
QT_MOC_LITERAL(5, 55, 12), // "originalInfo"
QT_MOC_LITERAL(6, 68, 12), // "enhancedInfo"
QT_MOC_LITERAL(7, 81, 13), // "onZoomChanged"
QT_MOC_LITERAL(8, 95, 8), // "zoomText"
QT_MOC_LITERAL(9, 104, 20), // "onCompareModeChanged"
QT_MOC_LITERAL(10, 125, 5), // "index"
QT_MOC_LITERAL(11, 131, 14), // "refreshPreview"
QT_MOC_LITERAL(12, 146, 11) // "savePreview"

    },
    "ImagePreviewWidget\0previewSaved\0\0path\0"
    "imageInfoUpdated\0originalInfo\0"
    "enhancedInfo\0onZoomChanged\0zoomText\0"
    "onCompareModeChanged\0index\0refreshPreview\0"
    "savePreview"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ImagePreviewWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       4,    2,   47,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    1,   52,    2, 0x0a /* Public */,
       9,    1,   55,    2, 0x0a /* Public */,
      11,    0,   58,    2, 0x0a /* Public */,
      12,    0,   59,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    5,    6,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ImagePreviewWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ImagePreviewWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->previewSaved((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->imageInfoUpdated((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: _t->onZoomChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->onCompareModeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->refreshPreview(); break;
        case 5: _t->savePreview(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ImagePreviewWidget::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImagePreviewWidget::previewSaved)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ImagePreviewWidget::*)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImagePreviewWidget::imageInfoUpdated)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ImagePreviewWidget::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_ImagePreviewWidget.data,
    qt_meta_data_ImagePreviewWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ImagePreviewWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ImagePreviewWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ImagePreviewWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ImagePreviewWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void ImagePreviewWidget::previewSaved(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ImagePreviewWidget::imageInfoUpdated(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
