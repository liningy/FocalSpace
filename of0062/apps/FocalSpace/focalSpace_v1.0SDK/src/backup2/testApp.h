//#pragma once

#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxNetwork.h"
#include "ofxFBOTexture.h"
#include "ofxShader.h"
#include "shaderBlur.h"

#include "grabVideo.h"
#include "conference.h"
#include "button.h"
#include "slider.h"
#include "talkBubble.h"
#include "SketchShareView.h"
#include <ctime>
#include "webRender.h"
#include "gesture.h"
#include "recordAndReplay.h"
#include <sstream>
#include "ofSoundStream.h"

//const int                     RENDER_WIDTH = 540;
//const int                     X_SHIFTS=540;
//const int                     RENDER_HEIGHT= 405;
//const float                   SCALE=0.844;

//const int DEPTH_THRESHOLD = 350;
class testApp : public ofBaseApp{
	
	public:
		char eventString[255]; // for printing the location of the mouse (for debugging)

		void setup();
		void update();
		void draw();
		void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);

		/////Gesture Checks
		bool rightHandRaised(); //checks whether the right hand of the skeleton we are tracking is currently raised (returns true if it is)



	private:
		KinectGrabber   g_kinectGrabber;
	
		ofTexture		texColorAlpha;
		ofTexture		texGray;
		ofTexture		texFocus;
		ofTexture		texBlur;
		ofTexture		texMobile;
		ofTexture		texMobileBlur;

		//ofxCvColorImage	blurImg;		
		unsigned char 	* colorAlphaPixels;
		unsigned char	* grayPixels;
		unsigned char	* focusPixels;
		unsigned char	* blurPixels;
		unsigned char	* mobilePixels;
		unsigned char	* mobileBlurPixels;

		int headPositionX, headPositionY, headPositionZ;
		int neckPositionX, neckPositionY, leftShoulderX, leftShoulderY, rightShoulderX, rightShoulderY,leftHandPX, leftHandPY, rightHandPX, rightHandPY;
		bool peopleSelectedbyMouse;
		
		shaderBlur      blur;
		float           maskValue;
		
		//GUI
		ofColor         bgColor;
		int             thresh;
		bool            activeFocus;  //to deside the active or inActive focus

		
		//gui buttons
		button**        buttons;
		int             nButtons;
		bool            buttonPressed[9]; //the number of the boolean should equal to nButtons
		slider**        sliders;
		int             nSliders;
		float           scaleParam; //for Zoom slider
		ofImage         header;
		ofImage         shadow;
		
		//talk bubbles
		talkBubble** talkBubbles;  
		int nBubbles; 

		//sketch viewer
		SketchShareView sketchShareView;
		bool            firstTimeSketchTrigger;
		
		//webRender
		WebRender webRender;

		//lock to a tracked person in manual mode
		bool            confirmSelection;  //confirm selection by pressing a button
		int             lockedPersonID;    

		//the activated skeleton
		int             closestID;

		//gesture
		Gesture         gesture;
		
		
		//network stuff
		ofxUDPManager udpConnection;
		ofTrueTypeFont  mono;
		string              message;

		//for record and replay
		recordAndReplay	recAndRep;
		/////Time
		time_t rawTime; //current time
		struct tm * timeinfo; //intermediate step in printing rawTime as a calendar date

		//others
		int              translateMouseX;
		int              translateMouseY;
};

#endif
	