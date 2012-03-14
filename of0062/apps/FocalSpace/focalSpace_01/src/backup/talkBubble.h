#ifndef _OF_BUBBLE		// by using this if statement you prevent the class to be called more than once - in other words what we're saying here is 
#define _OF_BUBBLE		//if the class has NOT been defined then define it

#include "grabVideo.h"

class talkBubble {
	
	
	
public:   
	
	void updateTimer();
	void draw();
	void updatePosition(int _x, int _y);
	void updateAttributes(string name);
	void setPosition();
	void timer();
	void drawName(int xPosition, int yPosition);
	void drawElapsedTime(int xPosition, int yPosition);
	void drawDate(int xPosition, int yPosition);

	talkBubble(float x, float y,string message, int talkTime);
	
	ofImage	backgroundImage;
	ofImage	backgroundImage_r;
	ofTrueTypeFont font;
	
	float x;
	float y;
	int z;
	int scale;
	int orientation;
	
	int min, sec, secsInMins;
	string cleanSeconds;
	
	int absoluteX, absoluteY, absoluteZ;
	int bubbleState; // Determines state of bubble (has it been set?)

	int active;
	string name;
	int talkTime;
	string message;
	string nameTx;

	int totalTime, startTime, deltaTime;
	//string name;
	
	

private:
	
	
};						//don't forget the semicolon in the end of the class definition

#endif 


