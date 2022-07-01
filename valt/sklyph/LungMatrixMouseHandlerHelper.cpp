#include "LungMatrixMouseHandlerHelper.h"

namespace sklyph {
namespace valt {

LungMatrixMouseHandlerHelper::LungMatrixMouseHandlerHelper()
    : AbstractToolMouseHandler()
    , activeTool_(EditorTool::DummyTool)
{
}

void LungMatrixMouseHandlerHelper::processMouseEvent(QMouseEvent* event)
{
    Q_ASSERT(event);

    if (tools_.contains(activeTool_)) {
        tools_[activeTool_]->processMouseEvent(event);
    }
}

void LungMatrixMouseHandlerHelper::setDicomView(DicomViewWidget* view)
{
    for (auto tool : tools_.values()) {
        tool->setDicomView(view);
    }
}

void LungMatrixMouseHandlerHelper::reset()
{
    for (auto tool : tools_.values()) {
        tool->reset();
    }
}

void LungMatrixMouseHandlerHelper::addTool(EditorTool type, AbstractToolMouseHandler* handler)
{
    tools_.insert(type, AbstractToolMouseHandlerPtr(handler));
}

void LungMatrixMouseHandlerHelper::setActiveTool(EditorTool tool)
{
    activeTool_ = tool;
}

} // namespace valt
} // namespace sklyph
