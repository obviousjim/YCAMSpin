//
//  CloudInterludeForceDrag.h
//  CloudsInterlude
//
//  Created by James George on 5/4/12.
//

#pragma once

#include "CloudInterludeForce.h"
class CloudInterludeForceGravity : public CloudInterludeForce {
  public:
    float gravity;
	float maxDelay;
    CloudInterludeForceGravity() {
        gravity = .1;
		maxDelay = 1;
    }
    
    void update(){};
    
    void applyForce(vector<CloudInterludeParticle>& particles){
        for(int i = 0; i < particles.size(); i++){
			if( ofGetElapsedTimef() - particles[i].birthTime > ofNoise(i)*maxDelay ){
				particles[i].force += ofVec3f(0, gravity, 0);
			}
        }
    }
};

