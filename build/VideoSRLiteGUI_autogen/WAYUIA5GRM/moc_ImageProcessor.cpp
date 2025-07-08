/****************************************************************************
** Meta object code from reading C++ file 'ImageProcessor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "UI/ImageProcessor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ImageProcessor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ImageProcessor_t {
    QByteArrayData data[20];
    char stringdata0[257];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ImageProcessor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ImageProcessor_t qt_meta_stringdata_ImageProcessor = {
    {
QT_MOC_LITERAL(0, 0, 14), // "ImageProcessor"
QT_MOC_LITERAL(1, 15, 14), // "imageProcessed"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 6), // "taskId"
QT_MOC_LITERAL(4, 38, 7), // "cv::Mat"
QT_MOC_LITERAL(5, 46, 13), // "originalImage"
QT_MOC_LITERAL(6, 60, 13), // "enhancedImage"
QT_MOC_LITERAL(7, 74, 14), // "processingTime"
QT_MOC_LITERAL(8, 89, 15), // "processingError"
QT_MOC_LITERAL(9, 105, 5), // "error"
QT_MOC_LITERAL(10, 111, 18), // "processingProgress"
QT_MOC_LITERAL(11, 130, 7), // "current"
QT_MOC_LITERAL(12, 138, 5), // "total"
QT_MOC_LITERAL(13, 144, 17), // "engineInitialized"
QT_MOC_LITERAL(14, 162, 7), // "success"
QT_MOC_LITERAL(15, 170, 17), // "processingStarted"
QT_MOC_LITERAL(16, 188, 17), // "processingStopped"
QT_MOC_LITERAL(17, 206, 16), // "processingPaused"
QT_MOC_LITERAL(18, 223, 17), // "processingResumed"
QT_MOC_LITERAL(19, 241, 15) // "processNextTask"

    },
    "ImageProcessor\0imageProcessed\0\0taskId\0"
    "cv::Mat\0originalImage\0enhancedImage\0"
    "processingTime\0processingError\0error\0"
    "processingProgress\0current\0total\0"
    "engineInitialized\0success\0processingStarted\0"
    "processingStopped\0processingPaused\0"
    "processingResumed\0processNextTask"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ImageProcessor[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,   59,    2, 0x06 /* Public */,
       8,    2,   68,    2, 0x06 /* Public */,
      10,    2,   73,    2, 0x06 /* Public */,
      13,    1,   78,    2, 0x06 /* Public */,
      15,    0,   81,    2, 0x06 /* Public */,
      16,    0,   82,    2, 0x06 /* Public */,
      17,    0,   83,    2, 0x06 /* Public */,
      18,    0,   84,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      19,    0,   85,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, 0x80000000 | 4, 0x80000000 | 4, QMetaType::LongLong,    3,    5,    6,    7,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    9,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   11,   12,
    QMetaType::Void, QMetaType::Bool,   14,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void ImageProcessor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ImageProcessor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->imageProcessed((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const cv::Mat(*)>(_a[2])),(*reinterpret_cast< const cv::Mat(*)>(_a[3])),(*reinterpret_cast< qint64(*)>(_a[4]))); break;
        case 1: _t->processingError((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: _t->processingProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->engineInitialized((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->processingStarted(); break;
        case 5: _t->processingStopped(); break;
        case 6: _t->processingPaused(); break;
        case 7: _t->processingResumed(); break;
        case 8: _t->processNextTask(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ImageProcessor::*)(const QString & , const cv::Mat & , const cv::Mat & , qint64 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageProcessor::imageProcessed)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ImageProcessor::*)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageProcessor::processingError)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ImageProcessor::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageProcessor::processingProgress)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (ImageProcessor::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageProcessor::engineInitialized)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (ImageProcessor::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageProcessor::processingStarted)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (ImageProcessor::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageProcessor::processingStopped)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (ImageProcessor::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageProcessor::processingPaused)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (ImageProcessor::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageProcessor::processingResumed)) {
                *result = 7;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ImageProcessor::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_ImageProcessor.data,
    qt_meta_data_ImageProcessor,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ImageProcessor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ImageProcessor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ImageProcessor.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ImageProcessor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void ImageProcessor::imageProcessed(const QString & _t1, const cv::Mat & _t2, const cv::Mat & _t3, qint64 _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ImageProcessor::processingError(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ImageProcessor::processingProgress(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ImageProcessor::engineInitialized(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void ImageProcessor::processingStarted()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void ImageProcessor::processingStopped()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void ImageProcessor::processingPaused()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void ImageProcessor::processingResumed()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
