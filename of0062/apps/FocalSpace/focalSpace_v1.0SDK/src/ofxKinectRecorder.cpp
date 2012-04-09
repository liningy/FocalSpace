/*
 * KinectRecorder.cpp
 *
 *  Created on: 03/01/2011
 *      Author: arturo
 */

#include "ofxKinectRecorder.h"

ofxKinectRecorder::ofxKinectRecorder() {
	f=NULL;
	fInt=NULL;
}

ofxKinectRecorder::~ofxKinectRecorder() {
	// TODO Auto-generated destructor stub
}

void ofxKinectRecorder::init(const string & filename, const string & filenameInt){
	
	f = fopen(ofToDataPath(filename).c_str(),"wb");

	fInt = fopen(ofToDataPath(filenameInt).c_str(),"wb");
	
	//nFramesRecorded = false;
}

void ofxKinectRecorder::newFrame(unsigned char* rgb, unsigned char * raw_depth, time_t rawtime, int headx, int heady, int headz, int leftshoulderx, int leftshouldery,
								int rightshoulderx, int rightshouldery, int lefthandx, int lefthandy, int righthandx, int righthandy, int faceOneID, int faceOneX, 
								int faceOneY, int faceOneZ, int faceTwoID, int faceTwoX, int faceTwoY, int faceTwoZ,  USHORT* depthbuff, unsigned char* blurPixels,
								int closID) {
	if(!f ||!raw_depth) return;

	if(rgb != NULL) {
		fwrite(rgb,640*480*4,1,f);
	}
	fwrite(raw_depth,640*480*4,1,f);

	fwrite(&rawtime,sizeof(time_t),1,f);
	fwrite(&headx,sizeof(int),1,f);
	fwrite(&heady,sizeof(int),1,f);
	fwrite(&headz,sizeof(int),1,f);
	fwrite(&leftshoulderx,sizeof(int),1,f);
	fwrite(&leftshouldery,sizeof(int),1,f);
	fwrite(&rightshoulderx,sizeof(int),1,f);
	fwrite(&rightshouldery,sizeof(int),1,f);
	fwrite(&lefthandx,sizeof(int),1,f);
	fwrite(&lefthandy,sizeof(int),1,f);
	fwrite(&righthandx,sizeof(int),1,f);
	fwrite(&righthandy,sizeof(int),1,f);

	fwrite(&faceOneID,sizeof(int),1,f);
	fwrite(&faceOneX,sizeof(int),1,f);
	fwrite(&faceOneY,sizeof(int),1,f);
	fwrite(&faceOneZ,sizeof(int),1,f);
	fwrite(&faceTwoID,sizeof(int),1,f);
	fwrite(&faceTwoX,sizeof(int),1,f);
	fwrite(&faceTwoY,sizeof(int),1,f);
	fwrite(&faceTwoZ,sizeof(int),1,f);

	fwrite(depthbuff,640*480*2,1,f);
	fwrite(blurPixels,640*480*4,1,f);

	fwrite(&closID,sizeof(int),1,f);
}

void ofxKinectRecorder::storeSideInfo(int genCat, int specCat, int frameNo){
	fwrite(&genCat,sizeof(int),1,fInt);
	fwrite(&specCat,sizeof(int),1,fInt);
	fwrite(&frameNo,sizeof(int),1,fInt);
}
void ofxKinectRecorder::close(){
	if(f){
		fclose(f);
		f=0;
	}
	if(fInt){
		fclose(fInt);
		fInt=0;
	}
}

bool ofxKinectRecorder::isOpened(){
	//return f;
	return (f || fInt); //(not sure if necessary). Also, think this is better than return f, but i'm not sure
}
