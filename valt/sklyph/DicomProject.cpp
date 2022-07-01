#include "DicomProject.h"

#include "DicomReader.h"

namespace sklyph {
namespace valt {

const QString DICOMDIR_FILENAME = "DICOMDIR";
const QString DICOM_DIRECTORY_NAME = "DICOM";

DicomProject::DicomProject(const QString& rootPath, QObject* parent)
    : QObject (parent)
    , kRootPath(rootPath)
    , loadProgress_(0)
{

}

DicomProject::~DicomProject()
{

}

QStringList DicomProject::getPatientList() const
{
    return patients_.keys();
}

DicomPatientPtr DicomProject::getPatient(const QString& name)
{
    DicomPatientPtr patient = patients_.value(name, DicomPatientPtr());
    if (!patient) {
        return DicomPatientPtr();
    }

    if (!patient->isLoaded()) {
        patient->reload();
    }

    return patient;
}

bool DicomProject::loadProjectFromRootDirectory()
{
    QDir rootDir(kRootPath);

    const QString projectFilePath = rootDir.filePath(DICOMDIR_FILENAME);

    bool ok = false;
    dataset_ = DicomReader::loadDatasetFromFile(projectFilePath, &ok);
    if (!ok) {
        return false;
    }

    const QString projectDirPath = rootDir.filePath(DICOM_DIRECTORY_NAME);

    return loadProjectFromDicomDirectory(projectDirPath);
}

bool DicomProject::loadProjectFromDicomDirectory(const QString& dicomDirPath)
{
    QDir dicomDir(dicomDirPath);
    if (!dicomDir.exists()) {
        return false;
    }

    const QStringList subDirs = dicomDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString& subDir : subDirs) {
        const QString patientDirPath = dicomDir.filePath(subDir);
        DicomPatientPtr patient(new DicomPatient(patientDirPath));
        patients_.insert(subDir, patient);
    }

    return true;
}

bool DicomProject::reload()
{
    bool success = false;

    QDir rootDir(kRootPath);
    if (rootDir.entryList().contains(DICOMDIR_FILENAME)) {
        success = loadProjectFromRootDirectory();
    }
    else if (rootDir.dirName() == DICOM_DIRECTORY_NAME) {
        success = loadProjectFromDicomDirectory(kRootPath);
    }
    else {
        DicomPatientPtr patient(new DicomPatient(kRootPath));
        patients_.insert(rootDir.dirName(), patient);
        success = patient->reload();
    }

    if (success) {
        emit loadFinished();
    }
    return success;
}

} // namespace valt
} // namespace sklyph
