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

#include "gpropitem-combobox.h"

// ----------------------------------------------------------------------------
// GPropItemObjPtr
// ----------------------------------------------------------------------------
struct G_EXPORT GPropItemObjPtr : public GPropItemComboBox {
	Q_OBJECT

public:
	GPropItemObjPtr(GPropItemParam* param);
	void update() override;

protected slots:
	void myCurrentIndexChanged(int index);
};

#endif // QT_GUI_LIB
