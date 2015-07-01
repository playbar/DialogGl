/*
 * Copyright (C) 2013 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef MediaFeatureNames_h
#define MediaFeatureNames_h
#include "wtf/text/AtomicString.h"

namespace WebCore {
namespace MediaFeatureNames {

#ifndef MEDIA_FEATURE_NAMES_HIDE_GLOBALS
extern const WTF::AtomicString animationMediaFeature;
extern const WTF::AtomicString devicePixelRatioMediaFeature;
extern const WTF::AtomicString maxDevicePixelRatioMediaFeature;
extern const WTF::AtomicString minDevicePixelRatioMediaFeature;
extern const WTF::AtomicString transform2dMediaFeature;
extern const WTF::AtomicString transform3dMediaFeature;
extern const WTF::AtomicString viewModeMediaFeature;
extern const WTF::AtomicString aspectRatioMediaFeature;
extern const WTF::AtomicString colorMediaFeature;
extern const WTF::AtomicString colorIndexMediaFeature;
extern const WTF::AtomicString deviceAspectRatioMediaFeature;
extern const WTF::AtomicString deviceHeightMediaFeature;
extern const WTF::AtomicString deviceWidthMediaFeature;
extern const WTF::AtomicString gridMediaFeature;
extern const WTF::AtomicString heightMediaFeature;
extern const WTF::AtomicString hoverMediaFeature;
extern const WTF::AtomicString maxAspectRatioMediaFeature;
extern const WTF::AtomicString maxColorMediaFeature;
extern const WTF::AtomicString maxColorIndexMediaFeature;
extern const WTF::AtomicString maxDeviceAspectRatioMediaFeature;
extern const WTF::AtomicString maxDeviceHeightMediaFeature;
extern const WTF::AtomicString maxDeviceWidthMediaFeature;
extern const WTF::AtomicString maxHeightMediaFeature;
extern const WTF::AtomicString maxMonochromeMediaFeature;
extern const WTF::AtomicString maxResolutionMediaFeature;
extern const WTF::AtomicString maxWidthMediaFeature;
extern const WTF::AtomicString minAspectRatioMediaFeature;
extern const WTF::AtomicString minColorMediaFeature;
extern const WTF::AtomicString minColorIndexMediaFeature;
extern const WTF::AtomicString minDeviceAspectRatioMediaFeature;
extern const WTF::AtomicString minDeviceHeightMediaFeature;
extern const WTF::AtomicString minDeviceWidthMediaFeature;
extern const WTF::AtomicString minHeightMediaFeature;
extern const WTF::AtomicString minMonochromeMediaFeature;
extern const WTF::AtomicString minResolutionMediaFeature;
extern const WTF::AtomicString minWidthMediaFeature;
extern const WTF::AtomicString monochromeMediaFeature;
extern const WTF::AtomicString orientationMediaFeature;
extern const WTF::AtomicString pointerMediaFeature;
extern const WTF::AtomicString resolutionMediaFeature;
extern const WTF::AtomicString scanMediaFeature;
extern const WTF::AtomicString widthMediaFeature;

#endif // MEDIA_FEATURE_NAMES_HIDE_GLOBALS

 void init();

} // MediaFeatureNames
} // WebCore

#endif
