/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "UI/MainWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[34];
    char stringdata0[430];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 8), // "openFile"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 12), // "openFileList"
QT_MOC_LITERAL(4, 34, 5), // "about"
QT_MOC_LITERAL(5, 40, 8), // "showHelp"
QT_MOC_LITERAL(6, 49, 4), // "exit"
QT_MOC_LITERAL(7, 54, 14), // "onFileSelected"
QT_MOC_LITERAL(8, 69, 8), // "filePath"
QT_MOC_LITERAL(9, 78, 18), // "onFileCountChanged"
QT_MOC_LITERAL(10, 97, 5), // "count"
QT_MOC_LITERAL(11, 103, 19), // "processCurrentImage"
QT_MOC_LITERAL(12, 123, 16), // "processAllImages"
QT_MOC_LITERAL(13, 140, 15), // "pauseProcessing"
QT_MOC_LITERAL(14, 156, 16), // "resumeProcessing"
QT_MOC_LITERAL(15, 173, 14), // "stopProcessing"
QT_MOC_LITERAL(16, 188, 16), // "onImageProcessed"
QT_MOC_LITERAL(17, 205, 7), // "cv::Mat"
QT_MOC_LITERAL(18, 213, 13), // "originalImage"
QT_MOC_LITERAL(19, 227, 13), // "enhancedImage"
QT_MOC_LITERAL(20, 241, 14), // "processingTime"
QT_MOC_LITERAL(21, 256, 19), // "onImageProcessError"
QT_MOC_LITERAL(22, 276, 5), // "error"
QT_MOC_LITERAL(23, 282, 20), // "onProcessingProgress"
QT_MOC_LITERAL(24, 303, 9), // "completed"
QT_MOC_LITERAL(25, 313, 5), // "total"
QT_MOC_LITERAL(26, 319, 14), // "onPreviewSaved"
QT_MOC_LITERAL(27, 334, 4), // "path"
QT_MOC_LITERAL(28, 339, 18), // "onImageInfoUpdated"
QT_MOC_LITERAL(29, 358, 12), // "originalInfo"
QT_MOC_LITERAL(30, 371, 12), // "enhancedInfo"
QT_MOC_LITERAL(31, 384, 17), // "onSettingsChanged"
QT_MOC_LITERAL(32, 402, 18), // "ProcessingSettings"
QT_MOC_LITERAL(33, 421, 8) // "settings"

    },
    "MainWindow\0openFile\0\0openFileList\0"
    "about\0showHelp\0exit\0onFileSelected\0"
    "filePath\0onFileCountChanged\0count\0"
    "processCurrentImage\0processAllImages\0"
    "pauseProcessing\0resumeProcessing\0"
    "stopProcessing\0onImageProcessed\0cv::Mat\0"
    "originalImage\0enhancedImage\0processingTime\0"
    "onImageProcessError\0error\0"
    "onProcessingProgress\0completed\0total\0"
    "onPreviewSaved\0path\0onImageInfoUpdated\0"
    "originalInfo\0enhancedInfo\0onSettingsChanged\0"
    "ProcessingSettings\0settings"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  104,    2, 0x08 /* Private */,
       3,    0,  105,    2, 0x08 /* Private */,
       4,    0,  106,    2, 0x08 /* Private */,
       5,    0,  107,    2, 0x08 /* Private */,
       6,    0,  108,    2, 0x08 /* Private */,
       7,    1,  109,    2, 0x08 /* Private */,
       9,    1,  112,    2, 0x08 /* Private */,
      11,    0,  115,    2, 0x08 /* Private */,
      12,    0,  116,    2, 0x08 /* Private */,
      13,    0,  117,    2, 0x08 /* Private */,
      14,    0,  118,    2, 0x08 /* Private */,
      15,    0,  119,    2, 0x08 /* Private */,
      16,    4,  120,    2, 0x08 /* Private */,
      21,    2,  129,    2, 0x08 /* Private */,
      23,    2,  134,    2, 0x08 /* Private */,
      26,    1,  139,    2, 0x08 /* Private */,
      28,    2,  142,    2, 0x08 /* Private */,
      31,    1,  147,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, 0x80000000 | 17, 0x80000000 | 17, QMetaType::LongLong,    8,   18,   19,   20,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    8,   22,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   24,   25,
    QMetaType::Void, QMetaType::QString,   27,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   29,   30,
    QMetaType::Void, 0x80000000 | 32,   33,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->openFile(); break;
        case 1: _t->openFileList(); break;
        case 2: _t->about(); break;
        case 3: _t->showHelp(); break;
        case 4: _t->exit(); break;
        case 5: _t->onFileSelected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->onFileCountChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->processCurrentImage(); break;
        case 8: _t->processAllImages(); break;
        case 9: _t->pauseProcessing(); break;
        case 10: _t->resumeProcessing(); break;
        case 11: _t->stopProcessing(); break;
        case 12: _t->onImageProcessed((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const cv::Mat(*)>(_a[2])),(*reinterpret_cast< const cv::Mat(*)>(_a[3])),(*reinterpret_cast< qint64(*)>(_a[4]))); break;
        case 13: _t->onImageProcessError((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 14: _t->onProcessingProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 15: _t->onPreviewSaved((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 16: _t->onImageInfoUpdated((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 17: _t->onSettingsChanged((*reinterpret_cast< const ProcessingSettings(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 18;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
