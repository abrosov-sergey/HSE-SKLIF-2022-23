#ifndef SKLYPH_DICOM_SERIES_H
#define SKLYPH_DICOM_SERIES_H

#include "LungMatrix.h"

#include "DcmDataset.h"
#include "DicomSlice.h"

#include <QStringList>

namespace sklyph {
namespace valt {

class DicomSerie: public QObject
{
    Q_OBJECT
public:
    explicit DicomSerie(const QString& path, QObject* parent=nullptr);
    virtual ~DicomSerie();

    bool reloadMetaData();
    bool reloadSlices();

    int getSliceCount() const;
    DicomSlicePtr getSlice(int index);
    LungMatrix* getLungMatrix() const;

    qreal getSliceThicknessCm() const;
    QPointF getCellSpacingCm() const;

    QString getDescription() const;

    bool isLoaded() const;

private:
    void setupLungMatrix();

    const QString kRootPath;

    bool isLoaded_;

    QString description_;
    qreal sliceThicknessCm_;
    QPointF cellSpacingCm_;
    QStringList sliceFilePaths_;
    QVector<DicomSlicePtr> slices_;
    LungMatrix* lungMatrix_;
};

typedef QSharedPointer<DicomSerie> DicomSeriePtr;

} // namespace valt
} // namespace sklyph

#endif // SKLYPH_DICOM_SERIES_H
