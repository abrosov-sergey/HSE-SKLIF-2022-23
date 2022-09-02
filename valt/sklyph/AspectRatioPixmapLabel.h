#ifndef ASPECTRATIOPIXMAPLABEL_H
#define ASPECTRATIOPIXMAPLABEL_H

#include <QLabel>
#include <QPixmap>
#include <QResizeEvent>

namespace sklyph {
namespace valt {

class AspectRatioPixmapLabel : public QLabel
{
    Q_OBJECT
public:
    explicit AspectRatioPixmapLabel(QWidget* parent = nullptr);

    int heightForWidth(int w) const override;

    qreal getCellSizeOnScreenCm() const;
    void setCellSizeOnScreenCm(qreal cellSizeCm);

    virtual void setPixmap(const QPixmap& pixmap);
    bool isPixmapSet() const;

    virtual void reset();

    qreal getScreenPixelsPerCell() const;

    const QRect& getViewport() const;

    const QPixmap& getPixmap() const;

    const QImage& getImage();

protected:
    AspectRatioPixmapLabel& operator=(const AspectRatioPixmapLabel& other);

    void paintEvent(QPaintEvent* event) override;

    void setLayer(int index, const QPixmap& pixmap);
    QPixmap layer(int index) const;

signals:
    void pixmapChanged();
    void viewportUpdated(const QRect& viewportRect);
    void viewChanged();

public slots:
    void setViewport(const QRect& viewportRect);
    void moveViewport(const QPoint& vectorCells);

protected slots:
    void renderPixmap();

private slots:
    void invalidateImage();

private:
    qreal getScreenDotsPerCm() const;

    void fitViewportInScene(QRect& viewport);
    void updateViewport(const QRect& viewport);

    void calculateScreenPixelsPerCell();
    void calculateCellSizeOnScreenCm();

    void generateImageForPixmap();

    const qreal kScreenDotsPerCm;
    QPixmap pixmap_;
    QImage image_;
    bool isImageUpToDate_;
    qreal cellSizeOnScreenCm_;
    qreal screenPixelsPerCell_;

    QRect viewport_;

    QMap<int,QPixmap> layers_;
};

} // namespace valt
} // namespace sklyph

#endif // ASPECTRATIOPIXMAPLABEL_H
