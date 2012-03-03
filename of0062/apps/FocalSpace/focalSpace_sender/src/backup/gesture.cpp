#include "gesture.h"
#include "ofMain.h"


Gesture::Gesture()
{
	cur_rhPx=0;
	last_rhPx=0;
}



void Gesture::get_last_rhPx(){
	if(cur_rhPx!=last_rhPx); 
}

bool Gesture::isRh_r(){
	if((cur_rhPx-last_rhPx)>100) rh_r=true;
	else rh_r=false;
	
	return rh_r;
}

