/*
 * KinectRecorder.h
 *
 *  Created on: 03/01/2011
 *      Author: arturo
 */

#pragma once

#include "ofMain.h"

class ofxKinectRecorder {
public:
	ofxKinectRecorder();
	virtual ~ofxKinectRecorder();
	void init(const string & filename, const string & filenameInt);
    void newFrame(unsigned char* rgb = NULL, unsigned char * raw_depth = NULL, time_t = NULL, int headx = NULL, int heady = NULL, int headz = NULL, 
					int leftshoulderx = NULL, int leftshouldery = NULL, int rightshoulderx = NULL, int rightshouldery = NULL, int lefthandx = NULL, 
					int lefthandy = NULL, int righthandx = NULL, int righthandy = NULL, int faceOneID = NULL, int faceOneX = NULL, int faceOneY = NULL, 
					int faceOneZ = NULL, int faceTwoID = NULL, int faceTwoX = NULL, int faceTwoY = NULL, int faceTwoZ = NULL, USHORT* depthbuff = NULL, 
					unsigned char* blurPixels = NULL, int closID = NULL);
	void storeSideInfo(int genCat = NULL, int specCat = NULL, int frameNo = NULL);//keeps track of gestures and stuff
	/*
	Guide:
	genCat 0 = number of frames
		specCat 0 = number of frames
	genCat 1 = gestures
		specCat 0 = thumbs up
	genCat 2 = faces
		specCat 0 = person ID 0
		specCat 1 = person ID 1
		specCat 2 = person ID 2
		specCat 3 = person ID 3
		specCat 4 = person ID 4
		specCat 5 = person ID 5
	*/
	void close();

	bool isOpened();

private:
	FILE * f;

	FILE * fInt;
	//bool nFramesRecorded;//true if we've already recorded nFrames in fInt (used to make fps enter fInt without being considered a gesture)
};
