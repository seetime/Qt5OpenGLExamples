Cmake + Qt5 + OpenGL tutorials

This is a collection of Qt5 applications. To build these application, you need to have Cmake and Qt5 installed.

My test environment is follows:

CMake v.3.4.1
Visual Studio 2013 C++
Qt 5.4


To create a project for Visual Studio 2013, follow below steps.

1. Open a command window and go to source directory
2. Run following commands in console window.

	${SourceDir} $ mkdir build
	${SourceDir} $ cd build
	${SourceDir}/build $ cmake .. -G "Visual Studio 12 2013"
3. Open project file in VS and compile it.




