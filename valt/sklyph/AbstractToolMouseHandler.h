#ifndef SKLYPH_ABSTRACTTOOLMOUSEHANDLER_H
#define SKLYPH_ABSTRACTTOOLMOUSEHANDLER_H

#include "DicomSlice.h"

#include "common.h"

#include <QMouseEvent>

namespace sklyph {
namespace valt {

class DicomViewWidget;

class AbstractToolMouseHandler: public QObject
{
    Q_OBJECT
public:
    AbstractToolMouseHandler();
    virtual ~AbstractToolMouseHandler() = default;

    virtual void processMouseEvent(QMouseEvent* event) = 0;

    virtual void setDicomView(DicomViewWidget* view);

    virtual void reset() = 0;

protected:
    DicomViewWidget* view_;
};

typedef QSharedPointer<AbstractToolMouseHandler> AbstractToolMouseHandlerPtr;

} // namespace valt
} // namespace sklyph

#endif // SKLYPH_ABSTRACTTOOLMOUSEHANDLER_H
