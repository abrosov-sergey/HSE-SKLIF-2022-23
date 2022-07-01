#include "AbstractToolMouseHandler.h"

namespace sklyph {
namespace valt {

AbstractToolMouseHandler::AbstractToolMouseHandler()
    : QObject(nullptr)
    , view_(nullptr)
{
}

void AbstractToolMouseHandler::setDicomView(DicomViewWidget* view)
{
    view_ = view;
}

} // namespace valt
} // namespace sklyph
