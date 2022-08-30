/****************************************************************************
** Meta object code from reading C++ file 'DicomViewWidget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../sklyph/DicomViewWidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DicomViewWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_sklyph__valt__DicomViewWidget[] = {

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
      30,   45,   45,   45, 0x05,

 // slots: signature, parameters, type, tag, flags
      46,   45,   45,   45, 0x0a,
      61,   45,   45,   45, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_sklyph__valt__DicomViewWidget[] = {
    "sklyph::valt::DicomViewWidget\0"
    "sliceUpdated()\0\0updatePixmap()\0"
    "onSliceUpdatedByMouseHandler()\0"
};

void sklyph::valt::DicomViewWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        DicomViewWidget *_t = static_cast<DicomViewWidget *>(_o);
        switch (_id) {
        case 0: _t->sliceUpdated(); break;
        case 1: _t->updatePixmap(); break;
        case 2: _t->onSliceUpdatedByMouseHandler(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData sklyph::valt::DicomViewWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject sklyph::valt::DicomViewWidget::staticMetaObject = {
    { &AspectRatioPixmapLabel::staticMetaObject, qt_meta_stringdata_sklyph__valt__DicomViewWidget,
      qt_meta_data_sklyph__valt__DicomViewWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &sklyph::valt::DicomViewWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *sklyph::valt::DicomViewWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *sklyph::valt::DicomViewWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_sklyph__valt__DicomViewWidget))
        return static_cast<void*>(const_cast< DicomViewWidget*>(this));
    return AspectRatioPixmapLabel::qt_metacast(_clname);
}

int sklyph::valt::DicomViewWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = AspectRatioPixmapLabel::qt_metacall(_c, _id, _a);
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
void sklyph::valt::DicomViewWidget::sliceUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
