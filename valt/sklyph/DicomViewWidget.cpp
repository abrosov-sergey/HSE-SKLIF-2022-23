#include "DicomViewWidget.h"

#include "AbstractToolMouseHandler.h"

#include <QTimer>
#include <QPainter>
#include <QVector2D>

namespace sklyph {
namespace valt {

const QPointF DEFAULT_PIXEL_SPACING(1.0, 1.0);

const int RULER_LAYER_INDEX = 334;
const QColor RULER_LINE_COLOR(Qt::darkBlue);
const QColor RULER_TEXT_COLOR(Qt::blue);
const QBrush RULER_BRUSH(RULER_LINE_COLOR);
const QPen RULER_PEN(RULER_BRUSH, 2);
const int RULER_TEXT_SPACING = 5;
const int RULER_TEXT_WIDTH = 50;
const int RULER_TEXT_HEIGHT = 40;
const QFont RULER_TEXT_FONT("Arial", 16);

DicomViewWidget::DicomViewWidget(QWidget* parent)
    : AspectRatioPixmapLabel (parent)
    , mouseHandler_(nullptr)
    , isRulerEnabled_(false)
{
    reset();

    connect(this, SIGNAL(sliceUpdated()), this, SLOT(updatePixmap()));
}

DicomViewWidget::~DicomViewWidget()
{
}

void DicomViewWidget::setMouseHandler(AbstractToolMouseHandler* mouseHandler)
{
    mouseHandler_ = mouseHandler;
    mouseHandler_->setDicomView(this);
}

void DicomViewWidget::reset()
{
    cellSpacingCm_ = DEFAULT_PIXEL_SPACING;

    if (mouseHandler_) {
        mouseHandler_->reset();
    }

    AspectRatioPixmapLabel::reset();
}

QPointF DicomViewWidget::getCellSpacingCm() const
{
    return cellSpacingCm_;
}

void DicomViewWidget::setCellSpacingCm(const QPointF& cellSpacing)
{
    cellSpacingCm_ = cellSpacing;
}

QSizeF DicomViewWidget::getSizeCm() const
{
    const QPixmap* pixmap = QLabel::pixmap();
    if (!pixmap) {
        return QSizeF();
    }

    const QSize sizeCells = pixmap->size();
    const qreal widthCm = sizeCells.width() * cellSpacingCm_.x();
    const qreal heightCm = sizeCells.height() * cellSpacingCm_.y();

    return QSizeF(widthCm, heightCm);
}

QPointF DicomViewWidget::getPointCmForScreenPixel(const QPoint& pixel) const
{
    if (!isPixmapSet()) {
        return QPointF();
    }

    const QRect visibleRect = QLabel::rect();
    const qreal relX = qreal(pixel.x()) / qreal(visibleRect.width());
    const qreal relY = qreal(pixel.y()) / qreal(visibleRect.height());

    const QRectF viewportCm = getViewportCm();
    const qreal xInViewportCm = relX * viewportCm.width();
    const qreal yInViewportCm = relY * viewportCm.height();

    const QPointF pointCm(viewportCm.x() + xInViewportCm, viewportCm.y() + yInViewportCm);
    return pointCm;
}

QPoint DicomViewWidget::getCellForScreenPixel(const QPoint& pixel) const
{
    if (!isPixmapSet()) {
        return QPoint();
    }

    const QRect visibleRect = QLabel::rect();
    const qreal relX = qreal(pixel.x()) / qreal(visibleRect.width());
    const qreal relY = qreal(pixel.y()) / qreal(visibleRect.height());

    const QRect viewport = getViewport();
    const int xInViewport = qRound(relX * viewport.width());
    const int yInViewport = qRound(relY * viewport.height());

    const QPoint pointCell(viewport.x() + xInViewport, viewport.y() + yInViewport);
    return pointCell;
}

QPoint DicomViewWidget::getScreenPixelForPointCm(const QPointF& pointCm) const
{
    if (!isPixmapSet()) {
        return QPoint();
    }

    const QRectF viewportCm = getViewportCm();
    const qreal relX = (pointCm.x() - viewportCm.x()) / viewportCm.width();
    const qreal relY = (pointCm.y() - viewportCm.y()) / viewportCm.height();

    const QRect visibleRect = QLabel::rect();
    const int pixelX = qRound(relX * visibleRect.width());
    const int pixelY = qRound(relY * visibleRect.height());

    const QPoint pixel(pixelX, pixelY);
    return pixel;
}

QRectF DicomViewWidget::getViewportCm() const
{
    const QRectF viewport(getViewport());
    const qreal cellSpacingCmX = cellSpacingCm_.x();
    const qreal cellSpacingCmY = cellSpacingCm_.y();

    const QRectF viewportCm(viewport.x() * cellSpacingCmX, viewport.y() * cellSpacingCmY,
                            viewport.width() * cellSpacingCmX, viewport.height() * cellSpacingCmY);
    return viewportCm;
}

void DicomViewWidget::setSlice(DicomSlicePtr slice)
{
    slice_ = slice;

    emit sliceUpdated();
}

DicomSlicePtr DicomViewWidget::getSlice() const
{
    return slice_;
}

bool DicomViewWidget::isRulerEnabled() const
{
    return isRulerEnabled_;
}

void DicomViewWidget::setRulerEnabled(bool enabled)
{
    isRulerEnabled_ = enabled;
}

void DicomViewWidget::mousePressEvent(QMouseEvent* event)
{
    if (!isPixmapSet()) {
        return;
    }

    mouseHandler_->processMouseEvent(event);
}

void DicomViewWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (!isPixmapSet()) {
        return;
    }

    mouseHandler_->processMouseEvent(event);
}

void DicomViewWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (!isPixmapSet()) {
        return;
    }

    mouseHandler_->processMouseEvent(event);
}

void DicomViewWidget::updatePixmap()
{
    if (!slice_) {
        return;
    }

    const auto viewport = getViewport();

    const QPixmap pixmap = slice_->pixmap();
    AspectRatioPixmapLabel::setPixmap(pixmap);

    setViewport(viewport);
}

void DicomViewWidget::onSliceUpdatedByMouseHandler()
{
    emit sliceUpdated();
}

void DicomViewWidget::paintEvent(QPaintEvent* event)
{
    QPixmap rulerLayer(this->size());
    if (isRulerEnabled_) {
        QPainter rulerPainter(&rulerLayer);
        drawRulerOnCanvas(rulerPainter);
    }
    setLayer(RULER_LAYER_INDEX, rulerLayer);

    AspectRatioPixmapLabel::paintEvent(event);
}

void DicomViewWidget::drawRulerOnCanvas(QPainter& painter)
{
    painter.setPen(RULER_PEN);
    painter.drawLine(currentRulerLine_);

    int textX = currentRulerLine_.p2().x() - RULER_TEXT_WIDTH / 2;
    int textY;
    if (currentRulerLine_.p2().y() > currentRulerLine_.p1().y()) {
        textY = currentRulerLine_.p2().y() + RULER_TEXT_SPACING + RULER_TEXT_FONT.pixelSize() / 2;
    } else {
        textY = currentRulerLine_.p2().y() - RULER_TEXT_SPACING - RULER_TEXT_FONT.pixelSize() / 2;
    }

    const qreal rulerLengthCm = getCurrentRulerLengthCm();
    const QString caption = QString("%1 cm").arg(rulerLengthCm, 0, 'f', 3);

    painter.setPen(RULER_TEXT_COLOR);
    painter.setFont(RULER_TEXT_FONT);
    painter.drawText(textX, textY, caption);
}

qreal DicomViewWidget::getCurrentRulerLengthCm() const
{
    const QPointF p1Cm = getPointCmForScreenPixel(currentRulerLine_.p1());
    const QPointF p2Cm = getPointCmForScreenPixel(currentRulerLine_.p2());
    const QVector2D vectorCm(p2Cm - p1Cm);

    return vectorCm.length();
}

} // namespace valt
} // namespace sklyph
