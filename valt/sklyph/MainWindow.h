
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "DicomViewWidget.h"
#include "DicomGriddedViewWidget.h"
#include "LungMatrix.h"
#include "DicomProject.h"

#include "LungMatrixMouseHandlerHelper.h"
#include "NavigationToolMouseHandler.h"
#include "PenToolMouseHandler.h"
#include "FillToolMouseHandler.h"
#include "PolygonToolMouseHandler.h"
#include "RubberToolMouseHandler.h"
#include "RulerToolMouseHandler.h"

#include "DcmDataset.h"

#include <QMainWindow>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QMenu>
#include <QToolBar>
#include <QAction>
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include <QProgressBar>
#include <QRadioButton>
#include <QButtonGroup>

namespace sklyph {
namespace valt {

class MainWindow: public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(const QString& filePath=QString(), QWidget *parent = nullptr);
    ~MainWindow() override;

    void moveToDesktopCenter();

protected:
    void wheelEvent(QWheelEvent *event) override;

signals:
    void serieChanged();
    void sliceUpdated();
    void projectRootChanged(const QString& path);

private slots:
    void executeOpenProjectDirDialog();
    void executeExportRawImageDialog();
    void executeExportImageWithMarksDialog();
    void executeOpenProjectFiles();
    void executeSaveProjectFiles();

    void reloadProject(const QString& path);

    void updateSliceViews();
    void updateVolumesInGui();
    void updateManualTotalVolume();

    void onProjectLoadFinished();
    void onPatientsCboxSelectionChanged(const QString& value);
    void onStudiesCboxSelectionChanged(const QString& value);
    void onSeriesCboxSelectionChanged(const QString& value);

    void onTotalVolumeAutoUpdateStateChanged(int);

    void onScalingSpinBoxValueChanged(int value);
    void onSliceSpinBoxValueChanged(int);

    void onSliceMatrixUpdated(const GcellMatrix& sliceMatrix);

    void onRulerCheckBoxStateChanged(int state);

    void onToolButtonGroupClicked(int);

    void onGridStepIndexChanged(int index);

    void onInfectionMarkModeStateChanged(int);

private: // METHODS
    Q_DISABLE_COPY(MainWindow)

    // View
    void setupUi();
    void setupTopBarLayout();
    void setupDicomWidgets();
    void setupToolsLayout();
    void setupWorkAreaLayout();
    void setupNavigationLayout();
    void setupLungVolumeInfoLayout();

    void createActions();
    void createMenus();
    void updateMenuActions();

    void updateScalingFactorSpinBox();
    void updateSliceSpinBox();

    DicomPatientPtr currentPatient() const;
    DicomStudyPtr currentStudy() const;
    DicomSeriePtr currentSerie() const;
    DicomSlicePtr currentSlice() const;
    int currentSliceIndex() const;
    QPointF currentCellSpacing() const;
    bool isTotalVolumeAutoUpdateEnabled() const;
    qreal currentTotalVolume() const;

    // Model
    void resetProject();
    void processPath(const QString& path);
    void loadProject(const QString& path);

    void setLungMatrixTotalVolumeAutoUpdate(bool enabled) const;
    void updateActiveTool();

    void renderRawImageToFile(const QString& filePath);
    void renderImageWithMarksToFile(const QString& filePath);
    void renderPixmapToFile(const QPixmap& pixmap, const QString& filePath);
    void saveProjectFies(const DicomSeriePtr& serie, const QString& dirPath);

private: // FIELDS
    DicomProject* project_;

    LungMatrixMouseHandlerHelper* editWidgetMouseHandlerHelper_;
    NavigationToolMouseHandler* viewWidgetMouseHandler_;
    PenToolMouseHandler* penToolMouseHandler_;
    FillToolMouseHandler* fillToolMouseHandler_;
    PolygonToolMouseHandler* polygonToolMouseHandler_;
    RubberToolMouseHandler* rubberToolMouseHandler_;
    RulerToolMouseHandler* rulerToolMouseHandler_;

    GcellSet gcellMatrix_;

    DicomViewWidget* viewWidget_;
    DicomGriddedViewWidget* editWidget_;

    QCheckBox* totalVolumeAutoUpdateCheckBox_;
    QLineEdit* totalVolumeWidget_;
    QLineEdit* infectedVolumeWidget_;
    QLineEdit* infectedVolumePartWidget_;
    QLineEdit* densityOfHounsfieldWidget_;
    QLineEdit* distanceFromTwoPointsWidget_;
    QLineEdit* densityOfHounsfieldInPolygonWidget_;

    QMenu* fileMenu_;
    QAction* openAction_;
    QAction* openProjectAction_;
    QAction* exportRawImageAction_;
    QAction* exportImageWithMarksAction_;
    QAction* saveProjectAction_;
    QAction* exitAction_;

    QVBoxLayout* mainLayout_;
    QHBoxLayout* topBarLayout_;
    QHBoxLayout* toolsLayout_;
    QHBoxLayout* topRadiosLayout_;
    QHBoxLayout* bottomRadiosLayout_;
    QVBoxLayout* verticalRadiosLayout_;
    QHBoxLayout* workAreaLayout_;
    QHBoxLayout* navigationLayout_;
    QGridLayout* lungVolumeInfoLayout_;

    QComboBox* patientsSelectCombo_;
    QComboBox* studiesSelectCombo_;
    QComboBox* seriesSelectCombo_;

    QSpinBox* scalingFactorSpinBox_;
    QSpinBox* sliceSpinBox_;
    QLabel* sliceTotalCountLabel_;

    QCheckBox* rulerCheckBox_;

    QButtonGroup* toolRadioButtonsGroup_;
    QRadioButton* navigationToolRadio_;
    QRadioButton* penToolRadio_;
    QRadioButton* fillToolRadio_;
    QRadioButton* polygonToolRadio_;
    QRadioButton* rubberToolRadio_;
    QRadioButton* rulerToolRadio_;

    QCheckBox* infectionMarkModeCheckBox_;

    QComboBox* gridStepCombo_;

    QString projectPath_;
};

} // namespace valt
} // namespace sklyph

#endif // MAINWINDOW_H
