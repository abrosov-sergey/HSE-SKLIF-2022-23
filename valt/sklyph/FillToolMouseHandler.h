#ifndef SKLYPH_FILLTOOLMOUSEHANDLER_H
#define SKLYPH_FILLTOOLMOUSEHANDLER_H

#include "AbstractToolMouseHandler.h"
#include "DicomSlice.h"

#include <QImage>
#include <QMouseEvent>

namespace sklyph {
namespace valt {

class DicomGriddedViewWidget;

struct FillAction {
    QPoint gcell;
    GcellState state;

    FillAction();
    explicit FillAction(QPoint gcell_, GcellState state_);
};

bool operator !=(const FillAction& left, const FillAction& right);
bool operator ==(const FillAction& left, const FillAction& right);

enum class FillToolMode {
    MarkInfection = 0,
    MarkTotalVolume
};

enum class GcellCompareMode {
    CompareRawPixelValue = 0,
    CompareDisplayColor
};

///
/// \brief The FillToolMouseHandler class
///
///
class FillToolMouseHandler: public AbstractToolMouseHandler
{
    Q_OBJECT
public:
    explicit FillToolMouseHandler();
    ~FillToolMouseHandler() override = default;

    void processMouseEvent(QMouseEvent* event) override;

    void setDicomView(DicomViewWidget* view) override;

    // Вызывается из DicomViewWidget::reset()
    void reset() override;

    void setMode(FillToolMode mode);

private: // methods
    void processMousePressEvent(QMouseEvent* event);
    void processMouseMoveEvent(QMouseEvent* event);

    GcellState getPenStateByMouseButtons(Qt::MouseButtons buttons) const;
    bool isFillActiveWithMouseButtons(Qt::MouseButtons buttons) const;

    void performFill(const QPoint& gcell, qreal sampleColor, GcellState penState);

    bool isMidGcellCountedAsSimilar(const QPoint& gcell) const;

    qreal getRawValueInPixel(const QPoint& pixel) const;
    qreal getMidGcellRawValue(const QPoint& gcell) const;
    bool isMidGcellRawValueSimilar(const QPoint& gcell, qreal sampleValue) const;
    bool isRawValueSimilar(qreal test, qreal sample) const;

    QRgb getColorInPixel(const QPoint& pixel) const;
    QRgb getMidGcellColor(const QPoint& gcell) const;
    bool isMidGcellColorSimilar(const QPoint& gcell, QRgb sampleColor) const;
    bool isColorSimilar(QRgb test, QRgb sample) const;

private: // fields
    DicomGriddedViewWidget* griddedView_;

    FillToolMode fillMode_;
    GcellCompareMode compareMode_;

    FillAction lastFillAction_;

    qreal currentRawPixelValueSample_;
    qreal rawValueDiffThreshold_;

    QRgb currentDisplayColorSample_;
    qint32 colorSqrDistanceThreshold_;
};

} // namespace valt
} // namespace sklyph

#endif // SKLYPH_FILLTOOLMOUSEHANDLER_H
