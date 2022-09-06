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
    griddedView_->startFillingMode();
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

    // Расстояние между двумя последними отмеченными точками в полигоне.
    if (gcellVertices_.size() >= 2) {
        distanceBetweenTwoPoints = GetDistanceBetweenTwoPoints(gcellVertices_[gcellVertices_.size() - 2],
                                                               gcellVertices_[gcellVertices_.size() - 1]);
    }
    griddedView_->finishFillingMode();
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

    maxXPosition = -100000;
    for (int index = 0; index < gcellVertices_.size() - 1; index++) {
        maxXPosition = std::max(maxXPosition, gcellVertices_[index].x());
    }

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

qreal PolygonToolMouseHandler::getDensityInPolygon(const QList<QPoint>&) {
    qreal densitySum = 0.0;

    for (int i = 0; i < pointsInPolygon.size(); ++i) {
        densitySum += getHounsfieldDensityFromGcell(this->view_->getImage(), griddedView_->getGcellCenterOnScreen(pointsInPolygon[i]));
    }

    return densitySum / pointsInPolygon.size();
}

// Начало: Абросов Сергей.
qreal PolygonToolMouseHandler::getHounsfieldDensityFromGcell(QImage image, QPoint pos) {
    // Константа, основанная на размерах QImage
    const int number = std::trunc(12 * griddedView_->gridStepCm());

    // Левая верхняя и правая нижняя позиции выбранной ячейки.
    const QPoint positionLeftCorner = QPoint((pos.x() / number - (pos.x() % number == 0)) * number,
                                             (pos.y() / number - (pos.y() % number == 0)) * number);

    const QPoint positionRightCorner = QPoint(positionLeftCorner.x() + number, positionLeftCorner.y() + number);

    // Сумма плотностей Хаунсфилда в выбранной области.
    long double sumHounsfieldDensity = 0.0;

    // Пересчитываем среднюю плотность в выбранной области.
    for (int x = positionLeftCorner.x(); x <= positionRightCorner.x(); ++x) {
        for (int y = positionLeftCorner.y(); y <= positionRightCorner.y(); ++y) {
            // Составляющая красного в RGB.
            const int redProportion = qRed(image.pixel(x, y));
            const long double copyRedProportion = redProportion;

            // Считаем плотность Хаунсфилда в точке (x, y).
            const long double nowHounsfieldDensity = -1000 + (4000 * copyRedProportion) / 255;

            // Прибавляем к сумме всех ранее посчитанных плотностей.
            sumHounsfieldDensity += nowHounsfieldDensity;
        }
    }

    // Количество точек, вошедших в подчест.
    const long double countPointsInCells = (positionRightCorner.x() - positionLeftCorner.x() + 1) * (positionRightCorner.y() - positionLeftCorner.y() + 1);

    // Считаем среднее значение (сумму делим на количество точек вошедших в подсчет).
    const long double finalHounsfieldDensity = sumHounsfieldDensity / countPointsInCells;

    return finalHounsfieldDensity;
}
// Конец: Абросов Сергей.

void PolygonToolMouseHandler::updateDensityInPolygon(const qreal density) {
    densityOfHounsfieldInPolygon = density;

    emit updateChanges();
}

void PolygonToolMouseHandler::invertRightGcells(const QPoint& p1, const QPoint& p2, int vertLineX)
{
    if (p1.y() == p2.y()) {
        return;
    }
    else if  (p1.x() == p2.x()) {
        invertRect(std::min(maxXPosition, p1.x()), p1.y(), std::min(maxXPosition, vertLineX), p2.y());
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
            invertLine(y, std::min(maxXPosition, x), std::min(maxXPosition, vertLineX));
        } else {
            invertLine(y, std::min(maxXPosition, vertLineX), std::min(maxXPosition, x));
        }
    }

    updateDensityInPolygon(getDensityInPolygon(pointsInPolygon));
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
    pointsInPolygon.push_back(QPoint(x0, y));
    gcellStates_[x0][y] = true;

    for (int x = x0 + 1; x < std::min(maxXPosition, x1); x++) {
        pointsInPolygon.push_back(QPoint(x, y));
        invertGcell(x, y);
    }
}

void PolygonToolMouseHandler::invertGcell(int x, int y)
{
    if (x <= maxXPosition) {
        gcellStates_[x][y] = !gcellStates_[x][y];
    }
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
