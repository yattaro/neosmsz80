TEMPLATE = subdirs

SUBDIRS = emu\
    gui\
    build
build.file = build.pro

CONFIG += c++11\
    ordered

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    common.pri
    