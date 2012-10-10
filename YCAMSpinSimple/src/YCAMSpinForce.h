//
//  YCAMSpinForce.h
//  YCAMSpin
//
//  Created by Jim on 9/28/12.
//
//

#ifndef YCAMSpin_YCAMSpinForce_h
#define YCAMSpin_YCAMSpinForce_h

#include "CloudInterludeForce.h"
class YCAMSpinForce : public CloudInterludeForce {
public:
    
    ofVec3f center;
	float power;
    
    YCAMSpinForce() {
        power = .1;
    }
    
    void update(){
//        currentOffset += speed;
    }
    
    void applyForce(vector<CloudInterludeParticle>& particles){
        //currentOffset = 0;
        for(int i = 0; i < particles.size(); i++){
            ofVec3f& pos = particles[i].position;
			particles[i].force += (pos-center).getNormalized().getRotated(90, ofVec3f(0,1,0)) *ofVec3f(1,0,1) * power;
			
//            particles[i].force +=  ofVec3f(ofSignedNoise(pos.x/density, pos.y/density, pos.z/density, currentOffset)*amplitude,
//                                           ofSignedNoise(pos.x/density, pos.y/density, pos.z/density, currentOffset+1000)*amplitude,
//                                           ofSignedNoise(pos.x/density, pos.y/density, pos.z/density, currentOffset+2000)*amplitude );
        }
    }
};


#endif
