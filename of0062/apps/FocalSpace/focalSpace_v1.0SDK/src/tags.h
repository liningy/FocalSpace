#ifndef _TAGS
#define _TAGS

class tags{
public:
	tags();
	virtual ~tags();
	void init();
	void close();
	void resetRHR() {rightHandUpCount = 0;};
	bool getBRightHandUp() {return bRightHandUp;};
	void resetTags();
	int getRHR(int index);
	void setRHR(int index, int value);
	void checkRightHandUp(int rightHandPY, int headPositionPY);

private: 
	bool bRightHandUp; // is true if the skeleton's right hand is raised for at least 20 frames
	int rightHandUpCount; //counts the number of frames that right hand has been up. resets every 100 frames
	int smallRHULocations[10];//the frame numbers of up to 20 locations on the slider at which small right hand raised buttons should be (stores the last 20)
	
};

#endif