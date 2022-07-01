#include "DicomPatient.h"

namespace sklyph {
namespace valt {

const QString PATIENT_DIRECTORY_PREFIX = "PA";

DicomPatient::DicomPatient(const QString& path, QObject* parent)
    : QObject(parent)
    , kRootPath(path)
    , isLoaded_(false)
{

}

DicomPatient::~DicomPatient()
{

}

bool DicomPatient::reload()
{
    QDir dir(kRootPath);
    if (!dir.exists()) {
        return false;
    }

    if (dir.dirName().startsWith(PATIENT_DIRECTORY_PREFIX)) {
        isLoaded_ = loadPatientFromDirectory();
    }
    else {
        DicomStudyPtr study(new DicomStudy(kRootPath));
        studies_.insert(dir.dirName(), study);
        isLoaded_ = study->reload();
    }

    return isLoaded_;
}

QStringList DicomPatient::getStudiesList() const
{
    return studies_.keys();
}

DicomStudyPtr DicomPatient::getStudy(const QString& name)
{
    DicomStudyPtr study = studies_.value(name, DicomStudyPtr());
    if (!study) {
        return DicomStudyPtr();
    }

    if (!study->isLoaded()) {
        study->reload();
    }

    return study;
}

bool DicomPatient::isLoaded() const
{
    return isLoaded_;
}

bool DicomPatient::loadPatientFromDirectory()
{
    QDir dir(kRootPath);

    const QStringList subDirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString& subDir : subDirs) {
        const QString subDirPath = dir.filePath(subDir);
        DicomStudyPtr study(new DicomStudy(subDirPath));
        studies_.insert(subDir, study);
    }

    return true;
}

} // namespace valt
} // namespace sklyph
