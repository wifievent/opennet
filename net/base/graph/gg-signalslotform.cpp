#ifdef QT_GUI_LIB

#include "gg-signalslotform.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

GGSignalSlotForm::GGSignalSlotForm(QWidget *parent) : QDialog(parent) {
	setWindowTitle("Signal and Slot");

	lwSignalList_ = new QListWidget(this);
	lwSlotList_ = new QListWidget(this);
	btnOK_ = new QPushButton(this);
	btnOK_->setText("OK");
	btnOK_->setDefault(true);
	btnCancel_ = new QPushButton(this);
	btnCancel_->setText("Cancel");

	QHBoxLayout* listViewLayout = new QHBoxLayout;
	listViewLayout->addWidget(lwSignalList_);
	listViewLayout->addWidget(lwSlotList_);

	QHBoxLayout* buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(btnOK_);
	buttonLayout->addWidget(btnCancel_);

	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->addLayout(listViewLayout);
	mainLayout->addLayout(buttonLayout);

	QObject::connect(btnOK_, &QPushButton::clicked, this, &GGSignalSlotForm::on_btnOK_clicked);
	QObject::connect(btnCancel_, &QPushButton::clicked, this, &GGSignalSlotForm::on_btnCancel_clicked);
	QObject::connect(lwSignalList_, &QListWidget::currentRowChanged, this, &GGSignalSlotForm::on_lwSignalList_currentRowChanged);
	QObject::connect(lwSlotList_, &QListWidget::currentRowChanged, this, &GGSignalSlotForm::on_lwSlotList_currentRowChanged);
	QObject::connect(lwSignalList_, &QListWidget::clicked, this, &GGSignalSlotForm::on_lwSignalList_clicked);
	QObject::connect(lwSlotList_, &QListWidget::clicked, this, &GGSignalSlotForm::on_lwSlotList_clicked);

	setControl();
}

GGSignalSlotForm::~GGSignalSlotForm() {
}

void GGSignalSlotForm::setControl() {
	bool ok = true;
	if (lwSignalList_->selectedItems().count() == 0) ok = false;
	if (lwSlotList_->selectedItems().count() == 0) ok = false;
	btnOK_->setEnabled(ok);
}

void GGSignalSlotForm::on_btnOK_clicked(bool) {
	this->done(QDialog::Accepted);
}

void GGSignalSlotForm::on_btnCancel_clicked(bool) {
	this->done(QDialog::Rejected);
}

void GGSignalSlotForm::on_lwSignalList_currentRowChanged(int) {
	setControl();
}

void GGSignalSlotForm::on_lwSlotList_currentRowChanged(int) {
	setControl();
}

void GGSignalSlotForm::on_lwSignalList_clicked(const QModelIndex&) {
	setControl();
}

void GGSignalSlotForm::on_lwSlotList_clicked(const QModelIndex&) {
	setControl();
}

#endif // QT_GUI_LIB
