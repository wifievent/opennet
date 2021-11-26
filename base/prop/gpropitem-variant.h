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

#include "gpropitem-lineedit.h"

// ----------------------------------------------------------------------------
// GPropItemVariant
// ----------------------------------------------------------------------------
struct G_EXPORT GPropItemVariant : public GPropItemLineEdit {
	Q_OBJECT

public:
	GPropItemVariant(GPropItemParam* param);
	void update() override;

protected slots:
	void myEditingFinished();
};

#endif // QT_GUI_LIB
