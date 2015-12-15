#ifndef __GRAPHICVIEW_H__
#define	__GRAPHICVIEW_H__

#include <QtWidgets>
#include <QGLWidget>

class GraphicsView : public QGraphicsView
{
public:
	GraphicsView();

	void initScene();

protected:
	void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
	QGraphicsTextItem *text;
};

#endif	// __GRAPHICVIEW_H__