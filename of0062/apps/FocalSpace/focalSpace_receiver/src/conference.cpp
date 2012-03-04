#include "conference.h"

///////////////////////////////////////////////////////////////////////////////
// TODO: Beginning attempt at state machine infrastructure. Doesn't do anything yet.
// Might want to expand or trash this portion.

struct conferenceManager {
		applicationState appState;
} conference;

void conference_init() {
	conference.appState = IDLE;
	//peopleSelectedbyMouse=false;
}


/*
void conference_update() {
	if (conference.appState == IDLE) {
		// do nothing; everything should be blurry
	} else if (conference.appState == SPEAKING) {
		//focus in on the speaker
	}
}
*/
//////////////////////////////////////////////////////////////////////////////


/*
	focusRGB: Sets the color pixels for display.
	Params: 
		videoBuff - RGBA values (4 BYTES) from the sensor
		depthBuff - depth values (BYTE) from the sensor 
		focusBuff - RGBA values correlated to depthBuff
		blurBuff - RGBA values of the blurred/blacked out image to paint over the focusBuff values
*/

void focusRGB(BYTE* videoBuff, USHORT* depthBuff, BYTE * focusBuff, BYTE* blurBuff, KinectGrabber* kinectGrabber,  bool focusMode, bool blackMode, bool zoomMode, float maskValue) {

	if (videoBuff && depthBuff) {
		LONG* pcolorx = new LONG();
		LONG* pcolory = new LONG();

		// Go through the buffer to prepare it for drawing
		int max_index = DEPTH_WIDTH * DEPTH_HEIGHT * 4;
		for( int y = 0 ; y < DEPTH_HEIGHT ; y++ ){
			for( int x = 0 ; x < DEPTH_WIDTH ; x++ ) {

				// Use the Kinect SDK to find the color pixel from the depth pixel
				kinectGrabber->Kinect_ColorFromDepth(x, y, pcolorx, pcolory);
				int index = (y * DEPTH_WIDTH) + x;
				int	color_index = ((*pcolory*VIDEO_WIDTH) + *pcolorx);
				
				// focusBuff points to the r,g,b video stream
				focusBuff[4*index + 0] = videoBuff[4*color_index + 0]; 
				focusBuff[4*index + 1] = videoBuff[4*color_index + 1];
				focusBuff[4*index + 2] = videoBuff[4*color_index + 2];
				focusBuff[4*index + 3] = 255;

				// use the commented portion of these lines to see a faded blurred background instead of a black background
				if(focusMode){
					blurBuff[4*index + 0] = videoBuff[4*color_index + 0]; //videoBuff[4*color_index + 0] / 2;
					blurBuff[4*index + 1] = videoBuff[4*color_index + 1]; //videoBuff[4*color_index + 1] / 2;
					blurBuff[4*index + 2] = videoBuff[4*color_index + 2]; //videoBuff[4*color_index + 2] / 2;
				}
				if(blackMode){
					blurBuff[4*index + 0] = videoBuff[4*color_index + 0]/maskValue; //videoBuff[4*color_index + 0] / 2;
					blurBuff[4*index + 1] = videoBuff[4*color_index + 1]/maskValue; //videoBuff[4*color_index + 1] / 2;
					blurBuff[4*index + 2] = videoBuff[4*color_index + 2]/maskValue; //videoBuff[4*color_index + 2] / 2;
				}
				if(zoomMode){
					blurBuff[4*index + 0] = 0; //videoBuff[4*color_index + 0] / 2;
					blurBuff[4*index + 1] = 0; //videoBuff[4*color_index + 1] / 2;
					blurBuff[4*index + 2] = 0; //videoBuff[4*color_index + 2] / 2;
				}
				// Determine which pixels on the blur layer should be visible.
				// If there are any detected skeletons, we want to focus in on one of them
				if (kinectGrabber->minDiscrepancyIdx >= 0 &&  kinectGrabber->minDiscrepancyIdx <= 6)
				//TODO: change the 6 to some constant indicating that total number of skeletons, like NUI_SKELETON_COUNT
				{
					
					// Get the head's depth of the person's X value that is closest to the audio
					//if that pixel's depth is near the speaker, make the blurred image invisible (alpha = 255)
					//otherwise, set the blur visible (alpha = 0)
					int headPositionZ = kinectGrabber->headZValues[kinectGrabber->minDiscrepancyIdx];
					int headPositionX = kinectGrabber->headXValues[kinectGrabber->minDiscrepancyIdx];

					if (depthBuff[index] > headPositionZ + DEPTH_THRESHOLD  || depthBuff[index] < headPositionZ - DEPTH_THRESHOLD ) {
						blurBuff[4*index + 3] = 255; //fully opaque
					} else {
						blurBuff[4*index + 3] = 0;   //fully transparent
					}

					//focus within a certain range around the active player
					if ((x<headPositionX-180) || (x>headPositionX+180)) blurBuff[4*index + 3] = 255;

				// If there are no detected skeletons, just fade out everything
				} else {
					blurBuff[4*index + 3] = 255;
				}
			}
		}  
		free(pcolorx);
		free(pcolory);
	  }
}


void focusRGB_manual(BYTE* videoBuff, USHORT* depthBuff, BYTE * focusBuff, BYTE* blurBuff, KinectGrabber* kinectGrabber,  bool focusMode, bool blackMode, bool zoomMode, int mouseX, int mouseY) {

	if (videoBuff && depthBuff) {
		LONG* pcolorx = new LONG();
		LONG* pcolory = new LONG();

		// Go through the buffer to prepare it for drawing
		int max_index = DEPTH_WIDTH * DEPTH_HEIGHT * 4;
		for( int y = 0 ; y < DEPTH_HEIGHT ; y++ ){
			for( int x = 0 ; x < DEPTH_WIDTH ; x++ ) {

				// Use the Kinect SDK to find the color pixel from the depth pixel
				kinectGrabber->Kinect_ColorFromDepth(x, y, pcolorx, pcolory);
				int index = (y * DEPTH_WIDTH) + x;
				int	color_index = ((*pcolory*VIDEO_WIDTH) + *pcolorx);
				
				// focusBuff points to the r,g,b video stream
				focusBuff[4*index + 0] = videoBuff[4*color_index + 0]; 
				focusBuff[4*index + 1] = videoBuff[4*color_index + 1];
				focusBuff[4*index + 2] = videoBuff[4*color_index + 2];
				focusBuff[4*index + 3] = 255;

				// use the commented portion of these lines to see a faded blurred background instead of a black background
				if(focusMode){
					blurBuff[4*index + 0] = videoBuff[4*color_index + 0]; //videoBuff[4*color_index + 0] / 2;
					blurBuff[4*index + 1] = videoBuff[4*color_index + 1]; //videoBuff[4*color_index + 1] / 2;
					blurBuff[4*index + 2] = videoBuff[4*color_index + 2]; //videoBuff[4*color_index + 2] / 2;
				}
				if(blackMode){
					blurBuff[4*index + 0] = videoBuff[4*color_index + 0]/3; //videoBuff[4*color_index + 0] / 2;
					blurBuff[4*index + 1] = videoBuff[4*color_index + 1]/3; //videoBuff[4*color_index + 1] / 2;
					blurBuff[4*index + 2] = videoBuff[4*color_index + 2]/3; //videoBuff[4*color_index + 2] / 2;
				}
				if(zoomMode){
					blurBuff[4*index + 0] = 0; //videoBuff[4*color_index + 0] / 2;
					blurBuff[4*index + 1] = 0; //videoBuff[4*color_index + 1] / 2;
					blurBuff[4*index + 2] = 0; //videoBuff[4*color_index + 2] / 2;
				}
				// Determine which pixels on the blur layer should be visible.
				// If there are any detected skeletons, we want to focus in on one of them
				if (kinectGrabber->minDiscrepancyIdx >= 0 &&  kinectGrabber->minDiscrepancyIdx <= 6)
				//TODO: change the 6 to some constant indicating that total number of skeletons, like NUI_SKELETON_COUNT
				{
					
					// Get the depth of the pixels where the mouse is pointing at
					int pointingPositionZ=depthBuff[mouseY*DEPTH_WIDTH+mouseX];
					if (depthBuff[index] > pointingPositionZ + DEPTH_THRESHOLD  || depthBuff[index] < pointingPositionZ - DEPTH_THRESHOLD ) {
						blurBuff[4*index + 3] = 255; //fully opaque
					} else {
						blurBuff[4*index + 3] = 0;   //fully transparent
					}

					// Get the depth of the pixels where the head is
					int headPositionZ = kinectGrabber->headZValues[kinectGrabber->minDiscrepancyIdx];
					//printf("pointingPositionZ %d \n",pointingPositionZ);
					//printf("headPositionZ %d \n",headPositionZ);
					//if (ABS(pointingPositionZ-headPositionZ)< 3000) peopleSelectedbyMouse=true;
					//else peopleSelectedbyMouse=false;
					
				// If there are no detected skeletons, just fade out everything
				} else {
					blurBuff[4*index + 3] = 255;
				}
			}
		}  
		free(pcolorx);
		free(pcolory);
	  }
}


void focusRGB_manualLocked(BYTE* videoBuff, USHORT* depthBuff, BYTE * focusBuff, BYTE* blurBuff, KinectGrabber* kinectGrabber,  bool focusMode, bool blackMode, bool zoomMode, int lockedPersonID) {

	if (videoBuff && depthBuff) {
		LONG* pcolorx = new LONG();
		LONG* pcolory = new LONG();

		// Go through the buffer to prepare it for drawing
		int max_index = DEPTH_WIDTH * DEPTH_HEIGHT * 4;
		for( int y = 0 ; y < DEPTH_HEIGHT ; y++ ){
			for( int x = 0 ; x < DEPTH_WIDTH ; x++ ) {

				// Use the Kinect SDK to find the color pixel from the depth pixel
				kinectGrabber->Kinect_ColorFromDepth(x, y, pcolorx, pcolory);
				int index = (y * DEPTH_WIDTH) + x;
				int	color_index = ((*pcolory*VIDEO_WIDTH) + *pcolorx);
				
				// focusBuff points to the r,g,b video stream
				focusBuff[4*index + 0] = videoBuff[4*color_index + 0]; 
				focusBuff[4*index + 1] = videoBuff[4*color_index + 1];
				focusBuff[4*index + 2] = videoBuff[4*color_index + 2];
				focusBuff[4*index + 3] = 255;

				// use the commented portion of these lines to see a faded blurred background instead of a black background
				if(focusMode){
					blurBuff[4*index + 0] = videoBuff[4*color_index + 0]; //videoBuff[4*color_index + 0] / 2;
					blurBuff[4*index + 1] = videoBuff[4*color_index + 1]; //videoBuff[4*color_index + 1] / 2;
					blurBuff[4*index + 2] = videoBuff[4*color_index + 2]; //videoBuff[4*color_index + 2] / 2;
				}
				if(blackMode){
					blurBuff[4*index + 0] = videoBuff[4*color_index + 0]/3; //videoBuff[4*color_index + 0] / 2;
					blurBuff[4*index + 1] = videoBuff[4*color_index + 1]/3; //videoBuff[4*color_index + 1] / 2;
					blurBuff[4*index + 2] = videoBuff[4*color_index + 2]/3; //videoBuff[4*color_index + 2] / 2;
				}
				if(zoomMode){
					blurBuff[4*index + 0] = 0; //videoBuff[4*color_index + 0] / 2;
					blurBuff[4*index + 1] = 0; //videoBuff[4*color_index + 1] / 2;
					blurBuff[4*index + 2] = 0; //videoBuff[4*color_index + 2] / 2;
				}
				// Determine which pixels on the blur layer should be visible.
				// If there are any detected skeletons, we want to focus in on one of them
				if (kinectGrabber->minDiscrepancyIdx >= 0 &&  kinectGrabber->minDiscrepancyIdx <= 6)
				//TODO: change the 6 to some constant indicating that total number of skeletons, like NUI_SKELETON_COUNT
				{
					int headPositionZ = kinectGrabber->headZValues[lockedPersonID];
					int headPositionX = kinectGrabber->headXValues[lockedPersonID];

					if (depthBuff[index] > headPositionZ + DEPTH_THRESHOLD  || depthBuff[index] < headPositionZ - DEPTH_THRESHOLD ) {
						blurBuff[4*index + 3] = 255; //fully opaque
					} else {
						blurBuff[4*index + 3] = 0;   //fully transparent
					}

					//focus within a certain range around the active player
					if ((x<headPositionX-180) || (x>headPositionX+180)) blurBuff[4*index + 3] = 255;
				} else {
					blurBuff[4*index + 3] = 255;
				}
			}
		}  
		free(pcolorx);
		free(pcolory);
	  }
}




/*
//creat transition between 0 to 255, not used anymore
void adjustOver(int range, BYTE * overBuff) {
	BYTE * new_alpha_buff;
	new_alpha_buff = (BYTE*) malloc (DEPTH_WIDTH*DEPTH_HEIGHT*sizeof(BYTE));

	for( int y = 0 ; y < DEPTH_HEIGHT ; y++ ){
			for( int x = 0 ; x < DEPTH_WIDTH ; x++ ) {
				
				int index = (y*DEPTH_WIDTH) + x;
	
				int sum = 0;
				int divisors = 0;
				
				for (int m = 0; m < range; m ++) {
					int w_offset = m * DEPTH_WIDTH;
					for (int n = 0; n < range; n ++) {
						int new_index = index - w_offset - n;
						if (new_index > 0) {
							sum += overBuff[4*(new_index) +3];
							divisors ++;
						}
						
						new_index = index + w_offset + n;
						if (new_index <  DEPTH_WIDTH* DEPTH_HEIGHT) {
							sum += overBuff[4*(new_index) +3];
							divisors ++;
						}

						new_index = index - w_offset + n;
						if (new_index > 0 && new_index <  DEPTH_WIDTH* DEPTH_HEIGHT) {
							sum += overBuff[4*(new_index) +3];
							divisors ++;
						}
				
						new_index = index + w_offset - n;
						if (new_index > 0 && new_index <  DEPTH_WIDTH* DEPTH_HEIGHT) {
							sum += overBuff[4*(new_index) +3];
							divisors ++;
						}

					}
				}
				new_alpha_buff[index] = (unsigned char)(sum / divisors);
			}
	}
	
	
	for( int y = 0 ; y < DEPTH_HEIGHT ; y++ ){
			for( int x = 0 ; x < DEPTH_WIDTH ; x++ ) {
				overBuff[4 * ((y*DEPTH_WIDTH) + x) + 3] = new_alpha_buff[(y*DEPTH_WIDTH) + x];
			}
	}

	free (new_alpha_buff);
}
*/