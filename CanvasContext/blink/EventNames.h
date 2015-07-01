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

#ifndef EventNames_h
#define EventNames_h
#include "wtf/text/AtomicString.h"

namespace WebCore {
namespace EventNames {

#ifndef EVENT_NAMES_HIDE_GLOBALS
extern const WTF::AtomicString AutocompleteErrorEvent;
extern const WTF::AtomicString BeforeUnloadEvent;
extern const WTF::AtomicString CompositionEvent;
extern const WTF::AtomicString CustomEvent;
extern const WTF::AtomicString ErrorEvent;
extern const WTF::AtomicString Event;
extern const WTF::AtomicString FocusEvent;
extern const WTF::AtomicString HashChangeEvent;
extern const WTF::AtomicString KeyboardEvent;
extern const WTF::AtomicString MessageEvent;
extern const WTF::AtomicString MouseEvent;
extern const WTF::AtomicString MutationEvent;
extern const WTF::AtomicString OverflowEvent;
extern const WTF::AtomicString PageTransitionEvent;
extern const WTF::AtomicString PopStateEvent;
extern const WTF::AtomicString ProgressEvent;
extern const WTF::AtomicString ResourceProgressEvent;
extern const WTF::AtomicString TextEvent;
extern const WTF::AtomicString TouchEvent;
extern const WTF::AtomicString TransitionEvent;
extern const WTF::AtomicString UIEvent;
extern const WTF::AtomicString WebKitAnimationEvent;
extern const WTF::AtomicString WheelEvent;
extern const WTF::AtomicString WebGLContextEvent;
extern const WTF::AtomicString StorageEvent;
extern const WTF::AtomicString SVGZoomEvent;
extern const WTF::AtomicString XMLHttpRequestProgressEvent;
extern const WTF::AtomicString DeviceOrientationEvent;
extern const WTF::AtomicString IDBVersionChangeEvent;
extern const WTF::AtomicString MediaStreamEvent;
extern const WTF::AtomicString MediaStreamTrackEvent;
extern const WTF::AtomicString RTCDTMFToneChangeEvent;
extern const WTF::AtomicString RTCDataChannelEvent;
extern const WTF::AtomicString RTCIceCandidateEvent;
extern const WTF::AtomicString SpeechRecognitionError;
extern const WTF::AtomicString SpeechRecognitionEvent;
extern const WTF::AtomicString CloseEvent;
extern const WTF::AtomicString BeforeLoadEvent;
extern const WTF::AtomicString DeviceMotionEvent;
extern const WTF::AtomicString MediaKeyMessageEvent;
extern const WTF::AtomicString MediaKeyNeededEvent;
extern const WTF::AtomicString SecurityPolicyViolationEvent;
extern const WTF::AtomicString CSSFontFaceLoadEvent;
extern const WTF::AtomicString MediaKeyEvent;
extern const WTF::AtomicString FetchEvent;
extern const WTF::AtomicString InstallEvent;
extern const WTF::AtomicString InstallPhaseEvent;
extern const WTF::AtomicString SpeechSynthesisEvent;
extern const WTF::AtomicString TrackEvent;
extern const WTF::AtomicString MIDIConnectionEvent;
extern const WTF::AtomicString MIDIMessageEvent;
extern const WTF::AtomicString SpeechInputEvent;
extern const WTF::AtomicString AudioProcessingEvent;
extern const WTF::AtomicString OfflineAudioCompletionEvent;

#endif // EVENT_NAMES_HIDE_GLOBALS

 void init();

} // EventNames
} // WebCore

#endif
