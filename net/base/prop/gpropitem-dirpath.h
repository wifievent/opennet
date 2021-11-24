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

#include <QHBoxLayout>
#include <QLineEdit>
#include <QToolButton>
#include "gpropitem-widget.h"

// ----------------------------------------------------------------------------
// GPropItemDirPath
// ----------------------------------------------------------------------------
struct G_EXPORT GPropItemDirPath : public GPropItemWidget {
	Q_OBJECT

public:
	GPropItemDirPath(GPropItemParam* param);
	void update() override;

public:
	QHBoxLayout* layout_{nullptr};
	QLineEdit* lineEdit_{nullptr};
	QToolButton* toolButton_{nullptr};

protected slots:
	void myEditingFinished();
	void myToolButtonClicked(bool checked);
};

#endif // QT_GUI_LIB
