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


#ifndef EventTargetInterfaces_h
#define EventTargetInterfaces_h

#if ENABLE(WEB_AUDIO)
#define EVENT_TARGET_INTERFACES_FOR_EACH_WEB_AUDIO(macro) \
    macro(AudioContext) \
    macro(AudioNode) \

#else
#define EVENT_TARGET_INTERFACES_FOR_EACH_WEB_AUDIO(macro)
#endif

#define EVENT_TARGET_INTERFACES_FOR_EACH(macro) \
    \
    macro(ApplicationCache) \
    macro(BatteryManager) \
    macro(DOMWindow) \
    macro(DedicatedWorkerGlobalScope) \
    macro(EventSource) \
    macro(FileReader) \
    macro(FileWriter) \
    macro(FontFaceSet) \
    macro(IDBDatabase) \
    macro(IDBOpenDBRequest) \
    macro(IDBRequest) \
    macro(IDBTransaction) \
    macro(InputMethodContext) \
    macro(MIDIAccess) \
    macro(MIDIInput) \
    macro(MIDIPort) \
    macro(MediaController) \
    macro(MediaKeySession) \
    macro(MediaSource) \
    macro(MediaStream) \
    macro(MediaStreamTrack) \
    macro(MessagePort) \
    macro(Node) \
    macro(Notification) \
    macro(Performance) \
    macro(RTCDTMFSender) \
    macro(RTCDataChannel) \
    macro(RTCPeerConnection) \
    macro(SVGElementInstance) \
    macro(Screen) \
    macro(ServiceWorkerGlobalScope) \
    macro(SharedWorker) \
    macro(SharedWorkerGlobalScope) \
    macro(SourceBuffer) \
    macro(SourceBufferList) \
    macro(SpeechRecognition) \
    macro(SpeechSynthesisUtterance) \
    macro(TextTrack) \
    macro(TextTrackCue) \
    macro(TextTrackList) \
    macro(WebKitMediaSource) \
    macro(WebKitSourceBufferList) \
    macro(WebSocket) \
    macro(Worker) \
    macro(XMLHttpRequest) \
    macro(XMLHttpRequestUpload) \
    \
    EVENT_TARGET_INTERFACES_FOR_EACH_WEB_AUDIO(macro) \

#endif // EventTargetInterfaces_h
