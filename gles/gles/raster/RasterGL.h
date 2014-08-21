/* Copyright (c) 2012 Scott Lembcke and Howling Moon Software
 * Copyright (c) 2012 cocos2d-x.org
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*
 * Code copied & pasted from SpacePatrol game https://github.com/slembcke/SpacePatrol
 *
 * Renamed and added some changes for cocos2d
 *
 */

#ifndef __RASTERGL_H__
#define __RASTERGL_H__

#include "CCNode.h"
#include "ccTypes.h"
#include "platformtype.h"

#include "tess.h"

/** CCDrawNode
 Node that draws dots, segments and polygons.
 Faster than the "drawing primitives" since they it draws everything in one single batch.
 
 @since v2.1
 */
class CC_DLL RasterGL //: public CCNode
{
protected:
    GLuint      m_uVao;
    GLuint      m_uVbo;
    
    unsigned int    m_uBufferCapacity;
    GLsizei         m_nBufferCount;
    ccV2F_C4B_T2F   *m_pBuffer;
    
    ccBlendFunc     m_sBlendFunc;
    
    bool            m_bDirty;

public:
    static RasterGL* create();
    virtual ~RasterGL();
    
    virtual bool init();
    virtual void draw();
    
    /** draw a dot at a position, with a given radius and color */
    void drawDot(const CCPoint &pos, float radius, const ccColor4F &color);
    
    /** draw a segment with a radius and color */
    void drawSegment(const CCPoint &from, const CCPoint &to, float radius, const ccColor4F &color);
    
	/** draw a polygon with a fill color and line color */
	void drawPolygon(CCPoint *verts, unsigned int count, const ccColor4F &fillColor, float borderWidth, const ccColor4F &borderColor);

	void drawTriangle( const CCPoint &p1, const CCPoint &p2, const CCPoint &p3, const ccColor4F &color );
    
    /** Clear the geometry in the node's buffer. */
    void clear();
    
    ccBlendFunc getBlendFunc() const;
    void setBlendFunc(const ccBlendFunc &blendFunc);
    
    RasterGL();

	void loadShaders();

public:
	void beginPolygon();
	void endPolygon();
	void drawAllPolygon();

private:
	GLUtesselator *tobj;
    
private:
    void ensureCapacity(unsigned int count);
    void render();

private:
	CCGLProgram *mProgram;
};

#endif // __CCDRAWNODES_CCDRAW_NODE_H__
