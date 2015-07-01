/*
 * Copyright (C) 2011 Apple Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef MediaControllerInterface_h
#define MediaControllerInterface_h

namespace WebCore {

class ExceptionState;

class MediaControllerInterface {
public:
    virtual ~MediaControllerInterface() { };

    // MediaControlElements:
    virtual double duration() const = 0;
    virtual double currentTime() const = 0;
    virtual void setCurrentTime(double, ExceptionState&) = 0;

    virtual bool paused() const = 0;
    virtual void play() = 0;
    virtual void pause() = 0;

    virtual double volume() const = 0;
    virtual void setVolume(double, ExceptionState&) = 0;

    virtual bool muted() const = 0;
    virtual void setMuted(bool) = 0;

    virtual void enterFullscreen() = 0;

    virtual bool hasAudio() const = 0;
    virtual bool hasVideo() const = 0;
    virtual bool hasClosedCaptions() const = 0;
    virtual void setClosedCaptionsVisible(bool) = 0;
    virtual bool closedCaptionsVisible() const = 0;

    virtual void beginScrubbing() = 0;
    virtual void endScrubbing() = 0;

    virtual bool canPlay() const = 0;
};

}

#endif
