#include "base/gobj.h"
#include "gpropitem-objref.h"

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GPropItemObjRef
// ----------------------------------------------------------------------------
GPropItemObjRef::GPropItemObjRef(GPropItemParam* param) : GPropItem(param) {
	const char* propName = param->mpro_.name();
	QVariant variant = object_->property(propName);
	GObj* childObj = qvariant_cast<GObjRef>(variant);
	Q_ASSERT(childObj != nullptr);
	childObj->propCreateItems(param->treeWidget_, item_, childObj);
}

#endif // QT_GUI_LIB
