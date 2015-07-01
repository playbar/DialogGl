
/*
 * Copyright 2012 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

 #ifndef SkTwoPointRadialGradient_DEFINED
 #define SkTwoPointRadialGradient_DEFINED

 #include "SkGradientShaderPriv.h"

class SkTwoPointRadialGradient : public SkGradientShaderBase {
public:
    SkTwoPointRadialGradient(const SkPoint& start, float startRadius,
                              const SkPoint& end, float endRadius,
                              const Descriptor&);

    virtual BitmapType asABitmap(SkBitmap* bitmap,
                                 SkMatrix* matrix,
                                 TileMode* xy) const SK_OVERRIDE;
    virtual GradientType asAGradient(GradientInfo* info) const SK_OVERRIDE;
    virtual GrEffectRef* asNewEffect(GrContext* context, const SkPaint&) const SK_OVERRIDE;

    virtual void shadeSpan(int x, int y, SkPMColor* dstCParam,
                           int count) SK_OVERRIDE;
    virtual bool setContext(const SkBitmap& device,
                            const SkPaint& paint,
                            const SkMatrix& matrix) SK_OVERRIDE;

    float getCenterX1() const { return fDiff.length(); }
    float getStartRadius() const { return fStartRadius; }
    float getDiffRadius() const { return fDiffRadius; }

    SK_DEVELOPER_TO_STRING()
    SK_DECLARE_PUBLIC_FLATTENABLE_DESERIALIZATION_PROCS(SkTwoPointRadialGradient)

protected:
    SkTwoPointRadialGradient(SkFlattenableReadBuffer& buffer);
    virtual void flatten(SkFlattenableWriteBuffer& buffer) const SK_OVERRIDE;

private:
    typedef SkGradientShaderBase INHERITED;
    const SkPoint fCenter1;
    const SkPoint fCenter2;
    const float fRadius1;
    const float fRadius2;
    SkPoint fDiff;
    float fStartRadius, fDiffRadius, fSr2D2, fA, fOneOverTwoA;

    void init();
};

#endif