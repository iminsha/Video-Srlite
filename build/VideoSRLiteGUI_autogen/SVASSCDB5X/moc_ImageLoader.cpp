/****************************************************************************
** Meta object code from reading C++ file 'ImageLoader.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "Core/ImageLoader.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ImageLoader.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ImageLoadTask_t {
    QByteArrayData data[9];
    char stringdata0[74];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ImageLoadTask_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ImageLoadTask_t qt_meta_stringdata_ImageLoadTask = {
    {
QT_MOC_LITERAL(0, 0, 13), // "ImageLoadTask"
QT_MOC_LITERAL(1, 14, 11), // "imageLoaded"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 8), // "filePath"
QT_MOC_LITERAL(4, 36, 7), // "cv::Mat"
QT_MOC_LITERAL(5, 44, 5), // "image"
QT_MOC_LITERAL(6, 50, 6), // "pixmap"
QT_MOC_LITERAL(7, 57, 10), // "loadFailed"
QT_MOC_LITERAL(8, 68, 5) // "error"

    },
    "ImageLoadTask\0imageLoaded\0\0filePath\0"
    "cv::Mat\0image\0pixmap\0loadFailed\0error"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ImageLoadTask[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   24,    2, 0x06 /* Public */,
       7,    2,   31,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, 0x80000000 | 4, QMetaType::QPixmap,    3,    5,    6,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    8,

       0        // eod
};

void ImageLoadTask::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ImageLoadTask *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->imageLoaded((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const cv::Mat(*)>(_a[2])),(*reinterpret_cast< const QPixmap(*)>(_a[3]))); break;
        case 1: _t->loadFailed((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ImageLoadTask::*)(const QString & , const cv::Mat & , const QPixmap & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageLoadTask::imageLoaded)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ImageLoadTask::*)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageLoadTask::loadFailed)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ImageLoadTask::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_ImageLoadTask.data,
    qt_meta_data_ImageLoadTask,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ImageLoadTask::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ImageLoadTask::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ImageLoadTask.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "QRunnable"))
        return static_cast< QRunnable*>(this);
    return QObject::qt_metacast(_clname);
}

int ImageLoadTask::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void ImageLoadTask::imageLoaded(const QString & _t1, const cv::Mat & _t2, const QPixmap & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ImageLoadTask::loadFailed(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
struct qt_meta_stringdata_ImageLoader_t {
    QByteArrayData data[23];
    char stringdata0[235];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ImageLoader_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ImageLoader_t qt_meta_stringdata_ImageLoader = {
    {
QT_MOC_LITERAL(0, 0, 11), // "ImageLoader"
QT_MOC_LITERAL(1, 12, 10), // "imageReady"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 8), // "filePath"
QT_MOC_LITERAL(4, 33, 7), // "cv::Mat"
QT_MOC_LITERAL(5, 41, 5), // "image"
QT_MOC_LITERAL(6, 47, 11), // "pixmapReady"
QT_MOC_LITERAL(7, 59, 6), // "pixmap"
QT_MOC_LITERAL(8, 66, 14), // "imageDataReady"
QT_MOC_LITERAL(9, 81, 9), // "ImageData"
QT_MOC_LITERAL(10, 91, 4), // "data"
QT_MOC_LITERAL(11, 96, 9), // "loadError"
QT_MOC_LITERAL(12, 106, 5), // "error"
QT_MOC_LITERAL(13, 112, 11), // "loadStarted"
QT_MOC_LITERAL(14, 124, 12), // "loadProgress"
QT_MOC_LITERAL(15, 137, 10), // "percentage"
QT_MOC_LITERAL(16, 148, 12), // "cacheUpdated"
QT_MOC_LITERAL(17, 161, 4), // "size"
QT_MOC_LITERAL(18, 166, 11), // "memoryUsage"
QT_MOC_LITERAL(19, 178, 13), // "onImageLoaded"
QT_MOC_LITERAL(20, 192, 12), // "onLoadFailed"
QT_MOC_LITERAL(21, 205, 12), // "cleanupCache"
QT_MOC_LITERAL(22, 218, 16) // "updateCacheStats"

    },
    "ImageLoader\0imageReady\0\0filePath\0"
    "cv::Mat\0image\0pixmapReady\0pixmap\0"
    "imageDataReady\0ImageData\0data\0loadError\0"
    "error\0loadStarted\0loadProgress\0"
    "percentage\0cacheUpdated\0size\0memoryUsage\0"
    "onImageLoaded\0onLoadFailed\0cleanupCache\0"
    "updateCacheStats"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ImageLoader[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   69,    2, 0x06 /* Public */,
       6,    2,   74,    2, 0x06 /* Public */,
       8,    2,   79,    2, 0x06 /* Public */,
      11,    2,   84,    2, 0x06 /* Public */,
      13,    1,   89,    2, 0x06 /* Public */,
      14,    2,   92,    2, 0x06 /* Public */,
      16,    2,   97,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      19,    3,  102,    2, 0x0a /* Public */,
      20,    2,  109,    2, 0x0a /* Public */,
      21,    0,  114,    2, 0x08 /* Private */,
      22,    0,  115,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, 0x80000000 | 4,    3,    5,
    QMetaType::Void, QMetaType::QString, QMetaType::QPixmap,    3,    7,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 9,    3,   10,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,   12,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,    3,   15,
    QMetaType::Void, QMetaType::Int, QMetaType::LongLong,   17,   18,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, 0x80000000 | 4, QMetaType::QPixmap,    3,    5,    7,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,   12,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ImageLoader::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ImageLoader *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->imageReady((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const cv::Mat(*)>(_a[2]))); break;
        case 1: _t->pixmapReady((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QPixmap(*)>(_a[2]))); break;
        case 2: _t->imageDataReady((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const ImageData(*)>(_a[2]))); break;
        case 3: _t->loadError((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 4: _t->loadStarted((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->loadProgress((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: _t->cacheUpdated((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 7: _t->onImageLoaded((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const cv::Mat(*)>(_a[2])),(*reinterpret_cast< const QPixmap(*)>(_a[3]))); break;
        case 8: _t->onLoadFailed((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 9: _t->cleanupCache(); break;
        case 10: _t->updateCacheStats(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ImageLoader::*)(const QString & , const cv::Mat & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageLoader::imageReady)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ImageLoader::*)(const QString & , const QPixmap & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageLoader::pixmapReady)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ImageLoader::*)(const QString & , const ImageData & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageLoader::imageDataReady)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (ImageLoader::*)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageLoader::loadError)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (ImageLoader::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageLoader::loadStarted)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (ImageLoader::*)(const QString & , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageLoader::loadProgress)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (ImageLoader::*)(int , qint64 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImageLoader::cacheUpdated)) {
                *result = 6;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ImageLoader::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_ImageLoader.data,
    qt_meta_data_ImageLoader,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ImageLoader::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ImageLoader::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ImageLoader.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ImageLoader::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void ImageLoader::imageReady(const QString & _t1, const cv::Mat & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ImageLoader::pixmapReady(const QString & _t1, const QPixmap & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ImageLoader::imageDataReady(const QString & _t1, const ImageData & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ImageLoader::loadError(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void ImageLoader::loadStarted(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void ImageLoader::loadProgress(const QString & _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void ImageLoader::cacheUpdated(int _t1, qint64 _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
struct qt_meta_stringdata_ImagePreviewManager_t {
    QByteArrayData data[13];
    char stringdata0[135];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ImagePreviewManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ImagePreviewManager_t qt_meta_stringdata_ImagePreviewManager = {
    {
QT_MOC_LITERAL(0, 0, 19), // "ImagePreviewManager"
QT_MOC_LITERAL(1, 20, 12), // "previewReady"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 8), // "filePath"
QT_MOC_LITERAL(4, 43, 7), // "preview"
QT_MOC_LITERAL(5, 51, 14), // "thumbnailReady"
QT_MOC_LITERAL(6, 66, 9), // "thumbnail"
QT_MOC_LITERAL(7, 76, 12), // "previewError"
QT_MOC_LITERAL(8, 89, 5), // "error"
QT_MOC_LITERAL(9, 95, 12), // "onImageReady"
QT_MOC_LITERAL(10, 108, 7), // "cv::Mat"
QT_MOC_LITERAL(11, 116, 5), // "image"
QT_MOC_LITERAL(12, 122, 12) // "onImageError"

    },
    "ImagePreviewManager\0previewReady\0\0"
    "filePath\0preview\0thumbnailReady\0"
    "thumbnail\0previewError\0error\0onImageReady\0"
    "cv::Mat\0image\0onImageError"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ImagePreviewManager[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   39,    2, 0x06 /* Public */,
       5,    2,   44,    2, 0x06 /* Public */,
       7,    2,   49,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    2,   54,    2, 0x08 /* Private */,
      12,    2,   59,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QPixmap,    3,    4,
    QMetaType::Void, QMetaType::QString, QMetaType::QPixmap,    3,    6,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    8,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, 0x80000000 | 10,    3,   11,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    8,

       0        // eod
};

void ImagePreviewManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ImagePreviewManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->previewReady((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QPixmap(*)>(_a[2]))); break;
        case 1: _t->thumbnailReady((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QPixmap(*)>(_a[2]))); break;
        case 2: _t->previewError((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 3: _t->onImageReady((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const cv::Mat(*)>(_a[2]))); break;
        case 4: _t->onImageError((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ImagePreviewManager::*)(const QString & , const QPixmap & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImagePreviewManager::previewReady)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ImagePreviewManager::*)(const QString & , const QPixmap & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImagePreviewManager::thumbnailReady)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (ImagePreviewManager::*)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ImagePreviewManager::previewError)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ImagePreviewManager::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_ImagePreviewManager.data,
    qt_meta_data_ImagePreviewManager,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ImagePreviewManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ImagePreviewManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ImagePreviewManager.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int ImagePreviewManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void ImagePreviewManager::previewReady(const QString & _t1, const QPixmap & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ImagePreviewManager::thumbnailReady(const QString & _t1, const QPixmap & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ImagePreviewManager::previewError(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
