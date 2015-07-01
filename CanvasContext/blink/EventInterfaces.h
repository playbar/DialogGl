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


#ifndef EventInterfaces_h
#define EventInterfaces_h

#if ENABLE(WEB_AUDIO)
#define EVENT_INTERFACES_FOR_EACH_WEB_AUDIO(macro) \
    macro(AudioProcessingEvent) \
    macro(OfflineAudioCompletionEvent) \

#else
#define EVENT_INTERFACES_FOR_EACH_WEB_AUDIO(macro)
#endif
#if ENABLE(INPUT_SPEECH)
#define EVENT_INTERFACES_FOR_EACH_INPUT_SPEECH(macro) \
    macro(SpeechInputEvent) \

#else
#define EVENT_INTERFACES_FOR_EACH_INPUT_SPEECH(macro)
#endif

#define EVENT_INTERFACES_FOR_EACH(macro) \
    \
    macro(AutocompleteErrorEvent) \
    macro(BeforeLoadEvent) \
    macro(BeforeUnloadEvent) \
    macro(CSSFontFaceLoadEvent) \
    macro(CloseEvent) \
    macro(CompositionEvent) \
    macro(CustomEvent) \
    macro(DeviceMotionEvent) \
    macro(DeviceOrientationEvent) \
    macro(ErrorEvent) \
    macro(Event) \
    macro(FetchEvent) \
    macro(FocusEvent) \
    macro(HashChangeEvent) \
    macro(IDBVersionChangeEvent) \
    macro(InstallEvent) \
    macro(InstallPhaseEvent) \
    macro(KeyboardEvent) \
    macro(MIDIConnectionEvent) \
    macro(MIDIMessageEvent) \
    macro(MediaKeyEvent) \
    macro(MediaKeyMessageEvent) \
    macro(MediaKeyNeededEvent) \
    macro(MediaStreamEvent) \
    macro(MediaStreamTrackEvent) \
    macro(MessageEvent) \
    macro(MouseEvent) \
    macro(MutationEvent) \
    macro(OverflowEvent) \
    macro(PageTransitionEvent) \
    macro(PopStateEvent) \
    macro(ProgressEvent) \
    macro(RTCDTMFToneChangeEvent) \
    macro(RTCDataChannelEvent) \
    macro(RTCIceCandidateEvent) \
    macro(ResourceProgressEvent) \
    macro(SVGZoomEvent) \
    macro(SecurityPolicyViolationEvent) \
    macro(SpeechRecognitionError) \
    macro(SpeechRecognitionEvent) \
    macro(SpeechSynthesisEvent) \
    macro(StorageEvent) \
    macro(TextEvent) \
    macro(TouchEvent) \
    macro(TrackEvent) \
    macro(TransitionEvent) \
    macro(UIEvent) \
    macro(WebGLContextEvent) \
    macro(WebKitAnimationEvent) \
    macro(WheelEvent) \
    macro(XMLHttpRequestProgressEvent) \
    \
    EVENT_INTERFACES_FOR_EACH_WEB_AUDIO(macro) \
    EVENT_INTERFACES_FOR_EACH_INPUT_SPEECH(macro) \

#endif // EventInterfaces_h
