#include "PenToolMouseHandler.h"

#include "DicomGriddedViewWidget.h"

const QPoint INVALID_GRID_CELL(-1, -1);

namespace sklyph {
namespace valt {

PenToolMouseHandler::PenToolMouseHandler()
    : AbstractToolMouseHandler()
    , mode_(PenToolMode::MarkTotalVolume)
    , lastRegisteredGridCell_(INVALID_GRID_CELL)
{
}

void PenToolMouseHandler::processMouseEvent(QMouseEvent* event)
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

void PenToolMouseHandler::setDicomView(DicomViewWidget* view)
{
    AbstractToolMouseHandler::setDicomView(view);

    griddedView_ = static_cast<DicomGriddedViewWidget*>(view_);
}

void PenToolMouseHandler::reset()
{

}

void PenToolMouseHandler::setMode(PenToolMode mode)
{
    mode_ = mode;
}

void PenToolMouseHandler::processMousePressEvent(QMouseEvent* event)
{
    lastRegisteredGridCell_ = INVALID_GRID_CELL;

    // Всю необходимую информацию о кнопках и координате мыши в процессе рисования
    // мы обрабатываем в событии движения мыши
    processMouseMoveEvent(event);
}

void PenToolMouseHandler::processMouseMoveEvent(QMouseEvent* event)
{
    const QPoint pos = event->pos();
    const QPoint gridCell = griddedView_->getGridCellByScreenPixel(pos);
    if (gridCell == lastRegisteredGridCell_) {
        event->ignore();
        return;
    }

    const bool isLmbPressed = event->buttons() & Qt::LeftButton;
    const bool isRmbPressed = event->buttons() & Qt::RightButton;

    if (!isLmbPressed && !isRmbPressed) {
        event->ignore();
        return;
    }

    const GcellState penState = getPenStateByMouseButtons(isLmbPressed, isRmbPressed);
    griddedView_->triggerStateActionInPixel(pos, penState);

    lastRegisteredGridCell_ = gridCell;

    event->accept();
}

GcellState PenToolMouseHandler::getPenStateByMouseButtons(bool isLmbPressed, bool isRmbPressed) const
{
    GcellState penState = GCELL_NONE;
    if (mode_ == PenToolMode::MarkInfection) {
        if (isLmbPressed) {
            penState = GCELL_INFECTED;
        }
        else if (isRmbPressed) {
            penState = GCELL_HEALTHY;
        }
    }
    else {
        if (isLmbPressed) {
            penState = GCELL_HEALTHY;
        }
        else if (isRmbPressed) {
            penState = GCELL_NONE;
        }
    }

    return penState;
}

} // namespace valt
} // namespace sklyph
