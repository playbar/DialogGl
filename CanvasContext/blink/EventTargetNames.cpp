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
#define EVENT_TARGET_NAMES_HIDE_GLOBALS 1
#endif

#include "EventTargetNames.h"

#include "wtf/StaticConstructors.h"

namespace WebCore {
namespace EventTargetNames {

using namespace WTF;
DEFINE_GLOBAL(AtomicString, FontFaceSet)
DEFINE_GLOBAL(AtomicString, MessagePort)
DEFINE_GLOBAL(AtomicString, Node)
DEFINE_GLOBAL(AtomicString, FileReader)
DEFINE_GLOBAL(AtomicString, Screen)
DEFINE_GLOBAL(AtomicString, MediaController)
DEFINE_GLOBAL(AtomicString, InputMethodContext)
DEFINE_GLOBAL(AtomicString, TextTrack)
DEFINE_GLOBAL(AtomicString, TextTrackCue)
DEFINE_GLOBAL(AtomicString, TextTrackList)
DEFINE_GLOBAL(AtomicString, ApplicationCache)
DEFINE_GLOBAL(AtomicString, EventSource)
DEFINE_GLOBAL(AtomicString, SVGElementInstance)
DEFINE_GLOBAL(AtomicString, Performance)
DEFINE_GLOBAL(AtomicString, DedicatedWorkerGlobalScope)
DEFINE_GLOBAL(AtomicString, SharedWorker)
DEFINE_GLOBAL(AtomicString, SharedWorkerGlobalScope)
DEFINE_GLOBAL(AtomicString, Worker)
DEFINE_GLOBAL(AtomicString, XMLHttpRequest)
DEFINE_GLOBAL(AtomicString, XMLHttpRequestUpload)
DEFINE_GLOBAL(AtomicString, BatteryManager)
DEFINE_GLOBAL(AtomicString, MediaKeySession)
DEFINE_GLOBAL(AtomicString, FileWriter)
DEFINE_GLOBAL(AtomicString, IDBDatabase)
DEFINE_GLOBAL(AtomicString, IDBOpenDBRequest)
DEFINE_GLOBAL(AtomicString, IDBRequest)
DEFINE_GLOBAL(AtomicString, IDBTransaction)
DEFINE_GLOBAL(AtomicString, MediaSource)
DEFINE_GLOBAL(AtomicString, SourceBuffer)
DEFINE_GLOBAL(AtomicString, SourceBufferList)
DEFINE_GLOBAL(AtomicString, WebKitMediaSource)
DEFINE_GLOBAL(AtomicString, WebKitSourceBufferList)
DEFINE_GLOBAL(AtomicString, MediaStream)
DEFINE_GLOBAL(AtomicString, MediaStreamTrack)
DEFINE_GLOBAL(AtomicString, RTCDTMFSender)
DEFINE_GLOBAL(AtomicString, RTCDataChannel)
DEFINE_GLOBAL(AtomicString, RTCPeerConnection)
DEFINE_GLOBAL(AtomicString, Notification)
DEFINE_GLOBAL(AtomicString, ServiceWorkerGlobalScope)
DEFINE_GLOBAL(AtomicString, SpeechRecognition)
DEFINE_GLOBAL(AtomicString, SpeechSynthesisUtterance)
DEFINE_GLOBAL(AtomicString, MIDIAccess)
DEFINE_GLOBAL(AtomicString, MIDIInput)
DEFINE_GLOBAL(AtomicString, MIDIPort)
DEFINE_GLOBAL(AtomicString, WebSocket)
DEFINE_GLOBAL(AtomicString, DOMWindow)
DEFINE_GLOBAL(AtomicString, AudioContext)
DEFINE_GLOBAL(AtomicString, AudioNode)

void init()
{
    StringImpl* FontFaceSetImpl = StringImpl::createStatic("FontFaceSet", 11, 15992392);
    StringImpl* MessagePortImpl = StringImpl::createStatic("MessagePort", 11, 7675160);
    StringImpl* NodeImpl = StringImpl::createStatic("Node", 4, 7481749);
    StringImpl* FileReaderImpl = StringImpl::createStatic("FileReader", 10, 1122872);
    StringImpl* ScreenImpl = StringImpl::createStatic("Screen", 6, 203466);
    StringImpl* MediaControllerImpl = StringImpl::createStatic("MediaController", 15, 5103076);
    StringImpl* InputMethodContextImpl = StringImpl::createStatic("InputMethodContext", 18, 6735798);
    StringImpl* TextTrackImpl = StringImpl::createStatic("TextTrack", 9, 6141630);
    StringImpl* TextTrackCueImpl = StringImpl::createStatic("TextTrackCue", 12, 12570487);
    StringImpl* TextTrackListImpl = StringImpl::createStatic("TextTrackList", 13, 15622086);
    StringImpl* ApplicationCacheImpl = StringImpl::createStatic("ApplicationCache", 16, 14896278);
    StringImpl* EventSourceImpl = StringImpl::createStatic("EventSource", 11, 10057717);
    StringImpl* SVGElementInstanceImpl = StringImpl::createStatic("SVGElementInstance", 18, 3308448);
    StringImpl* PerformanceImpl = StringImpl::createStatic("Performance", 11, 16508941);
    StringImpl* DedicatedWorkerGlobalScopeImpl = StringImpl::createStatic("DedicatedWorkerGlobalScope", 26, 16739179);
    StringImpl* SharedWorkerImpl = StringImpl::createStatic("SharedWorker", 12, 5095866);
    StringImpl* SharedWorkerGlobalScopeImpl = StringImpl::createStatic("SharedWorkerGlobalScope", 23, 14997459);
    StringImpl* WorkerImpl = StringImpl::createStatic("Worker", 6, 14821068);
    StringImpl* XMLHttpRequestImpl = StringImpl::createStatic("XMLHttpRequest", 14, 5139743);
    StringImpl* XMLHttpRequestUploadImpl = StringImpl::createStatic("XMLHttpRequestUpload", 20, 16716896);
    StringImpl* BatteryManagerImpl = StringImpl::createStatic("BatteryManager", 14, 5049459);
    StringImpl* MediaKeySessionImpl = StringImpl::createStatic("MediaKeySession", 15, 1701501);
    StringImpl* FileWriterImpl = StringImpl::createStatic("FileWriter", 10, 15152482);
    StringImpl* IDBDatabaseImpl = StringImpl::createStatic("IDBDatabase", 11, 1126017);
    StringImpl* IDBOpenDBRequestImpl = StringImpl::createStatic("IDBOpenDBRequest", 16, 8388818);
    StringImpl* IDBRequestImpl = StringImpl::createStatic("IDBRequest", 10, 16217380);
    StringImpl* IDBTransactionImpl = StringImpl::createStatic("IDBTransaction", 14, 13326789);
    StringImpl* MediaSourceImpl = StringImpl::createStatic("MediaSource", 11, 10009800);
    StringImpl* SourceBufferImpl = StringImpl::createStatic("SourceBuffer", 12, 3244577);
    StringImpl* SourceBufferListImpl = StringImpl::createStatic("SourceBufferList", 16, 5225283);
    StringImpl* WebKitMediaSourceImpl = StringImpl::createStatic("WebKitMediaSource", 17, 4407027);
    StringImpl* WebKitSourceBufferListImpl = StringImpl::createStatic("WebKitSourceBufferList", 22, 249264);
    StringImpl* MediaStreamImpl = StringImpl::createStatic("MediaStream", 11, 5095066);
    StringImpl* MediaStreamTrackImpl = StringImpl::createStatic("MediaStreamTrack", 16, 16623886);
    StringImpl* RTCDTMFSenderImpl = StringImpl::createStatic("RTCDTMFSender", 13, 7965945);
    StringImpl* RTCDataChannelImpl = StringImpl::createStatic("RTCDataChannel", 14, 16002100);
    StringImpl* RTCPeerConnectionImpl = StringImpl::createStatic("RTCPeerConnection", 17, 11238437);
    StringImpl* NotificationImpl = StringImpl::createStatic("Notification", 12, 7262241);
    StringImpl* ServiceWorkerGlobalScopeImpl = StringImpl::createStatic("ServiceWorkerGlobalScope", 24, 16474009);
    StringImpl* SpeechRecognitionImpl = StringImpl::createStatic("SpeechRecognition", 17, 1914212);
    StringImpl* SpeechSynthesisUtteranceImpl = StringImpl::createStatic("SpeechSynthesisUtterance", 24, 12730322);
    StringImpl* MIDIAccessImpl = StringImpl::createStatic("MIDIAccess", 10, 13252796);
    StringImpl* MIDIInputImpl = StringImpl::createStatic("MIDIInput", 9, 13226940);
    StringImpl* MIDIPortImpl = StringImpl::createStatic("MIDIPort", 8, 15710269);
    StringImpl* WebSocketImpl = StringImpl::createStatic("WebSocket", 9, 5027682);
    StringImpl* DOMWindowImpl = StringImpl::createStatic("DOMWindow", 9, 5154002);
    StringImpl* AudioContextImpl = StringImpl::createStatic("AudioContext", 12, 5207992);
    StringImpl* AudioNodeImpl = StringImpl::createStatic("AudioNode", 9, 5081561);
    new ((void*)&FontFaceSet) AtomicString(FontFaceSetImpl);
    new ((void*)&MessagePort) AtomicString(MessagePortImpl);
    new ((void*)&Node) AtomicString(NodeImpl);
    new ((void*)&FileReader) AtomicString(FileReaderImpl);
    new ((void*)&Screen) AtomicString(ScreenImpl);
    new ((void*)&MediaController) AtomicString(MediaControllerImpl);
    new ((void*)&InputMethodContext) AtomicString(InputMethodContextImpl);
    new ((void*)&TextTrack) AtomicString(TextTrackImpl);
    new ((void*)&TextTrackCue) AtomicString(TextTrackCueImpl);
    new ((void*)&TextTrackList) AtomicString(TextTrackListImpl);
    new ((void*)&ApplicationCache) AtomicString(ApplicationCacheImpl);
    new ((void*)&EventSource) AtomicString(EventSourceImpl);
    new ((void*)&SVGElementInstance) AtomicString(SVGElementInstanceImpl);
    new ((void*)&Performance) AtomicString(PerformanceImpl);
    new ((void*)&DedicatedWorkerGlobalScope) AtomicString(DedicatedWorkerGlobalScopeImpl);
    new ((void*)&SharedWorker) AtomicString(SharedWorkerImpl);
    new ((void*)&SharedWorkerGlobalScope) AtomicString(SharedWorkerGlobalScopeImpl);
    new ((void*)&Worker) AtomicString(WorkerImpl);
    new ((void*)&XMLHttpRequest) AtomicString(XMLHttpRequestImpl);
    new ((void*)&XMLHttpRequestUpload) AtomicString(XMLHttpRequestUploadImpl);
    new ((void*)&BatteryManager) AtomicString(BatteryManagerImpl);
    new ((void*)&MediaKeySession) AtomicString(MediaKeySessionImpl);
    new ((void*)&FileWriter) AtomicString(FileWriterImpl);
    new ((void*)&IDBDatabase) AtomicString(IDBDatabaseImpl);
    new ((void*)&IDBOpenDBRequest) AtomicString(IDBOpenDBRequestImpl);
    new ((void*)&IDBRequest) AtomicString(IDBRequestImpl);
    new ((void*)&IDBTransaction) AtomicString(IDBTransactionImpl);
    new ((void*)&MediaSource) AtomicString(MediaSourceImpl);
    new ((void*)&SourceBuffer) AtomicString(SourceBufferImpl);
    new ((void*)&SourceBufferList) AtomicString(SourceBufferListImpl);
    new ((void*)&WebKitMediaSource) AtomicString(WebKitMediaSourceImpl);
    new ((void*)&WebKitSourceBufferList) AtomicString(WebKitSourceBufferListImpl);
    new ((void*)&MediaStream) AtomicString(MediaStreamImpl);
    new ((void*)&MediaStreamTrack) AtomicString(MediaStreamTrackImpl);
    new ((void*)&RTCDTMFSender) AtomicString(RTCDTMFSenderImpl);
    new ((void*)&RTCDataChannel) AtomicString(RTCDataChannelImpl);
    new ((void*)&RTCPeerConnection) AtomicString(RTCPeerConnectionImpl);
    new ((void*)&Notification) AtomicString(NotificationImpl);
    new ((void*)&ServiceWorkerGlobalScope) AtomicString(ServiceWorkerGlobalScopeImpl);
    new ((void*)&SpeechRecognition) AtomicString(SpeechRecognitionImpl);
    new ((void*)&SpeechSynthesisUtterance) AtomicString(SpeechSynthesisUtteranceImpl);
    new ((void*)&MIDIAccess) AtomicString(MIDIAccessImpl);
    new ((void*)&MIDIInput) AtomicString(MIDIInputImpl);
    new ((void*)&MIDIPort) AtomicString(MIDIPortImpl);
    new ((void*)&WebSocket) AtomicString(WebSocketImpl);
    new ((void*)&DOMWindow) AtomicString(DOMWindowImpl);
    new ((void*)&AudioContext) AtomicString(AudioContextImpl);
    new ((void*)&AudioNode) AtomicString(AudioNodeImpl);
}

} // EventTarget
} // WebCore
