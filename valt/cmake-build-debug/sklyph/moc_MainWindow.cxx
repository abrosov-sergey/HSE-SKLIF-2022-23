/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../sklyph/MainWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_sklyph__valt__MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      22,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      25,   40,   40,   40, 0x05,
      41,   40,   40,   40, 0x05,
      56,   84,   40,   40, 0x05,

 // slots: signature, parameters, type, tag, flags
      89,   40,   40,   40, 0x08,
     119,   40,   40,   40, 0x08,
     149,   40,   40,   40, 0x08,
     185,   84,   40,   40, 0x08,
     208,   40,   40,   40, 0x08,
     227,   40,   40,   40, 0x08,
     248,   40,   40,   40, 0x08,
     274,   40,   40,   40, 0x08,
     298,  338,   40,   40, 0x08,
     344,  338,   40,   40, 0x08,
     383,  338,   40,   40, 0x08,
     421,   40,   40,   40, 0x08,
     462,  338,   40,   40, 0x08,
     496,   40,   40,   40, 0x08,
     528,  562,   40,   40, 0x08,
     574,  607,   40,   40, 0x08,
     613,   40,   40,   40, 0x08,
     643,  671,   40,   40, 0x08,
     677,   40,   40,   40, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_sklyph__valt__MainWindow[] = {
    "sklyph::valt::MainWindow\0serieChanged()\0"
    "\0sliceUpdated()\0projectRootChanged(QString)\0"
    "path\0executeOpenProjectDirDialog()\0"
    "executeExportRawImageDialog()\0"
    "executeExportImageWithMarksDialog()\0"
    "reloadProject(QString)\0updateSliceViews()\0"
    "updateVolumesInGui()\0updateManualTotalVolume()\0"
    "onProjectLoadFinished()\0"
    "onPatientsCboxSelectionChanged(QString)\0"
    "value\0onStudiesCboxSelectionChanged(QString)\0"
    "onSeriesCboxSelectionChanged(QString)\0"
    "onTotalVolumeAutoUpdateStateChanged(int)\0"
    "onScalingSpinBoxValueChanged(int)\0"
    "onSliceSpinBoxValueChanged(int)\0"
    "onSliceMatrixUpdated(GcellMatrix)\0"
    "sliceMatrix\0onRulerCheckBoxStateChanged(int)\0"
    "state\0onToolButtonGroupClicked(int)\0"
    "onGridStepIndexChanged(int)\0index\0"
    "onInfectionMarkModeStateChanged(int)\0"
};

void sklyph::valt::MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->serieChanged(); break;
        case 1: _t->sliceUpdated(); break;
        case 2: _t->projectRootChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->executeOpenProjectDirDialog(); break;
        case 4: _t->executeExportRawImageDialog(); break;
        case 5: _t->executeExportImageWithMarksDialog(); break;
        case 6: _t->reloadProject((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->updateSliceViews(); break;
        case 8: _t->updateVolumesInGui(); break;
        case 9: _t->updateManualTotalVolume(); break;
        case 10: _t->onProjectLoadFinished(); break;
        case 11: _t->onPatientsCboxSelectionChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 12: _t->onStudiesCboxSelectionChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 13: _t->onSeriesCboxSelectionChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 14: _t->onTotalVolumeAutoUpdateStateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->onScalingSpinBoxValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->onSliceSpinBoxValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->onSliceMatrixUpdated((*reinterpret_cast< const GcellMatrix(*)>(_a[1]))); break;
        case 18: _t->onRulerCheckBoxStateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: _t->onToolButtonGroupClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 20: _t->onGridStepIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 21: _t->onInfectionMarkModeStateChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData sklyph::valt::MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject sklyph::valt::MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_sklyph__valt__MainWindow,
      qt_meta_data_sklyph__valt__MainWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &sklyph::valt::MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *sklyph::valt::MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *sklyph::valt::MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_sklyph__valt__MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int sklyph::valt::MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 22)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 22;
    }
    return _id;
}

// SIGNAL 0
void sklyph::valt::MainWindow::serieChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void sklyph::valt::MainWindow::sliceUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void sklyph::valt::MainWindow::projectRootChanged(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
