#include "tags.h"

tags::tags(){
	rightHandUpCount = 0;
}
//--------------------------------------------------------------------
tags::~tags() {
	close();
}

void tags::init(){
	bRightHandUp = false;
	rightHandUpCount = 0;
	for(int i=0;i<10;i++){
		smallRHULocations[i] = 0;//initializes first to 0, and all unspecified ones to 0
	}
}
void tags::resetTags() {
	resetRHR();
}
int tags::getRHR(int index) {
	return smallRHULocations[index];
}
void tags::setRHR(int index, int value) {
	smallRHULocations[index] = value;
}
/////GESTURE CHECKS (return booleans)
void tags::checkRightHandUp(int rightHandPY, int headPositionY){
	//don't use this unless setting right hand up since there is a counter on it and the answer may differ in between calls
	//dimensions work oppositely top left is 0,0. Then y goes down i think? at least for comparisons
	if ((rightHandPY > headPositionY) || rightHandUpCount >= 100){
		rightHandUpCount = 0;
	}
	if ((rightHandPY < headPositionY)){
		rightHandUpCount ++;
		if (rightHandUpCount == 5){ //20
			bRightHandUp = true;
			return;
		}
	}
	bRightHandUp = false;
	return;
}

void tags::close(){
	bRightHandUp = false;
	rightHandUpCount = 0;
	for(int i=0;i<10;i++){
		smallRHULocations[i] = 0;//initializes first to 0, and all unspecified ones to 0
	}
}