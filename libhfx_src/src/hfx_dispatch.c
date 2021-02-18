#include <stdlib.h>
#include "GL/gl.h"

hfx_state *g_hfx_state = NULL;

void libhfx_init(void)
{
    g_hfx_state = hfx_init();
}

void glEnable (GLenum cap)
{
    hfx_enable(g_hfx_state, cap);
}

void glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
    hfx_clear_color_f(g_hfx_state, red, green, blue, alpha);
}

void glClear(GLbitfield mask)
{
    hfx_clear(g_hfx_state, mask);
}

void glLoadIdentity(void)
{
    hfx_load_identity(g_hfx_state);
}

void glTranslatef(GLfloat x, GLfloat y, GLfloat z)
{
    hfx_translate_f(g_hfx_state, x, y, z);
}

void glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z)
{
    hfx_rotate_f(g_hfx_state, angle, x, y, z);
}

void glScalef(GLfloat x, GLfloat y, GLfloat z)
{
    hfx_scale_f(g_hfx_state, x, y, z);
}

void glOrthof(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat zNear, GLfloat zFar)
{
    hfx_ortho_f(g_hfx_state, left, right, bottom, top, zNear, zFar);
}

void glMultMatrixf(const GLfloat *m)
{
    hfx_mult_matrix_f(g_hfx_state, (float*)m);
}

void glVertexPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
    hfx_vertex_pointer(g_hfx_state, size, type, stride, (void*)pointer);
}

void glColorPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
    hfx_color_pointer(g_hfx_state, size, type, stride, (void*)pointer);
}

void glTexCoordPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer)
{
    hfx_tex_coord_pointer(g_hfx_state, size, type, stride, (void*)pointer);
}

void glDrawArrays(GLenum mode, GLint first, GLsizei count)
{
    hfx_draw_arrays(g_hfx_state, mode, first, count);
}

void glGenTextures(GLsizei n, GLuint *textures)
{
    hfx_gen_textures(g_hfx_state, n, (uint32_t*)textures);
}

void glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels)
{
    hfx_tex_image_2d(g_hfx_state, target, level, internalformat, width, height, border, format, type, (void*)pixels);
}