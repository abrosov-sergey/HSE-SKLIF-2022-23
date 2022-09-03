#ifndef DICOM_GRIDDED_VIEW_WIDGET_H
#define DICOM_GRIDDED_VIEW_WIDGET_H

#include "DicomSlice.h"
#include "DicomViewWidget.h"

#include "common.h"

namespace sklyph {
namespace valt {

class DicomGriddedViewWidget: public DicomViewWidget
{
    Q_OBJECT
public:
    explicit DicomGriddedViewWidget(qreal baseGridStep, QWidget* parent=nullptr);
    ~DicomGriddedViewWidget() override;

    qreal gridStepCm() const;
    qreal getBaseGridStepOnPixmap() const;

    void setGridStepMultiplier(int multiplier);

    void setSlice(DicomSlicePtr slice) override;

    void reset() override;

    QPixmap getImageWithMarks() const;

    int getNumGcellsX() const;
    int getNumGcellsY() const;

    QPoint getGcellByScreenPixel(const QPoint& pixel) const;
    QRect getGcellRectOnPixmap(const QPoint& gcell) const;
    QPoint getGcellCenterOnPixmap(const QPoint& gcell) const;

    QPoint getGridCellByScreenPixel(const QPoint& pixel) const;
    QRect getGridCellRectOnPixmap(const QPoint& gridCell) const;

    void triggerStateActionInPixel(const QPoint& pixel, GcellState state);

    void setGcellState(const QPoint& gcell, GcellState state);
    void setGcellStates(const QVector<QPoint>& gcells, GcellState state);

    void startFillingMode();
    void finishFillingMode();

    void addTemporaryPolygonVertex(const QPoint& gcell);
    void clearTemporaryPolygon();

signals:
    void matrixUpdated(const GcellMatrix& matrix);

    void viewUpdated();

public slots:
    void setGcellMatrix(const GcellMatrix& matrix);

    void resetMatrix();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    void drawGcellsOnCanvas(QPainter& painter) const;
    void drawGcellsOnCanvas(QPainter& painter, const GcellSet& gcells, const QBrush& brush) const;
    void drawGridOnCanvas(QPainter& painter) const;
    void drawTemporaryObjectsOnCanvas(QPainter& painter);
    void drawTemporaryPolygonOnCanvas(QPainter& painter);

    void drawGcellsOnPixmap(QPainter& painter) const;
    void drawGcellsOnPixmap(QPainter& painter, const GcellSet& gcells, const QBrush& brush) const;

    QRect getGcellRectOnScreen(const QPoint& gcell) const;
    QRect getRectOnScreenByRectCm(const QRectF& rectCm) const;

    QPoint getGcellCenterOnScreen(const QPoint& gcell) const;

    QVector<QPoint> getGcellIndicesForScreenPixel(const QPoint& pixel) const;

    void fitToPixmap(QPoint& cell) const;

    const qreal kBaseGridStepCm;
    int gridStepMultiplier_;

    GcellSet sliceInfectedGcells_;
    GcellSet lungTissueGcells_;

    bool isInFillingMode_;

    QList<QPoint> tempPolygonGcells_;
};

} // namespace valt
} // namespace sklyph

#endif // DICOM_GRIDDED_VIEW_WIDGET_H
