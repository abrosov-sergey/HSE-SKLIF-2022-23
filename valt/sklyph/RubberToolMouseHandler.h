#ifndef SKLYPHVALT_RUBBERTOOLMOUSEHANDLER_H
#define SKLYPHVALT_RUBBERTOOLMOUSEHANDLER_H

#include "AbstractToolMouseHandler.h"

#include "QMouseEvent"

namespace sklyph{
namespace valt{

    class DicomGriddedViewWidget;

    enum class RubberToolMode {
        MarkInfection = 0,
        MarkTotalVolume
    };

    class RubberToolMouseHandler: public AbstractToolMouseHandler
    {
        Q_OBJECT
    public:
        explicit RubberToolMouseHandler();
        ~RubberToolMouseHandler() override = default;
        void processMouseEvent(QMouseEvent* event) override;
        void setDicomView(DicomViewWidget* view) override;

        void setMode(RubberToolMode mode);
        void reset() override;

    private:
        void processMousePressEvent(QMouseEvent* event);
        void processMouseMoveEvent(QMouseEvent* event);
    private: // fields
        DicomGriddedViewWidget* griddedView_; /// Указатель на тот же view, только приведённый к типу Gridded

        QPoint lastRegisteredGridCell_;
        RubberToolMode mode_;
    };
}
}

#endif //SKLYPHVALT_RUBBERTOOLMOUSEHANDLER_H
