#include "RulerToolMouseHandler.h"

#include "DicomGriddedViewWidget.h"

namespace sklyph {
    namespace valt {

        RulerToolMouseHandler::RulerToolMouseHandler()
                : AbstractToolMouseHandler()
                , griddedView_(nullptr)
        {
        }

        double getSqrOfNumber(double number) {
            return number * number;
        }

        double getDistanceBetweenTwoPoints(QPoint firstPoint, QPoint secondPoint) {
            return sqrt(getSqrOfNumber(firstPoint.x() - secondPoint.x()) + getSqrOfNumber(firstPoint.y() - secondPoint.y()));
        }

        void RulerToolMouseHandler::processMouseEvent(QMouseEvent* event)
        {
            Q_ASSERT(event);

            switch (event->type()) {
                case QMouseEvent::MouseButtonPress: {
                    processMousePressEvent(event);
                    break;
                }
                default: {
                    event->ignore();
                    break;
                }
            }
        }

        void RulerToolMouseHandler::setDicomView(DicomViewWidget* view)
        {
            AbstractToolMouseHandler::setDicomView(view);

            griddedView_ = static_cast<DicomGriddedViewWidget*>(view);

            clearVertices();
        }

        void RulerToolMouseHandler::reset()
        {
            clearVertices();
        }

        qreal RulerToolMouseHandler::getDistance()
        {
            return distanceBetweenTwoPoints_;
        }

        void RulerToolMouseHandler::processMousePressEvent(QMouseEvent* event)
        {
            const auto slice = view_->getSlice();
            if (!slice) {
                return;
            }

            const GcellState state = getPenStateByMouseButtons(event->buttons());
            if (state == GCELL_HEALTHY || isPointsContainsCoordinates())
            {
                clearVertices();
                distanceBetweenTwoPoints_ = 0;
                event->accept();

                emit updateChanges();
                return;
            }

            const auto posPixel = event->pos();
            const auto posGcell = griddedView_->getGcellByScreenPixel(posPixel);

            addPoint(posGcell);
            event->accept();

            if (isPointsContainsCoordinates()) {
                distanceBetweenTwoPoints_ = getDistanceBetweenTwoPoints(firstPoint,
                                                                       secondPoint);
            } else {
                distanceBetweenTwoPoints_ = 0;
            }

            emit updateChanges();
        }

        void RulerToolMouseHandler::addPoint(const QPoint& gcell)
        {
            firstPoint = secondPoint;
            secondPoint = gcell;
            griddedView_->addRulerPoint(gcell);
        }

        GcellState RulerToolMouseHandler::getPenStateByMouseButtons(const Qt::MouseButtons& buttons)
        {
            const bool isLmbPressed = buttons & Qt::LeftButton;

            if (isLmbPressed)
            {
                return GCELL_INFECTED;
            } else
            {
                return GCELL_HEALTHY;
            }
        }

        void RulerToolMouseHandler::clearVertices()
        {
            firstPoint = QPoint();
            secondPoint = QPoint();
            griddedView_->clearRuler();
        }

        bool RulerToolMouseHandler::isPointsContainsCoordinates()
        {
           return !(firstPoint.isNull() || secondPoint.isNull());
        }
    } // namespace valt
} // namespace sklyph