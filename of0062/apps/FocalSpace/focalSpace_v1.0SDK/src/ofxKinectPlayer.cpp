/*
 * KinectPlayer.cpp
 *
 *  Created on: 14/12/2010
 *      Author: arturo
 */

#include "ofxKinectPlayer.h"

#ifdef _MSC_VER
	#define uint16_t unsigned short
#endif

//--------------------------------------------------------------------
ofxKinectPlayer::ofxKinectPlayer(){
	f = 0;
	filename = "";
	fInt = 0;
	filenameInt = "";
	buf = 0;
	rgb = 0;
	depthbuff = 0;
	blurPixels = 0;
	bUseTexture = true;
	fps = 30;//default is 30. it can be set again in updateSideInfo with some more code if desired
	//fpsOffset = 8.2;
}

//-----------------------------------------------------------
ofxKinectPlayer::~ofxKinectPlayer() {
	close();

	if(buf) delete[] buf;
	if(rgb) delete[] rgb;
	if(depthbuff) delete[] depthbuff;
	if(blurPixels) delete[] blurPixels;

	//calibration.clear();
	depthTex.clear();
	videoTex.clear();
}

//-----------------------------------------------------------
void ofxKinectPlayer::setUseTexture(bool _bUseTexture){
	bUseTexture = _bUseTexture;
}

//-----------------------------------------------------------
void ofxKinectPlayer::setup(const string & file, const string & fileInt, bool video){
	f = fopen(ofToDataPath(file).c_str(), "rb");
	filename = file;
	fInt = fopen(ofToDataPath(fileInt).c_str(), "rb");
	filenameInt = fileInt;

	size = 2*(640*480*4)+20*sizeof(int)+sizeof(time_t)+ (640*480*2) + (640*480*4);      //Note, edit this everytime you change the size of a frame to be recorded!!
	
	//if(!buf) buf 		= new uint16_t[640*480];
	if(!buf) buf = new unsigned char[640*480*4];
	if(!rgb) rgb = new unsigned char[640*480*4];
	memset(rgb,255,640*480*4);
	if(!depthbuff) depthbuff = new USHORT[640*480*2];//was 640*480*4
	memset(depthbuff,255,640*480*4);
	if(!blurPixels) blurPixels = new unsigned char[640*480*4];
	memset(depthbuff,255,640*480*4);
	//pixels.setFromExternalPixels(rgb, 640, 480, OF_IMAGE_COLOR);
	if(!depthTex.bAllocated() && bUseTexture)
		depthTex.allocate(640,480,GL_LUMINANCE);
	if(!videoTex.bAllocated() && bUseTexture)
		videoTex.allocate(640,480,GL_RGB);
	bVideo = video;
	//calibration.init(3);
	lastFrameTime = ofGetElapsedTimeMillis();
}

//-----------------------------------------------------------
void ofxKinectPlayer::update(){
}
pair<unsigned char *,pair<unsigned char *,pair<time_t,pair<int,pair<int,pair<int,pair<int,
			pair<int,pair<int,pair<int,pair<int,pair<int,pair<int,pair<int,pair<int, pair<int,
			pair<int,pair<int,pair<int,pair<int,pair<int,pair<int, pair<USHORT*, pair<unsigned char *,int>>>>>>>>>>>>>>>>>>>>>>>> ofxKinectPlayer::updatea(){
	if(!f){
		printf("error!");
	}
	while((ofGetElapsedTimeMillis()-lastFrameTime)<(1000./float(fps)))
	{}//loops until fps delay is achieved
	lastFrameTime = ofGetElapsedTimeMillis();
	if(bVideo)
		fread(rgb,640*480*4,1,f);
	fread(buf,640*480*4,1,f);

	fread(&rawTime,sizeof(time_t),1,f);
	fread(&headx,sizeof(int),1,f);
	fread(&heady,sizeof(int),1,f);
	fread(&headz,sizeof(int),1,f);
	fread(&leftshoulderx,sizeof(int),1,f);
	fread(&leftshouldery,sizeof(int),1,f);
	fread(&rightshoulderx,sizeof(int),1,f);
	fread(&rightshouldery,sizeof(int),1,f);
	fread(&lefthandx,sizeof(int),1,f);
	fread(&lefthandy,sizeof(int),1,f);
	fread(&righthandx,sizeof(int),1,f);
	fread(&righthandy,sizeof(int),1,f);

	fread(&faceOneID,sizeof(int),1,f);
	fread(&faceOneX,sizeof(int),1,f);
	fread(&faceOneY,sizeof(int),1,f);
	fread(&faceOneZ,sizeof(int),1,f);
	fread(&faceTwoID,sizeof(int),1,f);
	fread(&faceTwoX,sizeof(int),1,f);
	fread(&faceTwoY,sizeof(int),1,f);
	fread(&faceTwoZ,sizeof(int),1,f);

	fread(depthbuff,640*480*2,1,f);
	fread(blurPixels,640*480*4,1,f);

	fread(&closID,sizeof(int),1,f);
	// loop?
	if(bLoop && std::feof(f) > 0) {
		f = fopen(ofToDataPath(filename).c_str(), "rb");
	}

	//calibration.update(buf);
	if(bUseTexture){
		//depthTex.loadData(calibration.getDepthPixels(),640,480,GL_LUMINANCE);
		//videoTex.loadData(rgb,640,480,GL_RGB);
	  return make_pair(rgb,make_pair(buf,make_pair(rawTime,make_pair(headx,make_pair(heady,make_pair(headz,make_pair(leftshoulderx,
		  make_pair(leftshouldery,make_pair(rightshoulderx,make_pair(rightshouldery,make_pair(lefthandx,make_pair(lefthandy,make_pair(righthandx,make_pair(righthandy,
		  make_pair(faceOneID,make_pair(faceOneX,make_pair(faceOneY,make_pair(faceOneZ,make_pair(faceTwoID,make_pair(faceTwoX,make_pair(faceTwoY,make_pair(faceTwoZ,
		  make_pair(depthbuff, make_pair(blurPixels, closID))))))))))))))))))))))));
	}
	printf("error!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
	return make_pair(rgb,make_pair(buf,make_pair(rawTime,make_pair(headx,make_pair(heady,make_pair(headz,make_pair(leftshoulderx,
	make_pair(leftshouldery,make_pair(rightshoulderx,make_pair(rightshouldery,make_pair(lefthandx,make_pair(lefthandy,make_pair(righthandx,make_pair(righthandy,
	make_pair(faceOneID,make_pair(faceOneX,make_pair(faceOneY,make_pair(faceOneZ,make_pair(faceTwoID,make_pair(faceTwoX,make_pair(faceTwoY,make_pair(faceTwoZ,
	make_pair(depthbuff, make_pair(blurPixels, closID))))))))))))))))))))))));
}

//-----------------------------------------------------------

pair<int, pair<int, int>> ofxKinectPlayer::updateSideInfo(){
	if(!fInt){
		printf("error!");
	}

	// loop?
	if(bLoop && std::feof(fInt) > 0) {
		fInt = fopen(ofToDataPath(filenameInt).c_str(), "rb");
	}

	fread(&genCat,sizeof(int),1,fInt);
	fread(&specCat,sizeof(int),1,fInt);
	fread(&nFrames,sizeof(int),1,fInt);

	//fread(&framesPerSecond,sizeof(int),1,fInt);
	//if (framesPerSecond != 0){//TODO this if may not be necessary
	//	fps = framesPerSecond - fpsOffset;
	//}


	return make_pair(genCat,make_pair(specCat, nFrames));
}

//-----------------------------------------------------------
void ofxKinectPlayer::seek(int frameNo){
	frameLoc = frameNo*size;
	fseek(f,frameLoc,SEEK_SET);
}
//-----------------------------------------------------------
void ofxKinectPlayer::draw(float x, float y){
	if(bUseTexture) {
		videoTex.draw(x,y);
	}
}

//-----------------------------------------------------------
void ofxKinectPlayer::draw(float x, float y, float w, float h){
	if(bUseTexture) {
		videoTex.draw(x,y,w,h);
	}
}

//----------------------------------------------------------
void ofxKinectPlayer::draw(const ofPoint & point){
	draw(point.x, point.y);
}

//----------------------------------------------------------
void ofxKinectPlayer::draw(const ofRectangle & rect){
	draw(rect.x, rect.y, rect.width, rect.height);
}

//---------------------------------------------------------------------------
void ofxKinectPlayer::drawDepth(float _x, float _y){
	if(bUseTexture) {
		depthTex.draw(_x, _y);
	}
}

//----------------------------------------------------------
void ofxKinectPlayer::drawDepth(float _x, float _y, float _w, float _h){
	if(bUseTexture) {
		depthTex.draw(_x, _y, _w, _h);
	}
}

//----------------------------------------------------------
void ofxKinectPlayer::drawDepth(const ofPoint & point){
	drawDepth(point.x, point.y);
}

//----------------------------------------------------------
void ofxKinectPlayer::drawDepth(const ofRectangle & rect){
	drawDepth(rect.x, rect.y, rect.width, rect.height);
}

//-----------------------------------------------------------
unsigned char * ofxKinectPlayer::getPixels(){
	return rgb;
}

//-----------------------------------------------------------
unsigned char * ofxKinectPlayer::getDepthPixels(){
	//return calibration.getDepthPixels();
	return NULL;
}

//-----------------------------------------------------------
float * ofxKinectPlayer::getDistancePixels(){
	//return calibration.getDistancePixels();
	return 0;
}

//-----------------------------------------------------------
unsigned char * ofxKinectPlayer::getCalibratedRGBPixels(){
	//return calibration.getCalibratedRGBPixels(rgb);
	return NULL;
}

//-----------------------------------------------------------
ofTexture & ofxKinectPlayer::getTextureReference(){
	return depthTex;
}

//-----------------------------------------------------------
ofTexture & ofxKinectPlayer::getDepthTextureReference(){
	return depthTex;
}

//---------------------------------------------------------------------------
//ofPixels & ofxKinectPlayer::getPixelsRef() {
//	return pixels;
//}

//-----------------------------------------------------------
float ofxKinectPlayer::getWidth(){
	return (float) width;
}

//-----------------------------------------------------------
float ofxKinectPlayer::getHeight(){
	return (float) height;
}

//-----------------------------------------------------------
bool ofxKinectPlayer::isFrameNew(){
	return true;
}

//-----------------------------------------------------------
void ofxKinectPlayer::close(){
	if(f)
		fclose(f);
	f = 0;
	filename = "";
	if(fInt) 
		fclose(fInt);
	fInt = 0;
	filenameInt = "";
}

//------------------------------------
/*float ofxKinectPlayer::getDistanceAt(int x, int y) {
	return calibration.getDistanceAt(x,y);
}*/

//------------------------------------
//ofVec3f ofxKinectPlayer::getWorldCoordinateFor(int x, int y) {
//	return calibration.getWorldCoordinateFor(x,y);
//}

