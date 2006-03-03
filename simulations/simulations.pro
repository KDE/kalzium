TEMPLATE = app
TARGET += 
DEPENDPATH += .
INCLUDEPATH += .

RESOURCES     = application.qrc

HEADERS += field.h \
           gamecontrols_impl.h \
           maindialog_impl.h \
           simulation.h \
           simulationfield.h \
           simulationsdialog.h \
           simulationsimplementations.h \
           stone.h \
           moc_gamecontrols_impl.cpp \
           moc_maindialog_impl.cpp \
           moc_simulation.cpp \
           moc_simulationfield.cpp \
           moc_simulationsdialog.cpp \
           moc_stone.cpp
FORMS += boltzmannform.ui \
         crystalform.ui \
         decompositionform.ui \
         gamecontrols.ui \
         lightabsorptionform.ui \
         maindialog.ui \
         raform.ui \
         settings.ui \
         volterraform.ui
SOURCES += field.cpp \
           gamecontrols_impl.cpp \
           main.cpp \
           maindialog_impl.cpp \
           simulation.cpp \
           simulationfield.cpp \
           simulationsdialog.cpp \
           simulationsimplementations.cpp \
           stone.cpp
