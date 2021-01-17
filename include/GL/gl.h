#ifndef GL_H
#define GL_H
#include <hfx.h>

#ifndef APIENTRY
#define APIENTRY
#endif
#ifndef GLAPI
#define GLAPI extern
#endif

#define GL_COLOR_BUFFER_BIT HFX_COLOR_BUFFER_BIT
#define GL_DEPTH_BUFFER_BIT HFX_DEPTH_BUFFER_BIT

#define GL_BYTE HFX_BYTE
#define GL_UNSIGNED_BYTE HFX_UNSIGNED_BYTE
#define GL_SHORT HFX_SHORT
#define GL_UNSIGNED_SHORT HFX_UNSIGNED_SHORT
#define GL_INT HFX_INT
#define GL_UNSIGNED_INT HFX_UNSIGNED_INT
#define GL_FLOAT HFX_FLOAT

#define GL_UNSIGNED_SHORT_5_5_5_1 HFX_UNSIGNED_SHORT_5_5_5_1

#define GL_TEXTURE_2D HFX_TEXTURE_2D

#define GL_RGBA HFX_RGBA

#define GL_TRIANGLES HFX_TRIANGLES

#define GL_VERTEX_ARRAY HFX_VERTEX_ARRAY
#define GL_COLOR_ARRAY HFX_COLOR_ARRAY

#define GL_DEPTH_TEST HFX_DEPTH_TEST

typedef unsigned int GLenum;
typedef unsigned char GLboolean;
typedef unsigned int GLbitfield;
typedef signed char GLbyte;
typedef short GLshort;
typedef int GLint;
typedef int GLsizei;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned int GLuint;
typedef float GLfloat;
typedef float GLclampf;
typedef void GLvoid;
typedef int GLintptrARB;
typedef int GLsizeiptrARB;
typedef int GLfixed;
typedef int GLclampx;
/* Internal convenience typedefs */
typedef void (*_GLfuncptr)();

/*************************************************************/

GLAPI void APIENTRY glActiveTexture (GLenum texture);
GLAPI void APIENTRY glAlphaFunc (GLenum func, GLclampf ref);
GLAPI void APIENTRY glAlphaFuncx (GLenum func, GLclampx ref);
GLAPI void APIENTRY glBindTexture (GLenum target, GLuint texture);
GLAPI void APIENTRY glBlendFunc (GLenum sfactor, GLenum dfactor);
GLAPI void APIENTRY glClear (GLbitfield mask);
GLAPI void APIENTRY glClearColor (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
GLAPI void APIENTRY glClearColorx (GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha);
GLAPI void APIENTRY glClearDepthf (GLclampf depth);
GLAPI void APIENTRY glClearDepthx (GLclampx depth);
GLAPI void APIENTRY glClearStencil (GLint s);
GLAPI void APIENTRY glClientActiveTexture (GLenum texture);
GLAPI void APIENTRY glColor4f (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
GLAPI void APIENTRY glColor4x (GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha);
GLAPI void APIENTRY glColorMask (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
GLAPI void APIENTRY glColorPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
GLAPI void APIENTRY glCompressedTexImage2D (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data);
GLAPI void APIENTRY glCompressedTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data);
GLAPI void APIENTRY glCopyTexImage2D (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
GLAPI void APIENTRY glCopyTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
GLAPI void APIENTRY glCullFace (GLenum mode);
GLAPI void APIENTRY glDeleteTextures (GLsizei n, const GLuint *textures);
GLAPI void APIENTRY glDepthFunc (GLenum func);
GLAPI void APIENTRY glDepthMask (GLboolean flag);
GLAPI void APIENTRY glDepthRangef (GLclampf zNear, GLclampf zFar);
GLAPI void APIENTRY glDepthRangex (GLclampx zNear, GLclampx zFar);
GLAPI void APIENTRY glDisable (GLenum cap);
GLAPI void APIENTRY glDisableClientState (GLenum array);
GLAPI void APIENTRY glDrawArrays (GLenum mode, GLint first, GLsizei count);
GLAPI void APIENTRY glDrawElements (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
GLAPI void APIENTRY glEnable (GLenum cap);
GLAPI void APIENTRY glEnableClientState (GLenum array);
GLAPI void APIENTRY glFinish (void);
GLAPI void APIENTRY glFlush (void);
GLAPI void APIENTRY glFogf (GLenum pname, GLfloat param);
GLAPI void APIENTRY glFogfv (GLenum pname, const GLfloat *params);
GLAPI void APIENTRY glFogx (GLenum pname, GLfixed param);
GLAPI void APIENTRY glFogxv (GLenum pname, const GLfixed *params);
GLAPI void APIENTRY glFrontFace (GLenum mode);
GLAPI void APIENTRY glFrustumf (GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
GLAPI void APIENTRY glFrustumx (GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);
GLAPI void APIENTRY glGenTextures (GLsizei n, GLuint *textures);
GLAPI GLenum APIENTRY glGetError (void);
GLAPI void APIENTRY glGetIntegerv (GLenum pname, GLint *params);
GLAPI const GLubyte * APIENTRY glGetString (GLenum name);
GLAPI void APIENTRY glHint (GLenum target, GLenum mode);
GLAPI void APIENTRY glLightModelf (GLenum pname, GLfloat param);
GLAPI void APIENTRY glLightModelfv (GLenum pname, const GLfloat *params);
GLAPI void APIENTRY glLightModelx (GLenum pname, GLfixed param);
GLAPI void APIENTRY glLightModelxv (GLenum pname, const GLfixed *params);
GLAPI void APIENTRY glLightf (GLenum light, GLenum pname, GLfloat param);
GLAPI void APIENTRY glLightfv (GLenum light, GLenum pname, const GLfloat *params);
GLAPI void APIENTRY glLightx (GLenum light, GLenum pname, GLfixed param);
GLAPI void APIENTRY glLightxv (GLenum light, GLenum pname, const GLfixed *params);
GLAPI void APIENTRY glLineWidth (GLfloat width);
GLAPI void APIENTRY glLineWidthx (GLfixed width);
GLAPI void APIENTRY glLoadIdentity (void);
GLAPI void APIENTRY glLoadMatrixf (const GLfloat *m);
GLAPI void APIENTRY glLoadMatrixx (const GLfixed *m);
GLAPI void APIENTRY glLogicOp (GLenum opcode);
GLAPI void APIENTRY glMaterialf (GLenum face, GLenum pname, GLfloat param);
GLAPI void APIENTRY glMaterialfv (GLenum face, GLenum pname, const GLfloat *params);
GLAPI void APIENTRY glMaterialx (GLenum face, GLenum pname, GLfixed param);
GLAPI void APIENTRY glMaterialxv (GLenum face, GLenum pname, const GLfixed *params);
GLAPI void APIENTRY glMatrixMode (GLenum mode);
GLAPI void APIENTRY glMultMatrixf (const GLfloat *m);
GLAPI void APIENTRY glMultMatrixx (const GLfixed *m);
GLAPI void APIENTRY glMultiTexCoord4f (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
GLAPI void APIENTRY glMultiTexCoord4x (GLenum target, GLfixed s, GLfixed t, GLfixed r, GLfixed q);
GLAPI void APIENTRY glNormal3f (GLfloat nx, GLfloat ny, GLfloat nz);
GLAPI void APIENTRY glNormal3x (GLfixed nx, GLfixed ny, GLfixed nz);
GLAPI void APIENTRY glNormalPointer (GLenum type, GLsizei stride, const GLvoid *pointer);
GLAPI void APIENTRY glOrthof (GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar);
GLAPI void APIENTRY glOrthox (GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);
GLAPI void APIENTRY glPixelStorei (GLenum pname, GLint param);
GLAPI void APIENTRY glPointSize (GLfloat size);
GLAPI void APIENTRY glPointSizex (GLfixed size);
GLAPI void APIENTRY glPolygonOffset (GLfloat factor, GLfloat units);
GLAPI void APIENTRY glPolygonOffsetx (GLfixed factor, GLfixed units);
GLAPI void APIENTRY glPopMatrix (void);
GLAPI void APIENTRY glPushMatrix (void);
GLAPI void APIENTRY glReadPixels (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
GLAPI void APIENTRY glRotatef (GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
GLAPI void APIENTRY glRotatex (GLfixed angle, GLfixed x, GLfixed y, GLfixed z);
GLAPI void APIENTRY glSampleCoverage (GLclampf value, GLboolean invert);
GLAPI void APIENTRY glSampleCoveragex (GLclampx value, GLboolean invert);
GLAPI void APIENTRY glScalef (GLfloat x, GLfloat y, GLfloat z);
GLAPI void APIENTRY glScalex (GLfixed x, GLfixed y, GLfixed z);
GLAPI void APIENTRY glScissor (GLint x, GLint y, GLsizei width, GLsizei height);
GLAPI void APIENTRY glShadeModel (GLenum mode);
GLAPI void APIENTRY glStencilFunc (GLenum func, GLint ref, GLuint mask);
GLAPI void APIENTRY glStencilMask (GLuint mask);
GLAPI void APIENTRY glStencilOp (GLenum fail, GLenum zfail, GLenum zpass);
GLAPI void APIENTRY glTexCoordPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
GLAPI void APIENTRY glTexEnvf (GLenum target, GLenum pname, GLfloat param);
GLAPI void APIENTRY glTexEnvfv (GLenum target, GLenum pname, const GLfloat *params);
GLAPI void APIENTRY glTexEnvx (GLenum target, GLenum pname, GLfixed param);
GLAPI void APIENTRY glTexEnvxv (GLenum target, GLenum pname, const GLfixed *params);
GLAPI void APIENTRY glTexImage2D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
GLAPI void APIENTRY glTexParameterf (GLenum target, GLenum pname, GLfloat param);
GLAPI void APIENTRY glTexParameterx (GLenum target, GLenum pname, GLfixed param);
GLAPI void APIENTRY glTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
GLAPI void APIENTRY glTranslatef (GLfloat x, GLfloat y, GLfloat z);
GLAPI void APIENTRY glTranslatex (GLfixed x, GLfixed y, GLfixed z);
GLAPI void APIENTRY glVertexPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
GLAPI void APIENTRY glViewport (GLint x, GLint y, GLsizei width, GLsizei height);

#endif