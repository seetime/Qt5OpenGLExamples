#include "graphicview.h"


GraphicsView::GraphicsView()
{
	setWindowTitle(tr("Boxes"));
	setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	//setRenderHints(QPainter::SmoothPixmapTransform);
}

void GraphicsView::resizeEvent(QResizeEvent *event) {
	if (scene())
		scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
	
	QGraphicsView::resizeEvent(event);
}

void GraphicsView::initScene()
{
	if (scene())
	{
		// The bounding rectangle of the scene
		// The scene rectangle defines the extent of the scene.
		// It is primarily used by QGraphicsView
		// to determine the view's default scrollable area,
		// and by QGraphicsScene to manage item indexing.

		scene()->setSceneRect(-150, -150, 300, 300);

		text = scene()->addText("bogotobogo.com", QFont("Arial", 20));
		// movable text
		text->setFlag(QGraphicsItem::ItemIsMovable);
	}
}