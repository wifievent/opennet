QT -= gui

TEMPLATE = lib
DEFINES += OPENNET_LIBRARY

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    arphdr.cpp \
    buf.cpp \
    dhcphdr.cpp \
    ethhdr.cpp \
    icmphdr.cpp \
    ip.cpp \
    ip6.cpp \
    ip6hdr.cpp \
    iphdr.cpp \
    iphdr.cpp \
    mac.cpp \
    opennet.cpp \
    packet.cpp \
    tcphdr.cpp \
    udphdr.cpp

HEADERS += \
    OpenNet_global.h \
    arphdr.h \
    buf.h \
    dhcphdr.h \
    ethhdr.h \
    icmphdr.h \
    ip.h \
    ip6.h \
    ip6hdr.h \
    iphdr.h \
    iphdr.h \
    mac.h \
    opennet.h \
    packet.h \
    tcphdr.h \
    udphdr.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
