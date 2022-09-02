#ifndef SKLYPH_COMMON_H
#define SKLYPH_COMMON_H

#include <QtCore>

// This function is outside of any namespace because it overloads Qt function
inline uint qHash(const QPoint& key)
{
    return qHash(QPair<int,int>(key.x(), key.y()));
}

namespace sklyph {
namespace valt {

const qreal BASE_GRID_CELL_SIZE_CM = 0.5;

typedef enum {
    GCELL_NONE,
    GCELL_HEALTHY,
    GCELL_INFECTED
} GcellState;

typedef QSet<QPoint> GcellSet;

struct GcellMatrix {
    GcellSet lungTissue;
    GcellSet infected;
};

inline void declareQtMetatypes() {
    qRegisterMetaType<GcellSet>("GcellSet");
    qRegisterMetaType<GcellMatrix>("GcellMatrix");
}

inline qreal getGcellWeight(GcellState state)
{
    switch (state) {
        case GCELL_INFECTED: {
            return 1.0;
        }
        default: {
            return 0.0;
        }
    }
}

namespace strutils {

inline QString realToString(qreal value) {
    return QString::number(value, 'f', 2);
}

inline QString intToString(int value) {
    return QString::number(value);
}

inline QString getUniqueStringByAddingNumber(const QString& baseString, const QStringList& list) {
    QString result;

    int saltIndex = 1;
    do {
        result = QString("%1 (%2)").arg(baseString).arg(intToString(saltIndex));
        saltIndex++;
    } while (list.contains(result));

    return result;
}

} // namespace str

} // namespace valt
} // namespace sklyph

#endif // SKLYPH_COMMON_H
