#pragma once

#include <pcap.h>
#include "pdu/gpdu.h"
#include "pdu/gethhdr.h"
#include "pdu/garphdr.h"
#include "pdu/giphdr.h"
#include "pdu/gip6hdr.h"
#include "pdu/gtcphdr.h"
#include "pdu/gudphdr.h"
#include "pdu/gicmphdr.h"

#include "pdu/gradiotaphdr.h"
#include "pdu/gdot11hdr.h"

class Packet {
public:
  Packet();
  ~Packet();
};