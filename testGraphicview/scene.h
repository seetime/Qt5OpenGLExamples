#ifndef __SCENE_H__
#define	__SCENE_H__

#include <QtWidgets>
#include <QGLWidget>
#include <QtGui/QOpenGLFunctions>

//#include "geometryengine.h"

class GeometryEngine;

class ParameterEdit : public QWidget
{
public:
	virtual void emitChange() = 0;
};

#if 0
class RenderOptionsDialog : public QDialog
{
	Q_OBJECT
public:
	RenderOptionsDialog();
	int addTexture(const QString &name);
	int addShader(const QString &name);
	void emitParameterChanged();
	protected slots:
	void setColorParameter(QRgb color, int id);
	void setFloatParameter(float value, int id);
signals:
	void dynamicCubemapToggled(int);
	void colorParameterChanged(const QString &, QRgb);
	void floatParameterChanged(const QString &, float);
	void textureChanged(int);
	void shaderChanged(int);
	void doubleClicked();
protected:
	virtual void mouseDoubleClickEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

	QVector<QByteArray> m_parameterNames;
	QComboBox *m_textureCombo;
	QComboBox *m_shaderCombo;
	QVector<ParameterEdit *> m_parameterEdits;
};
#endif

class ItemDialog : public QDialog
{
	Q_OBJECT
public:
	enum ItemType {
		QtBoxItem,
		CircleItem,
		SquareItem,
	};

	ItemDialog();
	public slots:
	void triggerNewQtBox();
	void triggerNewCircleItem();
	void triggerNewSquareItem();
signals:
	void doubleClicked();
	void newItemTriggered(ItemDialog::ItemType type);
protected:
	virtual void mouseDoubleClickEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
};

class Scene : public QGraphicsScene
{
	Q_OBJECT
public:
	Scene(int width, int height, int maxTextureSize);
	~Scene();
	virtual void drawBackground(QPainter *painter, const QRectF &rect) Q_DECL_OVERRIDE;

	public slots:
	void setShader(int index);
	void setTexture(int index);
	void setColorParameter(const QString &name, QRgb color);
	void setFloatParameter(const QString &name, float value);
	//void newItem(ItemDialog::ItemType type);
protected:
	void setStates();
	void setLights();
	void defaultStates();

	void renderScene();

	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
	virtual void wheelEvent(QGraphicsSceneWheelEvent * event) Q_DECL_OVERRIDE;
private:
	void initGL();
	QPointF pixelPosToViewPos(const QPointF& p);

	//RenderOptionsDialog *m_renderOptions;

	QTime m_time;
	int m_lastTime;
	int m_mouseEventTime;
	int m_distExp;
	int m_frame;
	int m_maxTextureSize;

	QOpenGLShaderProgram *m_program;

	GLuint m_posAttr;
	GLuint m_colAttr;
	GLuint m_matrixUniform;

	GeometryEngine *m_geometries;
	QOpenGLTexture *m_texture;

	QQuaternion m_rotation;
	QMatrix4x4	m_projection;
};

#endif	// __SCENE_H__