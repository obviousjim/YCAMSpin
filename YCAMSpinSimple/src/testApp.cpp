#include "testApp.h"
#include "ofxHotKeys.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetFrameRate(60);
	ofBackground(255*.15);
	ofEnableAlphaBlending();
	ofEnableSmoothing();
	ofToggleFullscreen();


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
	
	camTrack.getCameraTrack().camera = &cam;
	
	masterTimeline.addTrack("Camera", &camTrack);
	masterTimeline.addCurves("FarClip", ofRange(400, 3000), 3000);
	masterTimeline.addCurves("NearClip", ofRange(400, 3000), 400);
	masterTimeline.addCurves("TopClip", ofRange(0., 1.0), .0);
	masterTimeline.addCurves("BottomClip", ofRange(0., 1.0), 1.0);
	masterTimeline.addCurves("LeftClip",ofRange(0., 1.0), .0);
	masterTimeline.addCurves("RightClip", ofRange(0., 1.0), 1.0);

	
    masterTimeline.addPage("Particles");
    masterTimeline.addCurves("Max Particles", "maxParticles.xml", ofRange(0, absoluteMaxParticles) );
    masterTimeline.addCurves("Birthrate", "particleBirthrate.xml", ofRange(.000, .5) );
    masterTimeline.addCurves("Lifespan", "particleLifespan.xml", ofRange(2, 200) );
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

	masterTimeline.setCurrentPage(0);
	
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
	
	meshBuilder.setDepthOnly();
	
	meshBuilder.setSimplification(2);
	meshBuilder.cacheValidVertices = true;
	meshBuilder.setDepthPixels(sequence1.getDepthImageSequence()->getPixels());
	
	cam.setup();
	cam.autosavePosition = true;
	cam.loadCameraPosition();
	cam.setFarClip(4000);
	
    perlinForce = new CloudInterludeForcePerlin();
	spinForce = new YCAMSpinForce();
//    dragForce   = new CloudInterludeForceDrag();
//    meshForce   = new CloudInterludeForceMeshAttractor();
//    meshForce->mesh = &meshBuilder.getMesh();
    
    for(int i = 0; i < 320*240; i++){
    	CloudInterludeParticleGenerator g;
        g.addForce(perlinForce);
		g.addForce(spinForce);
		
//        g.addForce(dragForce);
		//        g.addForce(meshForce);
        emmiters.push_back(g);
    }
    
    for(int i = 0; i < absoluteMaxParticles; i++){
    	mesh.addVertex(ofVec3f(0,0,0));
        mesh.addColor(ofFloatColor(1.0,1.0,1.0,1.0));
        mesh.addTexCoord(ofVec2f(0.0,0.0));
    }
	
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
	   farClip != meshBuilder.farClip ||
	   nearClip != meshBuilder.nearClip ||
	   topClip != meshBuilder.topClip ||
	   bottomClip != meshBuilder.bottomClip ||
	   leftClip != meshBuilder.leftClip ||
	   rightClip != meshBuilder.rightClip)
	{
		meshBuilder.farClip = farClip;
		meshBuilder.nearClip = nearClip;
		meshBuilder.topClip = topClip;
		meshBuilder.bottomClip = bottomClip;
		meshBuilder.leftClip = leftClip;
		meshBuilder.rightClip = rightClip;
		
		meshBuilder.update();
	}
	
//    dragForce->dragForce = powf( masterTimeline.getValue("Drag Force"), 2.0);
    perlinForce->amplitude = powf( masterTimeline.getValue("Perlin Amplitude"), 2.0);
    perlinForce->density = powf( masterTimeline.getValue("Perlin Density"), 2.0);
    perlinForce->speed = powf( masterTimeline.getValue("Perlin Speed"), 2.0);
    
	
//    dragForce->update();
    perlinForce->update();
	//    meshForce->update();
    

    
    //GENERATOR
    float birthRate = masterTimeline.getValue("Birthrate");
    float lifeSpan  = masterTimeline.getValue("Lifespan");
    float lifeSpanVariance = masterTimeline.getValue("Lifespan Variance");
    int maxParticles = masterTimeline.getValue("Max Particles");
//    float typeChance = masterTimeline.getValue("Chance of Attaching Type");
    totalParticles = 0;
    for(int i = 0; i < emmiters.size(); i++){
    	emmiters[i].birthRate = 0;
        emmiters[i].freeze = false;
        totalParticles += emmiters[i].particles.size();
    }
	
    //for(int i = 0; i < meshBuilder.getMesh().getVertices().size(); i++){
	for(int i = 0; i < meshBuilder.validVertIndices.size(); i++){
	
        CloudInterludeParticleGenerator& g = emmiters[i];
//        bool valid = renderer.isVertexValid(i);
//		bool valid = meshBuilder.getMesh().getVertices()[i].z != 0;
        totalParticles += g.particles.size();
//        if(valid){
            g.birthRate = birthRate; //disable invisible verts
            g.lifespan  = lifeSpan;
            g.lifespanVariance = lifeSpanVariance;
            g.position =  meshBuilder.getMesh().getVertices()[meshBuilder.validVertIndices[i]];
//            if(useColors && colorPalette.isAllocated()){
//                g.texcoord = renderer.getMesh().getTexCoord( renderer.vertexIndex(i) );
//            }
            g.remainingParticles = maxParticles - totalParticles;
			g.showType = false;
//            g.showType = showType;
//            g.typeChance = typeChance;
//        }
    }
//    cout << " total particles " << totalParticles << endl;
    for(int i = 0; i < emmiters.size(); i++){
    	emmiters[i].update();
    }
	
	copyVertsToMesh();
}

//--------------------------------------------------------------
void testApp::copyVertsToMesh(){
    int meshIndex = 0;
    vector<ofVec3f>& meshVertices = mesh.getVertices();
    vector<ofFloatColor>& meshColors = mesh.getColors();
    vector<ofVec2f>& meshTexCoords = mesh.getTexCoords();
    for(int i = 0; i < emmiters.size(); i++){
        for(int v = 0; v < emmiters[i].particles.size(); v++){
            meshVertices[meshIndex] = emmiters[i].particles[v].position;
            float color = emmiters[i].particles[v].energy / emmiters[i].particles[v].initialEnergy;
            meshColors[meshIndex] = ofFloatColor(color,color,color,color);
//            if(useColors){
//                meshTexCoords[meshIndex] = emmiters[i].particles[v].texcoord;
//            }
            meshIndex++;
            if(meshIndex == meshVertices.size()){
                ofLogError("exceeded max particles");
                return;
            }
        }
    }
//	cout << "mesh index reached " << meshIndex << endl;
    memset(&(meshColors[meshIndex].r), 0, sizeof(ofFloatColor)*(meshColors.size()-meshIndex));
}

//--------------------------------------------------------------
void testApp::draw(){
	
	ofSetColor(255);
	masterTimeline.draw();
	sequenceTimeline.setOffset(masterTimeline.getBottomLeft());
	sequenceTimeline.draw();
	
	masterTimeline.getFont().drawString(ofToString(ofGetFrameRate()), 0, ofGetHeight()-10);

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
		meshBuilder.draw();
		float focalRange = powf(masterTimeline.getValue("DOF Range"), 2);
		float focalDistance = powf(masterTimeline.getValue("DOF Distance"), 2);

		//render dots
		ofPushStyle();
		glPushMatrix();
		glPushAttrib(GL_ENABLE_BIT);
		ofScale(1,-1,1);
		ofEnableAlphaBlending();
//		if(useShaderToggle){
		if(false){
			pointCloudDOF.begin();
//			if(depthRangeMultiply) focalRange *= 10;
			pointCloudDOF.setUniform1f("minSize", masterTimeline.getValue("Min Point Size"));
			pointCloudDOF.setUniform1f("maxSize", masterTimeline.getValue("Max Point Size"));
			pointCloudDOF.setUniform1f("focalRange", focalRange);
			pointCloudDOF.setUniform1f("focalDistance", focalDistance);
			pointCloudDOF.setUniform1i("useTexture", 0);
//			pointCloudDOF.setUniform1i("useTexture", useColors && colorPalette.isAllocated() ? 1 : 0);
		}
		else{
			glPointSize(masterTimeline.getValue("Min Point Size"));
		}
		
//		if(useColors && colorPalette.isAllocated()){
//			colorPalette.bind();
//		}
		//    if(drawPointcloud){
		//        ofPushMatrix();
		//        ofScale(1,-1,1);
		//        ofEnableAlphaBlending();
		//        renderer.drawPointCloud(false);
		//        ofPopMatrix();
		//	}
		
		//ofEnableBlendMode(OF_BLENDMODE_ADD);
		ofEnableBlendMode(OF_BLENDMODE_SCREEN);
		glEnable(GL_POINT_SMOOTH); // makes circular points
		glEnable(GL_VERTEX_PROGRAM_POINT_SIZE_ARB);	// allows per-point size
//		glPointSize(4);
		
//		cout << "Drawing " << mesh.getNumVertices() << " Particles " << endl;
		mesh.drawVertices();
		
//		if(useColors && colorPalette.isAllocated()){
//			colorPalette.unbind();
//		}
//		
		if(false){
			pointCloudDOF.end();
		}
		
		glPopAttrib();
		glPopMatrix();
		ofPopStyle();

		cam.end();
		ofPopStyle();
		 
	}
	else{
		ofSetColor(255, 100);
		sequence1.getCurrentDepthImage().draw(prev1);
	}


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
		meshBuilder.setPivotToMeshCenter();
		cam.setPosition(meshBuilder.pivot);
		cam.setOrientation(ofQuaternion(180, ofVec3f(0,1,0)));
	}
	
	if(key == 'C'){
		for(int i = 0; i < emmiters.size(); i++){
			emmiters[i].particles.clear();
		}
	}
	
	if(key == 'S'){
		meshBuilder.setPivotToMeshCenter();
		spinForce->center = meshBuilder.pivot;
	}
	
	if(key == 'T'){
		camTrack.sample();
	}
	if(key == 'L'){
		camTrack.lockCameraToTrack = !camTrack.lockCameraToTrack;
		cam.applyRotation = cam.applyTranslation = !camTrack.lockCameraToTrack;
		cout << "locked camera? " << camTrack.lockCameraToTrack << endl;
	}
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
	cam.applyRotation = cam.applyTranslation = !camTrack.lockCameraToTrack && mainRect.inside(x,y);
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
	camTrack.disable();
//	sequence2.disable();
//	sequence3.disable();
	
//	gui->saveSettings("GUI/guiSettings.xml");
}