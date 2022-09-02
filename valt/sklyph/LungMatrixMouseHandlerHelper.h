#ifndef SKLYPH_LUNGMATRIXMOUSEHANDLER_H
#define SKLYPH_LUNGMATRIXMOUSEHANDLER_H

#include "AbstractToolMouseHandler.h"

#include "common.h"

#include <QMouseEvent>

namespace sklyph {
namespace valt {

typedef enum {
    DummyTool = -1,
    NavigationTool,
    PenTool,
    FillTool,
    PolygonTool
} EditorTool;

class LungMatrixMouseHandlerHelper: public AbstractToolMouseHandler
{
    Q_OBJECT
public:
    explicit LungMatrixMouseHandlerHelper();
    ~LungMatrixMouseHandlerHelper() override = default;

    void processMouseEvent(QMouseEvent* event) override;

    void setDicomView(DicomViewWidget* view) override;

    void reset() override;

    void addTool(EditorTool type, AbstractToolMouseHandler* handler);

public slots:
    void setActiveTool(EditorTool tool);

private:
    EditorTool activeTool_;

    QHash<EditorTool, AbstractToolMouseHandlerPtr> tools_;
};

} // namespace valt
} // namespace sklyph

#endif // SKLYPH_LUNGMATRIXMOUSEHANDLER_H
