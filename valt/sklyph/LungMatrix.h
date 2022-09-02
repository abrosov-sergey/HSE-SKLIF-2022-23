#ifndef LUNGMATRIX_H
#define LUNGMATRIX_H

#include "common.h"

#include <QtCore>

namespace sklyph {
namespace valt {

class LungMatrix: public QObject
{
    Q_OBJECT
public:
    explicit LungMatrix(int sliceCount, qreal sliceThicknessCm, QObject* parent=nullptr);
    virtual ~LungMatrix();

    void setTotalVolume(qreal volumeCm3);
    void setTotalVolumeAutoUpdate(bool enabled);
    qreal getTotalVolume() const;

    qreal getInfectedVolume() const;

    qreal getInfectedVolumePercent() const;

    GcellMatrix getMatrix(int sliceIndex) const;

signals:
    void volumesChanged();

public slots:
    void setInfectionMatrix(int sliceIndex, const GcellSet& sliceInfectionMatrix);
    void setLungTissueMatrix(const GcellSet& lungTissueMatrix);

private:
    void updateSliceInfectedVolume(int sliceIndex);
    void updateTotalVolume();

    const int kSliceCount;
    const qreal kSliceThicknessCm;
    const qreal kBaseGcellSizeCm;
    const qreal kCellVolumeCm3;

    GcellSet lungTissueMatrix_;
    QVector<GcellSet> sliceInfectionMatrices_;
    QVector<qreal> sliceInfectedVolumes_;

    qreal totalVolume_;
    bool isTotalVolumeAutoUpdateEnabled_;
};

} // namespace valt
} // namespace sklyph

#endif // LUNGMATRIX_H
