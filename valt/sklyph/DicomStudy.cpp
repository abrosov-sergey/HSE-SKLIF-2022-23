#include "DicomStudy.h"

#include "common.h"

namespace sklyph {
namespace valt {

const QString STUDY_DIRECTORY_PREFIX = "ST";

DicomStudy::DicomStudy(const QString& path, QObject* parent)
    : QObject(parent)
    , kRootPath(path)
    , isLoaded_(false)
    , thickestSerieIndex_()
    , maxSliceThickness_(0.0)
{
}

DicomStudy::~DicomStudy()
{
}

bool DicomStudy::reload()
{
    QDir dir(kRootPath);
    if (!dir.exists()) {
        return false;
    }

    maxSliceThickness_ = 0.0;
    thickestSerieIndex_ = QString();

    if (dir.dirName().startsWith(STUDY_DIRECTORY_PREFIX)) {
        isLoaded_ = loadStudyFromDirectory();
    }
    else {
        isLoaded_ = preloadSerie(kRootPath);
    }

    return isLoaded_;
}

QStringList DicomStudy::getSeriesList() const
{
    return series_.keys();
}

DicomSeriePtr DicomStudy::getSerie(const QString& name)
{
    DicomSeriePtr serie = series_.value(name, DicomSeriePtr());
    if (!serie) {
        return DicomSeriePtr();
    }
    if (!serie->isLoaded()) {
        serie->reloadSlices();
    }
    return serie;
}

QString DicomStudy::getThickestSerieIndex() const
{
    return thickestSerieIndex_;
}

bool DicomStudy::isLoaded() const
{
    return isLoaded_;
}

bool DicomStudy::loadStudyFromDirectory()
{
    QDir dir(kRootPath);

    const QStringList subDirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString& serieDirName : subDirs) {
        const QString subDirPath = dir.filePath(serieDirName);
        preloadSerie(subDirPath);
    }

    return true;
}

bool DicomStudy::preloadSerie(const QString& path)
{
    const QString dirName = QDir(path).dirName();

    DicomSeriePtr serie(new DicomSerie(path));
    if (!serie->reloadMetaData()) {
        return false;
    }

    const qreal sliceThicknessMm = serie->getSliceThicknessCm() * 10.0;

    const QString serieIndex = getUniqueSerieDescription(serie, dirName);

    if (sliceThicknessMm > maxSliceThickness_) {
        maxSliceThickness_ = sliceThicknessMm;
        thickestSerieIndex_ = serieIndex;
    }

    series_.insert(serieIndex, serie);
    return true;
}

QString DicomStudy::getUniqueSerieDescription(const DicomSeriePtr& serie, const QString& dirName) const
{
    const qreal sliceThicknessMm = serie->getSliceThicknessCm() * 10.0;

    QString serieDescription = serie->getDescription();
    if (serieDescription.isEmpty()) {
        serieDescription = getSerieDescriptionFromDirNameAndSliceThickness(dirName, sliceThicknessMm);
    }
    if (series_.contains(serieDescription)) {
        serieDescription = sklyph::valt::strutils::getUniqueStringByAddingNumber(serieDescription, series_.keys());
    }

    return serieDescription;
}

QString DicomStudy::getSerieDescriptionFromDirNameAndSliceThickness(const QString& dirName, qreal sliceThickness) const
{
    return tr("%1 : %2 mm").arg(dirName).arg(sliceThickness, 0, 'f', 1);
}

} // namespace valt
} // namespace sklyph
