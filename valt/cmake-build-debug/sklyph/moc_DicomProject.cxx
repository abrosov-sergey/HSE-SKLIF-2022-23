/****************************************************************************
** Meta object code from reading C++ file 'DicomProject.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../sklyph/DicomProject.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DicomProject.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_sklyph__valt__DicomProject[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      27,   42,   42,   42, 0x05,

 // slots: signature, parameters, type, tag, flags
      43,   42,   52,   42, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_sklyph__valt__DicomProject[] = {
    "sklyph::valt::DicomProject\0loadFinished()\0"
    "\0reload()\0bool\0"
};

void sklyph::valt::DicomProject::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DicomProject *_t = static_cast<DicomProject *>(_o);
        switch (_id) {
        case 0: _t->loadFinished(); break;
        case 1: { bool _r = _t->reload();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData sklyph::valt::DicomProject::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject sklyph::valt::DicomProject::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_sklyph__valt__DicomProject,
      qt_meta_data_sklyph__valt__DicomProject, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &sklyph::valt::DicomProject::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *sklyph::valt::DicomProject::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *sklyph::valt::DicomProject::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_sklyph__valt__DicomProject))
        return static_cast<void*>(const_cast< DicomProject*>(this));
    return QObject::qt_metacast(_clname);
}

int sklyph::valt::DicomProject::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void sklyph::valt::DicomProject::loadFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
