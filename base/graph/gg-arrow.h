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

#include <QGraphicsLineItem>
#include <QPen>
#include "ggraph.h"

// ----------------------------------------------------------------------------
// GGArrow
// ----------------------------------------------------------------------------
struct GGText;
struct G_EXPORT GGArrow : QGraphicsLineItem {
public:
	enum { Type = UserType + 4 };
	GGArrow(GGText *startText, GGText *endText, GGraph::Connection* connection);
	~GGArrow() override;

	int type() const override { return Type; }
	QRectF boundingRect() const override;
	QPainterPath shape() const override;
	void setColor(const QColor &color) { myColor_ = color; }
	GGText *startText() const { return myStartText_; }
	GGText *endText() const { return myEndText_; }

	void updatePosition();

protected:
	void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) override;

private:
	GGText* myStartText_;
	GGText* myEndText_;
	QColor myColor_;
	QPolygonF arrowHead_;

public:
	GGraph::Connection* connection_;
};

#endif // QT_GUI_LIB
