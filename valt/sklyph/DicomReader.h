#ifndef SKLYPH_DICOM_READER_H
#define SKLYPH_DICOM_READER_H

#include "DcmDataset.h"

namespace sklyph {
namespace valt {

class DicomReader
{
public:
    static DcmDataset loadDatasetFromFile(const QString& path, bool* ok=nullptr);

    static QVariant findTaggedValue(const DcmDataset& dataset, const DcmTagKey& key);
    static QVariant findTaggedValue(DcmTagList& tagList, const DcmTagKey& key);
    static QVariantList findTaggedValues(const DcmDataset& dataset, const DcmTagKey& key);
    static QVariantList findTaggedValues(DcmTagList& tagList, const DcmTagKey& key);

    static qreal getSliceThicknessCm(const DcmDataset& dataset);
    static QPointF getCellSpacing(const DcmDataset& dataset);
    static QString getSeriesDescription(const DcmDataset& dataset);

    static QPixmap loadDicomImage(const DcmDataset& dataset);

    static const DcmTagKey DICOM_TAG_IMAGE_FILE_PATH;
    static const DcmTagKey DICOM_TAG_PIXEL_SPACING;
    static const DcmTagKey DICOM_TAG_SLICE_THICKNESS;
    static const DcmTagKey DICOM_TAG_SERIES_DESCRIPTION;
};

} // namespace valt
} // namespace sklyph

#endif // SKLYPH_DICOM_READER_H
