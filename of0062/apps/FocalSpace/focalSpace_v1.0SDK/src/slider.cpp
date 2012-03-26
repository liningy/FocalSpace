#include "ofMain.h"
#include "slider.h"

slider::slider(int x, int y, int _sWidth, int _sHeight)
{
	originX=x;
	originY=y;
	sWidth=_sWidth;
	sHeight =_sHeight;
	sliderPosX=x;
	sliderPosY=y;
	img.loadImage("images/button.png");
}

//this function has to be used under void mousePressed() function
void slider::getSliderPosX(int _mouseX, int _mouseY){
	if((_mouseX>=originX && _mouseX<originX+sWidth) && (_mouseY>=originY-5 && _mouseY<originY+20))
		sliderPosX=_mouseX;
}

void slider::drawSlider(float startFrom, float endTo){
	//ofFill();
    //ofSetColor(0xCCCCCC);
	//ofRect(originX,originY,sWidth,5);
	//ofSetColor(0xFF9933);
	//ofTriangle(sliderPosX,sliderPosY+5,sliderPosX+16,sliderPosY+5,sliderPosX+8,sliderPosY-8);
	img.draw(sliderPosX,sliderPosY);

	value=ofMap(sliderPosX,originX,originX+sWidth,startFrom,endTo);
	//printf("value= %d \n", value);
}

void slider::drawSliderasPic(float startFrom, float endTo){
	//ofFill();
    //ofSetColor(0xCCCCCC);
	ofRect(originX+sWidth,originY,3,sHeight);
	//ofSetColor(0xFF9933);
	//ofTriangle(sliderPosX,sliderPosY+5,sliderPosX+16,sliderPosY+5,sliderPosX+8,sliderPosY-8);
	//img.draw(sliderPosX,sliderPosY);
	ofRect(originX,originY,sliderPosX,sHeight);

	value=ofMap(sliderPosX,originX,originX+sWidth,startFrom,endTo);
	//printf("value= %d \n", value);
}

/*returns a pair<bool,float>. 
The bool is whether the slider was clicked,
The float indicates how far along the slider, the slider was clicked.
The float is returned as a percentage of the slider's width (is 0 if not clicked)*/
pair<bool,float> slider::sliderPressed(int x, int y){
		if((x>=originX && x<originX+sWidth) && (y>=originY && y<originY+sHeight)){//note: unlike button there is no trigger here
			return make_pair(true,(float)(x-originX)/(float)(sWidth));
		}else{
			return make_pair(false,0);
		}
}

void slider::setSliderPosX(int newSliderPosX){
	sliderPosX=newSliderPosX;
}


