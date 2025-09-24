#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	asteroids = 200;
	for (int i = 0; i < asteroids; i++)
	{
		body[i].setPosition(
			glm::vec3(
				ofRandom(-800,800),
				ofRandom(-800,800),
				ofRandom(-800,800))); // random somwhere in space

	}
}

//--------------------------------------------------------------
void ofApp::update(){
	cam.update(0.016); // 60 fps
}

//--------------------------------------------------------------
void ofApp::draw(){
	cam.begin();
	for (int i = 0; i < asteroids; i++)
		body[i].draw();
	cam.end();
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
