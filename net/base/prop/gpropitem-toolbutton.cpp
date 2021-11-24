#include "gpropitem-toolbutton.h"

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GPropItemToolButton
// ----------------------------------------------------------------------------
GPropItemToolButton::GPropItemToolButton(GPropItemParam* param) : GPropItem(param) {
	toolButton_ = new QToolButton(param->treeWidget_);
	toolButton_->setAutoRaise(true);
	param->treeWidget_->setItemWidget(item_, 1, toolButton_);
}

#endif // QT_GUI_LIB
