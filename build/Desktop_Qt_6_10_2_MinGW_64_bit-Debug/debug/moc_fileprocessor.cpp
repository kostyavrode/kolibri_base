/****************************************************************************
** Meta object code from reading C++ file 'fileprocessor.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/fileprocessor.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'fileprocessor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.10.2. It"
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
struct qt_meta_tag_ZN13FileProcessorE_t {};
} // unnamed namespace

template <> constexpr inline auto FileProcessor::qt_create_metaobjectdata<qt_meta_tag_ZN13FileProcessorE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "FileProcessor",
        "fileProgress",
        "",
        "filePath",
        "processedBytes",
        "totalBytes",
        "fileFinished",
        "batchFinished",
        "errorOccurred",
        "message",
        "processFiles",
        "files",
        "ProcessingConfig",
        "config"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'fileProgress'
        QtMocHelpers::SignalData<void(const QString &, qint64, qint64)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 }, { QMetaType::LongLong, 4 }, { QMetaType::LongLong, 5 },
        }}),
        // Signal 'fileFinished'
        QtMocHelpers::SignalData<void(const QString &)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
        // Signal 'batchFinished'
        QtMocHelpers::SignalData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'errorOccurred'
        QtMocHelpers::SignalData<void(const QString &)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 9 },
        }}),
        // Slot 'processFiles'
        QtMocHelpers::SlotData<void(const QStringList &, const ProcessingConfig &)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QStringList, 11 }, { 0x80000000 | 12, 13 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<FileProcessor, qt_meta_tag_ZN13FileProcessorE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject FileProcessor::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13FileProcessorE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13FileProcessorE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13FileProcessorE_t>.metaTypes,
    nullptr
} };

void FileProcessor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<FileProcessor *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->fileProgress((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<qint64>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<qint64>>(_a[3]))); break;
        case 1: _t->fileFinished((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->batchFinished(); break;
        case 3: _t->errorOccurred((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: _t->processFiles((*reinterpret_cast<std::add_pointer_t<QStringList>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<ProcessingConfig>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (FileProcessor::*)(const QString & , qint64 , qint64 )>(_a, &FileProcessor::fileProgress, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (FileProcessor::*)(const QString & )>(_a, &FileProcessor::fileFinished, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (FileProcessor::*)()>(_a, &FileProcessor::batchFinished, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (FileProcessor::*)(const QString & )>(_a, &FileProcessor::errorOccurred, 3))
            return;
    }
}

const QMetaObject *FileProcessor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FileProcessor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13FileProcessorE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int FileProcessor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void FileProcessor::fileProgress(const QString & _t1, qint64 _t2, qint64 _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2, _t3);
}

// SIGNAL 1
void FileProcessor::fileFinished(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void FileProcessor::batchFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void FileProcessor::errorOccurred(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}
QT_WARNING_POP
