#include "testApp.h"
#include <algorithm>

#define RECONNECT_TIME 400
//--------------------------------------------------------------
void testApp::setup(){	
	bgColor.r=0;
	bgColor.g=0;
	bgColor.b=0;
	ofBackground(bgColor.r,bgColor.g,bgColor.b);
	 // set up the blur shader
	blur.setup(DEPTH_WIDTH, DEPTH_HEIGHT); 
	thresh=300;
	// initialize kinect and application
	g_kinectGrabber.Kinect_Zero();
	g_kinectGrabber.Kinect_Init();
	conference_init();
	// initialize record and replay applicatoin
	recAndRep.init();
	// allocate memory for colar,focus and blur pixels
	colorAlphaPixels =  g_kinectGrabber.Kinect_getRGBBuffer(); //new unsigned char [DEPTH_WIDTH*DEPTH_HEIGHT*4];
	focusPixels = new unsigned char [DEPTH_WIDTH*DEPTH_HEIGHT*4];
	blurPixels = new unsigned char [DEPTH_WIDTH*DEPTH_HEIGHT*4];
	mobilePixels = new unsigned char [MOBILE_WIDTH*MOBILE_HEIGHT*4];
	mobileBlurPixels = new unsigned char [MOBILE_WIDTH*MOBILE_HEIGHT*4];
	// allocate memory for textures
	texColorAlpha.allocate(VIDEO_WIDTH,VIDEO_HEIGHT,GL_RGBA);
	texFocus.allocate(DEPTH_WIDTH, DEPTH_HEIGHT,GL_RGBA); 
	texBlur.allocate(DEPTH_WIDTH, DEPTH_HEIGHT,GL_RGBA); 
	//texGray.allocate(DEPTH_WIDTH, DEPTH_HEIGHT,GL_RGBA); // gray depth texture
	texMobile.allocate(MOBILE_WIDTH, MOBILE_HEIGHT, GL_RGBA);
	texMobileBlur.allocate(MOBILE_WIDTH, MOBILE_HEIGHT, GL_RGBA);
	//other parameters
	maskValue=3;
	rawTime = 0;  
	//gui interface
	nButtons=11;
	buttons=new button*[nButtons];
	buttons[0]=new button("setup",453,839,37,30,true,"images/set_a.png","images/set_b.png");
	buttons[1]=new button("active",694,836,100,30,true,"images/auto_a.png","images/auto_b.png");
	buttons[2]=new button("manual",786,836,100,30,true,"images/manual_a.png","images/manual_b.png");
	
	buttons[3]=new button("blur",409,606,100,30,false,"images/focus_a.png","images/focus_b.png");
	buttons[4]=new button("tint",409,676,100,30,false,"images/mask_a.png","images/mask_b.png");
	buttons[5]=new button("zoom", 409,744,100,30,false,"images/zoom_a.png","images/zoom_b.png");
	
	buttons[6]=new button("flipchart", 1285,695,100,100,true,"images/sketch_a.png","images/sketch_b.png");
	buttons[7]=new button("namebubble", 490,839,37,30,true,"images/bubble_a.png","images/bubble_b.png");
	buttons[8]=new button("ipad", 1413,695,100,100,true,"images/ipad_a.png","images/ipad_b.png");

	buttons[9]=new button("live",643,21,150,50,true,"images/live_a.png","images/live_b.png");
	buttons[10]=new button("review",800,21,150,50,true,"images/review_a.png","images/review_b.png");

	for(int i=0;i<nButtons;i++) buttonPressed[i]=false;
	buttonPressed[0]=true;
	buttonPressed[1]=true;
	buttonPressed[3]=true;
	buttonPressed[9]=true;

	nSliders=3;
	sliders=new slider*[nSliders];
	sliders[0]=new slider(517,617,645,0);
	sliders[1]=new slider(517,687,645,0);	
	sliders[2]=new slider(517,755,645,0);

	header.loadImage("images/head.png");
	shadow.loadImage("images/shadow.png");
	
	//talk bubbles
	nBubbles = 6; 
	talkBubbles = new talkBubble*[nBubbles];   
	talkBubbles[0] = new talkBubble(0,0,"--", 100);
	talkBubbles[1] = new talkBubble(0,0,"--", 100);
	talkBubbles[2] = new talkBubble(0,0,"--", 100);
	talkBubbles[3] = new talkBubble(0,0,"--", 100);	
	talkBubbles[4] = new talkBubble(0,0,"--", 100);	
	talkBubbles[5] = new talkBubble(0,0,"--", 100);
	//sketch viewer
	sketchShareView.initViewer();
	firstTimeSketchTrigger=true;
	//webRender
	webRender.setupWebcore();
	//other parameters
	confirmSelection=false;
	lockedPersonID=0;
}

//--------------------------------------------------------------
void testApp::update(){
	ofBackground(bgColor.r,bgColor.g,bgColor.b);
	
	if (!recAndRep.getBPlayback()){
		g_kinectGrabber.Kinect_Update();
	}else{}
	//g_kinectGrabber.Kinect_Update();

	translateMouseX=(mouseX-RENDER_WIDTH)/SCALE;
	translateMouseY=(mouseY-105)/SCALE;
    
	// There is a race on m_rgbBuffer. To ensure that the color data is only updated to the front end when the data has been fully copied, we must use a critical section.
	// This alone does not guarantee that the read and write threads execute interchangably. To ensure starvation freedom, we introduce a "writeTurn" variable. 
	// It is set to true when m_rgbBuffer should be written to, and false when it should be read from. 
	// This way, both the front end and backend threads will access m_rgbBuffer, and thier access should not introduce a race.
	/*if(g_kinectGrabber.getWriteTurn() == false) {
		// load the RGBA values into a texture
		if(g_kinectGrabber.tryLock()) {
			colorAlphaPixels = g_kinectGrabber.Kinect_getAlphaPixels();
			if(colorAlphaPixels != NULL) {
				texColorAlpha.loadData(colorAlphaPixels, VIDEO_WIDTH,VIDEO_HEIGHT, GL_RGBA);
			}
			g_kinectGrabber.unlock();
		}
		g_kinectGrabber.setWriteTurn(true);
	}*/

	//add record and replay
	if (recAndRep.getBPlayback()){//if bPlayback
		if(!recAndRep.getPaused()){//while paused we can't search for playback range
			recAndRep.readFrame();
			colorAlphaPixels = recAndRep.getColorAlphaPixels();
			grayPixels =       recAndRep.getGrayPixels();
			rawTime = recAndRep.getRawTime();
			headPositionX =    recAndRep.getHeadPositionX();
			headPositionY =    recAndRep.getHeadPositionY();
			headPositionZ =    recAndRep.getHeadPositionZ();
			leftShoulderX =    recAndRep.getLeftShoulderX();
			leftShoulderY =    recAndRep.getLeftShoulderY();
			rightShoulderX =   recAndRep.getRightShoulderX();
			rightShoulderY =   recAndRep.getRightShoulderY();
			leftHandPX =       recAndRep.getLeftHandPX();
			leftHandPY =       recAndRep.getLeftHandPY();
			rightHandPX =      recAndRep.getRightHandPX();
			rightHandPY =	   recAndRep.getRightHandPY();
			/*if (!recAndRep.getSoundfile().getIsPlaying()) {
				recAndRep.getSoundfile().play();
			}*/
		}
    }else{//if !bPlayback
		//colorAlphaPixels = g_kinectGrabber.Kinect_getAlphaPixels();
		if(g_kinectGrabber.getWriteTurn() == false) {
		// load the RGBA values into a texture
		if(g_kinectGrabber.tryLock()) {
			colorAlphaPixels = g_kinectGrabber.Kinect_getAlphaPixels();
			//if(colorAlphaPixels != NULL) {
			//	texColorAlpha.loadData(colorAlphaPixels, VIDEO_WIDTH,VIDEO_HEIGHT, GL_RGBA);
			//}
			g_kinectGrabber.unlock();
		}
		g_kinectGrabber.setWriteTurn(true);
		}

		//grayPixels = (BYTE*)g_kinectGrabber.Kinect_getDepthPixels();
		time(&rawTime);
		g_kinectGrabber.getJointsPoints(); // Gets the skeleton joints' positions (up to rightHandPY)
		headPositionX=g_kinectGrabber.headJoints_x;
		headPositionY=g_kinectGrabber.headJoints_y;
		headPositionZ=g_kinectGrabber.headJoints_z;
		leftShoulderX = g_kinectGrabber.shoulderLeft_x;
		leftShoulderY = g_kinectGrabber.shoulderLeft_y;
		rightShoulderX = g_kinectGrabber.shoulderRight_x;
		rightShoulderY = g_kinectGrabber.shoulderRight_y;
		leftHandPX = g_kinectGrabber.handLeft_x;
		leftHandPY = g_kinectGrabber.handLeft_y;
		rightHandPX = g_kinectGrabber.handRight_x;
		rightHandPY = g_kinectGrabber.handRight_y;
	}
	//add record and replay
	//regardless of bPlayback's value, do below
	timeinfo = localtime (&rawTime );

	if(colorAlphaPixels != NULL) texColorAlpha.loadData(colorAlphaPixels, VIDEO_WIDTH,VIDEO_HEIGHT, GL_RGBA); // load the RGBA values into a texture
	//ka I'm unsure if old stored RGBA values can work with (VIDEO_WIDTH, VIDEO_HEIGHT), since it may change in between recordings. If it matters, store video width and height as well
	//grayPixels = (BYTE*)g_kinectGrabber.Kinect_getDepthPixels();	
	//if (grayPixels != NULL) texGray.loadData(grayPixels,DEPTH_WIDTH,DEPTH_HEIGHT, GL_RGBA);
	
	USHORT* depthBuff = g_kinectGrabber.Kinect_getDepthBuffer();
	
	if(recAndRep.getBRecord()){// && kinectRecorder.isOpened()) {
		recAndRep.storeFrame(colorAlphaPixels, grayPixels, rawTime, headPositionX, headPositionY, headPositionZ, leftShoulderX, leftShoulderY, rightShoulderX,
							rightShoulderY,leftHandPX, leftHandPY, rightHandPX, rightHandPY);
	}

	// TODO: move this somewhere else. Probably goes in the conference.cpp file?
	// Find the skeleton index of the individuals head position is closest to that of the audio position.
	double minSoundDiscrepancy = 60;
	bool personSpeaking = false;
	//printf("-------------------------------------------\n"); 
	//printf(" Head Positions \n"); 
	//printf("-------------------------------------------\n"); 
	
	bool peopleSelectedbyMouse[6];
	// Loop through all of the 
	for (int i = 0; i < 6; i ++) { 
	// TODO: value should be some constant indicating number of skeletons
		
		// print out the x values for each of the players head tracking
		printf("head %d x value: %d \n", i, g_kinectGrabber.headXValues[i]);
		printf("soundPixel: %f \n", g_kinectGrabber.soundPixel);

		// compare to find the reading with the smallest discrepancy from the sound localization
		double discrepancy = abs(g_kinectGrabber.headXValues[i] - g_kinectGrabber.soundPixel);
		if (discrepancy < minSoundDiscrepancy) {
			minSoundDiscrepancy = discrepancy;
			g_kinectGrabber.minDiscrepancyIdx = i;
			personSpeaking = true;
		} 

		//talk bubbles update
		int headPositionX = g_kinectGrabber.rightShoulderXValues[i]*scaleParam*SCALE+X_SHIFTS+25;
		int headPositionY = g_kinectGrabber.headYValues[i]*scaleParam*SCALE+105-30;
		talkBubbles[i]->updatePosition(headPositionX,headPositionY);
		talkBubbles[i]->updateTimer();

		//manual selection match tracked head; activating talkBubble in manual mode; only when mouse within the camera view
		if((translateMouseX>0)&&(translateMouseX<DEPTH_WIDTH)&&(translateMouseY>0)&&(translateMouseY<DEPTH_HEIGHT)){
			int closestPersonDepth=g_kinectGrabber.headZValues[i];
			int mouseDepth=depthBuff[(int)(translateMouseY*DEPTH_WIDTH+translateMouseX)];
			if (ABS(mouseDepth-closestPersonDepth)< DEPTH_THRESHOLD) {
				peopleSelectedbyMouse[i]=true;
				lockedPersonID=i;
			} else if (confirmSelection){
				peopleSelectedbyMouse[i]=true;
			} else {
				peopleSelectedbyMouse[i]=false;
			}
		}
		
		if(buttonPressed[2]){
			if (peopleSelectedbyMouse[i] && lockedPersonID==i) talkBubbles[i]->active=true;
			else talkBubbles[i]->active=false;
		}
	}

	// print the closest match
	if(g_kinectGrabber.minDiscrepancyIdx>=0 && g_kinectGrabber.minDiscrepancyIdx<=6) {
		closestID=g_kinectGrabber.minDiscrepancyIdx;
	} else {
		closestID=0;
	}
	//printf(" closest person : %i \n", closestID); 
	//printf("confirmed selection?: %s",(confirmSelection)?"true":"false");
	//printf("-------------------------------------------\n"); 
	//printf("skeleton tracked?: %s",	(g_kinectGrabber.isSkeletonTracked)? "true":"false");
	//printf("-------------------------------------------\n"); 

	
	if(buttonPressed[1]) focusRGB(colorAlphaPixels, depthBuff, focusPixels, blurPixels, &g_kinectGrabber,buttonPressed[3],buttonPressed[4],buttonPressed[5],maskValue, closestID, personSpeaking);	
	else if(buttonPressed[2] && !confirmSelection) focusRGB_manual(colorAlphaPixels, depthBuff, focusPixels, blurPixels, &g_kinectGrabber,buttonPressed[3],buttonPressed[4],buttonPressed[5],translateMouseX,translateMouseY);	
	else if(buttonPressed[2] && confirmSelection)  focusRGB_manualLocked(colorAlphaPixels, depthBuff, focusPixels, blurPixels, &g_kinectGrabber,buttonPressed[3],buttonPressed[4],buttonPressed[5],lockedPersonID);	
	
	
	int headAbove = 50; //the space above head to be captured
	int headBelow = MOBILE_HEIGHT - headAbove;
	int bodyLeft = MOBILE_WIDTH / 2 - 20;
	int bodyRight = MOBILE_WIDTH / 2;

	int headX = g_kinectGrabber.headXValues[closestID];
	int headY = g_kinectGrabber.headYValues[closestID];
	//Next, ensure that yStart & xStart not out of range.
	int yStart; //the starting Y value to be parsed
	int xStart; //the starting X value to be parsed
	if (headY - headAbove < 0) yStart = 0;
	else if (headY - headAbove + MOBILE_HEIGHT > DEPTH_HEIGHT) yStart = DEPTH_HEIGHT - MOBILE_HEIGHT;
	else yStart = headY - headAbove;

	if (headX - bodyLeft < 0) xStart = 0;
	else if (headX - bodyLeft + MOBILE_WIDTH > DEPTH_WIDTH) xStart = DEPTH_WIDTH - MOBILE_WIDTH;
	else xStart = headX - bodyLeft;



	for( int y = 0 ; y < MOBILE_HEIGHT ; y++ ){
	for( int x = 0 ; x < MOBILE_WIDTH ; x++ ) {
	int indexM = y * MOBILE_WIDTH + x;
	int indexF = (y + yStart) * DEPTH_WIDTH + x + xStart;
	mobilePixels[4*indexM + 0] = focusPixels[4*indexF + 0];
	mobilePixels[4*indexM + 1] = focusPixels[4*indexF + 1];
	mobilePixels[4*indexM + 2] = focusPixels[4*indexF + 2];
	mobilePixels[4*indexM + 3] = 255;

	mobileBlurPixels[4*indexM + 0] = blurPixels[4*indexF + 0];
	mobileBlurPixels[4*indexM + 1] = blurPixels[4*indexF + 1];
	mobileBlurPixels[4*indexM + 2] = blurPixels[4*indexF + 2];
	mobileBlurPixels[4*indexM + 3] = blurPixels[4*indexF + 3];
	}
	}


	texFocus.loadData(focusPixels,DEPTH_WIDTH,DEPTH_HEIGHT, GL_RGBA);
	texBlur.loadData(blurPixels,DEPTH_WIDTH,DEPTH_HEIGHT, GL_RGBA);
	texMobile.loadData(mobilePixels,MOBILE_WIDTH,MOBILE_HEIGHT, GL_RGBA);
	texMobileBlur.loadData(mobileBlurPixels,MOBILE_WIDTH,MOBILE_HEIGHT, GL_RGBA);


	texFocus.loadData(focusPixels,DEPTH_WIDTH,DEPTH_HEIGHT, GL_RGBA);
	texBlur.loadData(blurPixels,DEPTH_WIDTH,DEPTH_HEIGHT, GL_RGBA);

	//gesture
	gesture.cur_rhPx=g_kinectGrabber.rightHandXValues[closestID];
	
	//activating talkBubble in auto mode
	if(buttonPressed[1]) {
		talkBubbles[closestID]->active=true; //the active talk bubbles
		for(int j=closestID+1;j<6;j++) talkBubbles[j]->active=false; //de-active other bubbles
		for(int j=closestID-1;j>=0;j--) talkBubbles[j]->active=false;
	}

	//sketch viewer
	//if(g_kinectGrabber.headZValues[closestID]>1300) buttonPressed[6]=true;  //tack people's movement
	//else buttonPressed[6]=false;
	if(buttonPressed[6]) sketchShareView.close=false;
	else if(!buttonPressed[8]) sketchShareView.close=true;

	sketchShareView.update(g_kinectGrabber.rightHandXValues[closestID]*scaleParam,g_kinectGrabber.rightHandYValues[closestID]*scaleParam,640+20,0+25);

	if((gesture.last_rhPx!=0) && (gesture.cur_rhPx-gesture.last_rhPx)>100 && buttonPressed[6]){
		sketchShareView.zoomIn=true;
		gesture.last_rhPx=gesture.cur_rhPx;
	}else if (((gesture.cur_rhPx-gesture.last_rhPx)<100 && buttonPressed[6]) || (gesture.last_rhPx==0)) {
		gesture.last_rhPx=gesture.cur_rhPx;
	}

	if(buttonPressed[6] && firstTimeSketchTrigger) {
		sliders[2]->sliderPosX=137;
		firstTimeSketchTrigger=false;
	}
	if(buttonPressed[6]) sketchShareView.close=false;
	else if(!buttonPressed[6]) {
		sketchShareView.close=true;
		sketchShareView.zoomIn=false;
	}

	//webRender
	webRender.updateWebcore();
	webRender.updateWebcoreCoord(g_kinectGrabber.rightHandXValues[closestID]*scaleParam,g_kinectGrabber.rightHandYValues[closestID]*scaleParam,640+20,0+25);
	if(buttonPressed[8]) webRender.close=false;
	else if(!buttonPressed[8]) webRender.close=true;
}

//--------------------------------------------------------------
void testApp::draw(){
	ofEnableSmoothing();
	ofEnableAlphaBlending();

	int blurParam=120; //different mode has different blurParameter control
	//scaleParam=1;

		if(buttonPressed[3]) {
		if(sliders[0]->value!=NULL) blurParam=sliders[0]->value;
		scaleParam=1;
	}
	if(buttonPressed[4]) {
		blurParam=110;
		scaleParam=1;
		maskValue=sliders[1]->value;
	}
	if(buttonPressed[5]) {
		blurParam=110;
		scaleParam=sliders[2]->value;
	}

	//draw a layer entirely clear
	texFocus.draw(0+533,0+105,RENDER_WIDTH*scaleParam, RENDER_HEIGHT*scaleParam); //draw the focus texture	  //520*390

	//draw another blured layer on top, with alpha(skeleton)=0;
	blur.setBlurParams(4,(float)blurParam/100);
	blur.beginRender();
	texBlur.draw(0,0,DEPTH_WIDTH, DEPTH_HEIGHT); //always 0
	blur.endRender();
	blur.draw(0+533, 0+105, RENDER_WIDTH*scaleParam, RENDER_HEIGHT*scaleParam, true);

	//draw the mobile version
	//texMobile.draw(0+1610, 0, MOBILE_WIDTH, MOBILE_HEIGHT);
	//blur.setBlurParams(4,(float)200/100);
	//blur.beginRender(); 
	//texMobileBlur.draw(0, 0, MOBILE_WIDTH*640/360, MOBILE_HEIGHT);
	//blur.endRender();
	//blur.draw(0+1610, 0, MOBILE_WIDTH, MOBILE_HEIGHT, true);

	header.draw(0,0);
	shadow.draw(0,513);
	buttons[9]->drawFont(buttonPressed[9]);
	buttons[10]->drawFont(buttonPressed[10]);

	//if buttonPressed[9], LIVE MODE; if buttonPressed[10],RECORD MODE;
	if(buttonPressed[9]){ //live mode
		for(int i=0;i<3;i++) {
			buttons[i]->drawFont(buttonPressed[i]);   //draw 3 buttons always existing at the bottom
			buttons[i]->trigger=true;
		}
		for(int i=6;i<9;i++){
			buttons[i]->drawFont(buttonPressed[i]);   
			buttons[i]->trigger=true;
		}
		if(buttonPressed[0]){  //draw 3 buttons triggered by pressing the setUp button; boolean trigger is used to disable the button pressing if it's not shown on the screen
			for(int i=3;i<6;i++){
				buttons[i]->trigger=true;
				buttons[i]->drawFont(buttonPressed[i]);
			}
			for(int i=0;i<nSliders;i++) sliders[i]->drawSlider(80,400);
			sliders[0]->drawSlider(110,400);
			sliders[1]->drawSlider(4,0.01);
			sliders[2]->drawSlider(1,0.1);
		} else if(!buttonPressed[0]){
			for(int i=3;i<6;i++) buttons[i]->trigger=false;
		}
	}
	else if (buttonPressed[10]){ //review mode
		for(int i=0;i<9;i++) buttons[i]->trigger=false; //disable all the buttons used in live mode
		//replay and record
		/*recAndRep.drawButtons();
		recAndRep.drawSliders();
		if (recAndRep.getBRightHandUp()){//make func for this in recadnrep
		ofCircle(60,VIDEO_HEIGHT - 20,20);
		printf("hands up detected");
		}
		recAndRep.drawSmallButtons();*/
	}
	
	//sketch viewer
	if(!sketchShareView.close){
		sketchShareView.scale=ofMap(sliders[2]->value,1,0.1,1,4);
		sketchShareView.drawBg();
		sketchShareView.drawVideo();
	}
	//webRender
	if(!webRender.close){
		webRender.drawBg();
		sliders[2]->value=0.5;
		webRender.drawWebcore(ofMap(sliders[2]->value,1,0.1,1,4));
		buttonPressed[5]=true;
	}
	//talk bubble
	for(int i=0;i<nBubbles;i++) talkBubbles[i]->draw();
	ofDisableAlphaBlending();	
	//replay and record
	recAndRep.drawButtons();
	recAndRep.drawSliders();
	if (recAndRep.getBRightHandUp()){//make func for this in recadnrep
		ofCircle(60,VIDEO_HEIGHT - 20,20);
	}
	recAndRep.drawSmallButtons();
	//unlock KinectGrabber
	g_kinectGrabber.unlock();
	g_kinectGrabber.setWriteTurn(true);
	
}
//-------------------------------------------------------------
void testApp::exit(){
	//printf("cleaning up\n");
	g_kinectGrabber.Kinect_UnInit();
	recAndRep.close();
	free(focusPixels);
	free(blurPixels);
}
//--------------------------------------------------------------
//int isNameTyping=false;
void testApp::keyPressed(int key){
 /*
    for (int i=0;i<nBubbles;i++){
        if (talkBubbles[i]->active && buttonPressed[1]) {
            if(key == '-') talkBubbles[i]->name.erase();  //erase name input for the active bubble
            else talkBubbles[i]->name.append(1,(char)key); //type in name for the active bubble
        }
    }
	*/

	if(key == 'r') {
		if (!recAndRep.getBPlayback()){
			recAndRep.standardRecord();
		}
	}
	else if (key == 'p'){
		if (!recAndRep.getBRecord()){
			recAndRep.standardReplay();
		}
	}
	else if (key =='s'){
		if (recAndRep.getBRecord() || recAndRep.getBPlayback()){
			recAndRep.standardStop();
		}
	}
	else if (key == 'b'){
		if (recAndRep.getBPlayback()){
			if (recAndRep.getPaused()){
				recAndRep.play();
			}
			else{
				recAndRep.pause();
			}
		}
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
}

//------------- -------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	sprintf(eventString, "mouseMoved = (%i,%i)", x, y);
	
	if(buttonPressed[8]) webRender.injectMouseMoved(x,y);	
}


//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	for(int i=0;i<nSliders;i++) sliders[i]->getSliderPosX(x,y);

	if(buttonPressed[8]) webRender.injectMouseDragged(x,y);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	//button pressing
	if(buttons[0]->buttonPressed(x,y)) buttonPressed[0]=!buttonPressed[0]; //setUpbutton
	if(buttons[6]->buttonPressed(x,y)) buttonPressed[6]=!buttonPressed[6]; //turn on/off the sketchviewer
	if(buttons[7]->buttonPressed(x,y)) buttonPressed[7]=!buttonPressed[7]; //turn on/off the talkBubble
	if(buttons[8]->buttonPressed(x,y)) buttonPressed[8]=!buttonPressed[8]; //turn on/off the iPad
	
	if(buttons[9]->buttonPressed(x,y)) {
		buttonPressed[9]=true; //turn on/off the live mode
		buttonPressed[10]=false;
	}
	if(buttons[10]->buttonPressed(x,y)) {
		buttonPressed[10]=true; //turn on/off the review mode
		buttonPressed[9]=false;
	}

	
	if(buttons[1]->buttonPressed(x,y)){
		buttonPressed[1]=true;
		buttonPressed[2]=false;
	} 
	if(buttons[2]->buttonPressed(x,y)){
		buttonPressed[2]=true;
		buttonPressed[1]=false;
	}
	if(buttons[3]->buttonPressed(x,y)){
		buttonPressed[3]=true;
		buttonPressed[4]=false;
		buttonPressed[5]=false;
	}
	if(buttons[4]->buttonPressed(x,y)){
		buttonPressed[4]=true;
		buttonPressed[3]=false;
		buttonPressed[5]=false;
	}	
	if(buttons[5]->buttonPressed(x,y)){
		buttonPressed[5]=true;
		buttonPressed[3]=false;
		buttonPressed[4]=false;
		printf("buttonPressed \n");
	}

	if(buttonPressed[6]) sketchShareView.zoomDetect(x,y);
	
	if(buttonPressed[8]){
	webRender.zoomDetect(x,y);
	webRender.injectMousePressed();
	}

	//manual mode, click to lock to the selected person
	if(buttonPressed[2] && y<480){
			confirmSelection=!confirmSelection;
	}

	//replay and record
	if (recAndRep.getStopButtonPressed(x,y)){
		recAndRep.standardStop();
	}
	else if (recAndRep.getReplayButtonPressed(x,y)){
		recAndRep.standardReplay();
	}
	else if (recAndRep.getRecordButtonPressed(x,y)){
		recAndRep.standardRecord();
	}
	else if (recAndRep.getGoodIdeaButtonPressed(x,y)){
		recAndRep.setSmallButtonActive(true);
	}
	else if (recAndRep.getTimerSliderPressed(x,y).first){
		if (recAndRep.getBPlayback() && !recAndRep.getPaused()){
			recAndRep.skipTo(recAndRep.getTimerSliderPressed(x,y).second);
		}
	}
	else if (recAndRep.getSecondSliderPressed(x,y).first){
		if (recAndRep.getBPlayback() && !recAndRep.getPaused()){
			recAndRep.skipTo(recAndRep.getSecondSliderPressed(x,y).second);
		}
	}
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	webRender.injectMouseReleased();
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}
