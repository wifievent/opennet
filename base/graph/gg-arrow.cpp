#ifdef QT_GUI_LIB

#include "gg-arrow.h"
#include "gg-text.h"
#include "gg-scene.h"
#include "ggraphwidget.h"
#include <cmath>

static constexpr qreal Pi = 3.14;

// ----------------------------------------------------------------------------
// GGArrow
// ----------------------------------------------------------------------------
GGArrow::GGArrow(GGText *startText, GGText *endText, GGraph::Connection* connection) : QGraphicsLineItem(nullptr) {
	myStartText_ = startText;
	myEndText_ = endText;

	setFlag(QGraphicsItem::ItemIsSelectable, true);
	myColor_ = Qt::black;
#ifdef Q_OS_ANDROID
	qreal width = 20;
#else // Q_OS_ANDROID
	qreal width = 5;
#endif // Q_OS_ANDROID
	setPen(QPen(myColor_, width, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
	setZValue(-1000.0);

	connection_ = connection;
}

GGArrow::~GGArrow() {
}

QRectF GGArrow::boundingRect() const {
	qreal extra = (pen().width() + 20) / 2.0;

	return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
		line().p2().y() - line().p1().y())).normalized().adjusted(-extra, -extra, extra, extra);
}

QPainterPath GGArrow::shape() const {
	QPainterPath path = QGraphicsLineItem::shape();
	path.addPolygon(arrowHead_);
	return path;
}

void GGArrow::updatePosition() {
	QLineF line(mapFromItem(myStartText_, 0, 0), mapFromItem(myEndText_, 0, 0));
	setLine(line);
}

void GGArrow::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*) {
	if (myStartText_->collidesWithItem(myEndText_))
		return;

	QPen myPen = pen();
	myPen.setColor(myColor_);
	qreal ArrowSize = 10;
	painter->setPen(myPen);
	painter->setBrush(myColor_);

	QPointF start, end;
	start.setX(myStartText_->pos().x() + myStartText_->boundingRect().width() / 2);
	start.setY(myStartText_->pos().y() + myStartText_->boundingRect().height() / 2);
	end.setX(myEndText_->pos().x() + myEndText_->boundingRect().width() / 2);
	end.setY(myEndText_->pos().y() + myEndText_->boundingRect().height() / 2);
	QLineF centerLine(start, end);

	QPointF intersectPoint;
	QLineF::IntersectType intersectType;
	QLineF polyLine;

	while (true) {
		polyLine = QLineF(myEndText_->sceneBoundingRect().topLeft(), myEndText_->sceneBoundingRect().topRight());
		intersectType = polyLine.intersects(centerLine, &intersectPoint);
		if (intersectType == QLineF::BoundedIntersection) break;

		polyLine = QLineF(myEndText_->sceneBoundingRect().topLeft(), myEndText_->sceneBoundingRect().bottomLeft());
		intersectType = polyLine.intersects(centerLine, &intersectPoint);
		if (intersectType == QLineF::BoundedIntersection) break;

		polyLine = QLineF(myEndText_->sceneBoundingRect().topRight(), myEndText_->sceneBoundingRect().bottomRight());
		intersectType = polyLine.intersects(centerLine, &intersectPoint);
		if (intersectType == QLineF::BoundedIntersection) break;

		polyLine = QLineF(myEndText_->sceneBoundingRect().bottomLeft(), myEndText_->sceneBoundingRect().bottomRight());
		intersectType = polyLine.intersects(centerLine, &intersectPoint);
		if (intersectType == QLineF::BoundedIntersection) break;

		break;
	}

	setLine(QLineF(intersectPoint, start));

	double angle = ::acos(line().dx() / line().length());
	if (line().dy() >= 0)
		angle = (Pi * 2) - angle;

	QPointF ArrowP1 = line().p1() + QPointF(sin(angle + Pi / 3) * ArrowSize, cos(angle + Pi / 3) * ArrowSize);
	QPointF ArrowP2 = line().p1() + QPointF(sin(angle + Pi - Pi / 3) * ArrowSize, cos(angle + Pi - Pi / 3) * ArrowSize);

	arrowHead_.clear();
	arrowHead_ << line().p1() << ArrowP1 << ArrowP2;

	painter->drawLine(line());
	painter->drawPolygon(arrowHead_);
	if (isSelected()) {
		painter->setPen(QPen(myColor_, 1, Qt::DashLine));
		QLineF myLine = line();
		myLine.translate(0, 4.0);
		painter->drawLine(myLine);
		myLine.translate(0,-8.0);
		painter->drawLine(myLine);
	}
}

#endif // QT_GUI_LIB
