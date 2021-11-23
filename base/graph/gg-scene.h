// ----------------------------------------------------------------------------
//
// G Library
//
// http://gilgil.net
//
// Copyright (c) Gilbert Lee All rights reserved
//
// ----------------------------------------------------------------------------

#pragma once

#ifdef QT_GUI_LIB

#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include <QTreeWidget>
#include <QMessageBox>

#include "gg-arrow.h"
#include "gg-text.h"
#include "gg-signalslotform.h"

// ----------------------------------------------------------------------------
// Scene
// ----------------------------------------------------------------------------
struct GGraphWidget;
struct G_EXPORT GGScene : QGraphicsScene {
	Q_OBJECT

public:
	GGScene(QObject *parent);
	virtual ~GGScene();

public:
	void clear();

public:
	enum Mode { InsertItem, MoveItem, InsertLine };
	Mode mode() { return mode_; }
	void setMode(Mode mode) { mode_ = mode; }

protected:
	Mode mode_;

public:
	GGraphWidget* graphWidget_;
	GGSignalSlotForm* signalSlotForm_;
	QGraphicsLineItem *dragLine_; // for drag and drop

public:
	GGText* createText(GObj* node, QPointF pos);
	GGArrow* createArrow(GGText* startText, GGText* endText, GGraph::Connection* connection);
	GGArrow* createArrow(QString startNodeName, QString endNodeName, GGraph::Connection* connection);
	GGText* findTextByObjectName(QString objectName);

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // QT_GUI_LIB
