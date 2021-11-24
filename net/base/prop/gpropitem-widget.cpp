#include "gpropitem-widget.h"

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GPropItemWidget
// ----------------------------------------------------------------------------
GPropItemWidget::GPropItemWidget(GPropItemParam* param) : GPropItem(param) {
	widget_ = new QWidget(param->treeWidget_);
	param->treeWidget_->setItemWidget(item_, 1, widget_);
}

#endif // QT_GUI_LIB
