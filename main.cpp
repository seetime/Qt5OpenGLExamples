#include "graphicview.h"
#include "scene.h"

inline bool matchString(const char *extensionString, const char *subString)
{
	int subStringLength = strlen(subString);
	return (strncmp(extensionString, subString, subStringLength) == 0)
		&& ((extensionString[subStringLength] == ' ') || (extensionString[subStringLength] == '\0'));
}

bool necessaryExtensionsSupported()
{
	const char *extensionString = reinterpret_cast<const char *>(glGetString(GL_EXTENSIONS));
	const char *p = extensionString;

	const int GL_EXT_FBO = 1;
	const int GL_ARB_VS = 2;
	const int GL_ARB_FS = 4;
	const int GL_ARB_SO = 8;
	int extensions = 0;

	while (*p) {
		if (matchString(p, "GL_EXT_framebuffer_object"))
			extensions |= GL_EXT_FBO;
		else if (matchString(p, "GL_ARB_vertex_shader"))
			extensions |= GL_ARB_VS;
		else if (matchString(p, "GL_ARB_fragment_shader"))
			extensions |= GL_ARB_FS;
		else if (matchString(p, "GL_ARB_shader_objects"))
			extensions |= GL_ARB_SO;
		while ((*p != ' ') && (*p != '\0'))
			++p;
		if (*p == ' ')
			++p;
	}
	return (extensions == 15);
}

int main(int argc, char **argv)
{
	QApplication app(argc, argv);

	if ((QGLFormat::openGLVersionFlags() & QGLFormat::OpenGL_Version_1_5) == 0) {
		QMessageBox::critical(0, "OpenGL features missing",
			"OpenGL version 1.5 or higher is required to run this demo.\n"
			"The program will now exit.");
		return -1;
	}

	int maxTextureSize = 1024;
	QGLWidget *widget = new QGLWidget(QGLFormat(QGL::SampleBuffers));
	widget->makeCurrent();

	if (!necessaryExtensionsSupported()) {
		QMessageBox::critical(0, "OpenGL features missing",
			"The OpenGL extensions required to run this demo are missing.\n"
			"The program will now exit.");
		delete widget;
		return -2;
	}

#if 0
	// Check if all the necessary functions are resolved.
	if (!getGLExtensionFunctions().resolve(widget->context())) {
		QMessageBox::critical(0, "OpenGL features missing",
			"Failed to resolve OpenGL functions required to run this demo.\n"
			"The program will now exit.");
		delete widget;
		return -3;
	}
#endif

	widget->makeCurrent(); // The current context must be set before calling Scene's constructor

	

	Scene scene(1024, 768, maxTextureSize);
	GraphicsView view;
	view.setViewport(widget);
	view.setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	view.setScene(&scene);

	view.initScene();
	view.show();

	view.resize(1024, 768);

    return app.exec();
}

