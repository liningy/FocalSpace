#include "ofMain.h"
#include "talkBubble.h"

talkBubble::talkBubble(float _x, float _y, string _name, int _talkTime)
{
	x = _x;
	y = _y;
	name = _name;
	talkTime = _talkTime;
	
	//Show bubble pointing left
	backgroundImage.loadImage("images/bubble.png");
	
	//Show bubble point right
	backgroundImage_r.loadImage("images/bubble_2.png");
	
	font.loadFont("DIN.otf",12); //40 = Font size
	
	//Adjust this value to change the relative size of the bubble
	scale = .5;
	
	totalTime=0;
	active=false;
	
	bubbleState = 0; // Default 0, no pos parameters set
	min = 0;
	sec = 0;
	nameTx="";
}

void talkBubble::updateTimer(){	
	if(active)
	{
		timer();
	}
	else {
		startTime = 0;
	}
	
}

void talkBubble::draw(){

		ofEnableAlphaBlending();
		if (active == 1) {
			ofSetColor(255,255,255,255);
		} else {
			ofSetColor(255,255,255,25);
		}
		backgroundImage.draw(x, y);
	
		
		if (active == 1) {
			ofSetColor(0,0,0,255);
		} else {
			ofSetColor(255,255,255,25);
		}
		font.drawString(name,x + (backgroundImage.width/2)-50, y + (backgroundImage.height/2)-13);

		
		//FORMAT TALK TIME	
		if (totalTime > 59) {
			min = totalTime/60;
		}
		secsInMins = 60 * min;
		sec = totalTime - secsInMins;
		
		if(sec < 10)
		{
			cleanSeconds = "0" + ofToString(sec);
		} else {
			cleanSeconds = ofToString(sec);
		}

		
		if (active == 1) {
			ofSetColor(255,255,255,255);
		} else {
			ofSetColor(255,255,255,25);
		}
		font.drawString(ofToString(min) + ":" + cleanSeconds,x + (backgroundImage.width/2)-50, y + (backgroundImage.height/2)+20);
		ofDisableAlphaBlending();

		ofSetColor(255,255,255,255);		
}


void talkBubble::updateAttributes(string _name)
{
	name = _name;	
}

void talkBubble::updatePosition(int _x, int _y)
{
	x=_x;
	y=_y;
}

void talkBubble::setPosition()
{
	switch (bubbleState) 
	{
		case 0:
			bubbleState = 1;
		break;
		case 1:
			bubbleState = 2;
		break;
		case 2:
			orientation = 1;
			bubbleState = 3;
		break;
		case 3:
			orientation = 0;
			bubbleState = 0;
		break;

		default:
		//
		break;
	}
}

void talkBubble::timer(){
	if(startTime == 0)
		startTime=ofGetElapsedTimeMillis(); 
	
	deltaTime = ofGetElapsedTimeMillis() - startTime;
	
	if(deltaTime > 1000){
		totalTime++;
		deltaTime = 0;
		startTime = ofGetElapsedTimeMillis();
	}
}

void talkBubble::drawName(int xPosition, int yPosition){
	font.drawString(name,xPosition, yPosition);
}

void talkBubble::drawElapsedTime(int xPosition, int yPosition){
	font.drawString(ofToString((int)(ofGetElapsedTimef())/60) + "'" + " " + ofToString((int)(ofGetElapsedTimef())%60) + "''",xPosition, yPosition);
}

void talkBubble::drawDate(int xPosition, int yPosition){
	time_t rawtime;
	time ( &rawtime );
	font.drawString(ctime (&rawtime), xPosition, yPosition);
}