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
	resetTags();
}

void tags::resetTagCounters(){
	for (int i = 0; i < lenGenCat; i++){
		for (int j = 0; j < lenSpecCat; j ++){
			for (int k = 0; k < lenGesArray; k ++){
				tagCounters[i][j][k] = 0;
			}
		}
	}
}

void tags::resetTagInfo(){
	for (int i = 0; i < lenGenCat; i++){
		for (int j = 0; j < lenSpecCat; j ++){
			for (int k = 0; k < lenGesArray; k ++){
				tagInfo[i][j][k] = -1;
			}
		}
	}
}

void tags::resetTags(){
	resetTagInfo();
	resetTagCounters();
}

void tags::setTagInfo(int genCat, int specCat, int value){
	currCounter = tagCounters[genCat][specCat][0];
	tagInfo[genCat][specCat][currCounter] = value;
	currCounter = (currCounter + 1) % lenGesArray;
	tagCounters[genCat][specCat][0] = currCounter;
}
int tags::getTagInfo(int genCat, int specCat, int index){
	return tagInfo[genCat][specCat][index];

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
	resetTags();
}