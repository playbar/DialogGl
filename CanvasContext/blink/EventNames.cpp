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

#include "config.h"

#ifdef SKIP_STATIC_CONSTRUCTORS_ON_GCC
#define EVENT_NAMES_HIDE_GLOBALS 1
#endif

#include "EventNames.h"

#include "wtf/StaticConstructors.h"

namespace WebCore {
namespace EventNames {

using namespace WTF;
DEFINE_GLOBAL(AtomicString, AutocompleteErrorEvent)
DEFINE_GLOBAL(AtomicString, BeforeUnloadEvent)
DEFINE_GLOBAL(AtomicString, CompositionEvent)
DEFINE_GLOBAL(AtomicString, CustomEvent)
DEFINE_GLOBAL(AtomicString, ErrorEvent)
DEFINE_GLOBAL(AtomicString, Event)
DEFINE_GLOBAL(AtomicString, FocusEvent)
DEFINE_GLOBAL(AtomicString, HashChangeEvent)
DEFINE_GLOBAL(AtomicString, KeyboardEvent)
DEFINE_GLOBAL(AtomicString, MessageEvent)
DEFINE_GLOBAL(AtomicString, MouseEvent)
DEFINE_GLOBAL(AtomicString, MutationEvent)
DEFINE_GLOBAL(AtomicString, OverflowEvent)
DEFINE_GLOBAL(AtomicString, PageTransitionEvent)
DEFINE_GLOBAL(AtomicString, PopStateEvent)
DEFINE_GLOBAL(AtomicString, ProgressEvent)
DEFINE_GLOBAL(AtomicString, ResourceProgressEvent)
DEFINE_GLOBAL(AtomicString, TextEvent)
DEFINE_GLOBAL(AtomicString, TouchEvent)
DEFINE_GLOBAL(AtomicString, TransitionEvent)
DEFINE_GLOBAL(AtomicString, UIEvent)
DEFINE_GLOBAL(AtomicString, WebKitAnimationEvent)
DEFINE_GLOBAL(AtomicString, WheelEvent)
DEFINE_GLOBAL(AtomicString, WebGLContextEvent)
DEFINE_GLOBAL(AtomicString, StorageEvent)
DEFINE_GLOBAL(AtomicString, SVGZoomEvent)
DEFINE_GLOBAL(AtomicString, XMLHttpRequestProgressEvent)
DEFINE_GLOBAL(AtomicString, DeviceOrientationEvent)
DEFINE_GLOBAL(AtomicString, IDBVersionChangeEvent)
DEFINE_GLOBAL(AtomicString, MediaStreamEvent)
DEFINE_GLOBAL(AtomicString, MediaStreamTrackEvent)
DEFINE_GLOBAL(AtomicString, RTCDTMFToneChangeEvent)
DEFINE_GLOBAL(AtomicString, RTCDataChannelEvent)
DEFINE_GLOBAL(AtomicString, RTCIceCandidateEvent)
DEFINE_GLOBAL(AtomicString, SpeechRecognitionError)
DEFINE_GLOBAL(AtomicString, SpeechRecognitionEvent)
DEFINE_GLOBAL(AtomicString, CloseEvent)
DEFINE_GLOBAL(AtomicString, BeforeLoadEvent)
DEFINE_GLOBAL(AtomicString, DeviceMotionEvent)
DEFINE_GLOBAL(AtomicString, MediaKeyMessageEvent)
DEFINE_GLOBAL(AtomicString, MediaKeyNeededEvent)
DEFINE_GLOBAL(AtomicString, SecurityPolicyViolationEvent)
DEFINE_GLOBAL(AtomicString, CSSFontFaceLoadEvent)
DEFINE_GLOBAL(AtomicString, MediaKeyEvent)
DEFINE_GLOBAL(AtomicString, FetchEvent)
DEFINE_GLOBAL(AtomicString, InstallEvent)
DEFINE_GLOBAL(AtomicString, InstallPhaseEvent)
DEFINE_GLOBAL(AtomicString, SpeechSynthesisEvent)
DEFINE_GLOBAL(AtomicString, TrackEvent)
DEFINE_GLOBAL(AtomicString, MIDIConnectionEvent)
DEFINE_GLOBAL(AtomicString, MIDIMessageEvent)
DEFINE_GLOBAL(AtomicString, SpeechInputEvent)
DEFINE_GLOBAL(AtomicString, AudioProcessingEvent)
DEFINE_GLOBAL(AtomicString, OfflineAudioCompletionEvent)

void init()
{
    StringImpl* AutocompleteErrorEventImpl = StringImpl::createStatic("AutocompleteErrorEvent", 22, 11231409);
    StringImpl* BeforeUnloadEventImpl = StringImpl::createStatic("BeforeUnloadEvent", 17, 6032602);
    StringImpl* CompositionEventImpl = StringImpl::createStatic("CompositionEvent", 16, 9205103);
    StringImpl* CustomEventImpl = StringImpl::createStatic("CustomEvent", 11, 13370215);
    StringImpl* ErrorEventImpl = StringImpl::createStatic("ErrorEvent", 10, 3070522);
    StringImpl* EventImpl = StringImpl::createStatic("Event", 5, 14343017);
    StringImpl* FocusEventImpl = StringImpl::createStatic("FocusEvent", 10, 8217765);
    StringImpl* HashChangeEventImpl = StringImpl::createStatic("HashChangeEvent", 15, 15010276);
    StringImpl* KeyboardEventImpl = StringImpl::createStatic("KeyboardEvent", 13, 6959608);
    StringImpl* MessageEventImpl = StringImpl::createStatic("MessageEvent", 12, 3580588);
    StringImpl* MouseEventImpl = StringImpl::createStatic("MouseEvent", 10, 12822602);
    StringImpl* MutationEventImpl = StringImpl::createStatic("MutationEvent", 13, 3855658);
    StringImpl* OverflowEventImpl = StringImpl::createStatic("OverflowEvent", 13, 1066761);
    StringImpl* PageTransitionEventImpl = StringImpl::createStatic("PageTransitionEvent", 19, 14165289);
    StringImpl* PopStateEventImpl = StringImpl::createStatic("PopStateEvent", 13, 13550868);
    StringImpl* ProgressEventImpl = StringImpl::createStatic("ProgressEvent", 13, 457487);
    StringImpl* ResourceProgressEventImpl = StringImpl::createStatic("ResourceProgressEvent", 21, 3169626);
    StringImpl* TextEventImpl = StringImpl::createStatic("TextEvent", 9, 9996370);
    StringImpl* TouchEventImpl = StringImpl::createStatic("TouchEvent", 10, 8776614);
    StringImpl* TransitionEventImpl = StringImpl::createStatic("TransitionEvent", 15, 4011983);
    StringImpl* UIEventImpl = StringImpl::createStatic("UIEvent", 7, 9523113);
    StringImpl* WebKitAnimationEventImpl = StringImpl::createStatic("WebKitAnimationEvent", 20, 3635677);
    StringImpl* WheelEventImpl = StringImpl::createStatic("WheelEvent", 10, 580237);
    StringImpl* WebGLContextEventImpl = StringImpl::createStatic("WebGLContextEvent", 17, 8713913);
    StringImpl* StorageEventImpl = StringImpl::createStatic("StorageEvent", 12, 5246637);
    StringImpl* SVGZoomEventImpl = StringImpl::createStatic("SVGZoomEvent", 12, 13387139);
    StringImpl* XMLHttpRequestProgressEventImpl = StringImpl::createStatic("XMLHttpRequestProgressEvent", 27, 740569);
    StringImpl* DeviceOrientationEventImpl = StringImpl::createStatic("DeviceOrientationEvent", 22, 858735);
    StringImpl* IDBVersionChangeEventImpl = StringImpl::createStatic("IDBVersionChangeEvent", 21, 7770749);
    StringImpl* MediaStreamEventImpl = StringImpl::createStatic("MediaStreamEvent", 16, 328267);
    StringImpl* MediaStreamTrackEventImpl = StringImpl::createStatic("MediaStreamTrackEvent", 21, 13109664);
    StringImpl* RTCDTMFToneChangeEventImpl = StringImpl::createStatic("RTCDTMFToneChangeEvent", 22, 16457152);
    StringImpl* RTCDataChannelEventImpl = StringImpl::createStatic("RTCDataChannelEvent", 19, 1105547);
    StringImpl* RTCIceCandidateEventImpl = StringImpl::createStatic("RTCIceCandidateEvent", 20, 6147283);
    StringImpl* SpeechRecognitionErrorImpl = StringImpl::createStatic("SpeechRecognitionError", 22, 13037355);
    StringImpl* SpeechRecognitionEventImpl = StringImpl::createStatic("SpeechRecognitionEvent", 22, 6869292);
    StringImpl* CloseEventImpl = StringImpl::createStatic("CloseEvent", 10, 11722244);
    StringImpl* BeforeLoadEventImpl = StringImpl::createStatic("BeforeLoadEvent", 15, 6334960);
    StringImpl* DeviceMotionEventImpl = StringImpl::createStatic("DeviceMotionEvent", 17, 12345260);
    StringImpl* MediaKeyMessageEventImpl = StringImpl::createStatic("MediaKeyMessageEvent", 20, 4827241);
    StringImpl* MediaKeyNeededEventImpl = StringImpl::createStatic("MediaKeyNeededEvent", 19, 15558110);
    StringImpl* SecurityPolicyViolationEventImpl = StringImpl::createStatic("SecurityPolicyViolationEvent", 28, 945681);
    StringImpl* CSSFontFaceLoadEventImpl = StringImpl::createStatic("CSSFontFaceLoadEvent", 20, 9053401);
    StringImpl* MediaKeyEventImpl = StringImpl::createStatic("MediaKeyEvent", 13, 7226908);
    StringImpl* FetchEventImpl = StringImpl::createStatic("FetchEvent", 10, 6447122);
    StringImpl* InstallEventImpl = StringImpl::createStatic("InstallEvent", 12, 13461741);
    StringImpl* InstallPhaseEventImpl = StringImpl::createStatic("InstallPhaseEvent", 17, 16110373);
    StringImpl* SpeechSynthesisEventImpl = StringImpl::createStatic("SpeechSynthesisEvent", 20, 17758);
    StringImpl* TrackEventImpl = StringImpl::createStatic("TrackEvent", 10, 14136022);
    StringImpl* MIDIConnectionEventImpl = StringImpl::createStatic("MIDIConnectionEvent", 19, 5515626);
    StringImpl* MIDIMessageEventImpl = StringImpl::createStatic("MIDIMessageEvent", 16, 12787442);
    StringImpl* SpeechInputEventImpl = StringImpl::createStatic("SpeechInputEvent", 16, 756970);
    StringImpl* AudioProcessingEventImpl = StringImpl::createStatic("AudioProcessingEvent", 20, 9607128);
    StringImpl* OfflineAudioCompletionEventImpl = StringImpl::createStatic("OfflineAudioCompletionEvent", 27, 6072331);
    new ((void*)&AutocompleteErrorEvent) AtomicString(AutocompleteErrorEventImpl);
    new ((void*)&BeforeUnloadEvent) AtomicString(BeforeUnloadEventImpl);
    new ((void*)&CompositionEvent) AtomicString(CompositionEventImpl);
    new ((void*)&CustomEvent) AtomicString(CustomEventImpl);
    new ((void*)&ErrorEvent) AtomicString(ErrorEventImpl);
    new ((void*)&Event) AtomicString(EventImpl);
    new ((void*)&FocusEvent) AtomicString(FocusEventImpl);
    new ((void*)&HashChangeEvent) AtomicString(HashChangeEventImpl);
    new ((void*)&KeyboardEvent) AtomicString(KeyboardEventImpl);
    new ((void*)&MessageEvent) AtomicString(MessageEventImpl);
    new ((void*)&MouseEvent) AtomicString(MouseEventImpl);
    new ((void*)&MutationEvent) AtomicString(MutationEventImpl);
    new ((void*)&OverflowEvent) AtomicString(OverflowEventImpl);
    new ((void*)&PageTransitionEvent) AtomicString(PageTransitionEventImpl);
    new ((void*)&PopStateEvent) AtomicString(PopStateEventImpl);
    new ((void*)&ProgressEvent) AtomicString(ProgressEventImpl);
    new ((void*)&ResourceProgressEvent) AtomicString(ResourceProgressEventImpl);
    new ((void*)&TextEvent) AtomicString(TextEventImpl);
    new ((void*)&TouchEvent) AtomicString(TouchEventImpl);
    new ((void*)&TransitionEvent) AtomicString(TransitionEventImpl);
    new ((void*)&UIEvent) AtomicString(UIEventImpl);
    new ((void*)&WebKitAnimationEvent) AtomicString(WebKitAnimationEventImpl);
    new ((void*)&WheelEvent) AtomicString(WheelEventImpl);
    new ((void*)&WebGLContextEvent) AtomicString(WebGLContextEventImpl);
    new ((void*)&StorageEvent) AtomicString(StorageEventImpl);
    new ((void*)&SVGZoomEvent) AtomicString(SVGZoomEventImpl);
    new ((void*)&XMLHttpRequestProgressEvent) AtomicString(XMLHttpRequestProgressEventImpl);
    new ((void*)&DeviceOrientationEvent) AtomicString(DeviceOrientationEventImpl);
    new ((void*)&IDBVersionChangeEvent) AtomicString(IDBVersionChangeEventImpl);
    new ((void*)&MediaStreamEvent) AtomicString(MediaStreamEventImpl);
    new ((void*)&MediaStreamTrackEvent) AtomicString(MediaStreamTrackEventImpl);
    new ((void*)&RTCDTMFToneChangeEvent) AtomicString(RTCDTMFToneChangeEventImpl);
    new ((void*)&RTCDataChannelEvent) AtomicString(RTCDataChannelEventImpl);
    new ((void*)&RTCIceCandidateEvent) AtomicString(RTCIceCandidateEventImpl);
    new ((void*)&SpeechRecognitionError) AtomicString(SpeechRecognitionErrorImpl);
    new ((void*)&SpeechRecognitionEvent) AtomicString(SpeechRecognitionEventImpl);
    new ((void*)&CloseEvent) AtomicString(CloseEventImpl);
    new ((void*)&BeforeLoadEvent) AtomicString(BeforeLoadEventImpl);
    new ((void*)&DeviceMotionEvent) AtomicString(DeviceMotionEventImpl);
    new ((void*)&MediaKeyMessageEvent) AtomicString(MediaKeyMessageEventImpl);
    new ((void*)&MediaKeyNeededEvent) AtomicString(MediaKeyNeededEventImpl);
    new ((void*)&SecurityPolicyViolationEvent) AtomicString(SecurityPolicyViolationEventImpl);
    new ((void*)&CSSFontFaceLoadEvent) AtomicString(CSSFontFaceLoadEventImpl);
    new ((void*)&MediaKeyEvent) AtomicString(MediaKeyEventImpl);
    new ((void*)&FetchEvent) AtomicString(FetchEventImpl);
    new ((void*)&InstallEvent) AtomicString(InstallEventImpl);
    new ((void*)&InstallPhaseEvent) AtomicString(InstallPhaseEventImpl);
    new ((void*)&SpeechSynthesisEvent) AtomicString(SpeechSynthesisEventImpl);
    new ((void*)&TrackEvent) AtomicString(TrackEventImpl);
    new ((void*)&MIDIConnectionEvent) AtomicString(MIDIConnectionEventImpl);
    new ((void*)&MIDIMessageEvent) AtomicString(MIDIMessageEventImpl);
    new ((void*)&SpeechInputEvent) AtomicString(SpeechInputEventImpl);
    new ((void*)&AudioProcessingEvent) AtomicString(AudioProcessingEventImpl);
    new ((void*)&OfflineAudioCompletionEvent) AtomicString(OfflineAudioCompletionEventImpl);
}

} // Event
} // WebCore
