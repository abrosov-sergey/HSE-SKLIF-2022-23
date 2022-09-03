#include "DicomGriddedViewWidget.h"

#include "AbstractToolMouseHandler.h"
#include "LungMatrixMouseHandlerHelper.h"

#include "common.h"

#include <QtCore>
#include <QPainter>

namespace sklyph {
namespace valt {

const int GRID_THICKNESS = 1;
const QColor GRID_COLOR(80, 80, 120, 100);
const QPen GRID_PEN(QBrush(GRID_COLOR), GRID_THICKNESS, Qt::DashLine);
const int TEMPORARY_POLYGON_THICKNESS = 1;
const QColor TEMPORARY_POLYGON_COLOR(230, 230, 230, 255);
const QPen TEMPORARY_POLYGON_PEN(QBrush(TEMPORARY_POLYGON_COLOR), TEMPORARY_POLYGON_THICKNESS, Qt::SolidLine);
const int TEMPORARY_POLYGON_START_POINT_RADIUS = 5;
const QBrush TEMPORARY_POLYGON_BRUSH(TEMPORARY_POLYGON_COLOR, Qt::SolidPattern);
const QColor TRANSPARENT_COLOR(Qt::transparent);
const QPen GCELL_BOUND_PEN(TRANSPARENT_COLOR);
const QColor GCELL_FULL_INFECTED_COLOR(200, 0, 0, 50);
const QColor GCELL_HALF_INFECTED_COLOR(200, 200, 0, 50);
const QColor GCELL_HEALTHY_COLOR(0, 200, 0, 35);
const QBrush GCELL_FULL_INFECTED_BRUSH(GCELL_FULL_INFECTED_COLOR, Qt::SolidPattern);
const QBrush GCELL_HALF_INFECTED_BRUSH(GCELL_HALF_INFECTED_COLOR, Qt::SolidPattern);
const QBrush GCELL_HEALTHY_BRUSH(GCELL_HEALTHY_COLOR, Qt::SolidPattern);

const int GRID_LAYER_INDEX = 112;
const int GCELL_LAYER_INDEX = 223;
const int TEMPORARY_LAYER_INDEX = 300;

DicomGriddedViewWidget::DicomGriddedViewWidget(qreal baseGridStep,
                                               QWidget* parent)
    : DicomViewWidget(parent)
    , kBaseGridStepCm(baseGridStep)
    , gridStepMultiplier_(2)
    , isInFillingMode_(false)
{
    reset();

    connect(this, SIGNAL(viewUpdated()), this, SLOT(repaint()));
}

void DicomGriddedViewWidget::paintEvent(QPaintEvent* event)
{
    QPixmap cellsLayer(this->size());
    QPainter cellsPainter(&cellsLayer);
    drawGcellsOnCanvas(cellsPainter);

    QPixmap gridLayer(this->size());
    QPainter gridPainter(&gridLayer);
    drawGridOnCanvas(gridPainter);

    QPixmap temporaryLayer(this->size());
    QPainter temporaryPainter(&temporaryLayer);
    drawTemporaryObjectsOnCanvas(temporaryPainter);

    setLayer(GCELL_LAYER_INDEX, cellsLayer);
    setLayer(GRID_LAYER_INDEX, gridLayer);
    setLayer(TEMPORARY_LAYER_INDEX, temporaryLayer);

    DicomViewWidget::paintEvent(event);
}

void DicomGriddedViewWidget::triggerStateActionInPixel(const QPoint& pixel, GcellState state)
{
    const QVector<QPoint> gcellIndices = getGcellIndicesForScreenPixel(pixel);

    setGcellStates(gcellIndices, state);
}

void DicomGriddedViewWidget::setGcellState(const QPoint& gcell, GcellState state)
{
    if (state == GcellState::GCELL_NONE) {
        lungTissueGcells_.remove(gcell);
        sliceInfectedGcells_.remove(gcell);
    }
    else if (state == GcellState::GCELL_HEALTHY) {
        lungTissueGcells_.insert(gcell);
        sliceInfectedGcells_.remove(gcell);
    }
    else {
        lungTissueGcells_.insert(gcell);
        sliceInfectedGcells_.insert(gcell);
    }

    if (!isInFillingMode_) {
        GcellMatrix matrix;
        matrix.lungTissue = lungTissueGcells_;
        matrix.infected = sliceInfectedGcells_;

        emit matrixUpdated(matrix);
        emit viewUpdated();
    }
}

void DicomGriddedViewWidget::drawGcellsOnCanvas(QPainter& painter) const
{
    painter.setPen(GCELL_BOUND_PEN);
    drawGcellsOnCanvas(painter, lungTissueGcells_, GCELL_HEALTHY_BRUSH);
    drawGcellsOnCanvas(painter, sliceInfectedGcells_, GCELL_FULL_INFECTED_BRUSH);
}

void DicomGriddedViewWidget::drawGcellsOnCanvas(QPainter& painter, const GcellSet& gcells, const QBrush& brush) const
{
    painter.setBrush(brush);
    for (const QPoint& gcell : gcells) {
        const QRect gcellRect = getGcellRectOnScreen(gcell);
        painter.drawRect(gcellRect);
    }
}

void DicomGriddedViewWidget::drawGridOnCanvas(QPainter& painter) const
{
    painter.setPen(GRID_PEN);

    const QRectF viewportCm = getViewportCm();
    const qreal gridStartCmY = qCeil(viewportCm.top() / gridStepCm()) * gridStepCm();
    const qreal gridStartCmX = qCeil(viewportCm.left() / gridStepCm()) * gridStepCm();
    const qreal gridEndCmY = qFloor(viewportCm.bottom() / gridStepCm()) * gridStepCm();
    const qreal gridEndCmX = qFloor(viewportCm.right() / gridStepCm()) * gridStepCm();

    for (qreal gridCmY = gridStartCmY; gridCmY <= gridEndCmY; gridCmY += gridStepCm()) {
        const QPoint lineStartPixel = getScreenPixelForPointCm(QPointF(0, gridCmY));
        painter.drawLine(0, lineStartPixel.y(), width(), lineStartPixel.y());
    }
    for (qreal gridCmX = gridStartCmX; gridCmX <= gridEndCmX; gridCmX += gridStepCm()) {
        const QPoint lineStartPixel = getScreenPixelForPointCm(QPointF(gridCmX, 0));
        painter.drawLine(lineStartPixel.x(), 0, lineStartPixel.x(), height());
    }
}

void DicomGriddedViewWidget::drawTemporaryObjectsOnCanvas(QPainter& painter) const
{
    drawTemporaryPolygonOnCanvas(painter);
}

void DicomGriddedViewWidget::drawTemporaryPolygonOnCanvas(QPainter& painter) const
{
    const int numTempPolygonIndices = tempPolygonGcells_.size();
    if (numTempPolygonIndices < 1) {
        return;
    }

    painter.setPen(TEMPORARY_POLYGON_PEN);
    painter.setBrush(TEMPORARY_POLYGON_BRUSH);

    const auto p0 = getGcellCenterOnScreen(tempPolygonGcells_[0]);
    painter.drawEllipse(p0, TEMPORARY_POLYGON_START_POINT_RADIUS, TEMPORARY_POLYGON_START_POINT_RADIUS);

    if (numTempPolygonIndices < 2) {
        return;
    }

    for (int vertexIndex = 0; vertexIndex < numTempPolygonIndices - 1; vertexIndex++) {
        const auto p1 = getGcellCenterOnScreen(tempPolygonGcells_[vertexIndex]);
        const auto p2 = getGcellCenterOnScreen(tempPolygonGcells_[vertexIndex + 1]);

        painter.drawLine(p1, p2);
    }
}

void DicomGriddedViewWidget::drawGcellsOnPixmap(QPainter& painter) const
{
    painter.setPen(GCELL_BOUND_PEN);
    drawGcellsOnPixmap(painter, lungTissueGcells_, GCELL_HEALTHY_BRUSH);
    drawGcellsOnPixmap(painter, sliceInfectedGcells_, GCELL_FULL_INFECTED_BRUSH);
}

void DicomGriddedViewWidget::drawGcellsOnPixmap(QPainter& painter, const GcellSet& gcells, const QBrush& brush) const
{
    painter.setBrush(brush);
    for (const QPoint& gcell : gcells) {
        const QRect gcellRect = getGcellRectOnPixmap(gcell);
        painter.drawRect(gcellRect);
    }
}

QRect DicomGriddedViewWidget::getGcellRectOnScreen(const QPoint& gcell) const
{
    QRectF matrixCellCm(QPointF(gcell) * kBaseGridStepCm,
                        QSizeF(kBaseGridStepCm, kBaseGridStepCm));

    return getRectOnScreenByRectCm(matrixCellCm);
}

QRect DicomGriddedViewWidget::getRectOnScreenByRectCm(const QRectF& rectCm) const
{
    const QRect rectOnScreen(getScreenPixelForPointCm(rectCm.topLeft()),
                             getScreenPixelForPointCm(rectCm.bottomRight()) - QPoint(1, 1));
    return rectOnScreen;
}

QPoint DicomGriddedViewWidget::getGcellCenterOnScreen(const QPoint& gcell) const
{
    const qreal halfBaseGridStepCm = kBaseGridStepCm * 0.5;
    const QPointF gcellCenterCm = QPointF(gcell) * kBaseGridStepCm + QPointF(halfBaseGridStepCm, halfBaseGridStepCm);

    return getScreenPixelForPointCm(gcellCenterCm);
//    return getGcellRectOnScreen(gcell).center();
}

QVector<QPoint> DicomGriddedViewWidget::getGcellIndicesForScreenPixel(const QPoint& pixel) const
{
    const auto scaledGridCell = getGridCellByScreenPixel(pixel);

    const int topLeftCellIndexX = scaledGridCell.x() * gridStepMultiplier_;
    const int topLeftCellIndexY = scaledGridCell.y() * gridStepMultiplier_;

    QVector<QPoint> cellIndices;
    for (int indexOffsetX = 0; indexOffsetX < gridStepMultiplier_; indexOffsetX++) {
        for (int indexOffsetY = 0; indexOffsetY < gridStepMultiplier_; indexOffsetY++) {
            const QPoint cellIndex(topLeftCellIndexX + indexOffsetX,
                                   topLeftCellIndexY + indexOffsetY);
            cellIndices << cellIndex;
        }
    }

    return cellIndices;
}

void DicomGriddedViewWidget::fitToPixmap(QPoint& cell) const
{
    const auto slice = getSlice();
    if (!slice) {
        return;
    }

    const int width = slice->getWidth();
    const int height = slice->getHeight();
    if (cell.x() < 0) {
        cell.rx() = 0;
    }
    else if (cell.x() >= width) {
        cell.rx() = width - 1;
    }
    if (cell.y() < 0) {
        cell.ry() = 0;
    }
    if (cell.ry() >= height) {
        cell.ry() = height - 1;
    }
}

void DicomGriddedViewWidget::setGcellStates(const QVector<QPoint>& gcells, GcellState state)
{
    bool wasAlreadyInFillingMode = isInFillingMode_;
    if (!wasAlreadyInFillingMode) {
        startFillingMode();
    }

    for (const QPoint& gcell : gcells) {
        setGcellState(gcell, state);
    }

    if (!wasAlreadyInFillingMode) {
        finishFillingMode();
    }
}

void DicomGriddedViewWidget::startFillingMode()
{
    isInFillingMode_ = true;
}

void DicomGriddedViewWidget::finishFillingMode()
{
    isInFillingMode_ = false;

    GcellMatrix matrix;
    matrix.lungTissue = lungTissueGcells_;
    matrix.infected = sliceInfectedGcells_;
    emit matrixUpdated(matrix);

    emit viewUpdated();
}

void DicomGriddedViewWidget::addTemporaryPolygonVertex(const QPoint& gcell)
{
    tempPolygonGcells_ << gcell;

    emit viewUpdated();
}

void DicomGriddedViewWidget::clearTemporaryPolygon()
{
    tempPolygonGcells_.clear();
}

qreal DicomGriddedViewWidget::getBaseGridStepOnPixmap() const
{
    const QPointF cellSpacingCm = getCellSpacingCm();
    return kBaseGridStepCm / cellSpacingCm.x();
}

void DicomGriddedViewWidget::setGridStepMultiplier(int multiplier)
{
    gridStepMultiplier_ = multiplier;

    emit viewUpdated();
}

DicomGriddedViewWidget::~DicomGriddedViewWidget()
{
}

qreal DicomGriddedViewWidget::gridStepCm() const
{
    return kBaseGridStepCm * gridStepMultiplier_;
}

void DicomGriddedViewWidget::setSlice(DicomSlicePtr slice)
{
    DicomViewWidget::setSlice(slice);

    resetMatrix();
}

void DicomGriddedViewWidget::reset()
{
    gridStepMultiplier_ = 2;
    sliceInfectedGcells_.clear();

    DicomViewWidget::reset();
}

QPixmap DicomGriddedViewWidget::getImageWithMarks() const
{
    QPixmap pixmap = slice_->pixmap();

    QPainter painter(&pixmap);
    drawGcellsOnPixmap(painter);

    return pixmap;
}

int DicomGriddedViewWidget::getNumGcellsX() const
{
    const auto slice = getSlice();
    if (!slice) {
        return 0;
    }

    const qreal baseGridStepOnPixmap = getBaseGridStepOnPixmap();
    const int sliceWidth = slice->getWidth();
    return int(qreal(sliceWidth) / baseGridStepOnPixmap);
}

int DicomGriddedViewWidget::getNumGcellsY() const
{
    const auto slice = getSlice();
    if (!slice) {
        return 0;
    }

    const qreal baseGridStepOnPixmap = getBaseGridStepOnPixmap();
    const int sliceHeight = slice->getHeight();
    return int(qreal(sliceHeight) / baseGridStepOnPixmap);
}

QPoint DicomGriddedViewWidget::getGcellByScreenPixel(const QPoint& pixel) const
{
    const QPointF pointCm = getPointCmForScreenPixel(pixel);
    const int gcellIndexX = qFloor(pointCm.x() / kBaseGridStepCm);
    const int gcellIndexY = qFloor(pointCm.y() / kBaseGridStepCm);

    return QPoint(gcellIndexX, gcellIndexY);
}

QRect DicomGriddedViewWidget::getGcellRectOnPixmap(const QPoint& gcell) const
{
    const qreal baseGridStepOnPixmap = getBaseGridStepOnPixmap();

    QPoint leftTopPixel = QPoint(int(baseGridStepOnPixmap * gcell.x()),
                                 int(baseGridStepOnPixmap * gcell.y()));
    QPoint bottomRightPixel = QPoint(int(baseGridStepOnPixmap * (gcell.x() + 1)) - 1,
                                     int(baseGridStepOnPixmap * (gcell.y() + 1)) - 1);

    fitToPixmap(leftTopPixel);
    fitToPixmap(bottomRightPixel);

    const QRect gcellRectOnPixmap(leftTopPixel, bottomRightPixel);
    return gcellRectOnPixmap;
}

QPoint DicomGriddedViewWidget::getGcellCenterOnPixmap(const QPoint& gcell) const
{
    const qreal baseGridStepOnPixmap = getBaseGridStepOnPixmap();

    QPoint center = QPoint(int(baseGridStepOnPixmap * (gcell.x() + 0.5)),
                           int(baseGridStepOnPixmap * (gcell.y() + 0.5)));
    fitToPixmap(center);

    return center;
}

QPoint DicomGriddedViewWidget::getGridCellByScreenPixel(const QPoint& pixel) const
{
    const QPointF pointCm = getPointCmForScreenPixel(pixel);
    const int gridCellIndexX = qFloor(pointCm.x() / gridStepCm());
    const int gridCellIndexY = qFloor(pointCm.y() / gridStepCm());

    return QPoint(gridCellIndexX, gridCellIndexY);
}

QRect DicomGriddedViewWidget::getGridCellRectOnPixmap(const QPoint& gridCell) const
{
    const int& gridMultiplier = gridStepMultiplier_;
    const qreal baseGridStepOnPixmap = getBaseGridStepOnPixmap();
    const qreal gridStepOnPixmap = baseGridStepOnPixmap * gridMultiplier;

    const QPoint leftTopPixel = QPoint(int(gridStepOnPixmap * gridCell.x()),
                                       int(gridStepOnPixmap * gridCell.y()));
    const QPoint bottomRightPixel = QPoint(int(gridStepOnPixmap * (gridCell.x() + 1)) - 1,
                                           int(gridStepOnPixmap * (gridCell.y() + 1)) - 1);

    const QRect gridCellRectOnPixmap(leftTopPixel, bottomRightPixel);
    return gridCellRectOnPixmap;
}

void DicomGriddedViewWidget::setGcellMatrix(const GcellMatrix& matrix)
{
    lungTissueGcells_ = matrix.lungTissue;
    sliceInfectedGcells_ = matrix.infected;

    emit viewUpdated();
}

void DicomGriddedViewWidget::resetMatrix()
{
    sliceInfectedGcells_.clear();
}

} // namespace valt
} // namespace sklyph
