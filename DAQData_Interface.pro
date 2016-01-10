#-------------------------------------------------
#
# Project created by QtCreator 2015-10-09T15:14:51
#
#-------------------------------------------------

QT       += core gui
QT       += opengl
QT       += widgets

TARGET = DAQData_Interface
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    DLGChannels.cpp \
    DLGOscillograph.cpp \
    DLGStereo.cpp \
    library.cpp \
    GLWidget.cpp \
    GLStereo.cpp

HEADERS  += mainwindow.h \
    library.h \
    DLGChannels.h \
    DLGOscillograph.h \
    DLGStereo.h \
    GLWidget.h \
    GLStereo.h

FORMS    += mainwindow.ui \
    dlgchannels.ui \
    dlgoscillograph.ui \
    dlgstereo.ui

win32:CONFIG(release, debug|release):

LIBS += -L"C:/Program Files (x86)/IVI Foundation/IVI/Lib_x64/msc/" -lVTEXDsa
LIBS += -L"C:/Program Files/HDF_Group/HDF5/1.8.15/lib" -lhdf5
LIBS += -L"C:/Program Files/HDF_Group/HDF5/1.8.15/lib" -lhdf5_cpp
LIBS += -L"C:/Program Files/HDF_Group/HDF5/1.8.15/lib" -lhdf5_hl
LIBS += -L"C:/Program Files/HDF_Group/HDF5/1.8.15/lib" -lhdf5_hl_cpp
LIBS += -L"C:/Program Files/HDF_Group/HDF5/1.8.15/lib" -lszip
LIBS += -L"C:/Program Files/HDF_Group/HDF5/1.8.15/lib" -lzlib

INCLUDEPATH += "C:/Program Files (x86)/IVI Foundation/VISA/WinNT/Include/"
DEPENDPATH += "C:/Program Files (x86)/IVI Foundation/VISA/WinNT/Include/"

INCLUDEPATH += "C:/Program Files (x86)/IVI Foundation/IVI/Include/"
DEPENDPATH += "C:/Program Files (x86)/IVI Foundation/IVI/Include/"

INCLUDEPATH += "C:/Program Files/HDF_Group/HDF5/1.8.15/include/"
DEPENDPATH += "C:/Program Files/HDF_Group/HDF5/1.8.15/include/"

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += "C:/Program Files (x86)/IVI Foundation/IVI/Lib_x64/msc/libVTEXDsa.a"
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += "C:/Program Files (x86)/IVI Foundation/IVI/Lib_x64/msc/VTEXDsa.lib"


