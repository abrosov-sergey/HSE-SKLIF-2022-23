#include "AspectRatioPixmapLabel.h"

#include <QPainter>

#include <algorithm>

namespace sklyph {
namespace valt {

const qreal INCHES_IN_CM = 0.393701;
const qreal DEFAULT_CELL_SIZE_ON_SCREEN_CM = 0.1;
const QSize DEFAULT_PIXMAP_CELLS_SIZE(512, 512);

AspectRatioPixmapLabel::AspectRatioPixmapLabel(QWidget* parent)
    : QLabel(parent)
    , kScreenDotsPerCm(getScreenDotsPerCm())
    , isImageUpToDate_(false)
{
    this->setMinimumSize(DEFAULT_PIXMAP_CELLS_SIZE);
    setScaledContents(false);

    connect(this, SIGNAL(viewChanged()), this, SLOT(renderPixmap()));
    connect(this, SIGNAL(pixmapChanged()), this, SLOT(invalidateImage()));

    reset();
}

int AspectRatioPixmapLabel::heightForWidth(int w) const
{
    return w;
}

qreal AspectRatioPixmapLabel::getCellSizeOnScreenCm() const
{
    return cellSizeOnScreenCm_;
}

void AspectRatioPixmapLabel::setCellSizeOnScreenCm(qreal cellSizeCm)
{
    cellSizeOnScreenCm_ = cellSizeCm;
    calculateScreenPixelsPerCell();

    QRect viewport = viewport_;
    fitViewportInScene(viewport);
    updateViewport(viewport);
}

void AspectRatioPixmapLabel::setPixmap(const QPixmap& pixmap)
{
    pixmap_ = pixmap;
    emit pixmapChanged();
}

bool AspectRatioPixmapLabel::isPixmapSet() const
{
    return !pixmap_.isNull();
}

void AspectRatioPixmapLabel::reset()
{
    pixmap_ = QPixmap();
    cellSizeOnScreenCm_ = DEFAULT_CELL_SIZE_ON_SCREEN_CM;
    screenPixelsPerCell_ = 1.0;
    viewport_ = QRect(0, 0, DEFAULT_PIXMAP_CELLS_SIZE.width(), DEFAULT_PIXMAP_CELLS_SIZE.height());

    QLabel::clear();
}

qreal AspectRatioPixmapLabel::getScreenPixelsPerCell() const
{
    return screenPixelsPerCell_;
}

const QRect& AspectRatioPixmapLabel::getViewport() const
{
    return viewport_;
}

const QPixmap& AspectRatioPixmapLabel::getPixmap() const
{
    return pixmap_;
}

const QImage& AspectRatioPixmapLabel::getImage()
{
    if (!isImageUpToDate_) {
        image_ = pixmap_.toImage();
    }

    return image_;
}

AspectRatioPixmapLabel& AspectRatioPixmapLabel::operator=(const AspectRatioPixmapLabel& other)
{
    pixmap_ = other.pixmap_;
    cellSizeOnScreenCm_ = other.cellSizeOnScreenCm_;
    screenPixelsPerCell_ = other.screenPixelsPerCell_;
    viewport_ = other.viewport_;

    return *this;
}

void AspectRatioPixmapLabel::paintEvent(QPaintEvent* event)
{
    QLabel::paintEvent(event);

    if (!isPixmapSet()) {
        return;
    }

    QPainter painter(this);
    painter.setCompositionMode(QPainter::CompositionMode_SourceAtop);

    for (const auto& layer : layers_) {
        painter.drawPixmap(this->rect(), layer);
    }
}

void AspectRatioPixmapLabel::setLayer(int index, const QPixmap& pixmap)
{
    layers_[index] = pixmap;
}

QPixmap AspectRatioPixmapLabel::layer(int index) const
{
    return layers_.value(index, QPixmap());
}

void AspectRatioPixmapLabel::setViewport(const QRect& viewportRect)
{
    viewport_ = viewportRect;

    emit viewChanged();
}

void AspectRatioPixmapLabel::moveViewport(const QPoint& vectorCells)
{
    if (!isPixmapSet()) {
        return;
    }

    QRect viewport(viewport_.topLeft() - vectorCells, viewport_.size());
    fitViewportInScene(viewport);
    updateViewport(viewport);
}

qreal AspectRatioPixmapLabel::getScreenDotsPerCm() const
{
    const qreal screenDotsPerCm = logicalDpiX() * INCHES_IN_CM;
    return screenDotsPerCm;
}

void AspectRatioPixmapLabel::fitViewportInScene(QRect& viewport)
{
    if (pixmap_.isNull()) {
        return;
    }

    const int canvasWidth = width();
    const int canvasHeight = height();
    const int pixmapWidth = pixmap_.width();
    const int pixmapHeight = pixmap_.height();

    int scaledImageWidth = qRound(qreal(pixmapWidth) * screenPixelsPerCell_);
    if (scaledImageWidth < canvasWidth) {
        screenPixelsPerCell_ = qreal(canvasWidth) / qreal(pixmapWidth);
        calculateCellSizeOnScreenCm();
        scaledImageWidth = canvasWidth;
    }

    int scaledImageHeight = qRound(qreal(pixmapHeight) * screenPixelsPerCell_);
    if (scaledImageHeight < canvasHeight) {
        screenPixelsPerCell_ = qreal(canvasHeight) / qreal(pixmapHeight);
        calculateCellSizeOnScreenCm();
        scaledImageHeight = canvasHeight;
    }

    int viewportWidth = qRound(qreal(pixmapWidth / qreal(scaledImageWidth) * qreal(canvasWidth)));
    int viewportHeight = qRound(qreal(pixmapHeight) / qreal(scaledImageHeight) * qreal(canvasHeight));

    viewport.setWidth(viewportWidth);
    viewport.setHeight(viewportHeight);

    if (viewport.left() < 0) {
        viewport.moveLeft(0);
    }
    if (viewport.top() < 0) {
        viewport.moveTop(0);
    }
    if (viewport.right() > pixmapWidth) {
        viewport.moveRight(pixmapWidth);
    }
    if (viewport.bottom() > pixmapHeight) {
        viewport.moveBottom(pixmapHeight);
    }
}

void AspectRatioPixmapLabel::updateViewport(const QRect& viewport)
{
    setViewport(viewport);
    emit viewportUpdated(viewport_);
}

void AspectRatioPixmapLabel::calculateScreenPixelsPerCell()
{
    screenPixelsPerCell_ = kScreenDotsPerCm * cellSizeOnScreenCm_;
}

void AspectRatioPixmapLabel::calculateCellSizeOnScreenCm()
{
    cellSizeOnScreenCm_ = screenPixelsPerCell_ / kScreenDotsPerCm;
}

void AspectRatioPixmapLabel::renderPixmap()
{
    if (pixmap_.isNull()) {
        return;
    }

    const QPixmap clippedPixmap = pixmap_.copy(viewport_);
    const QPixmap scaledPixmap = clippedPixmap.scaled(size());

    QLabel::setPixmap(scaledPixmap);
}

void AspectRatioPixmapLabel::invalidateImage()
{
    isImageUpToDate_ = false;
}

} // namespace valt
} // namespace sklyph
