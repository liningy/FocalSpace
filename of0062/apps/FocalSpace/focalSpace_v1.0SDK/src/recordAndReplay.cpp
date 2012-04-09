#include "recordAndReplay.h"
#include <algorithm>//I'm not sure if this is necessary


//--------------------------------------------------------------------
recordAndReplay::recordAndReplay(){
	myTags.init();
	//kinectPlayer.setup(ofToDataPath("recording.dat"), ofToDataPath("recordingInfo.dat"), true); // set record file and source
	//kinectRecorder.init(ofToDataPath("recording.dat"), ofToDataPath("recordingInfo.dat"));
}
//--------------------------------------------------------------------
recordAndReplay::~recordAndReplay() {
	close();
}
//void recordAndReplay::init(KinectGrabber g_kinectGrabber){
void recordAndReplay::init(){	

	VIDEO_WIDTH = 640;
	VIDEO_HEIGHT= 480;
	nFrames = 0;
	currFrame = 0;
	//g_kinectGrabber.recordAudioInit();
	
	bRecord = false;
	bPlayback = false;
	paused = false;
	nFramesValid = false;

	///GUI
	//Button Variables
	buttonWidth = 104; //in pixels
	buttonHeight = 81;
	goodIdeaButtonWidth = 93;
	goodIdeaButtonHeight = 63;
	smallButtonHeight = 25; //this is helpful to the second slider that skips to where you click (but below the main slider)
	smallButtonWidth = 22; //this helps in making the "good idea button" range lines
	stopButtonActive = false;
	pauseButtonOn = false;
	replayButtonActive = true;
	recordButtonActive = true;
	goodIdeaButtonActive = false;
	talkingHeadButtonActive = false;
	setSmallButtonActive(-1);
	smallGoodImg.loadImage("images\\r_small.png");
	smallZeroImg.loadImage("images\\zero.png");
	smallOneImg.loadImage("images\\one.png");
	smallTwoImg.loadImage("images\\two.png");
	smallThreeImg.loadImage("images\\three.png");
	smallFourImg.loadImage("images\\four.png");
	smallFiveImg.loadImage("images\\five.png");
	timelineImg.loadImage("images\\timeline.png");
	int hButtonOffset = -80;
	stopButton = new button("Stop", VIDEO_WIDTH - buttonWidth - hButtonOffset , VIDEO_HEIGHT + 50 , buttonWidth, buttonHeight, stopButtonActive, "images\\r_stop_disable.png", "images\\r_stop.png");
	replayButton = new button("Replay", VIDEO_WIDTH - hButtonOffset, VIDEO_HEIGHT + 50, buttonWidth, buttonHeight, replayButtonActive, "images\\r_pause.png", "images\\r_replay.png");
	recordButton = new button("Record", VIDEO_WIDTH + buttonWidth - hButtonOffset, VIDEO_HEIGHT + 50 , buttonWidth, buttonHeight, recordButtonActive, "images\\r_record_disable.png", "images\\r_record.png");
	goodIdeaButton = new button("Good Idea", 57 , 809 , goodIdeaButtonWidth, goodIdeaButtonHeight, goodIdeaButtonActive, "images\\r_good.png", "images\\r_good.png");
	talkingHeadButton = new button("Talking heads", 142 , 809 , goodIdeaButtonWidth, goodIdeaButtonHeight, talkingHeadButtonActive, "images\\person_01.png", "images\\person_01.png");
	zeroButton = new button("Zero", 57 + 3*goodIdeaButtonWidth , 809 , smallButtonWidth, smallButtonHeight, zeroButtonActive, "images\\zero.png", "images\\zero.png");
	oneButton = new button("One", 57 + 3*goodIdeaButtonWidth  + smallButtonWidth, 809 , smallButtonWidth, smallButtonHeight, bPlayback, "images\\one.png", "images\\one.png");
	twoButton = new button("Two", 57 + 3*goodIdeaButtonWidth + 2*smallButtonWidth, 809 , smallButtonWidth, smallButtonHeight, bPlayback, "images\\two.png", "images\\two.png");
	threeButton = new button("Three", 57 + 3*goodIdeaButtonWidth + 3*smallButtonWidth, 809 , smallButtonWidth, smallButtonHeight, bPlayback, "images\\three.png", "images\\three.png");
	fourButton = new button("Four", 57 + 3*goodIdeaButtonWidth + 4*smallButtonWidth, 809 , smallButtonWidth, smallButtonHeight, bPlayback, "images\\four.png", "images\\four.png");
	fiveButton = new button("Five", 57 + 3*goodIdeaButtonWidth +5*smallButtonWidth, 809 , smallButtonWidth, smallButtonHeight, bPlayback, "images\\five.png", "images\\five.png");
	//Slider variables
	lastPlaybackTime = 0;
	firstPlaybackTime = 0;
	prevTimeMillis = 0;
	timerProgression = 0;
	timerWidth = 1460;//VIDEO_WIDTH;
	timerHeight = 10;
	timerStartX =97;// VIDEO_WIDTH/2;
	timerStartY = 740;//1.5*VIDEO_HEIGHT;
	timer = new slider(timerStartX,timerStartY,timerWidth,timerHeight);
	secondSlider = new slider(timerStartX,timerStartY+timerHeight,timerWidth,smallButtonHeight);//a slider on top of the small (gesture) buttons
	//it makes it seems as if the buttons are active. A drawback - even if you click on a non-button spot it will skip
	(*timer).setSliderPosX(timerProgression*timerWidth);
	rhrIndex = 0;
	maxNoIndices = lenGesArray; //see note for lenGesArray in the .h file
	pauseAtNextFrame = false;

	nFrames = 0;
	numRecordedFrames = 0;
	bRightHandUp = false;
	closestID = 0; //default, so the first entry can be recorded as a change;
}
//--------------------------------------------------------------
//comments are in test app.h
void recordAndReplay::setup(){	
}
//--------------------------------------------------------------
void recordAndReplay::readFrame(){
	rgbdepthpair = kinectPlayer.updatea();

	if (nFramesValid){
		currFrame = (currFrame+1) % nFrames;
		timerProgression = ((float)currFrame/(float)nFrames);
	}
	else{
		timerProgression = 0;
	}
	(*timer).setSliderPosX(timerProgression*timerWidth);
	if (pauseAtNextFrame){
		if (!paused){
			standardReplay();
			pauseAtNextFrame = false;
		}
	}
}
void recordAndReplay::storeFrame(unsigned char* colorAlphaPixels, unsigned char * grayPixels, time_t rawTime, int headPositionX, int headPositionY, int headPositionZ,
						int leftShoulderX, int leftShoulderY, int rightShoulderX, int rightShoulderY, int leftHandPX, int leftHandPY, int rightHandPX, int rightHandPY,
						int faceOneID, int faceOneX, int faceOneY, int faceOneZ, int faceTwoID, int faceTwoX, int faceTwoY, int faceTwoZ, USHORT* depthBuff, unsigned char* blurPixels,
						int closID){
	nFrames++;
	kinectRecorder.newFrame(colorAlphaPixels, grayPixels, rawTime, headPositionX, headPositionY, headPositionZ, leftShoulderX, leftShoulderY, rightShoulderX,
							rightShoulderY,leftHandPX, leftHandPY, rightHandPX, rightHandPY, faceOneID, faceOneX, faceOneY, faceOneZ, 
							faceTwoID, faceTwoX, faceTwoY, faceTwoZ, depthBuff, blurPixels, closID);
	numRecordedFrames ++;
	myTags.checkRightHandUp(rightHandPY,headPositionY); //(every call to this increments a pointer - if right hand is raised. So do it only once per frame)
	if (myTags.getBRightHandUp()){//if there are more kinds of gestures and audio, send all of them together at end (otherwise, order will get mixed up I believe),but not with suggested improvement
		kinectRecorder.storeSideInfo(1,0,nFrames);
	}
	if (closID != closestID){
		closestID = closID;
		kinectRecorder.storeSideInfo(2,closestID,nFrames);
	}
}
//======================================================================
//BACKEND REPLAY/RECORD SECTION
/// make sure you don't record and playback simultaneously 
/// using the same file!!!
//void recordAndReplay::startRecording(KinectGrabber g_kinectGrabber) {
void recordAndReplay::startRecording() {
	numRecordedFrames = 0;
	if (bPlayback){
		stopPlayback(); // stop playback if recording
	}
	kinectRecorder.init(ofToDataPath("recording.dat"), ofToDataPath("recordingInfo.dat"));
	bRecord = true;
	nFrames = 0;
	//g_kinectGrabber.recordAudioStart();
	//startRecordTime = ofGetElapsedTimeMillis();
	return;
}


//void recordAndReplay::stopRecording(KinectGrabber g_kinectGrabber) {
void recordAndReplay::stopRecording() {
	bRecord = false;
	//audio
	//g_kinectGrabber.recordAudioEnd();
	//stopRecordTime = ofGetElapsedTimeMillis();
	if (numRecordedFrames!=0){
		//TODO: if statement avoids issues of playing back from file without
		//recording, but not doing this on file playback may cause problems?
		//videoFrameDelay = (stopRecordTime - startRecordTime)/numRecordedFrames;
		//videoFrameDelay = (stopRecordTime - startRecordTime)/nFrames;
	}

	//video
	if (nFrames != 0) { //without this, nFrames in recordingInfo would be overwritten with 0 when playback started (since it stops recording first)
		kinectRecorder.storeSideInfo(0,0,nFrames);
	}
	kinectRecorder.close();
	myTags.close();
	safeResetLiveMode();
	return;
}

//void recordAndReplay::startPlayback(KinectGrabber g_kinectGrabber) {
void recordAndReplay::startPlayback() {
	bPlayback = true;
	if (bRecord){
		stopRecording();  //stop recording if starting playback
	}
	kinectPlayer.setup(ofToDataPath("recording.dat"), ofToDataPath("recordingInfo.dat"), true); // set record file and source
	kinectPlayer.loop();
	getPlaybackRange();
	if (bPlayback && paused){//if already playing, unpause
		standardReplay();
	}
	play();
	return;

	//soundfile.loadSound("sounds/out.wav");
}

void recordAndReplay::stopPlayback() {
	kinectPlayer.close();
	myTags.close();
	bPlayback = false;
	nFramesValid = false;
	safeResetLiveMode();

	//soundfile.stop();
	return;
}

//PLAYBACK HELPER FUNCTIONS
///// functions for resetting playback and live modes respectively. Reset some backend and gui variables
void recordAndReplay::safeResetReplayMode(){//intended to work if you're already in playback mode
	if (bPlayback){
		play();
		stopButtonActive = true;
		replayButtonActive = true;
		recordButtonActive = false;
		goodIdeaButtonActive = true;
		talkingHeadButtonActive=true;
		pauseButtonOn = true;
		setSmallButtonActive(7);
		zeroButtonActive = true;
		oneButtonActive = true;
		twoButtonActive = true;
		threeButtonActive = true;
		fourButtonActive = true;
		fiveButtonActive = true;
		updateButtonTriggers();
	}
}

void recordAndReplay::safeResetLiveMode(){//intended to wrok if you're already in live mode
	play();
	currFrame = 0;
	nFrames = 0;
	nFramesValid = false;
	timerProgression = 0;
	(*timer).setSliderPosX(timerProgression*timerWidth);
	stopButtonActive = false;
	replayButtonActive = true;
	recordButtonActive = true;
	goodIdeaButtonActive = false;
	talkingHeadButtonActive=false;
	pauseButtonOn = false;
	setSmallButtonActive(-1);
	zeroButtonActive = true;
	oneButtonActive = true;
	twoButtonActive = true;
	threeButtonActive = true;
	fourButtonActive = true;
	fiveButtonActive = true;
	updateButtonTriggers();
}

/////function to get the number of frames (and where each type of gesture is)
//2 (one of the more important changes)
void recordAndReplay::getPlaybackRange(){ 

	while (!kinectPlayer.checkEndOfSideFile()){
		sideInfo = kinectPlayer.updateSideInfo();
		myTags.setTagInfo(sideInfo.first, sideInfo.second.first, sideInfo.second.second);
	}
	nFrames = myTags.getTagInfo(0,0,0);
	nFramesValid = true;
	//2
}

void recordAndReplay::skipTo(float percentOfSlider){//it only works while you are playing (not pausing)
	//Note: if StandardReplay is used, if program is paused b/n this and the pause that skip to does,
	//then the buttons will be incorrect (it will look like it's paused while it's not)
	if (getPaused()){
		play();
	}
	currFrame = (int) (percentOfSlider*nFrames); // I know it skips, and i think it readjusts the slider, i'm not exactly sure if the slider is still accurate though.
	kinectPlayer.seek(currFrame);
	pauseAtNextFrame = true;
}

void recordAndReplay::skipToFaceAt(int x, int y){
	//TODO: store skeletons as arrays of arrays
	if (getFaceOneX() == -1000){
		skipToNextFaceID(-1, true);
		return;
	}
	xOneDiff = abs((double)(getFaceOneX() - x));
	yOneDiff = abs((double)(getFaceOneY() - y));
	xTwoDiff = abs((double)(getFaceTwoX() - x));
	yTwoDiff = abs((double)(getFaceTwoY() - y));
	xOneSquared = pow(xOneDiff,2);
	yOneSquared = pow(yOneDiff,2); 
	xTwoSquared = pow(xTwoDiff,2);
	yTwoSquared = pow(yTwoDiff,2);
	minRadius = sqrt(xOneSquared + yOneSquared);
	if ((sqrt(xTwoSquared + yTwoSquared)) < minRadius){
		skipToNextFaceID(getFaceTwoID(), true);
	}
	else{
		skipToNextFaceID(getFaceOneID(), true);
	}
	minRadius = 0.0;
}
void recordAndReplay::skipToNextFaceID(int faceID, int wrapAround){
	//Note: If there is only one frame shift and you click on it, this function will bring you to the start of the current ID period
	if (faceID == -1){
		blurOn = true; //b/c the set small... func would set it to true
		ofRectangle(60,VIDEO_HEIGHT - 20,40,40);
		//skipTo(((float)currFrame)/nFrames);
		return;
	}
	for (int i=0;i<maxNoIndices;i++){
		if (myTags.getTagInfo(2,faceID,i)> currFrame){
			setSmallButtonActive(faceID);
			skipTo(((float)myTags.getTagInfo(2,faceID,i))/nFrames);
			return;
		}
	}
	if (wrapAround){
		setSmallButtonActive(faceID);
		skipTo(((float)myTags.getTagInfo(2,faceID,0))/nFrames);
		return;
	}
	//If you can find a frame with the given ID (after you, with wrapAround = false, 
	//or anywhere with wrapAround = true) return the currentFrame, and TODO: Possibly display text (for now I draw a square)?
	//(it probably shouldn't come here unless something is wrong)
	blurOn = true; //b/c the set small... func would set it to true
	ofRectangle(60,VIDEO_HEIGHT - 20,40,40);
	//skipTo(((float)currFrame)/nFrames);
	return;
}
/////BUTTON CONTROL SECTION

void recordAndReplay::setSmallButtonActive(int newValue){
	smallButtonActive = newValue;
	if (smallButtonActive == 6){blurOn = false;}
	else if (smallButtonActive == -1){blurOn = false;}
	else if (smallButtonActive >= 0 && smallButtonActive <= 5){blurOn = true;}
	else {blurOn = false;}
}
void recordAndReplay::standardStop(){
		if (bRecord){
			stopRecording();
			stopButtonActive = false;
			replayButtonActive = true;
			recordButtonActive = true;
			goodIdeaButtonActive = false;
			talkingHeadButtonActive=false;
			pauseButtonOn = false;
			setSmallButtonActive(-1);
			zeroButtonActive = true;
			oneButtonActive = true;
			twoButtonActive = true;
			threeButtonActive = true;
			fourButtonActive = true;
			fiveButtonActive = true;
		}
		if (bPlayback){
			stopPlayback();
			stopButtonActive = false;
			replayButtonActive = true;
			recordButtonActive = true;
			goodIdeaButtonActive = false;
			talkingHeadButtonActive=false;
			pauseButtonOn = false;
			setSmallButtonActive(-1);
			zeroButtonActive = true;
			oneButtonActive = true;
			twoButtonActive = true;
			threeButtonActive = true;
			fourButtonActive = true;
			fiveButtonActive = true;
		}
		updateButtonTriggers();
}

void recordAndReplay::standardReplay(){
		if (!bPlayback){
			startPlayback();
			stopButtonActive = true;
			replayButtonActive = true;
			recordButtonActive = false;
			goodIdeaButtonActive = true;
			talkingHeadButtonActive=true;
			pauseButtonOn = true; //note, this indicates whether the paues button is displayed or not
			setSmallButtonActive(7);
			zeroButtonActive = true;
			oneButtonActive = true;
			twoButtonActive = true;
			threeButtonActive = true;
			fourButtonActive = true;
			fiveButtonActive = true;
			}
		else{
			if (!paused){
				pause();
				pauseButtonOn = false;
				/*if (soundfile.getIsPlaying()) {
					currSoundPos = soundfile.getPosition();
				}
				soundfile.stop();*/
			}
			else if (paused){
				play();
				pauseButtonOn = true;
				/*soundfile.play();
				soundfile.setPosition(currSoundPos);*/
			}
		}
		updateButtonTriggers();
}

void recordAndReplay::standardRecord(){
		startRecording();
		stopButtonActive = true;
		replayButtonActive = true;
		recordButtonActive = false;
		goodIdeaButtonActive = false;
		talkingHeadButtonActive=false;
		setSmallButtonActive(-1);
		zeroButtonActive = true;
		oneButtonActive = true;
		twoButtonActive = true;
		threeButtonActive = true;
		fourButtonActive = true;
		fiveButtonActive = true;
		updateButtonTriggers();
}
void recordAndReplay::updateButtonTriggers(){
	(*stopButton).trigger = stopButtonActive;
	(*replayButton).trigger = replayButtonActive;
	(*recordButton).trigger = recordButtonActive;
	(*goodIdeaButton).trigger = goodIdeaButtonActive;
	(*talkingHeadButton).trigger = talkingHeadButtonActive; //lining
	(*zeroButton).trigger = zeroButtonActive;
	(*oneButton).trigger = oneButtonActive; 
	(*twoButton).trigger = twoButtonActive;
	(*threeButton).trigger = threeButtonActive;
	(*fourButton).trigger = fourButtonActive;
	(*fiveButton).trigger = fiveButtonActive;
	
	//note, small buttons don't need to be updated because they are made the moment before drawing them
}
void recordAndReplay::drawButtons(){
	timelineImg.draw(50,712);
	(*stopButton).drawFontwithAltPic(stopButtonActive);
	(*replayButton).drawFontwithAltPic(replayButtonActive, pauseButtonOn);
	(*recordButton).drawFontwithAltPic(recordButtonActive);
	(*goodIdeaButton).drawFontwithAltPic(goodIdeaButtonActive);
	(*talkingHeadButton).drawFontwithAltPic(talkingHeadButtonActive);
	(*zeroButton).drawFontwithAltPic(zeroButtonActive);
	(*oneButton).drawFontwithAltPic(oneButtonActive); 
	(*twoButton).drawFontwithAltPic(twoButtonActive);
	(*threeButton).drawFontwithAltPic(threeButtonActive);
	(*fourButton).drawFontwithAltPic(fourButtonActive);
	(*fiveButton).drawFontwithAltPic(fiveButtonActive);
}
void recordAndReplay::drawSliders(){
	(*timer).drawSliderasPic(0, 1);//unsure what the variables do!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}
void recordAndReplay::drawSmallButtons(){
	if (nFramesValid){// nFramesValid should imply playback
		gen = -1;
		spec = -1;
		if (smallButtonActive == 7){	
			gen = 1;
			spec = 0;
			tempSmallImg = smallGoodImg;
		}
		else if (smallButtonActive == -1){
			return;
		}
		else if (smallButtonActive == 0){
			gen = 2;
			spec = 0;
			tempSmallImg = smallZeroImg;
		}
		else if (smallButtonActive == 1){
			gen = 2;
			spec = 1;
			tempSmallImg = smallOneImg;
		}
		else if (smallButtonActive == 2){
			gen = 2;
			spec = 2;
			tempSmallImg = smallTwoImg;
		}
		else if (smallButtonActive == 3){
			gen = 2;
			spec = 3;
			tempSmallImg = smallThreeImg;
		}
		else if (smallButtonActive == 4){
			gen = 2;
			spec = 4;
			tempSmallImg = smallFourImg;
		}
		else if (smallButtonActive == 5){
			gen = 2;
			spec = 5;
			tempSmallImg = smallFiveImg;
		}
		else if (gen == -1){
			return;
		}

		for (int i = 0;i<maxNoIndices;i++){
			currNum = myTags.getTagInfo(gen,spec,i);
			if (currNum !=-1){
				ofEnableAlphaBlending();
				tempButtonX = (int) ((float) timerStartX + ((float) currNum/(float) nFrames)*timerWidth);
				tempSmallImg.draw(tempButtonX,timerStartY + timerHeight);
				ofSetColor(100,100,100);
				ofLine(tempButtonX - 10,timerStartY,tempButtonX - 10, timerStartY + timerHeight);//"good idea button" range lines
				ofLine(tempButtonX + smallButtonWidth + 10,timerStartY,tempButtonX + smallButtonWidth + 10, timerStartY + timerHeight);//"good idea button" range lines
				ofSetColor(255,255,255);
				ofDisableAlphaBlending();
			}
			//else{
			//	break;//TODO: are breaks recomended? Anyways, if any in current gesture are 0, all remaining are so we can skip them
			//}
		}	
	}
}

bool recordAndReplay::getBRightHandUp() {
	bRightHandUp = myTags.getBRightHandUp();
	return bRightHandUp;
}
void recordAndReplay::close(){
	kinectPlayer.close();
	kinectRecorder.close();
	myTags.close();
}
