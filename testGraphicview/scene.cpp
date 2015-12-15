#include <QtGui/QOpenGLFunctions>

#include "scene.h"
#include "geometryengine.h"

void qgluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);

//============================================================================//
//                                 ItemDialog                                 //
//============================================================================//

ItemDialog::ItemDialog()
	: QDialog(0, Qt::CustomizeWindowHint | Qt::WindowTitleHint)
{
	setWindowTitle(tr("Items (double click to flip)"));
	setWindowOpacity(0.75);
	resize(160, 100);

	QVBoxLayout *layout = new QVBoxLayout;
	setLayout(layout);
	QPushButton *button;

	button = new QPushButton(tr("Add Qt box"));
	layout->addWidget(button);
	connect(button, SIGNAL(clicked()), this, SLOT(triggerNewQtBox()));

	button = new QPushButton(tr("Add circle"));
	layout->addWidget(button);
	connect(button, SIGNAL(clicked()), this, SLOT(triggerNewCircleItem()));

	button = new QPushButton(tr("Add square"));
	layout->addWidget(button);
	connect(button, SIGNAL(clicked()), this, SLOT(triggerNewSquareItem()));

	layout->addStretch(1);
}

void ItemDialog::triggerNewQtBox()
{
	emit newItemTriggered(QtBoxItem);
}

void ItemDialog::triggerNewCircleItem()
{
	emit newItemTriggered(CircleItem);
}

void ItemDialog::triggerNewSquareItem()
{
	emit newItemTriggered(SquareItem);
}

void ItemDialog::mouseDoubleClickEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
		emit doubleClicked();
}

//============================================================================//
//                                    Scene                                   //
//============================================================================//

const static char environmentShaderText[] =
"uniform samplerCube env;"
"void main() {"
"gl_FragColor = textureCube(env, gl_TexCoord[1].xyz);"
"}";

//! [3]
static const char *vertexShaderSource =
"attribute highp vec4 posAttr;\n"
"attribute lowp vec4 colAttr;\n"
"varying lowp vec4 col;\n"
"uniform highp mat4 matrix;\n"
"void main() {\n"
"   col = colAttr;\n"
"   gl_Position = matrix * posAttr;\n"
"}\n";

static const char *fragmentShaderSource =
"varying lowp vec4 col;\n"
"void main() {\n"
"   gl_FragColor = col;\n"
"}\n";
//! [3]

Scene::Scene(int width, int height, int maxTextureSize)
	: m_distExp(600)
	, m_frame(0)
	, m_maxTextureSize(maxTextureSize)
	, m_geometries(0)
	, m_texture(0)
{
	setSceneRect(0, 0, width, height);
#if 0
	m_renderOptions = new RenderOptionsDialog;
	m_renderOptions->move(20, 120);
	m_renderOptions->resize(m_renderOptions->sizeHint());

	connect(m_renderOptions, SIGNAL(dynamicCubemapToggled(int)), this, SLOT(toggleDynamicCubemap(int)));
	connect(m_renderOptions, SIGNAL(colorParameterChanged(QString, QRgb)), this, SLOT(setColorParameter(QString, QRgb)));
	connect(m_renderOptions, SIGNAL(floatParameterChanged(QString, float)), this, SLOT(setFloatParameter(QString, float)));
	connect(m_renderOptions, SIGNAL(textureChanged(int)), this, SLOT(setTexture(int)));
	connect(m_renderOptions, SIGNAL(shaderChanged(int)), this, SLOT(setShader(int)));
#endif

	initGL();
}

Scene::~Scene()
{
	// Make sure the context is current when deleting the texture
	// and the buffers.
	//makeCurrent();
	delete m_texture;
	delete m_geometries;
	//doneCurrent();
}

void Scene::initGL()
{
#if 1
	m_program = new QOpenGLShaderProgram(this);
	m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "vshader.glsl");
	m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "fshader.glsl");
	m_program->link();
#else
	m_program = new QOpenGLShaderProgram(this);
	m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
	m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
	m_program->link();
	m_posAttr = m_program->attributeLocation("posAttr");
	m_colAttr = m_program->attributeLocation("colAttr");
	m_matrixUniform = m_program->uniformLocation("matrix");
#endif

	//connect(m_renderOptions, SIGNAL(doubleClicked()), twoSided, SLOT(flip()));


	// Load texture
	m_texture = new QOpenGLTexture(QImage(":/images/cube.png").mirrored());

	// Set nearest filtering mode for texture minification
	m_texture->setMinificationFilter(QOpenGLTexture::Nearest);

	// Set bilinear filtering mode for texture magnification
	m_texture->setMagnificationFilter(QOpenGLTexture::Linear);

	// Wrap texture coordinates by repeating
	// f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
	m_texture->setWrapMode(QOpenGLTexture::Repeat);

	//! [2]
	// Enable depth buffer
	glEnable(GL_DEPTH_TEST);

	// Enable back face culling
	glEnable(GL_CULL_FACE);
	//! [2]

	m_geometries = new GeometryEngine;
}

static void loadMatrix(const QMatrix4x4& m)
{
	// static to prevent glLoadMatrixf to fail on certain drivers
	static GLfloat mat[16];
	const float *data = m.constData();
	for (int index = 0; index < 16; ++index)
		mat[index] = data[index];
	glLoadMatrixf(mat);
}

void Scene::setStates()
{
	//glClearColor(0.25f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	//glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_NORMALIZE);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	setLights();

	float materialSpecular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpecular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 32.0f);
}

void Scene::setLights()
{
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	//float lightColour[] = {1.0f, 1.0f, 1.0f, 1.0f};
	float lightDir[] = { 0.0f, 0.0f, 1.0f, 0.0f };
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColour);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, lightColour);
	glLightfv(GL_LIGHT0, GL_POSITION, lightDir);
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0f);
	glEnable(GL_LIGHT0);
}

void Scene::defaultStates()
{
	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	//glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHT0);
	glDisable(GL_NORMALIZE);

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 0.0f);
	float defaultMaterialSpecular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, defaultMaterialSpecular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
}

void Scene::renderScene()
{
	//const qreal retinaScale = devicePixelRatio();
	//glViewport(0, 0, width() * retinaScale, height() * retinaScale);

	// Clear color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_program->bind();
	m_texture->bind();

	//! [6]
	// Calculate model view transformation
	QMatrix4x4 matrix;
	matrix.translate(0.0, 0.0, -5.0);
	matrix.rotate(m_rotation);

	// Set modelview-projection matrix
	m_program->setUniformValue("mvp_matrix", m_projection * matrix);
	//! [6]

	// Use texture unit 0 which contains cube.png
	m_program->setUniformValue("texture", 0);

	// Draw cube geometry
	m_geometries->drawCubeGeometry(m_program);

	++m_frame;
}

void Scene::drawBackground(QPainter *painter, const QRectF &)
{
	float width = float(painter->device()->width());
	float height = float(painter->device()->height());
	float aspect = width / height;

	painter->beginNativePainting();
	setStates();


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	qgluPerspective(60.0, aspect, 0.01, 15.0);

	// Reset projection
	m_projection.setToIdentity();

	// Set perspective projection
	m_projection.perspective(60, aspect, 0.01, 15.0);

	glMatrixMode(GL_MODELVIEW);

	renderScene();
#if 0
	QMatrix4x4 view;
	view.rotate(m_trackBalls[2].rotation());
	view(2, 3) -= 2.0f * exp(m_distExp / 1200.0f);
	renderBoxes(view);
#endif

	defaultStates();
	++m_frame;

	painter->endNativePainting();
}

QPointF Scene::pixelPosToViewPos(const QPointF& p)
{
	return QPointF(2.0 * float(p.x()) / width() - 1.0,
		1.0 - 2.0 * float(p.y()) / height());
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mouseMoveEvent(event);
	if (event->isAccepted())
		return;
#if 0
	if (event->buttons() & Qt::LeftButton) {
		m_trackBalls[0].move(pixelPosToViewPos(event->scenePos()), m_trackBalls[2].rotation().conjugate());
		event->accept();
	}
	else {
		m_trackBalls[0].release(pixelPosToViewPos(event->scenePos()), m_trackBalls[2].rotation().conjugate());
	}

	if (event->buttons() & Qt::RightButton) {
		m_trackBalls[1].move(pixelPosToViewPos(event->scenePos()), m_trackBalls[2].rotation().conjugate());
		event->accept();
	}
	else {
		m_trackBalls[1].release(pixelPosToViewPos(event->scenePos()), m_trackBalls[2].rotation().conjugate());
	}

	if (event->buttons() & Qt::MidButton) {
		m_trackBalls[2].move(pixelPosToViewPos(event->scenePos()), QQuaternion());
		event->accept();
	}
	else {
		m_trackBalls[2].release(pixelPosToViewPos(event->scenePos()), QQuaternion());
	}
#endif
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mousePressEvent(event);
	if (event->isAccepted())
		return;
#if 0
	if (event->buttons() & Qt::LeftButton) {
		m_trackBalls[0].push(pixelPosToViewPos(event->scenePos()), m_trackBalls[2].rotation().conjugate());
		event->accept();
	}

	if (event->buttons() & Qt::RightButton) {
		m_trackBalls[1].push(pixelPosToViewPos(event->scenePos()), m_trackBalls[2].rotation().conjugate());
		event->accept();
	}

	if (event->buttons() & Qt::MidButton) {
		m_trackBalls[2].push(pixelPosToViewPos(event->scenePos()), QQuaternion());
		event->accept();
	}
#endif
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsScene::mouseReleaseEvent(event);
	if (event->isAccepted())
		return;
#if 0
	if (event->button() == Qt::LeftButton) {
		m_trackBalls[0].release(pixelPosToViewPos(event->scenePos()), m_trackBalls[2].rotation().conjugate());
		event->accept();
	}

	if (event->button() == Qt::RightButton) {
		m_trackBalls[1].release(pixelPosToViewPos(event->scenePos()), m_trackBalls[2].rotation().conjugate());
		event->accept();
	}

	if (event->button() == Qt::MidButton) {
		m_trackBalls[2].release(pixelPosToViewPos(event->scenePos()), QQuaternion());
		event->accept();
	}
#endif
}

void Scene::wheelEvent(QGraphicsSceneWheelEvent * event)
{
	QGraphicsScene::wheelEvent(event);
	if (!event->isAccepted()) {
		m_distExp += event->delta();
		if (m_distExp < -8 * 120)
			m_distExp = -8 * 120;
		if (m_distExp > 10 * 120)
			m_distExp = 10 * 120;
		event->accept();
	}
}

void Scene::setShader(int index)
{

}

void Scene::setTexture(int index)
{

}

void Scene::setColorParameter(const QString &name, QRgb color)
{
	// set the color in all programs
	//foreach(QGLShaderProgram *program, m_programs) 
	{
		m_program->bind();
		m_program->setUniformValue(m_program->uniformLocation(name), QColor(color));
		m_program->release();
	}
}

void Scene::setFloatParameter(const QString &name, float value)
{
	// set the color in all programs
	//foreach(QGLShaderProgram *program, m_programs) 
	{
		m_program->bind();
		m_program->setUniformValue(m_program->uniformLocation(name), value);
		m_program->release();
	}
}

void qgluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
	const GLdouble ymax = zNear * tan(fovy * M_PI / 360.0);
	const GLdouble ymin = -ymax;
	const GLdouble xmin = ymin * aspect;
	const GLdouble xmax = ymax * aspect;
	glFrustum(xmin, xmax, ymin, ymax, zNear, zFar);
}