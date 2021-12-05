#
# include path
#
INCLUDEPATH *= $${PWD}/src
INCLUDEPATH *= $${PWD}/include
#
# link library
#
LIBS *= -L$${PWD}/bin -pthread -lopennet

!CONFIG(OPENNET_BUILD) {
   PRE_TARGETDEPS *= $${PWD}/bin/libOpenNet.a
}


#
# pcap
#
win32 {
        INCLUDEPATH *= $${PWD}/../../npcap/include
        LIBS *= -L$${PWD}/../../npcap/Lib/x64
        LIBS *= -lwpcap -lpacket -lws2_32 -liphlpapi
}

linux: LIBS *= -ldl -lpcap



