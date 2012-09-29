#pragma once

#define MAC_USE_CONTROL 1

#include "ofMain.h"
#include "ofxTimeline.h"
#include "ofxTLDepthImageSequence.h"
#include "ofxMayaCam.h"
#include "ofxRGBDMeshBuilder.h"
#include "ofxGameCamera.h"
#include "ofxUI.h"
#include "ofxTLCameraTrack.h"

#include "CloudInterludeParticleGenerator.h"
#include "CloudInterludeForcePerlin.h"
#include "CloudInterludeForceDrag.h"
#include "CloudInterludeForceMeshAttractor.h"
#include "YCAMSpinForce.h"

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
	ofxTLCameraTrack camTrack;
	
	ofxTimeline masterTimeline;
	ofxTimeline sequenceTimeline;
	
	ofxRGBDMeshBuilder meshBuilder;
	ofxGameCamera cam;
	
	bool colorMeshes;
	ofColor mesh1Color;

	bool showPointcloud;
	void calculatePreviewRects();
	ofRectangle prev1;

	string sequence1Path;

	ofxUICanvas* gui;
	
	ofRectangle mainRect;

	vector<CloudInterludeParticleGenerator> emmiters;
    CloudInterludeForcePerlin* perlinForce;
    CloudInterludeForceDrag* dragForce;
    CloudInterludeForceMeshAttractor* meshForce;
	YCAMSpinForce* spinForce;
	
    ofVboMesh mesh;
    ofShader pointCloudDOF;
    int totalParticles;
	
	void copyVertsToMesh();
};
