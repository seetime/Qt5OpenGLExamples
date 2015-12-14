﻿#ifndef GEOMETRYENGINE_H
#define	GEOMETRYENGINE_H

#include <QtWidgets>
#include <QGLWidget>

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

class GeometryEngine : protected QOpenGLFunctions
{
public:
	GeometryEngine();
	virtual ~GeometryEngine();

	void drawCubeGeometry(QOpenGLShaderProgram *program);

private:
	void initCubeGeometry();

	QOpenGLBuffer arrayBuf;
	QOpenGLBuffer indexBuf;
};

#endif	// GEOMETRYENGINE_H