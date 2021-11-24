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

#ifdef QT_GUI_LIB

#include "gpropitem.h"

// ----------------------------------------------------------------------------
// GPropItemObjRef
// ----------------------------------------------------------------------------
struct G_EXPORT GPropItemObjRef : public GPropItem {
	Q_OBJECT

public:
	GPropItemObjRef(GPropItemParam* param);
	void update() override {}
};

#endif // QT_GUI_LIB
