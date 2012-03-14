#pragma once

#include "ofMain.h"
#include "Awesomium/awesomium_capi.h"

#include <iostream>
#include <windows.h>


class WebRender
{
public:
	#define WEB_WIDTH   1440
	#define WEB_HEIGHT  900
#define URL "http://awwapp.com/draw.html#wqHCqkh2S8"
	#define SCROLLBAR_CSS   "::-webkit-scrollbar { width: 0px; height: 0px; } "


	int 				webWidth_s;
	int 				webHeight_s;
	int                 webWidth_l;
	int                 webHeight_l;

	ofImage             bgSmall, bgBig;
	int                 bgX_small, bgY_small;
	int                 bgX_small_current, bgY_small_current;
	int                 bgX_large, bgY_large;
	
	bool                zoomIn;
	bool                close;
	float               scale;



	
	ofTexture		texColor;
	unsigned char * pixels;
	unsigned char * pixelBuffer;
	unsigned char * rightPixelBuffer;
	const awe_renderbuffer* renderBuffer;
	const unsigned char * rPixelBuffer;
	awe_webview* webView;
	float * dest_buffer;
	
	//webRender function
	void setupWebcore();
	void updateWebcore();
	void drawWebcore(int scale);
	void convertBGRAToRGB(unsigned char* bgraBuffer, unsigned char* rgbBuffer, int width, int height);
	void injectMouseMoved(int x, int y);
	void injectMouseDragged(int x, int y);
	void injectMousePressed();
	void injectMouseReleased();

	//UI fuction
	void updateWebcoreCoord(int _xSmall, int _ySmall, int _xLarge, int _yLarge);
	void drawBg();
	void zoomDetect(int mouseX, int mouseY);
};

