! include( ../common.pri ) {
    error( "Couldn't find the common.pri file!" )
}

HEADERS += sms_rom.h\
    sms.h\
    sms_types.h \
    sms_vdp.h \
    z80mem.h\
    z80proc.h

LIBS += -lz80

SOURCES += sms_rom.cpp\
    sms.cpp\
    sms_vdp.cpp \
    z80mem.cpp\
    z80proc.cpp
