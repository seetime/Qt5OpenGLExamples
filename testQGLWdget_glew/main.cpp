#include <GL/glew.h>

#include "mainwindow.h"
#include <QApplication>
#include <QtWidgets>

static bool glew_dynamic_binding()
{
	int i;
	i = glewInit();

	const char *gl_extensions = (const char*)glGetString(GL_EXTENSIONS);

	// If the current opengl driver doesn't have framebuffers methods, check if an extension exists
	//if (QOpenGLFunctions::glGenFramebuffers == NULL)
	if (glGenFramebuffers == NULL)
	{
		//CCLOG("OpenGL: glGenFramebuffers is NULL, try to detect an extension\n");
		if (strstr(gl_extensions, "ARB_framebuffer_object"))
		{
			qDebug("OpenGL: ARB_framebuffer_object is supported\n");
#ifdef Q_OS_WIN_1
			glIsRenderbuffer = (PFNGLISRENDERBUFFERPROC)wglGetProcAddress("glIsRenderbuffer");
			glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)wglGetProcAddress("glBindRenderbuffer");
			glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)wglGetProcAddress("glDeleteRenderbuffers");
			glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)wglGetProcAddress("glGenRenderbuffers");
			glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)wglGetProcAddress("glRenderbufferStorage");
			glGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIVPROC)wglGetProcAddress("glGetRenderbufferParameteriv");
			glIsFramebuffer = (PFNGLISFRAMEBUFFERPROC)wglGetProcAddress("glIsFramebuffer");
			glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)wglGetProcAddress("glBindFramebuffer");
			glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)wglGetProcAddress("glDeleteFramebuffers");
			glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)wglGetProcAddress("glGenFramebuffers");
			glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)wglGetProcAddress("glCheckFramebufferStatus");
			glFramebufferTexture1D = (PFNGLFRAMEBUFFERTEXTURE1DPROC)wglGetProcAddress("glFramebufferTexture1D");
			glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)wglGetProcAddress("glFramebufferTexture2D");
			glFramebufferTexture3D = (PFNGLFRAMEBUFFERTEXTURE3DPROC)wglGetProcAddress("glFramebufferTexture3D");
			glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)wglGetProcAddress("glFramebufferRenderbuffer");
			glGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)wglGetProcAddress("glGetFramebufferAttachmentParameteriv");
			glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)wglGetProcAddress("glGenerateMipmap");
#else
#endif
		}
		else
			if (strstr(gl_extensions, "EXT_framebuffer_object"))
			{
				qDebug("OpenGL: EXT_framebuffer_object is supported\n");
#ifdef Q_OS_WIN_1
				glIsRenderbuffer = (PFNGLISRENDERBUFFERPROC)wglGetProcAddress("glIsRenderbufferEXT");
				glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)wglGetProcAddress("glBindRenderbufferEXT");
				glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)wglGetProcAddress("glDeleteRenderbuffersEXT");
				glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)wglGetProcAddress("glGenRenderbuffersEXT");
				glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)wglGetProcAddress("glRenderbufferStorageEXT");
				glGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIVPROC)wglGetProcAddress("glGetRenderbufferParameterivEXT");
				glIsFramebuffer = (PFNGLISFRAMEBUFFERPROC)wglGetProcAddress("glIsFramebufferEXT");
				glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)wglGetProcAddress("glBindFramebufferEXT");
				glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)wglGetProcAddress("glDeleteFramebuffersEXT");
				glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)wglGetProcAddress("glGenFramebuffersEXT");
				glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)wglGetProcAddress("glCheckFramebufferStatusEXT");
				glFramebufferTexture1D = (PFNGLFRAMEBUFFERTEXTURE1DPROC)wglGetProcAddress("glFramebufferTexture1DEXT");
				glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)wglGetProcAddress("glFramebufferTexture2DEXT");
				glFramebufferTexture3D = (PFNGLFRAMEBUFFERTEXTURE3DPROC)wglGetProcAddress("glFramebufferTexture3DEXT");
				glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)wglGetProcAddress("glFramebufferRenderbufferEXT");
				glGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC)wglGetProcAddress("glGetFramebufferAttachmentParameterivEXT");
				glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)wglGetProcAddress("glGenerateMipmapEXT");
#else

#endif
			}
			else
			{
				qDebug("OpenGL: No framebuffers extension is supported\n");
				qDebug("OpenGL: Any call to Fbo will crash!\n");
				return false;
			}
	}
	return true;
}


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
#if 1
	if ((QGLFormat::openGLVersionFlags() & QGLFormat::OpenGL_Version_1_5) == 0) {
		QMessageBox::critical(0, "OpenGL features missing",
			"OpenGL version 1.5 or higher is required to run this demo.\n"
			"The program will now exit.");
		return -1;
	}
#endif

	MainWindow* w = new MainWindow();

	w->setWindowTitle(QString::fromUtf8("QGLWidget Draw Circle with OpenGL "));

	MyGLWidget* glWidget = new MyGLWidget(w);
	glWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	w->setCentralWidget(glWidget);

	w->setFixedSize(400, 400);
	w->show();

	return a.exec();
}
