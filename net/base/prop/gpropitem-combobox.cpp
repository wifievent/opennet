#include "gpropitem-combobox.h"

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GPropItemComboBox
// ----------------------------------------------------------------------------
GPropItemComboBox::GPropItemComboBox(GPropItemParam* param) : GPropItem(param) {
	comboBox_ = new QComboBox(param->treeWidget_);
	param->treeWidget_->setItemWidget(item_, 1, comboBox_);
}

GPropItemComboBox::GPropItemComboBox(GPropItemParam* param, QStringList& items) : GPropItem(param) {
	comboBox_ = new QComboBox(param->treeWidget_);
	param->treeWidget_->setItemWidget(item_, 1, comboBox_);
	comboBox_->addItems(items);
}

#endif // QT_GUI_LIB
