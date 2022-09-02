#ifndef SKLYPH_POLYGONTOOLMOUSEHANDLER_H
#define SKLYPH_POLYGONTOOLMOUSEHANDLER_H

#include "AbstractToolMouseHandler.h"
#include "DicomSlice.h"

#include <QImage>
#include <QMouseEvent>

namespace sklyph {
namespace valt {

class DicomGriddedViewWidget;

enum class PolygonlToolMode {
    MarkInfection = 0,
    MarkTotalVolume
};

///
/// \brief The PolygonToolMouseHandler class
///
///
class PolygonToolMouseHandler: public AbstractToolMouseHandler
{
    Q_OBJECT
public:
    explicit PolygonToolMouseHandler();
    ~PolygonToolMouseHandler() override = default;

    void processMouseEvent(QMouseEvent* event) override;

    void setDicomView(DicomViewWidget* view) override;

    // Вызывается из DicomViewWidget::reset()
    void reset() override;

    void setMode(PolygonlToolMode mode);

    // Начало: Абросов Сергей
    qreal distanceBetweenTwoPoints = 0.0;
    // Конец: Абросов Сергей
private: // methods
    void processMousePressEvent(QMouseEvent* event);

    GcellState getPenStateByMouseButtons(Qt::MouseButtons buttons) const;

    void addGcellVertice(const QPoint& gcell);
    void finishDrawingPolygon();

    void fillPolygon();
    void pushGcellStatesToView(GcellState currentPenState);
    void clearVertices();

    int getCenterVerticalLineX() const;
    void invertRightGcells(const QPoint& p1, const QPoint& p2, int vertLineX);
    void invertRect(int x0, int y0, int x1, int y1);
    void invertRectNormalized(int x0, int y0, int x1, int y1);
    void invertLine(int y, int x0, int x1);
    void invertGcell(int x, int y);
    qreal lineCoef(const QPoint& p1, const QPoint& p2);

private: // fields
    DicomGriddedViewWidget* griddedView_;

    PolygonlToolMode fillMode_;

    GcellState currentPenState_;

    QList<QPoint> gcellVertices_;
    QVector<QVector<bool>> gcellStates_;
};

} // namespace valt
} // namespace sklyph

#endif // SKLYPH_POLYGONTOOLMOUSEHANDLER_H
