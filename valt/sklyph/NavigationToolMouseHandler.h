#ifndef SKLYPH_NAVIGATIONTOOLMOUSEHANDLER_H
#define SKLYPH_NAVIGATIONTOOLMOUSEHANDLER_H

#include "AbstractToolMouseHandler.h"
#include "DicomSlice.h"

#include "common.h"

#include <QMouseEvent>

namespace sklyph {
namespace valt {

class NavigationToolMouseHandler: public AbstractToolMouseHandler
{
    Q_OBJECT
public:
    NavigationToolMouseHandler();
    ~NavigationToolMouseHandler() override = default;

    void processMouseEvent(QMouseEvent* event) override;

    void reset() override;

private slots:
    void processDrag();

private: // methods
    void processMousePressEvent(QMouseEvent* event);
    void processMouseMoveEvent(QMouseEvent* event);

    ///
    /// \threadsafe
    ///
    void setMouseDragPosition(const QPoint& pos);
    bool isDragActive() const;

private: // fields
    QPoint mouseDragPosition_;
    QPoint lastAcceptedMouseDragPosition_;

    enum DragMode { DragOff, DragPosition, DragEditBrightness, DragRuler };
    DragMode dragMode_;
    QTimer* dragProcessTimer_;
    QMutex mouseDragMutex_;
};

} // namespace valt
} // namespace sklyph

#endif // SKLYPH_NAVIGATIONTOOLMOUSEHANDLER_H
