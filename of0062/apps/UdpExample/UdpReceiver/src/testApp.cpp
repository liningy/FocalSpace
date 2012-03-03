#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	// we don't want to be running to fast
	ofSetVerticalSync(true);
	ofSetFrameRate(30);
	
	//load our type
	mono.loadFont("type/mono.ttf", 9);
	monosm.loadFont("type/mono.ttf", 8);

    //create the socket and bind to port 11999
	udpConnection.Create();
	udpConnection.Bind(11999);
	udpConnection.SetNonBlocking(true);

	ofBackground(255, 255, 255);
	ofSetBackgroundAuto(false);
}

//--------------------------------------------------------------
void testApp::update(){
	ofBackground(100,100,100);

	char udpMessage[2304];
	udpConnection.Receive(udpMessage,2304);
	string message=udpMessage;

	
	if(message!=""){
		cout << message << endl;
	}

}
//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(0xffffff);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
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
