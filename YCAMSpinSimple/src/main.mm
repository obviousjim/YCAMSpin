#include "testApp.h"
#include "ofAppGlutWindow.h"
//#include "ofxCocoaWindow.h"

//--------------------------------------------------------------
int main(){
//    bool bUseCocoaWindow = true;
    
//    if( bUseCocoaWindow )
//    {
//        ofxCocoaWindow cocoaWindow;
//        ofSetupOpenGL( &cocoaWindow, 800, 600, OF_WINDOW );
//    }
//    else {
        ofAppGlutWindow window;
        ofSetupOpenGL( &window, 800, 600, OF_WINDOW );
//    }
	ofRunApp(new testApp()); // start the app
}
