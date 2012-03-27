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
    void newFrame(unsigned char* rgb = NULL, unsigned char * raw_depth = NULL, time_t = NULL, int headx = NULL, int heady = NULL, int headz = NULL, int leftshoulderx = NULL, int leftshouldery = NULL, int rightshoulderx = NULL, int rightshouldery = NULL, int lefthandx = NULL, int lefthandy = NULL, int righthandx = NULL, int righthandy = NULL, USHORT* depthbuff = NULL);
	void storeInt(int frameNo = NULL, bool endOfRecording = false);//note gestures are written in pairs of 10, and number of Frames comes at the end
	
	void close();
	bool isOpened();

private:
	FILE * f;

	FILE * fInt;
	int maxNoIndices; //the maximum number of gesture or audio indices accepted. It will ignore anything given to it past this number
	int indexCount; //counts number of recorded gesture or audio indices (to make sure each has maxNoIndices amount)
	int negOne;
	//bool nFramesRecorded;//true if we've already recorded nFrames in fInt (used to make fps enter fInt without being considered a gesture)
};
