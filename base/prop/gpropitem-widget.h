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

#include <QComboBox>
#include "gpropitem.h"

// ----------------------------------------------------------------------------
// GPropItemWidget
// ----------------------------------------------------------------------------
struct G_EXPORT GPropItemWidget : public GPropItem {
	GPropItemWidget(GPropItemParam* param);

public:
	QWidget* widget_{nullptr};
};

#endif // QT_GUI_LIB
