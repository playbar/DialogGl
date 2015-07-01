/*
 * Copyright 2011 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */



#ifndef GrGLIRect_DEFINED
#define GrGLIRect_DEFINED

#include "gl/glew.h"
#include "GrGLUtil.h"
#include "GrTypes.h"

/**
 * Helper struct for dealing with the fact that Ganesh and GL use different
 * window coordinate systems (top-down vs bottom-up)
 */
struct GrGLIRect
{
    GLint   fLeft;
    GLint   fBottom;
    GLsizei fWidth;
    GLsizei fHeight;

    void pushToGLViewport( ) const
	{
        glViewport(fLeft, fBottom, fWidth, fHeight);
	}

    void pushToGLScissor( ) const 
	{
        glScissor(fLeft, fBottom, fWidth, fHeight);
    }

    void setFromGLViewport( ) 
	{
        GR_STATIC_ASSERT(sizeof(GrGLIRect) == 4*sizeof(GLint));
		glGetIntegerv( GL_VIEWPORT, (GLint*)this);
    }

    // sometimes we have a SkIRect from the client that we
    // want to simultaneously make relative to GL's viewport
    // and (optionally) convert from top-down to bottom-up.
    void setRelativeTo(const GrGLIRect& glRect,
                       int leftOffset,
                       int topOffset,
                       int width,
                       int height,
                       GrSurfaceOrigin origin)
	{
        fLeft = glRect.fLeft + leftOffset;
        fWidth = width;
        if (kBottomLeft_GrSurfaceOrigin == origin)
		{
            fBottom = glRect.fBottom + (glRect.fHeight - topOffset - height);
        } else {
            fBottom = glRect.fBottom + topOffset;
        }
        fHeight = height;

    }

    bool contains(const GrGLIRect& glRect) const
	{
        return fLeft <= glRect.fLeft &&
               fBottom <= glRect.fBottom &&
               fLeft + fWidth >=  glRect.fLeft + glRect.fWidth &&
               fBottom + fHeight >=  glRect.fBottom + glRect.fHeight;
    }

    void invalidate() {fLeft = fWidth = fBottom = fHeight = -1;}

    bool operator ==(const GrGLIRect& glRect) const 
	{
        return 0 == memcmp(this, &glRect, sizeof(GrGLIRect));
    }

    bool operator !=(const GrGLIRect& glRect) const {return !(*this == glRect);}
};

#endif