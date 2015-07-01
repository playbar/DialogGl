/*
 * Copyright 2011 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef GrGLTexture_DEFINED
#define GrGLTexture_DEFINED

#include "GrGpu.h"
#include "GrGLRenderTarget.h"

/**
 * A ref counted tex id that deletes the texture in its destructor.
 */
class GrGLTexID : public SkRefCnt 
{
public:
    SK_DECLARE_INST_COUNT(GrGLTexID)

    GrGLTexID( GLuint texID, bool isWrapped):
        fTexID(texID), fIsWrapped(isWrapped) 
	{
    }

    virtual ~GrGLTexID()
	{
        if (0 != fTexID && !fIsWrapped) 
		{
            glDeleteTextures(1, &fTexID);
        }
    }

    void abandon() { fTexID = 0; }
    GLuint id() const { return fTexID; }

private:
    GLuint             fTexID;
    bool                 fIsWrapped;

    typedef SkRefCnt INHERITED;
};

////////////////////////////////////////////////////////////////////////////////


class GrGLTexture : public GrTexture 
{

public:
    struct TexParams {
        GLenum fMinFilter;
        GLenum fMagFilter;
        GLenum fWrapS;
        GLenum fWrapT;
        GLenum fSwizzleRGBA[4];
        void invalidate() { memset(this, 0xff, sizeof(TexParams)); }
    };

    struct Desc : public GrTextureDesc {
        GLuint        fTextureID;
        bool            fIsWrapped;
    };

    // creates a texture that is also an RT
    GrGLTexture(GrGpuGL* gpu,
                const Desc& textureDesc,
                const GrGLRenderTarget::Desc& rtDesc);

    // creates a non-RT texture
    GrGLTexture(GrGpuGL* gpu,
                const Desc& textureDesc);

    virtual ~GrGLTexture() { this->release(); }

    virtual intptr_t getTextureHandle() const SK_OVERRIDE;

    virtual void invalidateCachedState() SK_OVERRIDE { fTexParams.invalidate(); }

    // These functions are used to track the texture parameters associated with the texture.
    const TexParams& getCachedTexParams(GrGpu::ResetTimestamp* timestamp) const {
        *timestamp = fTexParamsTimestamp;
        return fTexParams;
    }

    void setCachedTexParams(const TexParams& texParams,
                            GrGpu::ResetTimestamp timestamp) {
        fTexParams = texParams;
        fTexParamsTimestamp = timestamp;
    }

    GLuint textureID() const { return (NULL != fTexIDObj.get()) ? fTexIDObj->id() : 0; }

protected:
    // overrides of GrTexture
    virtual void onAbandon() SK_OVERRIDE;
    virtual void onRelease() SK_OVERRIDE;

private:
    TexParams                       fTexParams;
    GrGpu::ResetTimestamp           fTexParamsTimestamp;
    SkAutoTUnref<GrGLTexID>         fTexIDObj;

    void init(GrGpuGL* gpu,
              const Desc& textureDesc,
              const GrGLRenderTarget::Desc* rtDesc);

    typedef GrTexture INHERITED;
};

#endif