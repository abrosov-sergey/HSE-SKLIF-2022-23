#include "DicomReader.h"

#include "DcmFile.h"
#include "DcmImage.h"
#include "DcmMonochromeImage.h"
#include "DcmTagKey.h"
#include "DcmTagSQ.h"

#include <QPixmap>
#include <QPainter>

namespace sklyph {
namespace valt {

const QPointF DEFAULT_CELL_SPACING_CM(0.1, 0.1);
const qreal DEFAULT_SLICE_THICKNESS_CM = 0.1;

const DcmTagKey DicomReader::DICOM_TAG_IMAGE_FILE_PATH(0x0004, 0x1500);
const DcmTagKey DicomReader::DICOM_TAG_PIXEL_SPACING(0x0028, 0x0030);
const DcmTagKey DicomReader::DICOM_TAG_SLICE_THICKNESS(0x0018, 0x0050);
const DcmTagKey DicomReader::DICOM_TAG_SERIES_DESCRIPTION(0x0008, 0x103e);

DcmDataset DicomReader::loadDatasetFromFile(const QString& path, bool* ok)
{
    DcmFile dcmFile(path);
    DcmDataset dataset = dcmFile.read();
    if (dcmFile.isError()) {
        if (ok) {
            *ok = false;
        }
        return DcmDataset();
    }

    if (ok) {
        *ok = true;
    }
    return dataset;
}

QVariant DicomReader::findTaggedValue(const DcmDataset& dataset, const DcmTagKey& key)
{
    DcmTagList tagList = dataset.tagGroups();
    return findTaggedValue(tagList, key);
}

QVariant DicomReader::findTaggedValue(DcmTagList& tagList, const DcmTagKey& key)
{
    for (DcmTag *tag : tagList.list()) {
        if (tag->tagKey() == key) {
            return tag->value();
        }

        if (tag->isGroup()) {
            DcmTagGroup *tagGroup = dynamic_cast<DcmTagGroup *>(tag);
            Q_ASSERT(tagGroup);
            QVariant result = findTaggedValue(tagGroup->tags(), key);
            if (!result.isNull()) {
                return result;
            }
        } else if (tag->vr() == DcmVr::SQ) {
            DcmTagSQ *tagSq = dynamic_cast<DcmTagSQ *>(tag);
            QVariant result = findTaggedValue(tagSq->items(), key);
            if (!result.isNull()) {
                return result;
            }
        } else if (tag->tagKey() == DcmTagKey::Item) {
            if (!tag->isBinary()) {
                DcmTagItem *tagItem = dynamic_cast<DcmTagItem *>(tag);
                QVariant result = findTaggedValue(tagItem->tagGroups(), key);
                if (!result.isNull()) {
                    return result;
                }
            }
        }
    }

    return QVariant();
}

QVariantList DicomReader::findTaggedValues(const DcmDataset& dataset, const DcmTagKey& key)
{
    DcmTagList tagList = dataset.tagGroups();
    return findTaggedValues(tagList, key);
}

QVariantList DicomReader::findTaggedValues(DcmTagList& tagList, const DcmTagKey& key)
{
    QVariantList values;

    for (DcmTag *tag : tagList.list()) {
        if (tag->tagKey() == key) {
            values.append(tag->value());
        }

        if (tag->isGroup()) {
            DcmTagGroup *tagGroup = dynamic_cast<DcmTagGroup *>(tag);
            Q_ASSERT(tagGroup);
            QVariantList results = findTaggedValues(tagGroup->tags(), key);
            values.append(results);
        } else if (tag->vr() == DcmVr::SQ) {
            DcmTagSQ *tagSq = dynamic_cast<DcmTagSQ *>(tag);
            QVariantList results = findTaggedValues(tagSq->items(), key);
            values.append(results);
        } else if (tag->tagKey() == DcmTagKey::Item) {
            if (!tag->isBinary()) {
                DcmTagItem *tagItem = dynamic_cast<DcmTagItem *>(tag);
                QVariantList results = findTaggedValues(tagItem->tagGroups(), key);
                values.append(results);
            }
        }
    }

    return values;
}

qreal DicomReader::getSliceThicknessCm(const DcmDataset& dataset)
{
    auto result = findTaggedValue(dataset, DICOM_TAG_SLICE_THICKNESS);
    if (result.isNull()) {
        return DEFAULT_SLICE_THICKNESS_CM;
    }

    const qreal sliceThicknessMm = result.toDouble();
    return sliceThicknessMm / 10.0;
}

QPointF DicomReader::getCellSpacing(const DcmDataset& dataset)
{
    const DcmTag* tag = dataset.findTag(DICOM_TAG_PIXEL_SPACING);
    if (tag) {
        const QString spacingStr = tag->value().toString();
        const QStringList parts = spacingStr.split("\\");
        if (parts.size() == 2) {
            return QPointF(parts[0].toDouble() / 10.0, parts[1].toDouble() / 10.0);
        }
    }

    return DEFAULT_CELL_SPACING_CM;
}

QString DicomReader::getSeriesDescription(const DcmDataset& dataset)
{
    const DcmTag* tag = dataset.findTag(DICOM_TAG_SERIES_DESCRIPTION);
    if (tag) {
        return tag->value().toString();
    }

    return QString();
}

QPixmap DicomReader::loadDicomImage(const DcmDataset& dataset)
{
    DcmImage image(dataset);
    if (image.photometricInterpretation().isGrayscale()) {
        DcmMonochromeImage monoImage(image.dataset());

        DcmImageTransferFunction tf;

        double windowCenter = monoImage.windowCenter();
        double windowWidth = monoImage.windowWidth();

        tf.addReferencePoint(windowCenter - windowWidth / 2, QColor(0, 0, 0));
        tf.addReferencePoint(windowCenter + windowWidth / 2, QColor(255, 255, 255));

        QImage qImage = monoImage.toQImage(tf);
        QPixmap pixmap = QPixmap::fromImage(qImage);
        return pixmap;
    }

    return QPixmap();
}

} // namespace valt
} // namespace sklyph
