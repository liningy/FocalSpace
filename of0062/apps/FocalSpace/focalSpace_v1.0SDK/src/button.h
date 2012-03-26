#ifndef _OF_BUTTON
#define _OF_BUTTON

#include <string>

class button{

public:
	//data
	int originX, originY;   //where the button starts
	int alternateOriginX, alternateOriginY; //origin for the 2nd picture
	int bWidth, bHeight;    //the size of the button
	string buttonName;
	string typeContents;
	ofTrueTypeFont font;
	ofColor selectedC, defaultC;
	int fontSize;
	bool trigger;
	ofImage img_a,img_b;
	
	//constructor
	button(string _buttonName, int _originX, int _originY, int _bWidth, int _bHeight, bool _trigger,string _imgName_a, string _imgName_b);

	
	//functionality
	void drawFont(bool _buttonPressed);
	void drawFontwithAltPic(bool _buttonPressed, bool alternatePic = false);//alternate pic forces secondary pic to be used regardless of buttonPress state
	bool buttonPressed(int x, int y);
	void typeWindow(int xPosition, int yPosition);
	void updateTrigger(bool newTrigger);//ka I'm not sure how triggers were updated in the past, or if they had to be
};

#endif

