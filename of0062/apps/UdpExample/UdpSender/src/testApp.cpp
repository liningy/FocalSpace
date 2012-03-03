#include "testApp.h"

#define RECONNECT_TIME 400



//--------------------------------------------------------------
void testApp::setup(){	 
	/////////////////network stuff//////////////////////
	// we don't want to be running to fast
	ofSetVerticalSync(true);
	ofSetFrameRate(30);
	
	//load our type
	mono.loadFont("type/mono.ttf",9);
	message = "";
    //create the socket and set to send to 127.0.0.1:11999
	udpConnection.Create();
	udpConnection.Connect("18.111.46.63",11999);
	udpConnection.SetNonBlocking(true);
	/////////////////network stuff//////////////////////
		
}


//--------------------------------------------------------------
void testApp::update(){
	
	ofBackground(100,100,100);

	/////////////////network stuff//////////////////////
	message = "sending contents";
	int sent = udpConnection.Send(message.c_str(),message.length());
	/////////////////network stuff//////////////////////
}

//--------------------------------------------------------------
void testApp::draw(){

}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	
}


//--------------------------------------------------------------
void testApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}
