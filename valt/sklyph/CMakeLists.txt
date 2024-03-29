project(sklyph)

set(SRCS
        main.cpp
        LungMatrix.cpp
        MainWindow.cpp
        AspectRatioPixmapLabel.cpp
        DicomViewWidget.cpp
        DicomGriddedViewWidget.cpp
        DicomProject.cpp
        DicomPatient.cpp
        DicomStudy.cpp
        DicomSeries.cpp
        DicomReader.cpp
        DicomSlice.cpp
        LungMatrixMouseHandlerHelper.cpp
        AbstractToolMouseHandler.cpp
        NavigationToolMouseHandler.cpp
        PenToolMouseHandler.cpp
        FillToolMouseHandler.cpp
        PolygonToolMouseHandler.cpp
        RubberToolMouseHandler.cpp
    )

set(HDRS
        LungMatrix.h
        MainWindow.h
        AspectRatioPixmapLabel.h
        DicomViewWidget.h
        DicomGriddedViewWidget.h
        common.h
        DicomProject.h
        DicomPatient.h
        DicomStudy.h
        DicomSeries.h
        DicomReader.h
        DicomSlice.h
        LungMatrixMouseHandlerHelper.h
        AbstractToolMouseHandler.h
        NavigationToolMouseHandler.h
        PenToolMouseHandler.h
        FillToolMouseHandler.h
        PolygonToolMouseHandler.h
        RubberToolMouseHandler.h
    )

set(MOCS
        MainWindow.h
        LungMatrix.h
        AspectRatioPixmapLabel.h
        DicomViewWidget.h
        DicomGriddedViewWidget.h
        DicomProject.h
        DicomPatient.h
        DicomStudy.h
        DicomSeries.h
        LungMatrixMouseHandlerHelper.h
        AbstractToolMouseHandler.h
        NavigationToolMouseHandler.h
        PenToolMouseHandler.h
        FillToolMouseHandler.h
        PolygonToolMouseHandler.h
        RubberToolMouseHandler.h
   )

set(LIBS
        dcmcore
        dcmimage
   )

set(sklyph_DISABLE_CONSOLE TRUE)
include(build_qt_executable)
