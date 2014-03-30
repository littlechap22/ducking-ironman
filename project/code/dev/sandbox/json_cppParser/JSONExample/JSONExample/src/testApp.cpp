#include "testApp.h"
#include <iostream>
#include <fstream>


//--------------------------------------------------------------
void testApp::setup(){
	ofSetFrameRate(12);
	ofBackground(255, 255, 255);
	myFont.loadFont("Verdana.ttf", 32);
	
	
	ofxHttpEvents.addListener(this);
	
	// Example of getting JSON from the Internets
	http.addUrl("http://4u.jeffcrouse.info/of/json.txt");
	

	// Example of getting JSON from a local file
	string str,strTotal;
	ifstream in;
	in.open(ofToDataPath("json.txt", true).c_str());
	getline(in,str);
	while ( in ) {
		strTotal += str;
		getline(in, str);
	}
	in.close();
	localResult = parseJSON(strTotal);
	
}

void testApp::newResponse(ofxHttpResponse & response){
	remoteResult = parseJSON(response.responseBody);
}

void testApp::newError(string error){
    printf("new error = %s\n", error.c_str());
}

string testApp::parseJSON(string s) {
	string result;
	result = "parsing JSON...\n";

	bool parsingSuccessful = reader.parse( s, root );
	if ( !parsingSuccessful ) {
		std::cout  << "Failed to parse JSON\n" << reader.getFormatedErrorMessages();
		return result;
	}
	
	// See this page for all of the ways to access data in a Json::Value
	// http://jsoncpp.sourceforge.net/class_json_1_1_value.html
	Json::Value text = root["widget"]["text"];
	result += "text members: " + ofToString((int)text.size()) + "\n";
	result += "data: " + text["data"].asString() + "\n";

	return result;
	/*
	 // You can also write JSON files...
	 Json::StyledWriter writer;
	 std::string outputConfig = writer.write( root );
	 */
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(0, 0, 0);
	myFont.drawString("LOCAL FILE", 100, 100);
	myFont.drawString(localResult, 100, 200);
	
	myFont.drawString("REMOTE FILE", 580, 100);
	myFont.drawString(remoteResult, 580, 200);
	
	
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

