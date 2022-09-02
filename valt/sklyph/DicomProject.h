#ifndef SKLYPH_DICOM_PROJECT_H
#define SKLYPH_DICOM_PROJECT_H

#include "DicomPatient.h"

#include "DcmDataset.h"

#include <QtCore>

namespace sklyph {
namespace valt {

class DicomProject: public QObject
{
    Q_OBJECT
public:
    explicit DicomProject(const QString& rootPath, QObject* parent=nullptr);
    ~DicomProject() override;

    QStringList getPatientList() const;
    DicomPatientPtr getPatient(const QString& name);

    bool loadProjectFromRootDirectory();
    bool loadProjectFromDicomDirectory(const QString& dicomDirPath);

signals:
    void loadFinished();

public slots:
    bool reload();

private:
    const QString kRootPath;

    QMap<QString, DicomPatientPtr> patients_;
    DcmDataset dataset_;

    int loadProgress_;
};

} // namespace valt
} // namespace sklyph

#endif // SKLYPH_DICOM_PROJECT_H
