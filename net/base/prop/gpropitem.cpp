#include "gpropitem.h"

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GPropItem
// ----------------------------------------------------------------------------
GPropItem::GPropItem(GPropItemParam* param) {
	Q_ASSERT(param->treeWidget_ != nullptr);
	setParent(param->treeWidget_);
	if (param->parent_ == nullptr)
		item_ = new QTreeWidgetItem(param->treeWidget_);
	else
		item_ = new QTreeWidgetItem(param->parent_);
	object_ = param->object_;
	mpro_ = param->mpro_;
	item_->setText(0, mpro_.name());
	if (param->parent_ == nullptr)
		param->treeWidget_->addTopLevelItem(item_);
	item_->setExpanded(true);
}

#endif // QT_GUI_LIB
