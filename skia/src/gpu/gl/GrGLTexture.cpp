/*
 * Copyright 2011 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "GrGLTexture.h"
#include "GrGpuGL.h"

#define GPUGL static_cast<GrGpuGL*>(getGpu())

void GrGLTexture::init(GrGpuGL* gpu,
                       const Desc& textureDesc,
                       const GrGLRenderTarget::Desc* rtDesc) {

    fTexParams.invalidate();
    fTexParamsTimestamp = GrGpu::kExpiredTimestamp;
    fTexIDObj.reset(SkNEW_ARGS(GrGLTexID, (textureDesc.fTextureID, textureDesc.fIsWrapped)));

    if (NULL != rtDesc) {
        GrGLIRect vp;
        vp.fLeft   = 0;
        vp.fWidth  = textureDesc.fWidth;
        vp.fBottom = 0;
        vp.fHeight = textureDesc.fHeight;

        fRenderTarget.reset(SkNEW_ARGS(GrGLRenderTarget, (gpu, *rtDesc, vp, fTexIDObj, this)));
    }
}

GrGLTexture::GrGLTexture(GrGpuGL* gpu,
                         const Desc& textureDesc)
    : INHERITED(gpu, textureDesc.fIsWrapped, textureDesc) {
    this->init(gpu, textureDesc, NULL);
}

GrGLTexture::GrGLTexture(GrGpuGL* gpu,
                         const Desc& textureDesc,
                         const GrGLRenderTarget::Desc& rtDesc)
    : INHERITED(gpu, textureDesc.fIsWrapped, textureDesc) {
    this->init(gpu, textureDesc, &rtDesc);
}

void GrGLTexture::onRelease() {
    GPUGL->notifyTextureDelete(this);
    fTexIDObj.reset(NULL);
    INHERITED::onRelease();
}

void GrGLTexture::onAbandon() {
    if (NULL != fTexIDObj.get()) {
        fTexIDObj->abandon();
        fTexIDObj.reset(NULL);
    }

    INHERITED::onAbandon();
}

intptr_t GrGLTexture::getTextureHandle() const {
    return static_cast<intptr_t>(this->textureID());
}