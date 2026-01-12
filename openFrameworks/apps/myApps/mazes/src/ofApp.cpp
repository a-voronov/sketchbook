#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetWindowShape(1024,768);

    ofSetFrameRate(60); // Limit the speed of our program to 60 frames per second

    ofSetBackgroundAuto(false); // Stop the background from being redrawn each frame
    // We still want to draw on a black background, so we need to draw
    // the background before we do anything with the brush
    ofBackground(0);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    // If the right mouse button is pressed...
    if (ofGetMousePressed(OF_MOUSE_BUTTON_RIGHT)) {
        ofBackground(0);  // Erase the screen with a black background
    }

    // If the left mouse button is pressed...
    if (ofGetMousePressed(OF_MOUSE_BUTTON_LEFT)) {
        int maxRadius = 100;  // Increase for a wider brush
        int radiusStepSize = 5;  // Decrease for more circles (i.e. a more opaque brush)
        int alpha = 3;  // Increase for a more opaque brush
        int maxOffsetDistance = 100;  // Increase for a larger spread of circles
        for (int radius=maxRadius; radius>0; radius-=radiusStepSize) {

            // Formula for converting from polar to Cartesian coordinates:
            //    x = cos(polar angle) * (polar distance)
            //    y = sin(polar angle) * (polar distance)
            // We need our angle to be in radians if we want to use sin() or cos()
            // so we can make use of an openFrameworks function to convert from degrees
            // to radians
            float angle = ofRandom(ofDegToRad(360.0));
            float distance = ofRandom(maxOffsetDistance);
            float xOffset = cos(angle) * distance;
            float yOffset = sin(angle) * distance;

            // Using the ofColor class, we will randomly select a color between orange and red
            ofColor myOrange(255, 132, 0, alpha);
            ofColor myRed(255, 6, 0, alpha);
            ofColor inBetween = myOrange.getLerped(myRed, ofRandom(1.0));
            ofSetColor(inBetween);

            ofCircle(ofGetMouseX()+xOffset, ofGetMouseY()+yOffset, radius);
        }
    }
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
