#include "testApp.h"
#include "ofxHotKeys.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetFrameRate(60);
	ofBackground(255*.15);
	ofEnableAlphaBlending();
	ofEnableSmoothing();
	ofToggleFullscreen();
	isSetup = false;

	ofxTimeline::removeCocoaMenusFromGlut("YCAM SPIN");
	
	colorMeshes = false;
	showPointcloud = false;

	masterTimeline.setName("Master");
	masterTimeline.setup();
	masterTimeline.setLoopType(OF_LOOP_NORMAL);
	sequenceTimeline.setLoopType(OF_LOOP_NORMAL);
	
	sequenceTimeline.setName("YCAM_Y_CAM1_TAKE_09_21_15_38_28");
	sequenceTimeline.setup();
	
	int absoluteMaxParticles = 50000;
	masterTimeline.setPageName("MeshClips");
	masterTimeline.addCurves("FarClip", ofRange(400, 3000), 3000);
	masterTimeline.addCurves("NearClip", ofRange(400, 3000), 400);
	masterTimeline.addCurves("TopClip", ofRange(0., 1.0), .0);
	masterTimeline.addCurves("BottomClip", ofRange(0., 1.0), 1.0);
	masterTimeline.addCurves("LeftClip",ofRange(0., 1.0), .0);
	masterTimeline.addCurves("RightClip", ofRange(0., 1.0), 1.0);
	
    masterTimeline.addPage("Particles");
    masterTimeline.addCurves("Max Particles", "maxParticles.xml", ofRange(0, absoluteMaxParticles) );
    masterTimeline.addCurves("Birthrate", "particleBirthrate.xml", ofRange(.000, .05) );
    masterTimeline.addCurves("Lifespan", "particleLifespan.xml", ofRange(2, 900) );
    masterTimeline.addCurves("Lifespan Variance", "particleLifespanVariance.xml", ofRange(0, 100) );
    masterTimeline.addCurves("Drag Force", "particleDragFroce.xml", ofRange(0, 1.0), 0);
    
    masterTimeline.addPage("DOF");
    masterTimeline.addCurves("DOF Distance", "DOFDistance.xml", ofRange(0,sqrtf(1000.0)), 10 );
    masterTimeline.addCurves("DOF Range", "DOFRange.xml", ofRange(10,sqrtf(500.0)) );
    masterTimeline.addCurves("DOF Blur", "DOFBlur.xml", ofRange(0,5.0) );
    masterTimeline.addCurves("Min Point Size", "dofMinPoint.xml", ofRange(1, 5));
    masterTimeline.addCurves("Max Point Size", "dofMaxPoint.xml", ofRange(10, 45));
    
    masterTimeline.addPage("Perlin");
    masterTimeline.addCurves("Perlin Amplitude", "perlinAmplitude.xml", ofRange(0, sqrtf(20)) );
    masterTimeline.addCurves("Perlin Density", "perlinDensity.xml", ofRange(0, sqrtf(2000)));
    masterTimeline.addCurves("Perlin Speed", "perlinSpeed.xml", ofRange(0, sqrtf(2)), 0);
	
	masterTimeline.addFlags("Sequencer");
	
	sequence1Path = "/Users/focus/Desktop/__RGBD_Bins/YCAM_SPIN/YCAM_Y_CAM1/TAKE_09_21_15_38_28/depth";

	sequence1.loadSequence(sequence1Path);
	sequenceTimeline.setDurationInMillis(sequence1.getDepthImageSequence()->getDurationInMillis());

	
	sequenceTimeline.addTrack("Depth", &sequence1);
	sequenceTimeline.addFlags("LoopFlags");
	
	
	mesh1Color = ofColor(255, 100, 20);
//	mesh2Color = ofColor(100, 255, 20);
//	mesh3Color = ofColor(100, 20, 255);

	masterTimeline.setDurationInSeconds(300);
	masterTimeline.setLoopType(OF_LOOP_NORMAL);
	calculatePreviewRects();
	
	mesh.setDepthOnly();
	
	mesh.setSimplification(2);
	
	mesh.setDepthPixels(sequence1.getDepthImageSequence()->getPixels());
	
	cam.setup();
	cam.autosavePosition = true;
	cam.loadCameraPosition();
	cam.setFarClip(4000);
	
//	gui = new ofxUICanvas(0,0, 200, ofGetHeight());
//	gui->setWidgetFontSize(OFX_UI_FONT_SMALL);
//	gui->loadSettings("GUI/guiSettings.xml");
//	isSetup = true;

}

//--------------------------------------------------------------
void testApp::update(){
	
	float farClip = masterTimeline.getValue("FarClip");
	float nearClip = masterTimeline.getValue("NearClip");
	float topClip = masterTimeline.getValue("TopClip");
	float bottomClip = masterTimeline.getValue("BottomClip");
	float rightClip = masterTimeline.getValue("RightClip");
	float leftClip = masterTimeline.getValue("LeftClip");
	
	if(sequence1.isFrameNew() ||
	   farClip != mesh.farClip ||
	   nearClip != mesh.nearClip ||
	   topClip != mesh.topClip ||
	   bottomClip != mesh.bottomClip ||
	   leftClip != mesh.leftClip ||
	   rightClip != mesh.rightClip)
	{
		mesh.farClip = farClip;
		mesh.nearClip = nearClip;
		mesh.topClip = topClip;
		mesh.bottomClip = bottomClip;
		mesh.leftClip = leftClip;
		mesh.rightClip = rightClip;
		
		mesh.update();
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	
	
	if(showPointcloud){
		ofPushStyle();
		
		ofSetColor(0);
		ofRect(mainRect);
		
		cam.begin(mainRect);
		//glEnable(GL_DEPTH_TEST);
		if(!colorMeshes){
			ofSetColor(255);
		}
		if(colorMeshes){
			ofSetColor(mesh1Color);
		}
		mesh.draw();

		cam.end();
		ofPopStyle();
	}
	else{
		ofSetColor(255, 100);
		sequence1.getCurrentDepthImage().draw(prev1);
//		sequence2.getCurrentDepthImage().draw(prev2);
//		sequence3.getCurrentDepthImage().draw(prev3);
	}

	ofSetColor(255);
	masterTimeline.draw();
	sequenceTimeline.setOffset(masterTimeline.getBottomLeft());
	sequenceTimeline.draw();
	
	masterTimeline.getFont().drawString(ofToString(ofGetFrameRate()), 0, ofGetHeight()-10);

//	cout << "draw complete" << endl;
}

//--------------------------------------------------------------
void testApp::calculatePreviewRects(){
	
	masterTimeline.setOffset(ofVec2f(0,0));
	masterTimeline.setWidth(ofGetWidth()/2);
	sequenceTimeline.setWidth(ofGetWidth()/2);
	sequenceTimeline.setOffset(masterTimeline.getBottomLeft());
	mainRect = ofRectangle(ofGetWidth()/2,0,
						   ofGetWidth()/2,
						   ofGetHeight());
	prev1 = ofRectangle(ofGetWidth()/2,0,640,480);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	if(key == ' '){
		masterTimeline.togglePlay();
		sequenceTimeline.togglePlay();
	}
	
	if(key == 'R'){
		showPointcloud = !showPointcloud;
	}
	
	if(key == 'P'){
		mesh.setPivotToMeshCenter();
		cout << mesh.pivot << endl;
		cam.setPosition(mesh.pivot);
		cam.setOrientation(ofQuaternion(180, ofVec3f(0,1,0)));
	}
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
	cam.applyRotation = cam.applyTranslation = mainRect.inside(x,y);
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
	calculatePreviewRects();
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void testApp::exit(){
	sequence1.disable();
//	sequence2.disable();
//	sequence3.disable();
	
//	gui->saveSettings("GUI/guiSettings.xml");
}