/****************************************************************************
** Meta object code from reading C++ file 'graphDSL.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../graphDSL.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'graphDSL.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.2. It"
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
struct qt_meta_tag_ZN19GraphDSLInterpreterE_t {};
} // unnamed namespace

template <> constexpr inline auto GraphDSLInterpreter::qt_create_metaobjectdata<qt_meta_tag_ZN19GraphDSLInterpreterE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "GraphDSLInterpreter"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<GraphDSLInterpreter, qt_meta_tag_ZN19GraphDSLInterpreterE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject GraphDSLInterpreter::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN19GraphDSLInterpreterE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN19GraphDSLInterpreterE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN19GraphDSLInterpreterE_t>.metaTypes,
    nullptr
} };

void GraphDSLInterpreter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<GraphDSLInterpreter *>(_o);
    (void)_t;
    (void)_c;
    (void)_id;
    (void)_a;
}

const QMetaObject *GraphDSLInterpreter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GraphDSLInterpreter::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN19GraphDSLInterpreterE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int GraphDSLInterpreter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    return _id;
}
namespace {
struct qt_meta_tag_ZN25GraphDSLSyntaxHighlighterE_t {};
} // unnamed namespace

template <> constexpr inline auto GraphDSLSyntaxHighlighter::qt_create_metaobjectdata<qt_meta_tag_ZN25GraphDSLSyntaxHighlighterE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "GraphDSLSyntaxHighlighter"
    };

    QtMocHelpers::UintData qt_methods {
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<GraphDSLSyntaxHighlighter, qt_meta_tag_ZN25GraphDSLSyntaxHighlighterE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject GraphDSLSyntaxHighlighter::staticMetaObject = { {
    QMetaObject::SuperData::link<QSyntaxHighlighter::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN25GraphDSLSyntaxHighlighterE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN25GraphDSLSyntaxHighlighterE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN25GraphDSLSyntaxHighlighterE_t>.metaTypes,
    nullptr
} };

void GraphDSLSyntaxHighlighter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<GraphDSLSyntaxHighlighter *>(_o);
    (void)_t;
    (void)_c;
    (void)_id;
    (void)_a;
}

const QMetaObject *GraphDSLSyntaxHighlighter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GraphDSLSyntaxHighlighter::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN25GraphDSLSyntaxHighlighterE_t>.strings))
        return static_cast<void*>(this);
    return QSyntaxHighlighter::qt_metacast(_clname);
}

int GraphDSLSyntaxHighlighter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSyntaxHighlighter::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
