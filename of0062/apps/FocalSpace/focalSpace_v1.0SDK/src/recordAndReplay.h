#ifndef _RANDR
#define _RANDR

#include "ofxKinectRecorder.h"
#include "ofxKinectPlayer.h"
//#include "grabVideo.h"
#include "tags.h"
#include "button.h"
#include "slider.h"
#include "time.h"
class recordAndReplay{
public:
	recordAndReplay();
	virtual ~recordAndReplay();

	void setup();
	//void init(KinectGrabber kinGrab);
	void init();
	void close();
	//BACKEND REPLAY/RECORD SECTION
	/// make sure you don't record and playback simultaneously 
	/// using the same file!!!
	/*void startRecording(KinectGrabber g_kinectGrabber); //deals with starting recording
	void stopRecording(KinectGrabber g_kinectGrabber); //deals with stopping recording, also affects gui (through safe reset live)
	void startPlayback(KinectGrabber g_kinectGrabber); //deals with starting playback*/
	void stopPlayback(); //deals with stopping playback, also affects gui (through safe reset live)
	void startRecording(); //deals with starting recording
	void stopRecording(); //deals with stopping recording, also affects gui (through safe reset live)
	void startPlayback(); //deals with starting playback*/
	/// interface with testApp
	void readFrame(); //(sets the variables testApp is going to read - is called during playback)
	void storeFrame(unsigned char* colorAlphaPixels, unsigned char * grayPixels, time_t rawTime, int headPositionX, int headPositionY, int headPositionZ,
					int leftShoulderX, int leftShoulderY, int rightShoulderX, int rightShoulderY, int leftHandPX, int leftHandPY, int rightHandPX, int rightHandPY,
					int faceOneID, int faceOneX, int faceOneY, int faceOneZ, int faceTwoID, int faceTwoX, int faceTwoY, int faceTwoZ, USHORT* depthBuff, unsigned char* blurPixels,
					int closID);
	void drawButtons();
	void drawSmallButtons();
	void drawSliders();
	void checkAndDrawGestureSignals();
	//(records variables given to it by testApp - is called during record)
	///
	//playback helper functions
	void safeResetReplayMode(); //resets variables (gui and backend) that may have changed (resets playback mode)
	void safeResetLiveMode(); //resets variables (gui and backend) that may have changed (resets live video mode)
	void getPlaybackRange(); //gets the playback range for the purposes of displaying a slider
	void skipTo(float percentOfSlider);//takes in a percent value of the slider progress, converts it to a percent value of the number of frames, then forwards until it gets there and pauses.
	void skipToFaceAt(int x, int y); //takes in coordinates, finds the closest face and runs skipToNextFaceID on it.
	void skipToNextFaceID(int faceID, int wrapAround); //accepts a face ID, and skips to the next time that face shows up. if the third value is true, it will wrap around (the playback bar)
	void pause() {paused = true;} //allows live and recorded video to be paused
	void play()  {paused = false;} //sets paused = false (undos the pause effect stated above)

	//======================================================================
	//BUTTON CONTROL SECTION
	//these three do the actions of the buttons safely (updating what needs to be updated, etc) 
	/*void standardStop(KinectGrabber g_kinectGrabber); //stops playback/recording and changes variables that inform buttons whether they should be active or not
	void standardReplay(KinectGrabber g_kinectGrabber); //replays saved files and changes variables that inform buttons whether they should be active or not
	void standardRecord(KinectGrabber g_kinectGrabber); //starts recording and changes variables that inform buttons whether they should be active or not*/
	void standardStop(); //stops playback/recording and changes variables that inform buttons whether they should be active or not
	void standardReplay(); //replays saved files and changes variables that inform buttons whether they should be active or not
	void standardRecord();
	void enterDemoVideoMode();
	void exitDemoVideoMode();
	void updateButtonTriggers(); //informs buttons whether they should be active or not
	//========================================================================

	//sound recording
	ofSoundPlayer  soundfile;
	int startRecordTime;
	int stopRecordTime;
	int videoFrameDelay;
	int numRecordedFrames;
	float currSoundPos;
	
	//========================================================================
	//SETTERS AND TOGGLERS
	void toggleGoodIdeasDrawn() {goodIdeasDrawn = !goodIdeasDrawn;};
	void toggleTalkingHeadsDrawn() {talkingHeadsDrawn = !talkingHeadsDrawn;};
	void setGoodIdeasDrawn(bool newValue) {goodIdeasDrawn = newValue;};
	void setTalkingHeadsDrawn(bool newValue) {talkingHeadsDrawn = newValue;};
	//GETTERS
	bool getBRecord() {return bRecord;};
	bool getBPlayback() {return bPlayback;};
	bool getPaused() {return paused;};
	bool getStopButtonActive() {return stopButtonActive;};
	bool getReplayButtonActive() {return replayButtonActive;};
	bool getRecordButtonActive() {return recordButtonActive;};
	bool getGoodIdeaButtonActive() {return goodIdeaButtonActive;};
	bool getTalkingHeadButtonActive() {return talkingHeadButtonActive;};
	bool getPauseButtonOn() {return pauseButtonOn;};
	bool getDemoVideoMode() {return demoVideoMode;};
	//TODO: maybe change the below to use, if not null, then - otherwise print an error
	//as of now, this function will return an unexpected value if it is called before any nextFrame call,
	//or maybe give them default values, and explain that if no value is assigned the default will be returned
	//ofSoundPlayer getSoundfile() {return soundfile;};
	unsigned char * getColorAlphaPixels() {return rgbdepthpair.first;};
	unsigned char * getGrayPixels() {return rgbdepthpair.second.first;};
	time_t getRawTime()				{return rgbdepthpair.second.second.first;};
	int getHeadPositionX()			{return rgbdepthpair.second.second.second.first;}
	int getHeadPositionY()			{return rgbdepthpair.second.second.second.second.first;};
	int getHeadPositionZ()			{return rgbdepthpair.second.second.second.second.second.first;};
	int getLeftShoulderX()			{return rgbdepthpair.second.second.second.second.second.second.first;};
	int getLeftShoulderY()			{return rgbdepthpair.second.second.second.second.second.second.second.first;};
	int getRightShoulderX()			{return rgbdepthpair.second.second.second.second.second.second.second.second.first;};
	int getRightShoulderY()			{return rgbdepthpair.second.second.second.second.second.second.second.second.second.first;};
	int getLeftHandPX()				{return rgbdepthpair.second.second.second.second.second.second.second.second.second.second.first;};
	int getLeftHandPY()				{return rgbdepthpair.second.second.second.second.second.second.second.second.second.second.second.first;};
	int getRightHandPX()			{return rgbdepthpair.second.second.second.second.second.second.second.second.second.second.second.second.first;};
	int getRightHandPY()			{return rgbdepthpair.second.second.second.second.second.second.second.second.second.second.second.second.second.first;};
	int getFaceOneID()				{return rgbdepthpair.second.second.second.second.second.second.second.second.second.second.second.second.second.second.first;};
	int getFaceOneX()				{return rgbdepthpair.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.first;};
	int getFaceOneY()				{return rgbdepthpair.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.first;};
	int getFaceOneZ()				{return rgbdepthpair.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.first;};
	int getFaceTwoID()				{return rgbdepthpair.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.first;};
	int getFaceTwoX()				{return rgbdepthpair.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.first;};
	int getFaceTwoY()				{return rgbdepthpair.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.first;};
	int getFaceTwoZ()				{return rgbdepthpair.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.first;};
	USHORT* getDepthBuff()			{return rgbdepthpair.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.first;};
	unsigned char * getBlurPixels()	{return rgbdepthpair.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.first;};
	int getClosID()					{return rgbdepthpair.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second.second;};

	pair<bool,float> getSecondSliderPressed(int x,int y) {return (*secondSlider).sliderPressed(x,y);};
	pair<bool,float> getTimerSliderPressed(int x,int y) {return (*timer).sliderPressed(x,y);};
	bool getStopButtonPressed(int x,int y) {return (*stopButton).buttonPressed(x,y);};
	bool getReplayButtonPressed(int x,int y) {return (*replayButton).buttonPressed(x,y);};
	bool getRecordButtonPressed(int x,int y) {return (*recordButton).buttonPressed(x,y);};
	bool getGoodIdeaButtonPressed(int x,int y) {return (*goodIdeaButton).buttonPressed(x,y);};
	bool getTalkingHeadButtonPressed(int x,int y) {return (*talkingHeadButton).buttonPressed(x,y);};
	bool getDemoVideoButtonPressed(int x,int y) {return (*demoVideoButton).buttonPressed(x,y);};

	//gesture - since i made recAndRep be the middle man b/n tags and testApp
	bool getBRightHandUp();
	//blur
	bool getBlurOn(){return blurOn;};
	//slider
	ofImage timelineImg;

private:
	int VIDEO_WIDTH; //TODO: in future can put this into one file (like "videoConstants" and use it for otehr files as well)
	int VIDEO_HEIGHT;
	int RENDER_WIDTH;
	int RENDER_HEIGHT;

	ofxKinectRecorder 	kinectRecorder; //accepts variables and stores them externally
	ofxKinectPlayer 	kinectPlayer; //reads variables off of a file and returns the variables
	tags myTags;  //controls data related to gesture tags, audio tags and face tags

	/////Playback
	bool bRecord;  //true if you are recording
	bool bPlayback;//true if you are in playback mode (even if paused)
	int nFrames; //number of frames in the recorded video you are playing
	int currFrame; //the frame number of the frame you are currently replaying (always <= nFrames)
	bool paused; //stores whether playback is currently paused or not
	bool nFramesValid; //is whether nFrames is valid or not
	int closestID; //keeps track of the closestID;
	bool blurOn; //keeps track of whether the blur is currently on or not
	/////Time
	int lastPlaybackTime; //timestamp of the last frame in the recording
	time_t firstPlaybackTime; //timestamp of the first frame in the recording
	time_t prevTimeMillis; //timestap of the previous frame that you saw

	///// Whether Buttons are Clickable or not
	bool stopButtonActive;
	bool replayButtonActive;
	bool recordButtonActive;
	bool pauseButtonOn;//checks whetehr replay button should change or not (true = it should be pause)
	bool goodIdeaButtonActive;
	bool talkingHeadButtonActive;
	bool demoVideoButtonActive;
	bool goodIdeasDrawn;
	bool talkingHeadsDrawn;
	bool demoVideoMode;
	pair<unsigned char *,pair<unsigned char *,pair<time_t,pair<int,pair<int,pair<int,pair<int,
		pair<int,pair<int,pair<int,pair<int,pair<int,pair<int,pair<int,pair<int,pair<int,
		pair<int,pair<int,pair<int,pair<int,pair<int,pair<int,pair<USHORT*,pair<unsigned char *,int>>>>>>>>>>>>>>>>>>>>>>>> rgbdepthpair;//variable that accepts the variables that kinectPlayer sends

	pair<int, pair<int, pair<int, pair<int, pair<int, pair<int, pair<int, pair<int, pair<int,
		pair<int, int>>>>>>>>>> frameInfo;//variable that accepts the variables that kinectPlayerInfo sends (nFrames and gesture audio indices)

	pair<int,pair<int,int>> sideInfo;

	//======================================================================
	//GUI PRIVATE SECTION
	/////Buttons
	button* stopButton;
	button* pauseButton;
	button* replayButton;
	button* recordButton;
	button* goodIdeaButton;
	button* talkingHeadButton;
	button* demoVideoButton;
	/////Button picture dimensions
	int buttonWidth; //in pixels
	int buttonHeight;// this and the above are the height and width for buttons (stop, record, replay and pause all have these dimensions)
	int goodIdeaButtonWidth;
	int goodIdeaButtonHeight;
	int talkingHeadButtonWidth;
	int talkingHeadButtonHeight;
	int demoVideoButtonWidth;
	int demoVideoButtonHeight;
	int smallButtonHeight; //this is helpful to the second slider that skips to where you click (but below the main slider)
	int smallButtonWidth;
	///// Small (and Big) Button variables & helpers
	ofImage smallGoodImg;
	ofImage smallPZeroImg;
	ofImage smallPOneImg;
	ofImage smallPTwoImg;
	ofImage smallPThreeImg;
	ofImage smallPFourImg;
	ofImage smallPFiveImg;
	ofImage smallPImgs[6]; //stores the images for the small person icons;
	ofImage tempSmallImg; 
	ofImage bigGoodImg;
	int bigTimer;
	int tempButtonX;
	int rhrIndex; //the index of the index after the last currently  filled slot in the small RHRLocations array
	int maxNoIndices;
	int currNum; //holder for frame no recovered from tagInfo
	int gen; //genCat for button (TODO: in future can implement buttons as arrays of buttons)
	int spec; //specCat for button
	static const int lenGesArray = 10;  //the max of each kind of gesture of audio allowable (this variable should be the same as the one with the same name in tags)

	double xOneSquared, yOneSquared, xTwoSquared, yTwoSquared; //helps with skipToNextFaceID
	double fox, foy, ftx, fty;
	double xOneDiff, yOneDiff, xTwoDiff, yTwoDiff;
	double radiusOne, radiusTwo; //helps in skipToFaceAt

	/////Sliders
	slider* timer;
	slider* secondSlider; //(right below first and with similar parameters). Use: To be able to click on gesture symols and stuff
	bool pauseAtNextFrame;

	/////Slider parameters
	int timerStartX;//replay slider start x
	int timerStartY;//replay slider start y
	int timerWidth; //replay timer width
	int timerHeight; //replay timer height
	float timerProgression; //the fraction (currFrame/nFrames)
	bool bRightHandUp;
};

#endif