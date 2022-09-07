//
// Created by Антон Калинин on 07.09.2022.
//

#include "RubberToolMouseHandler.h"
#include "DicomGriddedViewWidget.h"

const QPoint INVALID_GRID_CELL(-1, -1);

namespace sklyph
{
    namespace valt
    {

        RubberToolMouseHandler::RubberToolMouseHandler()
                : AbstractToolMouseHandler(),
                  lastRegisteredGridCell_(INVALID_GRID_CELL),
                  mode_(RubberToolMode::MarkTotalVolume)
        {

        }

        void RubberToolMouseHandler::processMouseEvent(QMouseEvent *event)
        {
            Q_ASSERT(event);

            switch (event->type())
            {
                case QMouseEvent::MouseButtonPress:
                {
                    processMousePressEvent(event);
                    break;
                }
                case QMouseEvent::MouseMove:
                {
                    processMouseMoveEvent(event);
                    break;
                }
                default:
                {
                    event->ignore();
                    break;
                }
            }
        }

        void RubberToolMouseHandler::setDicomView(DicomViewWidget *view)
        {
            AbstractToolMouseHandler::setDicomView(view);

            griddedView_ = static_cast<DicomGriddedViewWidget *>(view_);
        }

        void RubberToolMouseHandler::setMode(RubberToolMode mode)
        {
            mode_ = mode;
        }

        void RubberToolMouseHandler::reset()
        {

        }

        void RubberToolMouseHandler::processMousePressEvent(QMouseEvent *event)
        {
            lastRegisteredGridCell_ = INVALID_GRID_CELL;

            processMouseMoveEvent(event);
        }

        void RubberToolMouseHandler::processMouseMoveEvent(QMouseEvent *event)
        {
            griddedView_->startFillingMode();
            const QPoint pos = event->pos();
            const QPoint gridCell = griddedView_->getGridCellByScreenPixel(pos);
            if (gridCell == lastRegisteredGridCell_)
            {
                event->ignore();
                return;
            }

            const bool isLmbPressed = event->buttons() & Qt::LeftButton;
            const bool isRmbPressed = event->buttons() & Qt::RightButton;

            if (!isLmbPressed && !isRmbPressed)
            {
                event->ignore();
                return;
            }

            griddedView_->removeFromMatrix(pos);

            lastRegisteredGridCell_ = gridCell;

            griddedView_->finishFillingMode();
            event->accept();
        }
    } // namespace valt
} // namespace sklyph