#include "webRender.h"

void WebRender::setupWebcore(){
	awe_webcore_initialize_default();

    webView = awe_webcore_create_webview(WEB_WIDTH, WEB_HEIGHT, false);
	//awe_webview* webView = awe_webcore_create_webview(WIDTH, HEIGHT, false);
    awe_string* url_str = awe_string_create_from_ascii(URL, strlen(URL));

    awe_webview_load_url(webView, url_str, awe_string_empty(), awe_string_empty(), awe_string_empty());

    // Wait for or WebView to finish loading
    while(awe_webview_is_loading_page(webView))
    {
        awe_webcore_update();
    }

	renderBuffer = awe_webview_render(webView);

	

	pixelBuffer = new unsigned char [WEB_WIDTH*WEB_HEIGHT*4];
	rightPixelBuffer = new unsigned char [WEB_WIDTH*WEB_HEIGHT*3];
	texColor.allocate(WEB_WIDTH,WEB_HEIGHT,GL_RGB);

	//for render the GUI
	webWidth_l 		= 605;//602; // try to grab at this size. 
	webHeight_l 	= 430;//451;
	webWidth_s 		= 200;
	webHeight_s 	= 150;
	bgSmall.loadImage("images/smallerViewer.png");
	bgBig.loadImage("images/biggerViewer.png");
	zoomIn=false;
	close=false;
	bgX_small=0;
	bgY_small=0;
	bgX_small_current=0;
	bgY_small_current=0;
	bgX_large=0;
	bgY_large=0;
}

void WebRender::updateWebcoreCoord(int _xSmall, int _ySmall, int _xLarge, int _yLarge){
	bgX_small_current=_xSmall;
	bgY_small_current=_ySmall;
	if(abs(bgX_small-bgX_small_current)>20){
		bgX_small=bgX_small_current;
		bgY_small=bgY_small_current;
	}

	bgX_large=_xLarge;
	bgY_large=_yLarge;
}

void WebRender::drawBg(){	
		if(!zoomIn){
			bgSmall.draw(bgX_small,bgY_small);
		} else {
			bgBig.draw(bgX_large,bgY_large);
		}	
}

void WebRender::convertBGRAToRGB(unsigned char* bgraBuffer, unsigned char* rgbBuffer, int width, int height){
	for(int i = 0; i < width * height; i++)
    {
        rgbBuffer[i * 3 + 2] = bgraBuffer[i * 4 + 0];
        rgbBuffer[i * 3 + 1] = bgraBuffer[i * 4 + 1];
        rgbBuffer[i * 3 + 0] = bgraBuffer[i * 4 + 2];
    }
}

void WebRender::updateWebcore(){
	Sleep(50);
    awe_webcore_update();
	rPixelBuffer = awe_renderbuffer_get_buffer(renderBuffer);
	pixelBuffer = const_cast<unsigned char *> (rPixelBuffer);
	convertBGRAToRGB(pixelBuffer,rightPixelBuffer,WEB_WIDTH, WEB_HEIGHT);
	texColor.loadData(rightPixelBuffer, WEB_WIDTH, WEB_HEIGHT, GL_RGB);	
}

void WebRender::drawWebcore(int scale){
	ofPushMatrix();	
		//ofTranslate(bgX,bgY);
		ofSetColor(0xffffff);
		if(!zoomIn){
			ofTranslate(bgX_small, bgY_small);
			texColor.draw(3,36,webWidth_s*scale,webHeight_s*scale);
		} else {
			ofTranslate(bgX_large, bgY_large);
			texColor.draw(20,40,webWidth_l,webHeight_l);
		}

	ofPopMatrix();
}

void WebRender::zoomDetect(int mouseX, int mouseY){	
	if(!zoomIn){
		if((mouseX>bgX_small+163 && mouseX<bgX_small+163+28)&&(mouseY>bgY_small+4 && mouseY<bgY_small+4+28)) zoomIn=true;
		
	} else {
		if((mouseX>bgX_large+584)&&(mouseX<bgX_large+584+28)&&(mouseY>bgY_large+6)&&(mouseY<bgY_large+6+28)) zoomIn=false;
	}
	

}

void WebRender:: injectMouseMoved(int x, int y){
	if(!zoomIn){
		int webCoreX=bgX_small+3;
		int webCoreY=bgY_small+36;
		awe_webview_inject_mouse_move (webView,(x-webCoreX)*7,(y-webCoreY)*6); 
	}
	else if(zoomIn){
		int webCoreX=bgX_large+20+2;
		int webCoreY=bgY_large+40-3;
		awe_webview_inject_mouse_move (webView,(x-webCoreX)*2.38,(y-webCoreY)*2.1); 
	}
}
void WebRender::injectMouseDragged(int x, int y){
	if(!zoomIn){
		int webCoreX=bgX_small+3;
		int webCoreY=bgY_small+36;
		awe_webview_inject_mouse_move (webView,(x-webCoreX)*7,(y-webCoreY)*6); 
	}
	else if(zoomIn){
		int webCoreX=bgX_large+20+2;
		int webCoreY=bgY_large+40-3;
		awe_webview_inject_mouse_move (webView,(x-webCoreX)*2.38,(y-webCoreY)*2.1); 
	}
}
void WebRender::injectMousePressed(){
	awe_webview_inject_mouse_down (webView,AWE_MB_LEFT);
}
void WebRender::injectMouseReleased(){
	awe_webview_inject_mouse_up (webView,AWE_MB_LEFT);
}