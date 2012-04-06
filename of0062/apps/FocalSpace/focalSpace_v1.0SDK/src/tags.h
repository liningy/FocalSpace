#ifndef _TAGS
#define _TAGS

class tags{
public:
	tags();
	virtual ~tags();
	void init();
	void close();
	
	
	void resetTagCounters();
	void resetTagInfo();
	void resetTags();

	bool getBRightHandUp() {return bRightHandUp;};
	void checkRightHandUp(int rightHandPY, int headPositionPY);

	void setTagInfo(int genCat, int specCat, int value);
	int getTagInfo(int genCat, int specCat, int index);

private: 
	static const int lenGenCat = 3;
	static const int lenSpecCat = 6;
	static const int lenGesArray = 10;

	bool bRightHandUp; // is true if the skeleton's right hand is raised for at least 20 frames
	int rightHandUpCount; //counts the number of frames that right hand has been up. resets every 100 frames
	int tagInfo[lenGenCat][lenSpecCat][lenGesArray];// this follows the definition of storeSideInfo in kinectRecorder.cpp
	/*
	Guide:
	genCat 0 = number of frames
		specCat 0 = number of frames
	genCat 1 = gestures
		specCat 0 = thumbs up
	genCat 2 = faces
		specCat 0 = person ID 0
		specCat 1 = person ID 1
		specCat 2 = person ID 2
		specCat 3 = person ID 3
		specCat 4 = person ID 4
		specCat 5 = person ID 5
	*/
	int tagCounters[lenGenCat][lenSpecCat][1]; //stores where the pointer is (sort of to help append)
	int currCounter;
	int smallRHULocations[10];//the frame numbers of up to 20 locations on the slider at which small right hand raised buttons should be (stores the last 20)
	
};

#endif