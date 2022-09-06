#include "PenToolMouseHandler.h"

#include "DicomGriddedViewWidget.h"

const QPoint INVALID_GRID_CELL(-1, -1);

// Начало: Абросов Сергей
// #include <bits/stdc++.h>
// using namespace std;
// Конец: Абросов Сергей

namespace sklyph {
namespace valt {

PenToolMouseHandler::PenToolMouseHandler()
    : AbstractToolMouseHandler()
    , mode_(PenToolMode::MarkTotalVolume)
    , lastRegisteredGridCell_(INVALID_GRID_CELL)
{
}

void PenToolMouseHandler::processMouseEvent(QMouseEvent* event)
{
    Q_ASSERT(event);

    switch (event->type()) {
        case QMouseEvent::MouseButtonPress: {
            processMousePressEvent(event);
            break;
        }
        case QMouseEvent::MouseMove: {
            processMouseMoveEvent(event);
            break;
        }
        default: {
            event->ignore();
            break;
        }
    }
}

void PenToolMouseHandler::setDicomView(DicomViewWidget* view)
{
    AbstractToolMouseHandler::setDicomView(view);

    griddedView_ = static_cast<DicomGriddedViewWidget*>(view_);
}

void PenToolMouseHandler::reset()
{

}

void PenToolMouseHandler::setMode(PenToolMode mode)
{
    mode_ = mode;
}

void PenToolMouseHandler::processMousePressEvent(QMouseEvent* event)
{
    lastRegisteredGridCell_ = INVALID_GRID_CELL;

    // Всю необходимую информацию о кнопках и координате мыши в процессе рисования
    // мы обрабатываем в событии движения мыши
    processMouseMoveEvent(event);
}

// Начало: Абросов Сергей.
void PenToolMouseHandler::getHounsfieldDensity(QImage image, QPoint pos) {
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

    // Обновляем плотность в выбранной области.
    densityOfHounsfield = finalHounsfieldDensity;

    emit densityChanged();
}
// Конец: Абросов Сергей.

void PenToolMouseHandler::processMouseMoveEvent(QMouseEvent* event)
{
    griddedView_->startFillingMode();
    const QPoint pos = event->pos();
    const QPoint gridCell = griddedView_->getGridCellByScreenPixel(pos);
    if (gridCell == lastRegisteredGridCell_) {
        event->ignore();
        return;
    }

    const bool isLmbPressed = event->buttons() & Qt::LeftButton;
    const bool isRmbPressed = event->buttons() & Qt::RightButton;

    if (!isLmbPressed && !isRmbPressed) {
        event->ignore();
        return;
    }

    const GcellState penState = getPenStateByMouseButtons(isLmbPressed, isRmbPressed);
    griddedView_->triggerStateActionInPixel(pos, penState);

    lastRegisteredGridCell_ = gridCell;

    getHounsfieldDensity(this->view_->getImage(), pos);
    griddedView_->finishFillingMode();
    event->accept();
}

GcellState PenToolMouseHandler::getPenStateByMouseButtons(bool isLmbPressed, bool isRmbPressed) const
{
    GcellState penState = GCELL_NONE;
    if (mode_ == PenToolMode::MarkInfection) {
        if (isLmbPressed) {
            penState = GCELL_INFECTED;
        }
        else if (isRmbPressed) {
            penState = GCELL_HEALTHY;
        }
    }
    else {
        if (isLmbPressed) {
            penState = GCELL_HEALTHY;
        }
        else if (isRmbPressed) {
            penState = GCELL_NONE;
        }
    }

    return penState;
}
} // namespace valt
} // namespace sklyph
