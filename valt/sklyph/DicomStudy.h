#ifndef SKLYPH_DICOM_STUDY_H
#define SKLYPH_DICOM_STUDY_H

#include "DicomSeries.h"

#include <QMap>

namespace sklyph {
namespace valt {

class DicomStudy: public QObject
{
    Q_OBJECT
public:
    explicit DicomStudy(const QString& path, QObject* parent=nullptr);
    virtual ~DicomStudy();

    bool reload();

    QStringList getSeriesList() const;
    DicomSeriePtr getSerie(const QString& name);
    QString getThickestSerieIndex() const;

    bool isLoaded() const;

private:
    bool loadStudyFromDirectory();
    bool preloadSerie(const QString& path);
    QString getUniqueSerieDescription(const DicomSeriePtr& serie, const QString& dirName) const;
    QString getSerieDescriptionFromDirNameAndSliceThickness(const QString& dirName, qreal sliceThickness) const;

    const QString kRootPath;

    bool isLoaded_;
    QMap<QString, DicomSeriePtr> series_;
    QString thickestSerieIndex_;
    qreal maxSliceThickness_;
};

typedef QSharedPointer<DicomStudy> DicomStudyPtr;

} // namespace valt
} // namespace sklyph

#endif // SKLYPH_DICOM_STUDY_H
