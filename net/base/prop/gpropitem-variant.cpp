#include "gpropitem-variant.h"

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GPropItemVariant
// ----------------------------------------------------------------------------
GPropItemVariant::GPropItemVariant(GPropItemParam* param) : GPropItemLineEdit(param) {
	QObject::connect(lineEdit_, SIGNAL(editingFinished()), this, SLOT(myEditingFinished()));
}

void GPropItemVariant::update() {
	QVariant value = object_->property(mpro_.name());
	lineEdit_->setText(value.toString());
}

void GPropItemVariant::myEditingFinished() {
	bool res = object_->setProperty(mpro_.name(), QVariant::fromValue<QString>(lineEdit_->text()));
	if (!res) {
		qWarning() << QString("object->setProperty(%1, %2) return false").arg(mpro_.name(), lineEdit_->text());
	}
	update();
}

#endif // QT_GUI_LIB
