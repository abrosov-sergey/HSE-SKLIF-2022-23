#ifndef DICOM_VIEW_WIDGET_H
#define DICOM_VIEW_WIDGET_H

#include "AspectRatioPixmapLabel.h"
#include "DicomSlice.h"

#include <QMutex>

namespace sklyph {
namespace valt {

class AbstractToolMouseHandler;

class DicomViewWidget: public AspectRatioPixmapLabel
{
    Q_OBJECT
public:
    explicit DicomViewWidget(QWidget* parent=nullptr);
    ~DicomViewWidget() override;

    virtual void setMouseHandler(AbstractToolMouseHandler* mouseHandler);

    void reset() override;

    QPointF getCellSpacingCm() const;
    void setCellSpacingCm(const QPointF& cellSpacing);
    QSizeF getSizeCm() const;
    QPointF getPointCmForScreenPixel(const QPoint& pixel) const;
    QPoint getCellForScreenPixel(const QPoint& pixel) const;
    QPoint getScreenPixelForPointCm(const QPointF& pointCm) const;
    QRectF getViewportCm() const;

    virtual void setSlice(DicomSlicePtr slice);
    DicomSlicePtr getSlice() const;

    bool isRulerEnabled() const;
    void setRulerEnabled(bool enabled);

signals:
    void sliceUpdated();

public slots:
    void updatePixmap();

    void onSliceUpdatedByMouseHandler();

protected:
    void paintEvent(QPaintEvent* event) override;

    void mouseEvent(QMouseEvent* event);

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

    DicomSlicePtr slice_;

    AbstractToolMouseHandler* mouseHandler_;

private:
    void drawRulerOnCanvas(QPainter& painter);
    qreal getCurrentRulerLengthCm() const;

    QPointF cellSpacingCm_;

    QLine currentRulerLine_;
    bool isRulerEnabled_;
};

} // namespace valt
} // namespace sklyph

#endif // DICOM_VIEW_WIDGET_H
