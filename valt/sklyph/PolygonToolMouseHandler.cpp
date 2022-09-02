#include "PolygonToolMouseHandler.h"

#include "DicomGriddedViewWidget.h"

#include <QQueue>

// Правки начало, Сергей Абросов
#include <iostream>
#include <algorithm>
#include <iomanip>
// Праввки конец, Сергей Абросов

namespace sklyph {
namespace valt {

PolygonToolMouseHandler::PolygonToolMouseHandler()
    : AbstractToolMouseHandler()
    , griddedView_(nullptr)
    , fillMode_(PolygonlToolMode::MarkTotalVolume)
    , currentPenState_(GcellState::GCELL_INFECTED)
{
}

// Правки начало, Сергей Абросов
double GetSqrOfNumber(double number) {
    return number * number;
}

double GetDistanceBetweenTwoPoints(QPoint firstPoint, QPoint secondPoint) {
    return sqrt(GetSqrOfNumber(firstPoint.x() - secondPoint.x()) + GetSqrOfNumber(firstPoint.y() - secondPoint.y()));
}
// Правки конец, Сергей Абросов

void PolygonToolMouseHandler::processMouseEvent(QMouseEvent* event)
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

void PolygonToolMouseHandler::setDicomView(DicomViewWidget* view)
{
    AbstractToolMouseHandler::setDicomView(view);

    griddedView_ = static_cast<DicomGriddedViewWidget*>(view);

    clearVertices();
}

void PolygonToolMouseHandler::reset()
{
    clearVertices();
}

void PolygonToolMouseHandler::setMode(PolygonlToolMode mode)
{
    fillMode_ = mode;

    clearVertices();
}

void PolygonToolMouseHandler::processMousePressEvent(QMouseEvent* event)
{
    const auto slice = view_->getSlice();
    if (!slice) {
        return;
    }

    const auto state = getPenStateByMouseButtons(event->buttons());
    if (state != currentPenState_) {
        clearVertices();

        currentPenState_ = state;
    }

    const auto posPixel = event->pos();
    const auto posGcell = griddedView_->getGcellByScreenPixel(posPixel);

    if (gcellVertices_.contains(posGcell)) {
        addGcellVertice(posGcell);

        finishDrawingPolygon();

        event->accept();
        return;
    }

    addGcellVertice(posGcell);
    event->accept();
    // Правки начало, Сергей Абросов
// Расстояние между двумя последними отмеченными точками в полигоне.
    if (gcellVertices_.size() >= 2) {
//        std::cout << std::fixed << std::showpoint << std::setprecision(9);
//        std::cout << GetDistanceBetweenTwoPoints(gcellVertices_[gcellVertices_.size() - 2],
//                                                 gcellVertices_[gcellVertices_.size() - 1]) /*/
//                                             (double (scalingFactorSpinBox_->value()) / 100)*/ << std::endl;
        distanceBetweenTwoPoints = GetDistanceBetweenTwoPoints(gcellVertices_[gcellVertices_.size() - 2],
                                                               gcellVertices_[gcellVertices_.size() - 1]);
    }
// Правки конец, Сергей Абросов
}

GcellState PolygonToolMouseHandler::getPenStateByMouseButtons(Qt::MouseButtons buttons) const
{
    const bool isLmbPressed = buttons & Qt::LeftButton;

    if (fillMode_ == PolygonlToolMode::MarkInfection) {
        if (isLmbPressed) {
            return GCELL_INFECTED;
        }
        else {
            return GCELL_HEALTHY;
        }
    }
    else {
        if (isLmbPressed) {
            return GCELL_HEALTHY;
        }
        else {
            return GCELL_NONE;
        }
    }
}

void PolygonToolMouseHandler::addGcellVertice(const QPoint& gcell)
{
    gcellVertices_ << gcell;

    griddedView_->addTemporaryPolygonVertex(gcell);
}

void PolygonToolMouseHandler::finishDrawingPolygon()
{
    fillPolygon();
    clearVertices();
}

void PolygonToolMouseHandler::fillPolygon()
{
    if (gcellVertices_.size() < 3) {
        return;
    }

    const auto width = griddedView_->getNumGcellsX();

    for (int index = 0; index < gcellVertices_.size() - 1; index++) {
        invertRightGcells(gcellVertices_[index], gcellVertices_[index + 1], width);
    }

    pushGcellStatesToView(currentPenState_);
}

void PolygonToolMouseHandler::pushGcellStatesToView(GcellState currentPenState)
{
    griddedView_->startFillingMode();

    for (int y = 0; y < gcellStates_.size(); y++) {
        const auto row = gcellStates_[y];
        for (int x = 0; x < row.size(); x++) {
            if (gcellStates_[x][y]) {
                griddedView_->setGcellState(QPoint(x, y), currentPenState);
            }
        }
    }

    griddedView_->finishFillingMode();
}

void PolygonToolMouseHandler::clearVertices()
{
    gcellVertices_.clear();

    const auto sizeX = griddedView_->getNumGcellsX();
    const auto sizeY = griddedView_->getNumGcellsY();

    gcellStates_.resize(sizeX);
    for (auto& row : gcellStates_) {
        row.resize(sizeY);
        row.fill(false);
    }

    griddedView_->clearTemporaryPolygon();
}

int PolygonToolMouseHandler::getCenterVerticalLineX() const
{
    if (gcellVertices_.isEmpty()) {
        return 0;
    }

    int sumX = 0;
    for (const auto& gcellVertice : gcellVertices_) {
        sumX += gcellVertice.x();
    }

    return sumX / gcellVertices_.size();
}

void PolygonToolMouseHandler::invertRightGcells(const QPoint& p1, const QPoint& p2, int vertLineX)
{
    if (p1.y() == p2.y()) {
        return;
    }
    else if  (p1.x() == p2.x()) {
        invertRect(p1.x(), p1.y(), vertLineX, p2.y());
        return;
    }

    const auto k = lineCoef(p1, p2);

    const int x0 = p1.x();
    const int y0 = p1.y();
    const int y1 = p2.y();
    const int yStart = qMin(y0, y1);
    const int yEnd = qMax(y0, y1);

    int dx = 0;
    int x = 0;
    for (int y = yStart; y <= yEnd; y++) {
        dx = qRound(k * qreal(y - y0));
        x = x0 + dx;

        if (x <= vertLineX) {
            invertLine(y, x, vertLineX);
        } else {
            invertLine(y, vertLineX, x);
        }
    }
}

void PolygonToolMouseHandler::invertRect(int x0, int y0, int x1, int y1)
{
    if (x0 <= x1) {
        if (y0 < y1) {
            invertRectNormalized(x0, y0, x1, y1);
        } else {
            invertRectNormalized(x0, y1, x1, y0);
        }
    } else {
        if (y0 <= y1) {
            invertRectNormalized(x1, y0, x0, y1);
        } else {
            invertRectNormalized(x1, y1, x0, y0);
        }
    }
}

void PolygonToolMouseHandler::invertRectNormalized(int x0, int y0, int x1, int y1)
{
    for (int y = y0; y <= y1; y++) {
        invertLine(y, x0, x1);
    }
}

void PolygonToolMouseHandler::invertLine(int y, int x0, int x1)
{
    gcellStates_[x0][y] = true;
    for (int x = x0 + 1; x < x1; x++) {
        invertGcell(x, y);
    }
}

void PolygonToolMouseHandler::invertGcell(int x, int y)
{
    gcellStates_[x][y] = !gcellStates_[x][y];
}

qreal PolygonToolMouseHandler::lineCoef(const QPoint& p1, const QPoint& p2)
{
    const qreal x0f = p1.x();
    const qreal x1f = p2.x();
    const qreal y0f = p1.y();
    const qreal y1f = p2.y();

    return (x1f - x0f) / (y1f - y0f);
}

} // namespace valt
} // namespace sklyph
