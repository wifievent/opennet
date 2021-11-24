#include "gpropitem-stringlist.h"

#ifdef QT_GUI_LIB

// ----------------------------------------------------------------------------
// GPropItemStringList
// ----------------------------------------------------------------------------
GPropItemStringList::GPropItemStringList(GPropItemParam* param) : GPropItemPlainTextEdit(param) {
	QObject::connect(plainTextEdit_, SIGNAL(editingFinished()), this, SLOT(myEditingFinished()));
}

void GPropItemStringList::update() {
	QVariant value = object_->property(mpro_.name());
	plainTextEdit_->setPlainText(value.toStringList().join("\n"));
}

void GPropItemStringList::myEditingFinished() {
	bool res = object_->setProperty(mpro_.name(), QVariant::fromValue<QStringList>(plainTextEdit_->toPlainText().split('\n')));
	if (!res) {
		qWarning() << QString("object->setProperty(%1, %2) return false").arg(mpro_.name(), plainTextEdit_->toPlainText());
	}
	update();
}

#endif // QT_GUI_LIB
