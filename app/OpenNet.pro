CONFIG -= qt
TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++11
CONFIG += c99
CONFIG += W_BUILD
include($${PWD}/../opennet.pri)
DESTDIR = $${PWD}/../bin

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../src/arphdr.cpp \
    ../src/buf.cpp \
    ../src/capture.cpp \
    ../src/dhcphdr.cpp \
    ../src/ethhdr.cpp \
    ../src/icmphdr.cpp \
    ../src/intf.cpp \
    ../src/intflist.cpp \
    ../src/ip.cpp \
    ../src/ip6.cpp \
    ../src/ip6hdr.cpp \
    ../src/iphdr.cpp \
    ../src/iphdr.cpp \
    ../src/mac.cpp \
    ../src/net.cpp \
    ../src/netinfo.cpp \
    ../src/obj.cpp \
    ../src/opennet.cpp \
    ../src/packet.cpp \
    ../src/pcapcapture.cpp \
    ../src/pcapdevice.cpp \
    ../src/pdu.cpp \
    ../src/rtm.cpp \
    ../src/rtmentry.cpp \
    ../src/tcpblock.cpp \
    ../src/tcphdr.cpp \
    ../src/udphdr.cpp \
    ../src/win/winipaddapterinfo.cpp \
    ../src/win/winipforwardtable.cpp

HEADERS += \
    ../src/arphdr.h \
    ../src/buf.h \
    ../src/capture.h \
    ../src/dhcphdr.h \
    ../src/ethhdr.h \
    ../src/icmphdr.h \
    ../src/intf.h \
    ../src/intflist.h \
    ../src/ip.h \
    ../src/ip6.h \
    ../src/ip6hdr.h \
    ../src/iphdr.h \
    ../src/iphdr.h \
    ../src/mac.h \
    ../src/net.h \
    ../src/netinfo.h \
    ../src/obj.h \
    ../src/opennet.h \
    ../src/packet.h \
    ../src/pcapcapture.h \
    ../src/pcapdevice.h \
    ../src/pdu.h \
    ../src/rtm.h \
    ../src/rtmentry.h \
    ../src/tcpblock.h \
    ../src/tcphdr.h \
    ../src/udphdr.h \
    ../src/win/winipaddapterinfo.h \
    ../src/win/winipforwardtable.h

win32 {
HEADERS += \
        ../src/win/wipadapterinfo.h \
        ../src/win/wipforwardtable.h

SOURCES += \
        ../src/win/wipadapterinfo.cpp \
        ../src/win/wipforwardtable.cpp
}

