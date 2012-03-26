//#pragma once

#ifndef _CONF
#define _CONF

#include <windows.h>
#include "grabVideo.h"
#include "ofMain.h"

// number of millimeters threshold between a skeleton's depth reading and visible image
const int                     DEPTH_THRESHOLD = 350;
const int                     RENDER_WIDTH = 540;
const int                     X_SHIFTS=540;
const int                     RENDER_HEIGHT= 405;
const float                   SCALE=0.844;
const int                     MOBILE_WIDTH= 360;
const int                     MOBILE_HEIGHT=480;

//bool peopleSelectedbyMouse;

// not used yet
enum applicationState {IDLE, SPEAKING}; 
void conference_init();

// sets the color pixels for display_activeMode
void focusRGB(BYTE* videoBuff, USHORT* playerBuff, BYTE* focusBuff, BYTE* blurBuff, KinectGrabber* kinectGrabber, bool focusMode, bool blackMode, bool zoomMode, float maskValue, int closestID, bool personSpeaking);
void focusRGB_manual(BYTE* videoBuff, USHORT* playerBuff, BYTE* focusBuff, BYTE* blurBuff, KinectGrabber* kinectGrabber, bool focusMode, bool blackMode, bool zoomMode, int mouseX, int mouseY);
void focusRGB_manualLocked(BYTE* videoBuff, USHORT* playerBuff, BYTE* focusBuff, BYTE* blurBuff, KinectGrabber* kinectGrabber, bool focusMode, bool blackMode, bool zoomMode, int lockedPersonID);

//void focusRGB(BYTE* videoBuff, USHORT* depthBuff,BYTE* focusBuff, BYTE* blurBuff, KinectGrabber* kinectGrabber, int pointX, int pointY, bool activeFocus);

//no longer used
void adjustOver(int range, BYTE* blurBuff);

#endif