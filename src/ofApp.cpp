#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	// initalize member vars
	time_elapsed = 0.f;

	// setup asteroids
	asteroids = 200;
	for (int i = 0; i < asteroids; i++) {
		body[i].setPosition(glm::vec3(
				ofRandom(-800,800),
				ofRandom(-800,800),
				ofRandom(-800,800)));
	}

	// setup sound
	try {
		background_music.load("bg_music.mp3");
		background_music.setLoop(true);
		background_music.setVolume(0.5f);
		background_music.play();
	}
	catch (...) {
		cout << "Music file could not be loaded. Ensure bin/data/bg_music.mp3 exists." << endl;
	}
}

//--------------------------------------------------------------
void ofApp::update() {
	cam.update(TIME_INC);
	time_elapsed += TIME_INC;
}

//--------------------------------------------------------------
void ofApp::draw() {

	// handle world-coord objects
	cam.begin();

	// asteroids
	for (int i = 0; i < asteroids; i++)
		body[i].draw();

	cam.end();

	// HUD elements
	ofDrawBitmapString("Time Elapsed: " + ofToString(time_elapsed), ofGetWidth() - 160, 30);

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
