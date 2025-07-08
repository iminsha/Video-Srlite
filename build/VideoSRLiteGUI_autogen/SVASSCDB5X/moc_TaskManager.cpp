/****************************************************************************
** Meta object code from reading C++ file 'TaskManager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "Core/TaskManager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TaskManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TaskManager_t {
    QByteArrayData data[39];
    char stringdata0[524];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TaskManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TaskManager_t qt_meta_stringdata_TaskManager = {
    {
QT_MOC_LITERAL(0, 0, 11), // "TaskManager"
QT_MOC_LITERAL(1, 12, 11), // "taskCreated"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 6), // "taskId"
QT_MOC_LITERAL(4, 32, 8), // "filePath"
QT_MOC_LITERAL(5, 41, 11), // "taskStarted"
QT_MOC_LITERAL(6, 53, 13), // "taskCompleted"
QT_MOC_LITERAL(7, 67, 7), // "cv::Mat"
QT_MOC_LITERAL(8, 75, 13), // "originalImage"
QT_MOC_LITERAL(9, 89, 13), // "enhancedImage"
QT_MOC_LITERAL(10, 103, 14), // "processingTime"
QT_MOC_LITERAL(11, 118, 10), // "taskFailed"
QT_MOC_LITERAL(12, 129, 5), // "error"
QT_MOC_LITERAL(13, 135, 13), // "taskCancelled"
QT_MOC_LITERAL(14, 149, 12), // "batchStarted"
QT_MOC_LITERAL(15, 162, 10), // "totalTasks"
QT_MOC_LITERAL(16, 173, 13), // "batchProgress"
QT_MOC_LITERAL(17, 187, 9), // "completed"
QT_MOC_LITERAL(18, 197, 5), // "total"
QT_MOC_LITERAL(19, 203, 14), // "batchCompleted"
QT_MOC_LITERAL(20, 218, 14), // "batchCancelled"
QT_MOC_LITERAL(21, 233, 17), // "engineInitialized"
QT_MOC_LITERAL(22, 251, 7), // "success"
QT_MOC_LITERAL(23, 259, 11), // "engineError"
QT_MOC_LITERAL(24, 271, 17), // "processingStarted"
QT_MOC_LITERAL(25, 289, 16), // "processingPaused"
QT_MOC_LITERAL(26, 306, 17), // "processingResumed"
QT_MOC_LITERAL(27, 324, 17), // "processingStopped"
QT_MOC_LITERAL(28, 342, 15), // "startProcessing"
QT_MOC_LITERAL(29, 358, 15), // "pauseProcessing"
QT_MOC_LITERAL(30, 374, 16), // "resumeProcessing"
QT_MOC_LITERAL(31, 391, 14), // "stopProcessing"
QT_MOC_LITERAL(32, 406, 16), // "initializeEngine"
QT_MOC_LITERAL(33, 423, 16), // "onImageProcessed"
QT_MOC_LITERAL(34, 440, 17), // "onProcessingError"
QT_MOC_LITERAL(35, 458, 20), // "onProcessingProgress"
QT_MOC_LITERAL(36, 479, 7), // "current"
QT_MOC_LITERAL(37, 487, 19), // "onEngineInitialized"
QT_MOC_LITERAL(38, 507, 16) // "updateTaskStatus"

    },
    "TaskManager\0taskCreated\0\0taskId\0"
    "filePath\0taskStarted\0taskCompleted\0"
    "cv::Mat\0originalImage\0enhancedImage\0"
    "processingTime\0taskFailed\0error\0"
    "taskCancelled\0batchStarted\0totalTasks\0"
    "batchProgress\0completed\0total\0"
    "batchCompleted\0batchCancelled\0"
    "engineInitialized\0success\0engineError\0"
    "processingStarted\0processingPaused\0"
    "processingResumed\0processingStopped\0"
    "startProcessing\0pauseProcessing\0"
    "resumeProcessing\0stopProcessing\0"
    "initializeEngine\0onImageProcessed\0"
    "onProcessingError\0onProcessingProgress\0"
    "current\0onEngineInitialized\0"
    "updateTaskStatus"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TaskManager[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      25,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      15,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,  139,    2, 0x06 /* Public */,
       5,    1,  144,    2, 0x06 /* Public */,
       6,    4,  147,    2, 0x06 /* Public */,
      11,    2,  156,    2, 0x06 /* Public */,
      13,    1,  161,    2, 0x06 /* Public */,
      14,    1,  164,    2, 0x06 /* Public */,
      16,    2,  167,    2, 0x06 /* Public */,
      19,    0,  172,    2, 0x06 /* Public */,
      20,    0,  173,    2, 0x06 /* Public */,
      21,    1,  174,    2, 0x06 /* Public */,
      23,    1,  177,    2, 0x06 /* Public */,
      24,    0,  180,    2, 0x06 /* Public */,
      25,    0,  181,    2, 0x06 /* Public */,
      26,    0,  182,    2, 0x06 /* Public */,
      27,    0,  183,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      28,    0,  184,    2, 0x0a /* Public */,
      29,    0,  185,    2, 0x0a /* Public */,
      30,    0,  186,    2, 0x0a /* Public */,
      31,    0,  187,    2, 0x0a /* Public */,
      32,    0,  188,    2, 0x0a /* Public */,
      33,    4,  189,    2, 0x08 /* Private */,
      34,    2,  198,    2, 0x08 /* Private */,
      35,    2,  203,    2, 0x08 /* Private */,
      37,    1,  208,    2, 0x08 /* Private */,
      38,    0,  211,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    4,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 7, 0x80000000 | 7, QMetaType::LongLong,    3,    8,    9,   10,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,   12,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Int,   15,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   17,   18,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   22,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 7, 0x80000000 | 7, QMetaType::LongLong,    3,    8,    9,   10,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,   12,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   36,   18,
    QMetaType::Void, QMetaType::Bool,   22,
    QMetaType::Void,

       0        // eod
};

void TaskManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TaskManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->taskCreated((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 1: _t->taskStarted((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->taskCompleted((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const cv::Mat(*)>(_a[2])),(*reinterpret_cast< const cv::Mat(*)>(_a[3])),(*reinterpret_cast< qint64(*)>(_a[4]))); break;
        case 3: _t->taskFailed((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 4: _t->taskCancelled((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->batchStarted((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->batchProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->batchCompleted(); break;
        case 8: _t->batchCancelled(); break;
        case 9: _t->engineInitialized((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->engineError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 11: _t->processingStarted(); break;
        case 12: _t->processingPaused(); break;
        case 13: _t->processingResumed(); break;
        case 14: _t->processingStopped(); break;
        case 15: _t->startProcessing(); break;
        case 16: _t->pauseProcessing(); break;
        case 17: _t->resumeProcessing(); break;
        case 18: _t->stopProcessing(); break;
        case 19: _t->initializeEngine(); break;
        case 20: _t->onImageProcessed((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const cv::Mat(*)>(_a[2])),(*reinterpret_cast< const cv::Mat(*)>(_a[3])),(*reinterpret_cast< qint64(*)>(_a[4]))); break;
        case 21: _t->onProcessingError((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 22: _t->onProcessingProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 23: _t->onEngineInitialized((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 24: _t->updateTaskStatus(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TaskManager::*)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TaskManager::taskCreated)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (TaskManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TaskManager::taskStarted)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (TaskManager::*)(const QString & , const cv::Mat & , const cv::Mat & , qint64 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TaskManager::taskCompleted)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (TaskManager::*)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TaskManager::taskFailed)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (TaskManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TaskManager::taskCancelled)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (TaskManager::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TaskManager::batchStarted)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (TaskManager::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TaskManager::batchProgress)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (TaskManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TaskManager::batchCompleted)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (TaskManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TaskManager::batchCancelled)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (TaskManager::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TaskManager::engineInitialized)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (TaskManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TaskManager::engineError)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (TaskManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TaskManager::processingStarted)) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (TaskManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TaskManager::processingPaused)) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (TaskManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TaskManager::processingResumed)) {
                *result = 13;
                return;
            }
        }
        {
            using _t = void (TaskManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TaskManager::processingStopped)) {
                *result = 14;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject TaskManager::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_TaskManager.data,
    qt_meta_data_TaskManager,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TaskManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TaskManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TaskManager.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int TaskManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 25)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 25;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 25)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 25;
    }
    return _id;
}

// SIGNAL 0
void TaskManager::taskCreated(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TaskManager::taskStarted(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void TaskManager::taskCompleted(const QString & _t1, const cv::Mat & _t2, const cv::Mat & _t3, qint64 _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void TaskManager::taskFailed(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void TaskManager::taskCancelled(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void TaskManager::batchStarted(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void TaskManager::batchProgress(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void TaskManager::batchCompleted()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void TaskManager::batchCancelled()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void TaskManager::engineInitialized(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void TaskManager::engineError(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void TaskManager::processingStarted()
{
    QMetaObject::activate(this, &staticMetaObject, 11, nullptr);
}

// SIGNAL 12
void TaskManager::processingPaused()
{
    QMetaObject::activate(this, &staticMetaObject, 12, nullptr);
}

// SIGNAL 13
void TaskManager::processingResumed()
{
    QMetaObject::activate(this, &staticMetaObject, 13, nullptr);
}

// SIGNAL 14
void TaskManager::processingStopped()
{
    QMetaObject::activate(this, &staticMetaObject, 14, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
