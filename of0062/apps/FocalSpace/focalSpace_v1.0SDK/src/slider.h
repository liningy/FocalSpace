#ifndef _OF_SLIDER
#define _OF_SLIDER

#include <string>

class slider{

public:
	//data
	int originX,originY;
	int sWidth;
	int sHeight;
	float value;
	int sliderPosX,sliderPosY;
	ofTrueTypeFont font;
	ofImage img;

	slider(int x, int y, int sWidth,int sHeight);

	void drawSlider(float startFrom, float endTo);
	void drawSliderasPic(float startFrom, float endTo);
	void getSliderPosX(int _mouseX, int _mouseY);
	pair<bool,float> sliderPressed(int x, int y);
	void setSliderPosX(int newSliderPosX);
};

#endif
