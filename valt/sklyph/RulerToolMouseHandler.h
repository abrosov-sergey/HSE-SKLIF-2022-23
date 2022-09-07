#ifndef SKLYPHVALT_RULERTOOLMOUSEHANDLER_H
#define SKLYPHVALT_RULERTOOLMOUSEHANDLER_H

#include "AbstractToolMouseHandler.h"
#include "DicomSlice.h"

namespace sklyph {
    namespace valt {

        class DicomGriddedViewWidget;

        class RulerToolMouseHandler: public AbstractToolMouseHandler
        {
            Q_OBJECT
        public:
            explicit RulerToolMouseHandler();
            ~RulerToolMouseHandler() override = default;

            void processMouseEvent(QMouseEvent* event) override;

            void setDicomView(DicomViewWidget* view) override;

            qreal getDistance();

            void reset() override;

        private:
            void addPoint(const QPoint& gcell);
            void processMousePressEvent(QMouseEvent* event);
            void clearVertices();
            static GcellState getPenStateByMouseButtons(const Qt::MouseButtons& buttons);
            bool isPointsContainsCoordinates();

        private: // fields
            DicomGriddedViewWidget* griddedView_;
            qreal distanceBetweenTwoPoints_ = 0.0;

            QPoint firstPoint = QPoint();
            QPoint secondPoint = QPoint();
        signals:
            void updateChanges();
        };

    } // namespace valt
}

#endif //SKLYPHVALT_RULERTOOLMOUSEHANDLER_H
