/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef __CC_EGLVIEW_WIN32_H__
#define __CC_EGLVIEW_WIN32_H__

#include "windows.h"
#include "platformtype.h"
#include "ccTypes.h"

typedef enum 
{
	/// sets a 2D projection (orthogonal projection)
	kCCDirectorProjection2D,

	/// sets a 3D projection with a fovy=60, znear=0.5f and zfar=1500.
	kCCDirectorProjection3D,

	/// Default projection is 3D projection
	kCCDirectorProjectionDefault = kCCDirectorProjection3D,
} ccDirectorProjection;

enum ResolutionPolicy
{
	// The entire application is visible in the specified area without trying to preserve the original aspect ratio.
	// Distortion can occur, and the application may appear stretched or compressed.
	kResolutionExactFit,
	// The entire application fills the specified area, without distortion but possibly with some cropping,
	// while maintaining the original aspect ratio of the application.
	kResolutionNoBorder,
	// The entire application is visible in the specified area without distortion while maintaining the original
	// aspect ratio of the application. Borders can appear on two sides of the application.
	kResolutionShowAll,
	// The application takes the height of the design resolution size and modifies the width of the internal
	// canvas so that it fits the aspect ratio of the device
	// no distortion will occur however you must make sure your application works on different
	// aspect ratios
	kResolutionFixedHeight,
	// The application takes the width of the design resolution size and modifies the height of the internal
	// canvas so that it fits the aspect ratio of the device
	// no distortion will occur however you must make sure your application works on different
	// aspect ratios
	kResolutionFixedWidth,

	kResolutionUnKnown,
};


typedef LRESULT (*CUSTOM_WND_PROC)(UINT message, WPARAM wParam, LPARAM lParam, BOOL* pProcessed);

class CCEGL;

class CC_DLL EGLView //: public CCEGLViewProtocol
{
public:
    EGLView();
    virtual ~EGLView();

    /* override functions */
    virtual bool isOpenGLReady();
    virtual void end();
    virtual void swapBuffers();
    virtual void setFrameSize(float width, float height);
    virtual void setIMEKeyboardState(bool bOpen);

    void setMenuResource(LPCWSTR menu);
    void setWndProc(CUSTOM_WND_PROC proc);

	bool CreateGL(HWND hwnd );

	//void MsgLBtnDown( UINT nFlags, long x, long y );
	//void MsgLBtnUp( UINT nFlags, long x, long y );
	//void MsgMouseMove( 

	 void setGLDefaultValues(void);
	 void setAlphaBlending(bool bOn);
	 void setDepthTest(bool bOn);
	 void setProjection(ccDirectorProjection kProjection);

 virtual bool Create( HWND hwnd );
private:
   
	
    bool initGL();
    void destroyGL();
public:
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

    // win32 platform function
    HWND getHWnd();
    void resize(int width, int height);
    /* 
     * Set zoom factor for frame. This method is for debugging big resolution (e.g.new ipad) app on desktop.
     */
    void setFrameZoomFactor(float fZoomFactor);
	float getFrameZoomFactor();
    void centerWindow();

    typedef void (*LPFN_ACCELEROMETER_KEYHOOK)( UINT message,WPARAM wParam, LPARAM lParam );

    virtual void setViewPortInPoints(float x , float y , float w , float h);
    virtual void setScissorInPoints(float x , float y , float w , float h);
    
    // static function
    /**
    @brief    get the shared main open gl window
    */
    static EGLView* sharedOpenGLView();
private:
	ccDirectorProjection m_eProjection;
    bool m_bCaptured;
    HWND m_hWnd;
    HDC  m_hDC;
    HGLRC m_hRC;

    LPCWSTR m_menu;
    CUSTOM_WND_PROC m_wndproc;

    float m_fFrameZoomFactor;

private:

	// real screen size
	CCSize m_obScreenSize;
	// resolution size, it is the size appropriate for the app resources.
	CCSize m_obDesignResolutionSize;
	// the view port size
	CCRect m_obViewPortRect;
	// the view name
	char   m_szViewName[50];

	float  m_fScaleX;
	float  m_fScaleY;
	ResolutionPolicy m_eResolutionPolicy;
};

#endif    // end of __CC_EGLVIEW_WIN32_H__
