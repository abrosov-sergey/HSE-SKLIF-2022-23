#include "DicomSeries.h"

#include "DicomReader.h"

#include <QPixmap>

namespace sklyph {
namespace valt {

DicomSerie::DicomSerie(const QString& path, QObject* parent)
    : QObject (parent)
    , kRootPath(path)
    , isLoaded_(false)
    , description_()
    , sliceThicknessCm_(1.0)
    , cellSpacingCm_(1.0, 1.0)
    , slices_()
    , lungMatrix_(nullptr)
{
}

DicomSerie::~DicomSerie()
{
}

bool DicomSerie::reloadMetaData()
{
    QDir dir(kRootPath);
    if (!dir.exists()) {
        return false;
    }

    const QStringList filePaths = dir.entryList(QDir::Files);
    if (filePaths.isEmpty()) {
        return false;
    }
    QString fileFullPath = dir.filePath(filePaths[0]);
    bool ok = false;
    DcmDataset dataset = DicomReader::loadDatasetFromFile(fileFullPath, &ok);
    if (!ok) {
        return false;
    }
    description_ = DicomReader::getSeriesDescription(dataset);
    cellSpacingCm_ = DicomReader::getCellSpacing(dataset);
    sliceThicknessCm_ = DicomReader::getSliceThicknessCm(dataset);

    if (qIsNull(cellSpacingCm_.x()) || qIsNull(cellSpacingCm_.y()) || qIsNull(sliceThicknessCm_)) {
        return false;
    }
    return true;
}

bool DicomSerie::reloadSlices()
{
    QDir dir(kRootPath);
    if (!dir.exists()) {
        return false;
    }

    const QStringList filePaths = dir.entryList(QDir::Files);
    if (filePaths.isEmpty()) {
        return false;
    }

    sliceFilePaths_.clear();
    slices_.clear();
    slices_.resize(filePaths.size());

    for (const QString& filePath : filePaths) {
        QString fileFullPath = dir.filePath(filePath);
        sliceFilePaths_.append(fileFullPath);
    }

    setupLungMatrix();

    isLoaded_ = true;
    return true;
}

int DicomSerie::getSliceCount() const
{
    return slices_.size();
}

DicomSlicePtr DicomSerie::getSlice(int index)
{
    DicomSlicePtr& slice = slices_[index];
    if (slice.isNull()) {
        bool ok = false;
        DcmDataset dataset = DicomReader::loadDatasetFromFile(sliceFilePaths_[index], &ok);
        if (ok) {
            slice = DicomSlicePtr(new DicomSlice);
            slice->loadFromDataset(&dataset);
        }
    }

    return slice;
}

LungMatrix* DicomSerie::getLungMatrix() const
{
    return lungMatrix_;
}

qreal DicomSerie::getSliceThicknessCm() const
{
    return sliceThicknessCm_;
}

QPointF DicomSerie::getCellSpacingCm() const
{
    return cellSpacingCm_;
}

QString DicomSerie::getDescription() const
{
    return description_;
}

bool DicomSerie::isLoaded() const
{
    return isLoaded_;
}

void DicomSerie::setupLungMatrix()
{
    if (lungMatrix_) {
        delete lungMatrix_;
    }
    lungMatrix_ = new LungMatrix(slices_.size(), sliceThicknessCm_, this);
}

} // namespace valt
} // namespace sklyph
