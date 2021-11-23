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

#include "gpropitem-toolbutton.h"

// ----------------------------------------------------------------------------
// GPropItemObjRefArrayPtr
// ----------------------------------------------------------------------------
struct G_EXPORT GPropItemObjRefArrayPtr : public GPropItemToolButton {
	Q_OBJECT

public:
	GPropItemObjRefArrayPtr(GPropItemParam* param);
	void update() override;

protected slots:
	void myClicked(bool checked);
};

#endif // QT_GUI_LIB
