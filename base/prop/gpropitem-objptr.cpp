#include "gpropitem-objptr.h"
#include <QMetaEnum> // gilgil temp 2016.10.10
#include "base/graph/ggraph.h"

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GPropItemObjPtr
// ----------------------------------------------------------------------------
GPropItemObjPtr::GPropItemObjPtr(GPropItemParam* param) : GPropItemComboBox(param) {
	GGraph* graph = dynamic_cast<GGraph*>(param->object_->parent());
	if (graph != nullptr) {
		const QObjectList& objectList = graph->children();
		for (QObject* object: objectList) {
			if (dynamic_cast<GObj*>(object) == nullptr) continue;
			comboBox_->addItem(object->objectName());
		}
	}
	QObject::connect(comboBox_, SIGNAL(currentIndexChanged(int)), this, SLOT(myCurrentIndexChanged(int)));
}

void GPropItemObjPtr::update() {
	GGraph* graph = dynamic_cast<GGraph*>(object_->parent());
	if (graph != nullptr) {
		QVariant variant = object_->property(mpro_.name());
		GObj* obj = qvariant_cast<GObjPtr>(variant);
		if (obj == nullptr) {
			comboBox_->setCurrentIndex(-1);
		} else {
			comboBox_->setCurrentText(obj->objectName());
		}
	}
}

void GPropItemObjPtr::myCurrentIndexChanged(int index) {
	(void)index;
	GGraph* graph = dynamic_cast<GGraph*>(object_->parent());
	if (graph != nullptr) {
		QString key = comboBox_->currentText();
		GObj* node = graph->nodes_.findNode(key);
		if (node != nullptr) {
			GObjPtr objPtr = node;
			bool res = object_->setProperty(this->mpro_.name(), QVariant::fromValue<GObjPtr>(objPtr));
			if (!res) {
				qWarning() << QString("object->setProperty(%1, %2) return false").arg(mpro_.name(), key);
			}
			QVariant variant = object_->property(mpro_.name());
			GObj* obj = qvariant_cast<GObjPtr>(variant);
			if (obj == nullptr) {
				comboBox_->setCurrentIndex(-1);
			}
		}
	}
}

#endif // QT_GUI_LIB
