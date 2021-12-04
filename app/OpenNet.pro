CONFIG -= qt
TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++11
CONFIG += OPENNET_BUILD
include($${PWD}/../opennet.pri)
DESTDIR = $${PWD}/../bin
INCLUDEPATH += $${PWD}/../include

SOURCES += \
    ../src/arphdr.cpp \
    ../src/buf.cpp \
    ../src/capture.cpp \
    ../src/dhcphdr.cpp \
    ../src/ethhdr.cpp \
    ../src/ethpacket.cpp \
    ../src/icmphdr.cpp \
    ../src/intf.cpp \
    ../src/intflist.cpp \
    ../src/ip.cpp \
    ../src/ip6.cpp \
    ../src/ip6hdr.cpp \
    ../src/iphdr.cpp \
    ../src/iphdr.cpp \
    ../src/ippacket.cpp \
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
    ../src/arpspoof.cpp \

HEADERS += \
    ../src/arphdr.h \
    ../src/buf.h \
    ../src/capture.h \
    ../src/dhcphdr.h \
    ../src/ethhdr.h \
    ../src/ethpacket.h \
    ../src/icmphdr.h \
    ../src/intf.h \
    ../src/intflist.h \
    ../src/ip.h \
    ../src/ip6.h \
    ../src/ip6hdr.h \
    ../src/iphdr.h \
    ../src/iphdr.h \
    ../src/ippacket.h \
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
    ../src/arpspoof.h \
    ../src/flow.h \


win32 {
HEADERS += \
        ../src/win/wipadapterinfo.h \
        ../src/win/wipforwardtable.h

SOURCES += \
        ../src/win/wipadapterinfo.cpp \
        ../src/win/wipforwardtable.cpp
}
