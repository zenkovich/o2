#ifndef APPLICATION_WIN_H
#define APPLICATION_WIN_H

#include <Windows.h>

#include "public.h"
#include "../application_base_interface.h"

OPEN_O2_NAMESPACE

class cTimer;

/** Windows applacation. 
  * Contains window handle, options, timer
  */
class cApplication:public cApplicationBaseInterface
{
protected:
	friend class grRenderSystem;
	friend class cDeviceInfo;

	HWND         mHWnd;                /**< Window handle. */
	unsigned int mWndStyle;            /**< Window styles data. */
		         					   
	bool         mWindowed;            /**< True if app in windowed mode, false if in fullscreen mode. */
	bool         mWindowResizible;     /**< True, if window can be sized by user. */
	vec2i        mWindowedSize;        /**< Size of window. */
	vec2i        mWindowedPos;         /**< Position of window. */
									   
	bool         mAutoAjustByScreen;   /**< True, if app in windowed mode and window ajusting by available screen space. */
	vec2f        mAutoAjustScale;      /**< Auto ajust scale. */
									   
	std::string  mWndCaption;          /**< Window caption. */
									   
	bool         mActive;              /**< True, if window is active. */
									   
	cTimer*      mTimer;               /**< Timer for detecting delta time for update. */

	static cApplication* mApplication; /**< Static ptr for wndProc func. */

public:
	cApplication();
	~cApplication();

	/** Launching application cycle. */
	void launch();

	/** Called on updating. */
	void onUpdate(float dt);

	/** Called on drawing. */ 
	void onDraw();

	/** Sets option for application. */
	void setOption(cApplicationOption::type option, ...);

	/** Getting option. */
	void getOption(cApplicationOption::type option, ...);

	/** Called on message processing. */
	virtual void processMessage(cApplacationMessage::type message);

protected:
	/** Initializing window. */
	void initializeWindow();

	/** Sets window parametres as windowed mode */
	void setWindowed();

	/** Sets window parametres as fullscreen mode. */
	void setFullscreen();

	/** Sets window parametres as adjusting by screen mode. */
	void autoAjustByScreenSpace();

	/** Reset window by window parametres. */
	void resetWnd();

	/** Window proc func. */
	static LRESULT wndProc(HWND wnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void draw();
};

CLOSE_O2_NAMESPACE

#endif //APPLICATION_WIN_H