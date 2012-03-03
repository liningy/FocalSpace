#ifndef _OF_GUESTURE
#define _OF_GUSTURE

class Gesture
{
public:
	Gesture();

	void setGestureValue(int _cur_rhPx);

	bool rh_r; //right hand swiping towards  the right
    bool isRh_r();
	void get_last_rhPx(); 
	
	int last_rhPx; //last right hand position X
	int cur_rhPx;  //current right hand position X




};

#endif

