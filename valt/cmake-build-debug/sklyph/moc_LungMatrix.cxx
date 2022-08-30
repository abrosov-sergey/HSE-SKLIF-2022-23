/****************************************************************************
** Meta object code from reading C++ file 'LungMatrix.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../sklyph/LungMatrix.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LungMatrix.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_sklyph__valt__LungMatrix[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      25,   42,   42,   42, 0x05,

 // slots: signature, parameters, type, tag, flags
      43,   76,   42,   42, 0x0a,
     108,  138,   42,   42, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_sklyph__valt__LungMatrix[] = {
    "sklyph::valt::LungMatrix\0volumesChanged()\0"
    "\0setInfectionMatrix(int,GcellSet)\0"
    "sliceIndex,sliceInfectionMatrix\0"
    "setLungTissueMatrix(GcellSet)\0"
    "lungTissueMatrix\0"
};

void sklyph::valt::LungMatrix::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        LungMatrix *_t = static_cast<LungMatrix *>(_o);
        switch (_id) {
        case 0: _t->volumesChanged(); break;
        case 1: _t->setInfectionMatrix((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const GcellSet(*)>(_a[2]))); break;
        case 2: _t->setLungTissueMatrix((*reinterpret_cast< const GcellSet(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData sklyph::valt::LungMatrix::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject sklyph::valt::LungMatrix::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_sklyph__valt__LungMatrix,
      qt_meta_data_sklyph__valt__LungMatrix, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &sklyph::valt::LungMatrix::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *sklyph::valt::LungMatrix::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *sklyph::valt::LungMatrix::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_sklyph__valt__LungMatrix))
        return static_cast<void*>(const_cast< LungMatrix*>(this));
    return QObject::qt_metacast(_clname);
}

int sklyph::valt::LungMatrix::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void sklyph::valt::LungMatrix::volumesChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
