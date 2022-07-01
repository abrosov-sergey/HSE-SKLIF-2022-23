#ifndef SKLYPH_DICOM_SLICE_H
#define SKLYPH_DICOM_SLICE_H

#include "DcmMonochromeImage.h"

#include <QPixmap>
#include <QSharedPointer>

namespace sklyph {
namespace valt {

class DicomSlice
{
public:
    explicit DicomSlice();
    virtual ~DicomSlice();

    void loadFromDataset(const DcmDataset* dataset);

    int getWindowWidth() const;
    void setWindowWidth(int width);
    int getWindowCenter() const;
    void setWindowCenter(int center);

    void update();

    QPixmap pixmap() const;

    double getPixel(const QPoint& cell) const;
    double getPixel(int x, int y) const;
    int getWidth() const;
    int getHeight() const;

private:
    void resetImage();
    void setupTransferFunction();
    void render();

    DcmImage* image_;
    DcmMonochromeImage* monoImage_;
    DcmImageTransferFunction transferFunction_;
    QPixmap pixmap_;

    qreal windowCenter_;
    qreal windowWidth_;
};

typedef QSharedPointer<DicomSlice> DicomSlicePtr;

} // namespace valt
} // namespace sklyph

#endif // SKLYPH_DICOM_SLICE_H
