#include "FillToolMouseHandler.h"

#include "DicomGriddedViewWidget.h"

#include <QQueue>

const qreal DEFAULT_VALUE_DIFF_THRESHOLD = 30.0;
const qint32 DEFAULT_COLOR_SQUARE_DISTANCE_THRESHOLD = 50;

namespace sklyph {
namespace valt {

FillToolMouseHandler::FillToolMouseHandler()
    : AbstractToolMouseHandler()
    , griddedView_(nullptr)
    , fillMode_(FillToolMode::MarkTotalVolume)
    , compareMode_(GcellCompareMode::CompareDisplayColor)
    , rawValueDiffThreshold_(DEFAULT_VALUE_DIFF_THRESHOLD)
    , colorSqrDistanceThreshold_(DEFAULT_COLOR_SQUARE_DISTANCE_THRESHOLD)
{
}

void FillToolMouseHandler::processMouseEvent(QMouseEvent* event)
{
    Q_ASSERT(event);

    switch (event->type()) {
        case QMouseEvent::MouseButtonPress: {
            processMousePressEvent(event);
            break;
        }
        case QMouseEvent::MouseMove: {
            processMouseMoveEvent(event);
            break;
        }
        default: {
            event->ignore();
            break;
        }
    }
}

void FillToolMouseHandler::setDicomView(DicomViewWidget* view)
{
    AbstractToolMouseHandler::setDicomView(view);

    griddedView_ = static_cast<DicomGriddedViewWidget*>(view);
}

void FillToolMouseHandler::reset()
{
}

void FillToolMouseHandler::setMode(FillToolMode mode)
{
    fillMode_ = mode;
}

void FillToolMouseHandler::processMousePressEvent(QMouseEvent* event)
{
    processMouseMoveEvent(event);
}

void FillToolMouseHandler::processMouseMoveEvent(QMouseEvent* event)
{
    const auto slice = view_->getSlice();
    if (!slice) {
        return;
    }

    const auto buttons = event->buttons();
    if (!isFillActiveWithMouseButtons(buttons)) {
        event->ignore();
        return;
    }

    const QPoint& pos = event->pos();
    const GcellState penState = getPenStateByMouseButtons(buttons);
    const FillAction action(pos, penState);
    if (action == lastFillAction_) {
        event->accept();
        return;
    }

    const QPoint gcell = griddedView_->getGcellByScreenPixel(pos);
    const qreal sampleColor = getMidGcellRawValue(gcell);
    performFill(gcell, sampleColor, penState);

    lastFillAction_ = action;
}

GcellState FillToolMouseHandler::getPenStateByMouseButtons(Qt::MouseButtons buttons) const
{
    const bool isLmbPressed = buttons & Qt::LeftButton;

    if (fillMode_ == FillToolMode::MarkInfection) {
        if (isLmbPressed) {
            return GCELL_INFECTED;
        }
        else {
            return GCELL_HEALTHY;
        }
    }
    else {
        if (isLmbPressed) {
            return GCELL_HEALTHY;
        }
        else {
            return GCELL_NONE;
        }
    }
}

bool FillToolMouseHandler::isFillActiveWithMouseButtons(Qt::MouseButtons buttons) const
{
    const bool isLmbPressed = buttons & Qt::LeftButton;
    const bool isRmbPressed = buttons & Qt::RightButton;
    return isLmbPressed || isRmbPressed;
}

qreal FillToolMouseHandler::getRawValueInPixel(const QPoint& pixel) const
{
    const auto slice = view_->getSlice();

    const QPoint cell = griddedView_->getCellForScreenPixel(pixel);

    return slice->getPixel(cell);
}

qreal FillToolMouseHandler::getMidGcellRawValue(const QPoint& gcell) const
{
    const auto slice = view_->getSlice();

    const QPoint center = griddedView_->getGcellCenterOnPixmap(gcell);
    const qreal pixelValue = slice->getPixel(center);

    return pixelValue;
}

void FillToolMouseHandler::performFill(const QPoint& gcell, qreal sampleColor, GcellState penState)
{
    const auto slice = view_->getSlice();

    const int width = griddedView_->getNumGcellsX();
    const int height = griddedView_->getNumGcellsY();

    if (!isMidGcellRawValueSimilar(gcell, sampleColor)) {
        return;
    }

    QSet<QPoint> crossedGcells;
    QQueue<QPoint> queue;
    queue << gcell;

    griddedView_->startFillingMode();

    while (!queue.isEmpty()) {
        const QPoint gcell = queue.dequeue();
        if (crossedGcells.contains(gcell)) {
            continue;
        }

        QPoint west = gcell;
        while ((west.x() >= 0) && isMidGcellRawValueSimilar(west, sampleColor)) {
            if (west.y() > 0) {
                QPoint north = west;
                north.ry()--;
                if (isMidGcellRawValueSimilar(north, sampleColor)) {
                    queue << north;
                }
            }
            if (west.y() < height - 2) {
                QPoint south = west;
                south.ry()++;
                if (isMidGcellRawValueSimilar(south, sampleColor)) {
                    queue << south;
                }
            }
            west.rx()--;
        }
        west.rx()++;

        QPoint east = gcell;
        east.rx()++;
        while ((east.x() < width - 1) && isMidGcellRawValueSimilar(east, sampleColor)) {
            if (east.y() > 0) {
                QPoint north = east;
                north.ry()--;
                if (isMidGcellRawValueSimilar(north, sampleColor)) {
                    queue << north;
                }
            }
            if (east.y() < height - 2) {
                QPoint south = east;
                south.ry()++;
                if (isMidGcellRawValueSimilar(south, sampleColor)) {
                    queue << south;
                }
            }
            east.rx()++;
        }
        east.rx()--;

        while (west.x() <= east.x()) {
            griddedView_->setGcellState(west, penState);
            crossedGcells << west;
            west.rx()++;
        }
    }

    griddedView_->finishFillingMode();
}

bool FillToolMouseHandler::isMidGcellCountedAsSimilar(const QPoint& gcell) const
{
    if (compareMode_ == GcellCompareMode::CompareRawPixelValue) {
        return isMidGcellRawValueSimilar(gcell, currentRawPixelValueSample_);
    }
    else if (compareMode_ == GcellCompareMode::CompareDisplayColor) {
        return isMidGcellColorSimilar(gcell, currentDisplayColorSample_);
    }
    else {
        return false;
    }
}

bool FillToolMouseHandler::isMidGcellRawValueSimilar(const QPoint& gcell, qreal sampleValue) const
{
    const auto value = getMidGcellRawValue(gcell);
    return isRawValueSimilar(value, sampleValue);
}

bool FillToolMouseHandler::isRawValueSimilar(qreal test, qreal sample) const
{
    if (test >= sample) {
        return (test - sample) < rawValueDiffThreshold_;
    }
    else {
        return (sample - test) < rawValueDiffThreshold_;
    }
}

QRgb FillToolMouseHandler::getColorInPixel(const QPoint& pixel) const
{
    const auto& image = view_->getImage();

    const QPoint cell = griddedView_->getCellForScreenPixel(pixel);
    const QRgb pixelColor = image.pixel(cell);

    return pixelColor;
}

QRgb FillToolMouseHandler::getMidGcellColor(const QPoint& gcell) const
{
    const auto& image = view_->getImage();

    const QPoint center = griddedView_->getGcellCenterOnPixmap(gcell);
    const QRgb pixelColor = image.pixel(center);

    return pixelColor;
}

bool FillToolMouseHandler::isMidGcellColorSimilar(const QPoint& gcell, QRgb sampleColor) const
{
    const auto color = getMidGcellColor(gcell);
    return isColorSimilar(color, sampleColor);
}

bool FillToolMouseHandler::isColorSimilar(QRgb test, QRgb sample) const
{
    const int testR = qRed(test);
    const int testG = qGreen(test);
    const int testB = qBlue(test);

    const int sampleR = qRed(sample);
    const int sampleG = qGreen(sample);
    const int sampleB = qBlue(sample);

    const int dR = testR - sampleR;
    const int dG = testG - sampleG;
    const int dB = testB - sampleB;

    const int sqrD = dR*dR + dG*dG + dB*dB;
    return (sqrD < colorSqrDistanceThreshold_);
}

FillAction::FillAction()
    : state(GcellState::GCELL_NONE)
{
}

FillAction::FillAction(QPoint gcell_, GcellState state_)
    : gcell(gcell_)
    , state(state_)
{
}

bool operator !=(const FillAction& left, const FillAction& right)
{
    return (left.state != right.state) || (left.gcell != right.gcell);
}

bool operator ==(const FillAction& left, const FillAction& right)
{
    // Non-equal operator is faster in average case, so we use it
    return !(left != right);
}

} // namespace valt
} // namespace sklyph
