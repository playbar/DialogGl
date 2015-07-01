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

#ifndef EventTargetNames_h
#define EventTargetNames_h
#include "wtf/text/AtomicString.h"

namespace WebCore {
namespace EventTargetNames {

#ifndef EVENT_TARGET_NAMES_HIDE_GLOBALS
extern const WTF::AtomicString FontFaceSet;
extern const WTF::AtomicString MessagePort;
extern const WTF::AtomicString Node;
extern const WTF::AtomicString FileReader;
extern const WTF::AtomicString Screen;
extern const WTF::AtomicString MediaController;
extern const WTF::AtomicString InputMethodContext;
extern const WTF::AtomicString TextTrack;
extern const WTF::AtomicString TextTrackCue;
extern const WTF::AtomicString TextTrackList;
extern const WTF::AtomicString ApplicationCache;
extern const WTF::AtomicString EventSource;
extern const WTF::AtomicString SVGElementInstance;
extern const WTF::AtomicString Performance;
extern const WTF::AtomicString DedicatedWorkerGlobalScope;
extern const WTF::AtomicString SharedWorker;
extern const WTF::AtomicString SharedWorkerGlobalScope;
extern const WTF::AtomicString Worker;
extern const WTF::AtomicString XMLHttpRequest;
extern const WTF::AtomicString XMLHttpRequestUpload;
extern const WTF::AtomicString BatteryManager;
extern const WTF::AtomicString MediaKeySession;
extern const WTF::AtomicString FileWriter;
extern const WTF::AtomicString IDBDatabase;
extern const WTF::AtomicString IDBOpenDBRequest;
extern const WTF::AtomicString IDBRequest;
extern const WTF::AtomicString IDBTransaction;
extern const WTF::AtomicString MediaSource;
extern const WTF::AtomicString SourceBuffer;
extern const WTF::AtomicString SourceBufferList;
extern const WTF::AtomicString WebKitMediaSource;
extern const WTF::AtomicString WebKitSourceBufferList;
extern const WTF::AtomicString MediaStream;
extern const WTF::AtomicString MediaStreamTrack;
extern const WTF::AtomicString RTCDTMFSender;
extern const WTF::AtomicString RTCDataChannel;
extern const WTF::AtomicString RTCPeerConnection;
extern const WTF::AtomicString Notification;
extern const WTF::AtomicString ServiceWorkerGlobalScope;
extern const WTF::AtomicString SpeechRecognition;
extern const WTF::AtomicString SpeechSynthesisUtterance;
extern const WTF::AtomicString MIDIAccess;
extern const WTF::AtomicString MIDIInput;
extern const WTF::AtomicString MIDIPort;
extern const WTF::AtomicString WebSocket;
extern const WTF::AtomicString DOMWindow;
extern const WTF::AtomicString AudioContext;
extern const WTF::AtomicString AudioNode;

#endif // EVENT_TARGET_NAMES_HIDE_GLOBALS

 void init();

} // EventTargetNames
} // WebCore

#endif
