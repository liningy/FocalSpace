#ifndef _OF_BUTTON
#define _OF_BUTTON

#include <string>

class button{

public:
	//data
	int originX, originY;   //where the button starts
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
	bool buttonPressed(int x, int y);
	void typeWindow(int xPosition, int yPosition);
};

#endif

