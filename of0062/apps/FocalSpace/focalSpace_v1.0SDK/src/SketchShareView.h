#pragma once
#include "ofMain.h"

class SketchShareView
{
public:
	
	ofVideoGrabber 		vidGrabber;
	ofTexture			videoTexture;
	
	int 				camWidth_s;
	int 				camHeight_s;
	int                 camWidth_l;
	int                 camHeight_l;
	float               scale;

	ofImage             bgSmall, bgBig;
	int                 bgX_small, bgY_small;
	int                 bgX_small_current, bgY_small_current;
	int                 bgX_large, bgY_large;
	
	bool                zoomIn;
	bool                close;
	//int                 isClosed;
	//SketchShareView();
	
	void initViewer();
	void update(int _xSmall, int _ySmall, int _xLarge, int _yLarge);
	void drawBg();
	void drawVideo();
	void zoomDetect(int mouseX, int mouseY);
	void closeDetect(int mouseX, int mouseY);
	void setSmallViewOrigin(int scale);
	//void switchMode(int x);
};

