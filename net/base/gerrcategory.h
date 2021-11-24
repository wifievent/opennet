// ----------------------------------------------------------------------------
//
// G Library
//
// http://gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#pragma once

struct GErrCategory {
	enum {
		BASE = 10000,
		NET = 20000,
		PCAP = 2100,
		HTTP = 2200,

		END = 99999
	};
};
