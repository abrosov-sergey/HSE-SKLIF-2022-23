#include "DicomSlice.h"

#include <QPixmap>

namespace sklyph {
namespace valt {

const QColor COLOR_ZERO(0, 0, 0);
const QColor COLOR_MID(127, 127, 127);
const QColor COLOR_MAX(255, 255, 255);

DicomSlice::DicomSlice()
    : image_(nullptr)
    , monoImage_(nullptr)
    , transferFunction_()
    , windowCenter_(0)
    , windowWidth_(0)
{

}

DicomSlice::~DicomSlice()
{
    resetImage();
}

void DicomSlice::loadFromDataset(const DcmDataset* dataset)
{
    resetImage();

    if (!dataset) {
        return;
    }

    image_ = new DcmImage(*dataset);
    if (!image_->photometricInterpretation().isGrayscale()) {
        resetImage();
        return;
    }

    monoImage_ = new DcmMonochromeImage(image_->dataset());

    windowCenter_ = monoImage_->windowCenter();
    windowWidth_ = monoImage_->windowWidth();

    update();
}

int DicomSlice::getWindowWidth() const
{
    return qRound(windowWidth_);
}

void DicomSlice::setWindowWidth(int width)
{
    windowWidth_ = width;

    if (windowWidth_ < 0.0) {
        windowWidth_ = 0.0;
    }
}

int DicomSlice::getWindowCenter() const
{
    return qRound(windowCenter_);
}

void DicomSlice::setWindowCenter(int center)
{
    windowCenter_ = center;
}

void DicomSlice::update()
{
    setupTransferFunction();
    render();
}

QPixmap DicomSlice::pixmap() const
{
    return pixmap_;
}

double DicomSlice::getPixel(const QPoint& cell) const
{
    return getPixel(cell.x(), cell.y());
}

double DicomSlice::getPixel(int x, int y) const
{
    return monoImage_->rescaledPixel(x, y);
}

int DicomSlice::getWidth() const
{
    return monoImage_->width();
}

int DicomSlice::getHeight() const
{
    return monoImage_->height();
}

void DicomSlice::resetImage()
{
    if (image_ || monoImage_) {
        delete monoImage_;
        monoImage_ = nullptr;
        delete image_;
        image_ = nullptr;
    }
}

void DicomSlice::setupTransferFunction()
{
    const double minValue = windowCenter_ - windowWidth_ / 2;
    const double maxValue = windowCenter_ + windowWidth_ / 2;

    transferFunction_.clearReferencePoints();
    transferFunction_.addReferencePoint(minValue, COLOR_ZERO);
    transferFunction_.addReferencePoint(windowCenter_, COLOR_MID);
    transferFunction_.addReferencePoint(maxValue, COLOR_MAX);
}

void DicomSlice::render()
{
    if (!monoImage_) {
        return;
    }

    QImage qImage = monoImage_->toQImage(transferFunction_);
    pixmap_ = QPixmap::fromImage(qImage);
}

} // namespace valt
} // namespace sklyph
