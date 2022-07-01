#ifndef SKLYPH_DICOM_PATIENT_H
#define SKLYPH_DICOM_PATIENT_H

#include "DicomStudy.h"

#include <QMap>

namespace sklyph {
namespace valt {

class DicomPatient: public QObject
{
    Q_OBJECT
public:
    explicit DicomPatient(const QString& path, QObject* parent=nullptr);
    virtual ~DicomPatient();

    bool reload();

    QStringList getStudiesList() const;
    DicomStudyPtr getStudy(const QString& name);

    bool isLoaded() const;

private:
    bool loadPatientFromDirectory();

    const QString kRootPath;

    bool isLoaded_;
    QMap<QString, DicomStudyPtr> studies_;
};

typedef QSharedPointer<DicomPatient> DicomPatientPtr;

} // namespace valt
} // namespace sklyph

#endif // SKLYPH_DICOM_PATIENT_H
