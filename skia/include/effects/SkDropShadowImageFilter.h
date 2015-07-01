/*
 * Copyright 2013 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "SkColor.h"
#include "SkImageFilter.h"
#include "SkScalar.h"

class SK_API SkDropShadowImageFilter : public SkImageFilter {
public:
    SkDropShadowImageFilter(float dx, float dy, float sigma, SkColor, SkImageFilter* input = NULL);
    SkDropShadowImageFilter(float dx, float dy, float sigmaX, float sigmaY, SkColor, SkImageFilter* input = NULL, const CropRect* cropRect = NULL);
    SK_DECLARE_PUBLIC_FLATTENABLE_DESERIALIZATION_PROCS(SkDropShadowImageFilter)

protected:
    explicit SkDropShadowImageFilter(SkFlattenableReadBuffer&);
    virtual void flatten(SkFlattenableWriteBuffer&) const SK_OVERRIDE;
    virtual bool onFilterImage(Proxy*, const SkBitmap& source, const SkMatrix&, SkBitmap* result, SkIPoint* loc) SK_OVERRIDE;

private:
    float fDx, fDy, fSigmaX, fSigmaY;
    SkColor fColor;
    typedef SkImageFilter INHERITED;
};