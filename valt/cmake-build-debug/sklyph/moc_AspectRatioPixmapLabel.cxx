/****************************************************************************
** Meta object code from reading C++ file 'AspectRatioPixmapLabel.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../sklyph/AspectRatioPixmapLabel.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AspectRatioPixmapLabel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_sklyph__valt__AspectRatioPixmapLabel[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      37,   53,   53,   53, 0x05,
      54,   77,   53,   53, 0x05,
      90,   53,   53,   53, 0x05,

 // slots: signature, parameters, type, tag, flags
     104,   77,   53,   53, 0x0a,
     123,  144,   53,   53, 0x0a,
     156,   53,   53,   53, 0x09,
     171,   53,   53,   53, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_sklyph__valt__AspectRatioPixmapLabel[] = {
    "sklyph::valt::AspectRatioPixmapLabel\0"
    "pixmapChanged()\0\0viewportUpdated(QRect)\0"
    "viewportRect\0viewChanged()\0"
    "setViewport(QRect)\0moveViewport(QPoint)\0"
    "vectorCells\0renderPixmap()\0invalidateImage()\0"
};

void sklyph::valt::AspectRatioPixmapLabel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        AspectRatioPixmapLabel *_t = static_cast<AspectRatioPixmapLabel *>(_o);
        switch (_id) {
        case 0: _t->pixmapChanged(); break;
        case 1: _t->viewportUpdated((*reinterpret_cast< const QRect(*)>(_a[1]))); break;
        case 2: _t->viewChanged(); break;
        case 3: _t->setViewport((*reinterpret_cast< const QRect(*)>(_a[1]))); break;
        case 4: _t->moveViewport((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 5: _t->renderPixmap(); break;
        case 6: _t->invalidateImage(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData sklyph::valt::AspectRatioPixmapLabel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject sklyph::valt::AspectRatioPixmapLabel::staticMetaObject = {
    { &QLabel::staticMetaObject, qt_meta_stringdata_sklyph__valt__AspectRatioPixmapLabel,
      qt_meta_data_sklyph__valt__AspectRatioPixmapLabel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &sklyph::valt::AspectRatioPixmapLabel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *sklyph::valt::AspectRatioPixmapLabel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *sklyph::valt::AspectRatioPixmapLabel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_sklyph__valt__AspectRatioPixmapLabel))
        return static_cast<void*>(const_cast< AspectRatioPixmapLabel*>(this));
    return QLabel::qt_metacast(_clname);
}

int sklyph::valt::AspectRatioPixmapLabel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void sklyph::valt::AspectRatioPixmapLabel::pixmapChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void sklyph::valt::AspectRatioPixmapLabel::viewportUpdated(const QRect & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void sklyph::valt::AspectRatioPixmapLabel::viewChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE
