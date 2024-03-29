/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#ifndef __CCSHADER_H__
#define __CCSHADER_H__

//#include "platform/CCPlatformMacros.h"

#include "GL/glew.h"

#define CC_GL_DEPTH24_STENCIL8		GL_DEPTH24_STENCIL8

// These macros are only for making CCTexturePVR.cpp complied without errors since they are not included in GLEW.
#define GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG                      0x8C00
#define GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG                      0x8C01
#define GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG                     0x8C02
#define GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG                     0x8C03

/**
 * @addtogroup shaders
 * @{
 */

extern  const GLchar * ccPosition_uColor_frag;
extern  const GLchar * ccPosition_uColor_vert;

extern  const GLchar * ccPositionColor_frag;
extern  const GLchar * ccPositionColor_vert;

extern  const GLchar * ccPositionTexture_frag;
extern  const GLchar * ccPositionTexture_vert;

extern  const GLchar * ccPositionTextureA8Color_frag;
extern  const GLchar * ccPositionTextureA8Color_vert;

extern  const GLchar * ccPositionTextureColor_frag;
extern  const GLchar * ccPositionTextureColor_vert;

extern  const GLchar * ccPositionTextureColorAlphaTest_frag;

extern  const GLchar * ccPositionTexture_uColor_frag;
extern  const GLchar * ccPositionTexture_uColor_vert;

extern  const GLchar * ccPositionColorLengthTexture_frag;
extern  const GLchar * ccPositionColorLengthTexture_vert;

extern  const GLchar * ccExSwitchMask_frag;

// end of shaders group
/// @}


#endif /* __CCSHADER_H__ */
