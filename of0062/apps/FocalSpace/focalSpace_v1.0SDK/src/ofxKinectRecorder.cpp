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
	maxNoIndices = 10;
	indexCount = 0;
	negOne = -1;//default value to be written on unused index positions
	//nFramesRecorded = false;
}

void ofxKinectRecorder::newFrame(unsigned char* rgb, unsigned char * raw_depth, time_t rawtime, int headx, int heady, int headz, int leftshoulderx, int leftshouldery, int rightshoulderx, int rightshouldery, int lefthandx, int lefthandy, int righthandx, int righthandy, USHORT* depthbuff) {
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
	fwrite(depthbuff,640*480*2,1,f);

}
/* endOfRecording is true when the video has ended (and we're out of gestures. future - it should also be true when switching b/n gestures)
*/
void ofxKinectRecorder::storeInt(int frameNo, bool endOfRecording){
	if (!fInt) return;
	if(frameNo || frameNo == 0){
		if (endOfRecording){
			while(indexCount<maxNoIndices){//if end of frame but still haven't recorded maxNoIndices, then record a bunch of negative ones
				fwrite(&negOne,sizeof(int),1,fInt);
				indexCount++;
			}
			indexCount = 0;
			fwrite(&frameNo,sizeof(int),1,fInt);
			//nFramesRecorded = true;
		}
		//else if (nFramesRecorded){
		//	fwrite(&frameNo,sizeof(int),1,fInt);
		//	nFramesRecorded = false;//resets nFramesRecorded
		//}
		else if (indexCount<maxNoIndices){
				fwrite(&frameNo,sizeof(int),1,fInt);
				indexCount++;
		}
		else{
			printf("error in record information storage");
		}
	}
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
	maxNoIndices = 10;
	indexCount = 0;
	int negOne = -1;
}

bool ofxKinectRecorder::isOpened(){
	//return f;
	return (f || fInt); //(not sure if necessary). Also, think this is better than return f, but i'm not sure
}
