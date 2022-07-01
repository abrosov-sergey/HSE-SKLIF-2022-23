#include "LungMatrix.h"

namespace sklyph {
namespace valt {

using sklyph::valt::BASE_GRID_CELL_SIZE_CM;

LungMatrix::LungMatrix(int sliceCount, qreal sliceThicknessCm, QObject* parent)
    : QObject (parent)
    , kSliceCount(sliceCount)
    , kSliceThicknessCm(sliceThicknessCm)
    , kBaseGcellSizeCm(BASE_GRID_CELL_SIZE_CM)
    , kCellVolumeCm3(kBaseGcellSizeCm * kBaseGcellSizeCm * kSliceThicknessCm)
    , totalVolume_(0.0)
    , isTotalVolumeAutoUpdateEnabled_(true)

{
    sliceInfectionMatrices_.resize(kSliceCount);

    sliceInfectedVolumes_.resize(kSliceCount);
    sliceInfectedVolumes_.fill(0.0, kSliceCount);
}

LungMatrix::~LungMatrix()
{
}

void LungMatrix::setTotalVolume(qreal volumeCm3)
{
    totalVolume_ = volumeCm3;
}

void LungMatrix::setTotalVolumeAutoUpdate(bool enabled)
{
    isTotalVolumeAutoUpdateEnabled_ = enabled;

    if (enabled) {
        updateTotalVolume();
    }
}

qreal LungMatrix::getTotalVolume() const
{
    return totalVolume_;
}

qreal LungMatrix::getInfectedVolume() const
{
    qreal infectedVolume = 0.0;

    for (int sliceIndex = 0; sliceIndex < kSliceCount; sliceIndex++) {
        infectedVolume += sliceInfectedVolumes_[sliceIndex];
    }

    return infectedVolume;
}

qreal LungMatrix::getInfectedVolumePercent() const
{
    qreal part = 0.0;

    qreal infectedVolume = getInfectedVolume();

    if (!qIsNull(totalVolume_) && (totalVolume_ >= infectedVolume)) {
        part = (infectedVolume / totalVolume_) * 100;
    }

    return part;
}

GcellMatrix LungMatrix::getMatrix(int sliceIndex) const
{
    Q_ASSERT(sliceIndex >= 0);

    GcellMatrix matrix;
    matrix.lungTissue = lungTissueMatrix_;

    if (sliceIndex < kSliceCount) {
        matrix.infected = sliceInfectionMatrices_[sliceIndex];
    }

    return matrix;
}

void LungMatrix::setInfectionMatrix(int sliceIndex, const GcellSet& sliceInfectionMatrix)
{
    sliceInfectionMatrices_[sliceIndex] = sliceInfectionMatrix;

    updateSliceInfectedVolume(sliceIndex);

    emit volumesChanged();
}

void LungMatrix::setLungTissueMatrix(const GcellSet& lungTissueMatrix)
{
    lungTissueMatrix_ = lungTissueMatrix;

    if (isTotalVolumeAutoUpdateEnabled_) {
        updateTotalVolume();
    }
}

void LungMatrix::updateSliceInfectedVolume(int sliceIndex)
{
    const auto& sliceMatrix = sliceInfectionMatrices_[sliceIndex];
    const auto infectedGcellCount = sliceMatrix.size();

    sliceInfectedVolumes_[sliceIndex] = infectedGcellCount * kCellVolumeCm3;
}

void LungMatrix::updateTotalVolume()
{
    const auto totalGcellCount = lungTissueMatrix_.size();
    totalVolume_ = totalGcellCount * kSliceCount * kCellVolumeCm3;

    emit volumesChanged();
}

} // namespace valt
} // namespace sklyph
