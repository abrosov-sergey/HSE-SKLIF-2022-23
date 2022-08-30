/****************************************************************************
** Meta object code from reading C++ file 'DicomGriddedViewWidget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../sklyph/DicomGriddedViewWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DicomGriddedViewWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_sklyph__valt__DicomGriddedViewWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      37,   64,   71,   71, 0x05,
      72,   71,   71,   71, 0x05,

 // slots: signature, parameters, type, tag, flags
      86,   64,   71,   71, 0x0a,
     114,   71,   71,   71, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_sklyph__valt__DicomGriddedViewWidget[] = {
    "sklyph::valt::DicomGriddedViewWidget\0"
    "matrixUpdated(GcellMatrix)\0matrix\0\0"
    "viewUpdated()\0setGcellMatrix(GcellMatrix)\0"
    "resetMatrix()\0"
};

void sklyph::valt::DicomGriddedViewWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DicomGriddedViewWidget *_t = static_cast<DicomGriddedViewWidget *>(_o);
        switch (_id) {
        case 0: _t->matrixUpdated((*reinterpret_cast< const GcellMatrix(*)>(_a[1]))); break;
        case 1: _t->viewUpdated(); break;
        case 2: _t->setGcellMatrix((*reinterpret_cast< const GcellMatrix(*)>(_a[1]))); break;
        case 3: _t->resetMatrix(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData sklyph::valt::DicomGriddedViewWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject sklyph::valt::DicomGriddedViewWidget::staticMetaObject = {
    { &DicomViewWidget::staticMetaObject, qt_meta_stringdata_sklyph__valt__DicomGriddedViewWidget,
      qt_meta_data_sklyph__valt__DicomGriddedViewWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &sklyph::valt::DicomGriddedViewWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *sklyph::valt::DicomGriddedViewWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *sklyph::valt::DicomGriddedViewWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_sklyph__valt__DicomGriddedViewWidget))
        return static_cast<void*>(const_cast< DicomGriddedViewWidget*>(this));
    return DicomViewWidget::qt_metacast(_clname);
}

int sklyph::valt::DicomGriddedViewWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DicomViewWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void sklyph::valt::DicomGriddedViewWidget::matrixUpdated(const GcellMatrix & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void sklyph::valt::DicomGriddedViewWidget::viewUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
