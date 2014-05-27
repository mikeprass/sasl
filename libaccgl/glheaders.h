#ifdef USE_GLES1
#include <GLES/gl.h>
#else
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#ifdef _MSC_VER
#include <windows.h>
#endif
#include <GL/gl.h>
#ifndef WINDOWS
#include <GL/glx.h>
#endif
#endif
#endif
