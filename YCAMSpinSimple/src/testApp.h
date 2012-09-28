#pragma once

#define MAC_USE_CONTROL 1

#include "ofMain.h"
#include "ofxTimeline.h"
#include "ofxTLDepthImageSequence.h"
#include "ofxMayaCam.h"
#include "ofxRGBDMeshBuilder.h"
#include "ofxGameCamera.h"
#include "ofxUI.h"

class testApp : public ofBaseApp{
  public:
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	void exit();
	
	ofxTLDepthImageSequence sequence1;
//	ofxTLDepthImageSequence sequence2;
//	ofxTLDepthImageSequence sequence3;
	
	ofxTimeline masterTimeline;
	ofxTimeline sequenceTimeline;
	
	ofxRGBDMeshBuilder mesh;
	ofxGameCamera cam;
	
//	float farClip;
//	float nearClip;
	
	bool colorMeshes;
	ofColor mesh1Color;

	bool showPointcloud;
	void calculatePreviewRects();
	ofRectangle prev1;

	string sequence1Path;
//	string sequence2Path;
//	string sequence3Path;
//	string sequence1PathXML;
//	string sequence2PathXML;
//	string sequence3PathXML;

	ofxUICanvas* gui;
	
	ofRectangle mainRect;
	bool isSetup;
};
