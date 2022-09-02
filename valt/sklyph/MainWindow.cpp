#include "MainWindow.h"

#include "common.h"
#include "DcmFile.h"
#include "DcmImage.h"
#include "DcmMonochromeImage.h"
#include "DcmTagSQ.h"
#include "DcmTagItem.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QList>
#include <QMenuBar>
#include <QMouseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include <QWheelEvent>
#include <QLineEdit>
#include <QCheckBox>

namespace sklyph {
namespace valt {

// View
const int DEFAULT_OPTIONS_SPACING = 50;
const qreal DEFAULT_CELL_SIZE_ON_SCREEN_CM = 0.1;

const qreal BASE_GRID_STEP_CM = 0.5;
const QVector<int> GRID_MULTIPLIERS = { 1, 2, 4, 8 };
const int DEFAULT_GRID_MULTIPLIER_INDEX = 1;

const QPointF DEFAULT_CELL_SPACING_CM(0.1, 0.1);

const int MIN_ZOOM_PERCENT = 30;
const int MAX_ZOOM_PERCENT = 500;
const int ZOOM_SINGLE_STEP = 30;

const QString IMAGE_FORMAT_FILTERS_STRING = "(*.png;*.jpg;*.jpeg;*.tiff)";

MainWindow::MainWindow(const QString& filePath, QWidget *parent)
    : QMainWindow(parent)
    , project_(nullptr)
    , editWidgetMouseHandlerHelper_(nullptr)
    , viewWidgetMouseHandler_(nullptr)
    , penToolMouseHandler_(nullptr)
    , fillToolMouseHandler_(nullptr)
    , polygonToolMouseHandler_(nullptr)
{
    declareQtMetatypes();

    // View
    createActions();
    createMenus();
    setupUi();
    resetProject();

    connect(this, SIGNAL(serieChanged()), this, SLOT(updateSliceViews()));
    connect(this, SIGNAL(sliceUpdated()), this, SLOT(updateSliceViews()));
    connect(this, SIGNAL(projectRootChanged(const QString&)), this, SLOT(reloadProject(const QString&)));

    showMaximized();

    if (!filePath.isEmpty()) {
        processPath(filePath);
    }
}

MainWindow::~MainWindow()
{
}

void MainWindow::moveToDesktopCenter()
{
    QRect r = frameGeometry();
    QRect d = QApplication::desktop()->availableGeometry();
    r.moveCenter(QPoint(d.width()/2, d.height()/2));
    move(r.topLeft());
}

void MainWindow::wheelEvent(QWheelEvent* event)
{
    if (event->orientation() == Qt::Horizontal) {
        QMainWindow::wheelEvent(event);
    }

    const int deltaDegrees = event->delta() / 8;

    if (event->modifiers() & Qt::ControlModifier) {
        int currentScaleFactor = scalingFactorSpinBox_->value();
        currentScaleFactor += deltaDegrees;
        scalingFactorSpinBox_->setValue(currentScaleFactor);
    } else {
        if (deltaDegrees > 0) {
            sliceSpinBox_->stepDown();
        } else {
            sliceSpinBox_->stepUp();
        }
    }

    event->accept();
}

void MainWindow::executeOpenProjectDirDialog()
{
    projectPath_ = QFileDialog::getExistingDirectory(this,
                                                         tr("Open DICOM directory"),
                                                         "~");
    if (projectPath_.isEmpty()) {
        return;
    }

    processPath(projectPath_);
}

void MainWindow::executeExportRawImageDialog()
{
    const QString fileName = QFileDialog::getSaveFileName(this,
                                                          tr("Export slice to image file"),
                                                          "~",
                                                          tr("Images ") + IMAGE_FORMAT_FILTERS_STRING);
    if (fileName.isEmpty()) {
        return;
    }

    renderRawImageToFile(fileName);
}

void MainWindow::executeExportImageWithMarksDialog()
{
    const QString fileName = QFileDialog::getSaveFileName(this,
                                                          tr("Export slice with marks to image file"),
                                                          "~",
                                                          tr("Images ") + IMAGE_FORMAT_FILTERS_STRING);
    if (fileName.isEmpty()) {
        return;
    }

    renderImageWithMarksToFile(fileName);
}

void MainWindow::executeOpenProjectFiles()
{
    projectPath_ = QFileDialog::getExistingDirectory(this,
                                                           tr("Open DICOM directory"),
                                                           "~");
    if (projectPath_.isEmpty()) {
        return;
    }

    processPath(projectPath_);
};

void MainWindow::executeSaveProjectFiles()
{
    const QString dirName = QFileDialog::getExistingDirectory(this,
                                                          tr("Save project file"),
                                                          "~");
    if (dirName.isEmpty()) {
        return;
    }

    const DicomSeriePtr serie = currentSerie();

    if (!serie) {
        return;
    }

    saveProjectFies(serie, dirName);
}

void MainWindow::reloadProject(const QString& path)
{
    resetProject();
    loadProject(path);
}

void MainWindow::updateSliceViews()
{
    DicomSeriePtr serie = currentSerie();
    if (!serie) {
        return;
    }

    updateSliceSpinBox();

    updateMenuActions();

    const QPointF cellSpacingCm = serie->getCellSpacingCm();
    const int sliceIndex = currentSliceIndex();
    DicomSlicePtr slice = serie->getSlice(sliceIndex);
    if (!slice) {
        return;
    }

    LungMatrix* lungMatrix = serie->getLungMatrix();
    qreal totalVolume = lungMatrix->getTotalVolume();
    totalVolumeWidget_->setText(strutils::realToString(totalVolume));
    updateVolumesInGui();

    disconnect(this, SLOT(updateVolumesInGui()));
    connect(lungMatrix, SIGNAL(volumesChanged()), this, SLOT(updateVolumesInGui()));

    viewWidget_->setCellSpacingCm(cellSpacingCm);
    viewWidget_->setSlice(slice);

    editWidget_->setCellSpacingCm(cellSpacingCm);
    editWidget_->setSlice(slice);
    editWidget_->setGcellMatrix(lungMatrix->getMatrix(sliceIndex));

    updateScalingFactorSpinBox();
}

void MainWindow::updateVolumesInGui()
{
    DicomSeriePtr serie = currentSerie();
    LungMatrix* lungMatrix = serie->getLungMatrix();

    qreal total = currentTotalVolume();
    qreal infected = lungMatrix->getInfectedVolume();
    qreal infectedPart = lungMatrix->getInfectedVolumePercent();
    // Начало: Абросов Сергей
    qreal densityOfHounsfield = penToolMouseHandler_->densityOfHounsfield;
    qreal distanceBetweenTwoPoints = polygonToolMouseHandler_->distanceBetweenTwoPoints;
    // Конец: Абросов Сергей

    const auto bs = blockSignals(true);

    infectedVolumeWidget_->setText(strutils::realToString(infected));
    infectedVolumePartWidget_->setText(QString("%1 %").arg(strutils::realToString(infectedPart)));
    totalVolumeWidget_->setText(strutils::realToString(total));
    // Начало: Абросов Сергей
    densityOfHounsfieldWidget_->setText(strutils::realToString(densityOfHounsfield));
    distanceFromTwoPointsWidget_->setText(strutils::realToString(distanceBetweenTwoPoints));
    // Конец: Абросов Сергей

    blockSignals(bs);
}

void MainWindow::updateManualTotalVolume()
{
    DicomSeriePtr serie = currentSerie();
    if (!serie) {
        return;
    }
    LungMatrix* lungMatrix = serie->getLungMatrix();
    if (!lungMatrix) {
        return;
    }

    bool ok = false;
    qreal total = totalVolumeWidget_->text().toDouble(&ok);
    Q_ASSERT(ok);
    if (!ok) {
        return;
    }

    lungMatrix->setTotalVolume(total);
    updateVolumesInGui();
}

void MainWindow::onProjectLoadFinished()
{
    const QStringList patients = project_->getPatientList();

    bool bs = patientsSelectCombo_->blockSignals(true);
    patientsSelectCombo_->clear();
    patientsSelectCombo_->addItems(patients);
    patientsSelectCombo_->blockSignals(bs);

    if (patientsSelectCombo_->count() > 0) {
        patientsSelectCombo_->setCurrentIndex(0);
        onPatientsCboxSelectionChanged(patientsSelectCombo_->currentText());
    }
}

void MainWindow::onPatientsCboxSelectionChanged(const QString& value)
{
    const DicomPatientPtr patient = project_->getPatient(value);
    if (!patient) {
        return;
    }

    const QStringList studies = patient->getStudiesList();

    bool bs = studiesSelectCombo_->blockSignals(true);
    studiesSelectCombo_->clear();
    studiesSelectCombo_->addItems(studies);
    studiesSelectCombo_->blockSignals(bs);

    if (studiesSelectCombo_->count() > 0) {
        studiesSelectCombo_->setCurrentIndex(0);
        onStudiesCboxSelectionChanged(studiesSelectCombo_->currentText());
    }
}

void MainWindow::onStudiesCboxSelectionChanged(const QString& value)
{
    const DicomPatientPtr patient = currentPatient();
    if (!patient) {
        return;
    }
    const DicomStudyPtr study = patient->getStudy(value);
    if (!study) {
        return;
    }

    const QStringList series = study->getSeriesList();

    bool bs = seriesSelectCombo_->blockSignals(true);
    seriesSelectCombo_->clear();
    seriesSelectCombo_->addItems(series);
    seriesSelectCombo_->blockSignals(bs);

    if (seriesSelectCombo_->count() > 0) {
        int index = series.indexOf(study->getThickestSerieIndex());
        seriesSelectCombo_->setCurrentIndex(index);
        onSeriesCboxSelectionChanged(seriesSelectCombo_->currentText());
    }
}

void MainWindow::onSeriesCboxSelectionChanged(const QString& value)
{
    const DicomStudyPtr study = currentStudy();
    if (!study) {
        return;
    }
    const DicomSeriePtr serie = study->getSerie(value);
    if (!serie) {
        return;
    }
    emit serieChanged();
}

void MainWindow::onTotalVolumeAutoUpdateStateChanged(int)
{
    const auto isAutoUpdateEnabled = isTotalVolumeAutoUpdateEnabled();
    setLungMatrixTotalVolumeAutoUpdate(isAutoUpdateEnabled);

    totalVolumeWidget_->setReadOnly(isAutoUpdateEnabled);

    if (isAutoUpdateEnabled) {
        const qreal volume = currentTotalVolume();
        totalVolumeWidget_->setText(strutils::realToString(volume));
    }

    updateVolumesInGui();
}

void MainWindow::onScalingSpinBoxValueChanged(int value)
{
    const qreal scaleFactor = qreal(value) / 100.0;
    const qreal cellSizeOnScreenCm = scaleFactor * currentCellSpacing().x();
    viewWidget_->setCellSizeOnScreenCm(cellSizeOnScreenCm);
    editWidget_->setCellSizeOnScreenCm(cellSizeOnScreenCm);

    updateScalingFactorSpinBox();
}

void MainWindow::onSliceSpinBoxValueChanged(int)
{
    emit sliceUpdated();
}

void MainWindow::onSliceMatrixUpdated(const GcellMatrix& sliceMatrix)
{
    DicomSeriePtr serie = currentSerie();
    Q_ASSERT(serie);

    LungMatrix* lungMatrix = serie->getLungMatrix();
    Q_ASSERT(lungMatrix);

    lungMatrix->setInfectionMatrix(currentSliceIndex(), sliceMatrix.infected);
    lungMatrix->setLungTissueMatrix(sliceMatrix.lungTissue);
}

void MainWindow::onRulerCheckBoxStateChanged(int state)
{
    viewWidget_->setRulerEnabled(state > 0);
    editWidget_->setRulerEnabled(state > 0);
}

void MainWindow::onToolButtonGroupClicked(int)
{
    updateActiveTool();
}

void MainWindow::onGridStepIndexChanged(int index)
{
    editWidget_->setGridStepMultiplier(GRID_MULTIPLIERS[index]);
}

void MainWindow::onInfectionMarkModeStateChanged(int)
{
    if (infectionMarkModeCheckBox_->isChecked()) {
        penToolMouseHandler_->setMode(PenToolMode::MarkTotalVolume);
        fillToolMouseHandler_->setMode(FillToolMode::MarkTotalVolume);
        polygonToolMouseHandler_->setMode(PolygonlToolMode::MarkTotalVolume);
    }
    else {
        penToolMouseHandler_->setMode(PenToolMode::MarkInfection);
        fillToolMouseHandler_->setMode(FillToolMode::MarkInfection);
        polygonToolMouseHandler_->setMode(PolygonlToolMode::MarkInfection);
    }
}

void MainWindow::setupUi()
{
    setWindowTitle(tr("DICOM Viewer"));

    setupTopBarLayout();
    setupWorkAreaLayout();

    mainLayout_ = new QVBoxLayout;
    mainLayout_->addLayout(topBarLayout_);
    mainLayout_->addLayout(workAreaLayout_);

    QWidget* centralWidget = new QWidget;
    centralWidget->setLayout(mainLayout_);
    setCentralWidget(centralWidget);
}

void MainWindow::setupTopBarLayout()
{
    setupNavigationLayout();
    setupToolsLayout();

    topBarLayout_ = new QHBoxLayout;
    topBarLayout_->addLayout(navigationLayout_);
    topBarLayout_->addSpacing(10);
    topBarLayout_->addLayout(toolsLayout_);
    topBarLayout_->addStretch();

    topBarLayout_->setAlignment(Qt::AlignTop);
}

void MainWindow::setupDicomWidgets()
{
    viewWidgetMouseHandler_ = new NavigationToolMouseHandler;

    viewWidget_ = new DicomViewWidget;
    viewWidget_->setAlignment(Qt::AlignCenter);
    //viewWidget_->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    viewWidget_->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    viewWidget_->setMouseHandler(viewWidgetMouseHandler_);

    editWidgetMouseHandlerHelper_ = new LungMatrixMouseHandlerHelper;
    editWidgetMouseHandlerHelper_->setActiveTool(EditorTool::NavigationTool);
    editWidgetMouseHandlerHelper_->addTool(EditorTool::NavigationTool, new NavigationToolMouseHandler);
    penToolMouseHandler_ = new PenToolMouseHandler;
    editWidgetMouseHandlerHelper_->addTool(EditorTool::PenTool, penToolMouseHandler_);
    fillToolMouseHandler_ = new FillToolMouseHandler;
    editWidgetMouseHandlerHelper_->addTool(EditorTool::FillTool, fillToolMouseHandler_);
    polygonToolMouseHandler_ = new PolygonToolMouseHandler;
    editWidgetMouseHandlerHelper_->addTool(EditorTool::PolygonTool, polygonToolMouseHandler_);

    editWidget_ = new DicomGriddedViewWidget(BASE_GRID_STEP_CM);
    editWidget_->setCellSizeOnScreenCm(DEFAULT_CELL_SIZE_ON_SCREEN_CM);
    editWidget_->setAlignment(Qt::AlignCenter);
    editWidget_->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    editWidget_->setMouseHandler(editWidgetMouseHandlerHelper_);
    editWidget_->setGridStepMultiplier(GRID_MULTIPLIERS[DEFAULT_GRID_MULTIPLIER_INDEX]);

    connect(viewWidget_, SIGNAL(viewportUpdated(const QRect&)), editWidget_, SLOT(setViewport(const QRect&)));
    connect(editWidget_, SIGNAL(viewportUpdated(const QRect&)), viewWidget_, SLOT(setViewport(const QRect&)));
    connect(viewWidget_, SIGNAL(sliceUpdated()), editWidget_, SLOT(updatePixmap()));
    connect(editWidget_, SIGNAL(sliceUpdated()), viewWidget_, SLOT(updatePixmap()));

    connect(editWidget_, SIGNAL(matrixUpdated(const GcellMatrix&)), this, SLOT(onSliceMatrixUpdated(const GcellMatrix&)));
}

void MainWindow::setupToolsLayout()
{
    QLabel* gridStepLabel = new QLabel(tr("Grid step:"));
    gridStepCombo_ = new QComboBox;
    gridStepCombo_->setMinimumWidth(200);
    for (int multiplier : GRID_MULTIPLIERS) {
        gridStepCombo_->addItem(QString("%1 cm").arg(strutils::realToString(BASE_GRID_STEP_CM * multiplier)));
    }
    gridStepCombo_->setCurrentIndex(DEFAULT_GRID_MULTIPLIER_INDEX);
    connect(gridStepCombo_, SIGNAL(currentIndexChanged(int)), this, SLOT(onGridStepIndexChanged(int)));

    QLabel* toolsLabel = new QLabel(tr("Tools:"));
    toolRadioButtonsGroup_ = new QButtonGroup;
    navigationToolRadio_ = new QRadioButton(tr("Nav"));
    navigationToolRadio_->setChecked(true);
    penToolRadio_ = new QRadioButton(tr("Pen"));
    fillToolRadio_ = new QRadioButton(tr("Fill"));
    polygonToolRadio_ = new QRadioButton(tr("Polygon"));

    toolRadioButtonsGroup_->setExclusive(true);
    toolRadioButtonsGroup_->addButton(navigationToolRadio_);
    toolRadioButtonsGroup_->setId(navigationToolRadio_, 0);
    toolRadioButtonsGroup_->addButton(penToolRadio_);
    toolRadioButtonsGroup_->setId(penToolRadio_, 1);
    toolRadioButtonsGroup_->addButton(fillToolRadio_);
    toolRadioButtonsGroup_->setId(fillToolRadio_, 2);
    toolRadioButtonsGroup_->addButton(polygonToolRadio_);
    toolRadioButtonsGroup_->setId(polygonToolRadio_, 3);
    connect(toolRadioButtonsGroup_, SIGNAL(buttonClicked(int)), this, SLOT(onToolButtonGroupClicked(int)));

    infectionMarkModeCheckBox_ = new QCheckBox(tr("Total volume mark mode"));
    infectionMarkModeCheckBox_->setChecked(true);
    connect(infectionMarkModeCheckBox_, SIGNAL(stateChanged(int)), this, SLOT(onInfectionMarkModeStateChanged(int)));

    toolsLayout_ = new QHBoxLayout;
    toolsLayout_->addWidget(gridStepLabel);
    toolsLayout_->addWidget(gridStepCombo_);
    toolsLayout_->addSpacing(DEFAULT_OPTIONS_SPACING);
    toolsLayout_->addWidget(toolsLabel);
    toolsLayout_->addWidget(navigationToolRadio_);
    toolsLayout_->addWidget(penToolRadio_);
    toolsLayout_->addWidget(fillToolRadio_);
    toolsLayout_->addWidget(polygonToolRadio_);
    toolsLayout_->addSpacing(DEFAULT_OPTIONS_SPACING);
    toolsLayout_->addWidget(infectionMarkModeCheckBox_);
}

void MainWindow::setupWorkAreaLayout()
{
    if (polygonToolMouseHandler_ != nullptr) {
        qreal distanceBetweenTwoPoints = polygonToolMouseHandler_->distanceBetweenTwoPoints;
        distanceFromTwoPointsWidget_->setText(strutils::realToString(distanceBetweenTwoPoints));
    }

    setupDicomWidgets();
    setupLungVolumeInfoLayout();

    workAreaLayout_ = new QHBoxLayout;
    workAreaLayout_->addWidget(viewWidget_);
    workAreaLayout_->addWidget(editWidget_);
    workAreaLayout_->addLayout(lungVolumeInfoLayout_);
}

void MainWindow::setupNavigationLayout()
{
    QLabel* patientsLabel = new QLabel(tr("Patient:"));
    patientsSelectCombo_ = new QComboBox;
    patientsSelectCombo_->setMinimumWidth(200);
    connect(patientsSelectCombo_, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(onPatientsCboxSelectionChanged(const QString&)));

    QLabel* studiesLabel = new QLabel(tr("Study:"));
    studiesSelectCombo_ = new QComboBox;
    studiesSelectCombo_->setMinimumWidth(200);
    connect(studiesSelectCombo_, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(onStudiesCboxSelectionChanged(const QString&)));

    QLabel* seriesLabel = new QLabel(tr("Serie:"));
    seriesSelectCombo_ = new QComboBox;
    seriesSelectCombo_->setMinimumWidth(200);
    connect(seriesSelectCombo_, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(onSeriesCboxSelectionChanged(const QString&)));

    QLabel* scalingLabel = new QLabel(tr("Scaling:"));
    scalingFactorSpinBox_ = new QSpinBox;
    scalingFactorSpinBox_->setRange(MIN_ZOOM_PERCENT, MAX_ZOOM_PERCENT);
    scalingFactorSpinBox_->setSuffix(tr("%"));
    scalingFactorSpinBox_->setSingleStep(ZOOM_SINGLE_STEP);
    scalingFactorSpinBox_->setValue(100);
    connect(scalingFactorSpinBox_, SIGNAL(valueChanged(int)), this, SLOT(onScalingSpinBoxValueChanged(int)));

    QLabel* sliceLabel = new QLabel(tr("Slice"));
    sliceSpinBox_ = new QSpinBox;
    sliceSpinBox_->setValue(1);
    connect(sliceSpinBox_, SIGNAL(valueChanged(int)), this, SLOT(onSliceSpinBoxValueChanged(int)));
    sliceTotalCountLabel_ = new QLabel(tr("of 0"));

    QHBoxLayout* sliceLayout = new QHBoxLayout;
    sliceLayout->addWidget(sliceLabel);
    sliceLayout->addWidget(sliceSpinBox_);
    sliceLayout->addWidget(sliceTotalCountLabel_);

//    rulerCheckBox_ = new QCheckBox(tr("Ruler"));
//    rulerCheckBox_->setEnabled(true);
//    rulerCheckBox_->setChecked(false);
//    connect(rulerCheckBox_, SIGNAL(stateChanged(int)), this, SLOT(onRulerCheckBoxStateChanged(int)));

    QVBoxLayout* fileInfoVBox = new QVBoxLayout();
    fileInfoVBox->addWidget(patientsLabel);
    fileInfoVBox->addWidget(patientsSelectCombo_);
    fileInfoVBox->addWidget(studiesLabel);
    fileInfoVBox->addWidget(studiesSelectCombo_);
    fileInfoVBox->addWidget(seriesLabel);
    fileInfoVBox->addWidget(seriesSelectCombo_);

    navigationLayout_ = new QHBoxLayout;
    navigationLayout_->addLayout(fileInfoVBox);
    navigationLayout_->addLayout(sliceLayout);
    navigationLayout_->addSpacing(DEFAULT_OPTIONS_SPACING);
    navigationLayout_->addWidget(scalingLabel);
    navigationLayout_->addWidget(scalingFactorSpinBox_);
    navigationLayout_->setAlignment(Qt::AlignTop);
    //navigationLayout_->addSpacing(DEFAULT_OPTIONS_SPACING);
    //navigationLayout_->addWidget(rulerCheckBox_);
    navigationLayout_->addStretch();
}

void MainWindow::setupLungVolumeInfoLayout()
{
    totalVolumeAutoUpdateCheckBox_ = new QCheckBox(tr("Auto update"));
    totalVolumeAutoUpdateCheckBox_->setChecked(true);
    connect(totalVolumeAutoUpdateCheckBox_, SIGNAL(stateChanged(int)),
            this, SLOT(onTotalVolumeAutoUpdateStateChanged(int)));

    QLabel* totalVolumeLabel = new QLabel(tr("Total volume:"));
    totalVolumeLabel->setMaximumWidth(100);
    totalVolumeWidget_ = new QLineEdit;
    totalVolumeWidget_->setText("0.0");
    totalVolumeWidget_->setMaximumWidth(100);
    connect(totalVolumeWidget_, SIGNAL(textEdited(const QString&)), this, SLOT(updateManualTotalVolume()));
    QLabel* totalVolumeUnitsLabel = new QLabel(tr("cm3"));
    totalVolumeUnitsLabel->setMaximumWidth(30);

    QLabel* infectedVolumeLabel = new QLabel(tr("Infected volume:"));
    infectedVolumeLabel->setMaximumWidth(100);
    infectedVolumeWidget_ = new QLineEdit;
    infectedVolumeWidget_->setText("0.0");
    infectedVolumeWidget_->setReadOnly(true);
    infectedVolumeWidget_->setMaximumWidth(100);
    QLabel* infectedVolumeUnitsLabel = new QLabel(tr("cm3"));
    infectedVolumeUnitsLabel->setMaximumWidth(30);

    QLabel* infectedVolumePartLabel = new QLabel(tr("Infected volume percent:"));
    infectedVolumePartLabel->setMaximumWidth(100);
    infectedVolumePartWidget_ = new QLineEdit;
    infectedVolumePartWidget_->setText("0.0");
    infectedVolumePartWidget_->setReadOnly(true);
    infectedVolumePartWidget_->setMaximumWidth(100);
    QLabel* infectedVolumePartUnitsLabel = new QLabel(tr("%"));
    infectedVolumePartUnitsLabel->setMaximumWidth(30);

    // Начало: Абросов Сергей
    QLabel* densityOfHounsfieldPartLabel = new QLabel(tr("Density Hounsfield:"));
    densityOfHounsfieldPartLabel->setMaximumWidth(100);
    densityOfHounsfieldWidget_ = new QLineEdit;
    densityOfHounsfieldWidget_->setText("None");
    densityOfHounsfieldWidget_->setReadOnly(true);
    densityOfHounsfieldWidget_->setMaximumWidth(100);
    QLabel* densityHounsfieldPartUnitsLabel = new QLabel(tr("HU"));
    densityHounsfieldPartUnitsLabel->setMaximumWidth(30);

    QLabel* distanceFromTwoPointsLabel = new QLabel(tr("Distance (2 points):"));
    distanceFromTwoPointsLabel->setMaximumWidth(100);
    distanceFromTwoPointsWidget_ = new QLineEdit;
    distanceFromTwoPointsWidget_->setText("0.0");
    distanceFromTwoPointsWidget_->setReadOnly(true);
    distanceFromTwoPointsWidget_->setMaximumWidth(100);
    QLabel* distanceFromTwoPointsUnitsLabel = new QLabel(tr("cm"));
    distanceFromTwoPointsUnitsLabel->setMaximumWidth(30);
    // Конец: Абросов Сергей

    lungVolumeInfoLayout_ = new QGridLayout;
    lungVolumeInfoLayout_->addWidget(totalVolumeLabel, 0, 0);
    lungVolumeInfoLayout_->addWidget(totalVolumeAutoUpdateCheckBox_, 1, 0);
    lungVolumeInfoLayout_->addWidget(totalVolumeWidget_, 2, 0);
    lungVolumeInfoLayout_->addWidget(totalVolumeUnitsLabel, 2, 1);
    lungVolumeInfoLayout_->addWidget(infectedVolumeLabel, 3, 0);
    lungVolumeInfoLayout_->addWidget(infectedVolumeWidget_, 4, 0);
    lungVolumeInfoLayout_->addWidget(infectedVolumeUnitsLabel, 4, 1);
    lungVolumeInfoLayout_->addWidget(infectedVolumePartLabel, 5, 0);
    lungVolumeInfoLayout_->addWidget(infectedVolumePartWidget_, 6, 0);
    lungVolumeInfoLayout_->addWidget(infectedVolumePartUnitsLabel, 6, 1);
    // Начало: Абросов Сергей
    lungVolumeInfoLayout_->addWidget(densityOfHounsfieldPartLabel, 7, 0);
    lungVolumeInfoLayout_->addWidget(densityOfHounsfieldWidget_, 8, 0);
    lungVolumeInfoLayout_->addWidget(densityHounsfieldPartUnitsLabel, 8, 1);

    lungVolumeInfoLayout_->addWidget(distanceFromTwoPointsLabel, 9, 0);
    lungVolumeInfoLayout_->addWidget(distanceFromTwoPointsWidget_, 10, 0);
    lungVolumeInfoLayout_->addWidget(distanceFromTwoPointsUnitsLabel, 10, 1);
    // Конец: Абросов Сергей
    lungVolumeInfoLayout_->addItem(new QSpacerItem(0, 0), 11, 0);
    lungVolumeInfoLayout_->setRowStretch(11, 1);
}

void MainWindow::createActions()
{
    openAction_ = new QAction(tr("Open DICOM folder"), this);
    openAction_->setStatusTip(tr("Open DICOM folder"));
    connect(openAction_, SIGNAL(triggered()), this, SLOT(executeOpenProjectDirDialog()));

    openProjectAction_ = new QAction(tr("Open project"), this);
    openProjectAction_->setStatusTip(tr("Open project folder"));
    // TODO: Change the method member
    connect(openProjectAction_, SIGNAL(triggered()), this, SLOT(executeOpenProjectFiles()));

    exportRawImageAction_ = new QAction(tr("Export raw image"), this);
    exportRawImageAction_->setStatusTip(tr("Render current slice to image file"));
    connect(exportRawImageAction_, SIGNAL(triggered()), this, SLOT(executeExportRawImageDialog()));

    exportImageWithMarksAction_ = new QAction(tr("Export image with marks"), this);
    exportImageWithMarksAction_->setStatusTip(tr("Render current slice with marked infected area to image file"));
    connect(exportImageWithMarksAction_, SIGNAL(triggered()), this, SLOT(executeExportImageWithMarksDialog()));

    saveProjectAction_ = new QAction(tr("Save project"), this);
    saveProjectAction_->setStatusTip(tr("Save the hole project"));
    connect(saveProjectAction_, SIGNAL(triggered()), this, SLOT(executeSaveProjectFiles()));

    exitAction_ = new QAction(tr("E&xit"), this);
    exitAction_->setShortcuts(QKeySequence::Quit);
    exitAction_->setStatusTip(tr("Quit the application"));
    connect(exitAction_, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));
}

void MainWindow::createMenus()
{
    fileMenu_ = menuBar()->addMenu(tr("&File"));
    fileMenu_->addAction(openAction_);
    fileMenu_->addAction(openProjectAction_);
    fileMenu_->addAction(exportRawImageAction_);
    fileMenu_->addAction(exportImageWithMarksAction_);
    fileMenu_->addAction(saveProjectAction_);
    fileMenu_->addSeparator();
    fileMenu_->addAction(exitAction_);

    updateMenuActions();
}

void MainWindow::updateMenuActions()
{
    const DicomSlicePtr slice = currentSlice();
    const bool sliceIsAvailable = !slice.isNull();

    exportRawImageAction_->setEnabled(sliceIsAvailable);
    exportImageWithMarksAction_->setEnabled(sliceIsAvailable);
    saveProjectAction_->setEnabled(sliceIsAvailable);
}

void MainWindow::resetProject()
{
    if (project_) {
        delete project_;
        project_ = nullptr;
    }

    editWidget_->reset();
    viewWidget_->reset();
}

void MainWindow::processPath(const QString& path)
{
    const QFileInfo info(path);
    if (info.isDir()) {
        emit projectRootChanged(path);
    } else {
        const QString projectPath = info.dir().absolutePath();
        emit projectRootChanged(projectPath);
    }
}

void MainWindow::loadProject(const QString& path)
{
    project_ = new DicomProject(path);
    connect(project_, SIGNAL(loadFinished()), this, SLOT(onProjectLoadFinished()));
    project_->reload();
}

void MainWindow::renderRawImageToFile(const QString& filePath)
{
    DicomSlicePtr slice = currentSlice();
    if (!slice) {
        return;
    }
    const QPixmap pixmap = slice->pixmap();

    renderPixmapToFile(pixmap, filePath);
}

void MainWindow::renderImageWithMarksToFile(const QString& filePath)
{
    const QPixmap pixmap = editWidget_->getImageWithMarks();

    renderPixmapToFile(pixmap, filePath);
}

void MainWindow::saveProjectFies(const DicomSeriePtr& serie, const QString& dirPath)
{

    editWidget_->pixmap()->save(dirPath + QDir::separator() + "pixmap1234134124");
//    for(int i = 0; i < serie->getSliceCount(); ++i)
//    {
//        const QPixmap pixmap = serie->getSlice(i)->pixmap();
//
//        renderPixmapToFile(pixmap, dirPath + "/" + "image" + QString::number(i + 1));
//    }

//    QDir dir(projectPath_);

//    foreach (QString d, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
//    {
//        QString dst_path = dirPath + QDir::separator() + d;
//        dir.mkpath(dst_path);
//        copyPath(projectPath_ + QDir::separator() + d, dst_path);
//    }


//    QStringList files = dir.entryList(QDir::Files);
//    for(int i = 0; i < files.count(); ++i)
//    {
//        QFile::copy(projectPath_ + QDir::separator() + files[i],
//                    dirPath + QDir::separator() + files[i]);
//    }
}

void MainWindow::renderPixmapToFile(const QPixmap& pixmap, const QString& filePath)
{
    pixmap.save(filePath + ".png","PNG");
}

void MainWindow::updateScalingFactorSpinBox()
{
    const qreal actualCellSizeOnScreenCmAfterUpdate = viewWidget_->getCellSizeOnScreenCm();
    const qreal actualScaleFactorAfterUpdate = actualCellSizeOnScreenCmAfterUpdate / currentCellSpacing().x();

    bool bs = scalingFactorSpinBox_->blockSignals(true);
    scalingFactorSpinBox_->setValue(qRound(actualScaleFactorAfterUpdate * 100));
    scalingFactorSpinBox_->blockSignals(bs);
}

void MainWindow::updateSliceSpinBox()
{
    const DicomSeriePtr serie = currentSerie();
    if (!serie) {
        return;
    }

    const int totalSliceCount = serie->getSliceCount();

    bool bs = sliceSpinBox_->blockSignals(true);
    sliceSpinBox_->setMinimum(1);
    sliceSpinBox_->setMaximum(totalSliceCount);
    sliceSpinBox_->blockSignals(bs);

    sliceTotalCountLabel_->setText(tr("of %1").arg(strutils::intToString(totalSliceCount)));
}

DicomPatientPtr MainWindow::currentPatient() const
{
    if (!project_) {
        return DicomPatientPtr();
    }

    return project_->getPatient(patientsSelectCombo_->currentText());
}

DicomStudyPtr MainWindow::currentStudy() const
{
    DicomPatientPtr patient = currentPatient();
    if (!patient) {
        return DicomStudyPtr();
    }

    return patient->getStudy(studiesSelectCombo_->currentText());
}

DicomSeriePtr MainWindow::currentSerie() const
{
    DicomStudyPtr study = currentStudy();
    if (!study) {
        return DicomSeriePtr();
    }

    return study->getSerie(seriesSelectCombo_->currentText());
}

DicomSlicePtr MainWindow::currentSlice() const
{
    DicomSeriePtr serie = currentSerie();
    if (!serie) {
        return DicomSlicePtr();
    }

    const int sliceIndex = currentSliceIndex();
    return serie->getSlice(sliceIndex);
}

int MainWindow::currentSliceIndex() const
{
    return sliceSpinBox_->value() - 1;
}

QPointF MainWindow::currentCellSpacing() const
{
    const DicomSeriePtr serie = currentSerie();
    if (!serie) {
        return DEFAULT_CELL_SPACING_CM;
    }

    return serie->getCellSpacingCm();
}

bool MainWindow::isTotalVolumeAutoUpdateEnabled() const
{
    return totalVolumeAutoUpdateCheckBox_->isChecked();
}

qreal MainWindow::currentTotalVolume() const
{
    if (isTotalVolumeAutoUpdateEnabled()) {
        DicomSeriePtr serie = currentSerie();
        if (serie) {
            LungMatrix* lungMatrix = serie->getLungMatrix();
            if (lungMatrix) {
                return lungMatrix->getTotalVolume();
            }
        }
    }

    const auto volumeStr = totalVolumeWidget_->text();
    bool ok = false;
    const qreal volume = volumeStr.toDouble(&ok);
    if (ok) {
        return volume;
    }

    return 0.0;
}

void MainWindow::setLungMatrixTotalVolumeAutoUpdate(bool enabled) const
{
    DicomSeriePtr serie = currentSerie();
    if (serie) {
        LungMatrix* lungMatrix = serie->getLungMatrix();
        if (lungMatrix) {
            lungMatrix->setTotalVolumeAutoUpdate(enabled);
        }
    }
}

void MainWindow::updateActiveTool()
{
    int checkedId = toolRadioButtonsGroup_->checkedId();
    EditorTool tool = static_cast<EditorTool>(checkedId);
    editWidgetMouseHandlerHelper_->setActiveTool(tool);
}

} // namespace valt
} // namespace sklyph
