#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxHttpUtils.h"
#include "ofxHttpEvents.h"
#include "json.h"

class testApp : public ofBaseApp, ofxHttpEventListener{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
	
		void newResponse(ofxHttpResponse & response);
		void newError(string error);
		string parseJSON(string s);
		
		ofTrueTypeFont myFont;
	
		ofxHttpUtils http;
		Json::Value root;   // will contains the root value after parsing.
		Json::Reader reader;
		string remoteResult, localResult;
};

#endif
