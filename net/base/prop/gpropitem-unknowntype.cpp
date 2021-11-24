#include "gpropitem-unknowntype.h"

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GPropItemUnknownType
// ----------------------------------------------------------------------------
GPropItemUnknownType::GPropItemUnknownType(GPropItemParam* param) : GPropItem(param) {
	item_->setBackground(1, QBrush(Qt::lightGray));
}

#endif // QT_GUI_LIB
