#include "NavigationToolMouseHandler.h"

#include "DicomViewWidget.h"

namespace sklyph {
namespace valt {

const int MOUSE_DRAG_THRESHOLD_PIXELS = 50;
const int MOUSE_DRAG_TICK_PERIOD_MSEC = 30;
const qreal MOUSE_DRAG_EDIT_BRIGHTNTESS_COEF = 2.0;

NavigationToolMouseHandler::NavigationToolMouseHandler()
    : AbstractToolMouseHandler()
    , dragProcessTimer_(nullptr)
{
    dragProcessTimer_ = new QTimer(this);
    connect(dragProcessTimer_, SIGNAL(timeout()), this, SLOT(processDrag()));
    dragProcessTimer_->start(MOUSE_DRAG_TICK_PERIOD_MSEC);
}

void NavigationToolMouseHandler::processMouseEvent(QMouseEvent* event)
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
            break;
        }
    }
}

void NavigationToolMouseHandler::reset()
{
    mouseDragMutex_.lock();
    mouseDragPosition_ = QPoint(0, 0);
    lastAcceptedMouseDragPosition_ = QPoint(0, 0);
    dragMode_ = DragOff;
    mouseDragMutex_.unlock();
}

void NavigationToolMouseHandler::processDrag()
{
    const auto slice = view_->getSlice();
    if ((slice.isNull()) || (dragMode_ == DragOff)) {
        return;
    }

    mouseDragMutex_.lock();
    const QPoint mouseDragPosition = mouseDragPosition_;
    const QPoint lastMouseDragPosition = lastAcceptedMouseDragPosition_;
    lastAcceptedMouseDragPosition_ = mouseDragPosition_;
    mouseDragMutex_.unlock();

    if (dragMode_ == DragPosition) {
        const QPoint newCell = view_->getCellForScreenPixel(mouseDragPosition);
        const QPoint lastCell = view_->getCellForScreenPixel(lastMouseDragPosition);
        view_->moveViewport(newCell - lastCell);
    }
    else if (dragMode_ == DragEditBrightness) {
        const int windowWidth = slice->getWindowWidth();
        const int windowCenter = slice->getWindowCenter();

        const QPoint vector = mouseDragPosition - lastMouseDragPosition;
        const int deltaX = qRound(qreal(vector.x()) * MOUSE_DRAG_EDIT_BRIGHTNTESS_COEF);
        const int deltaY = qRound(qreal(vector.y()) * MOUSE_DRAG_EDIT_BRIGHTNTESS_COEF);

        slice->setWindowWidth(windowWidth + deltaX);
        slice->setWindowCenter(windowCenter + deltaY);
        slice->update();

        view_->onSliceUpdatedByMouseHandler();
    }
}

void NavigationToolMouseHandler::processMousePressEvent(QMouseEvent* event)
{
    const bool isLeftButtonPressed = event->buttons() & Qt::LeftButton;
    const bool isRightButtonPressed = event->buttons() & Qt::RightButton;
    const bool isMiddleButtonPressed = event->buttons() & Qt::MiddleButton;

    if (isLeftButtonPressed && isRightButtonPressed) {
        return;
    }

    if (isLeftButtonPressed || isRightButtonPressed || isMiddleButtonPressed) {
        setMouseDragPosition(event->pos());

        dragMode_ = DragOff;
        event->accept();
        return;
    }
}

void NavigationToolMouseHandler::processMouseMoveEvent(QMouseEvent* event)
{
    const bool isLeftButtonPressed = event->buttons() & Qt::LeftButton;
    const bool isRightButtonPressed = event->buttons() & Qt::RightButton;
    const bool isMiddleButtonPressed = event->buttons() & Qt::MiddleButton;

    if (isLeftButtonPressed && isRightButtonPressed) {
        return;
    }

    event->accept();

    const QPoint& pos = event->pos();
    const QPoint moveVector = pos - mouseDragPosition_;

    if (isLeftButtonPressed) {
        if (dragMode_ == DragOff) {
            if (moveVector.manhattanLength() > MOUSE_DRAG_THRESHOLD_PIXELS) {
                dragMode_ = DragPosition;

                mouseDragMutex_.lock();
                lastAcceptedMouseDragPosition_ = mouseDragPosition_;
                mouseDragMutex_.unlock();
            }
        }
    }
    else if (isMiddleButtonPressed) {
        if (dragMode_ == DragOff) {
            if (moveVector.manhattanLength() > MOUSE_DRAG_THRESHOLD_PIXELS) {
                dragMode_ = DragEditBrightness;

                mouseDragMutex_.lock();
                lastAcceptedMouseDragPosition_ = mouseDragPosition_;
                mouseDragMutex_.unlock();
            }
        }
    }

    if (dragMode_ != DragOff) {
        setMouseDragPosition(pos);
    }
}

void NavigationToolMouseHandler::setMouseDragPosition(const QPoint& pos)
{
    mouseDragMutex_.lock();
    mouseDragPosition_ = pos;
    mouseDragMutex_.unlock();
}

bool NavigationToolMouseHandler::isDragActive() const
{
    return dragMode_ != DragOff;
}


} // namespace valt
} // namespace sklyph
