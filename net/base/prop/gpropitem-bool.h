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

#include "gpropitem-checkbox.h"

// ----------------------------------------------------------------------------
// GPropItemBool
// ----------------------------------------------------------------------------
struct G_EXPORT GPropItemBool : public GPropItemCheckBox {
	Q_OBJECT

public:
	GPropItemBool(GPropItemParam* param);
	void update() override;

protected slots:
	void myClicked(bool checked);
};

#endif // QT_GUI_LIB
