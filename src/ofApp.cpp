#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	player = new PlayerGameObject(glm::vec3(0, 0, -10), 1, 75.0f, 0.75f, cam);
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
	float delta_time = ofGetLastFrameTime();
	// cam.update(0.016); // 60 fps
	player->update(delta_time);
}

//--------------------------------------------------------------
void ofApp::draw(){
	player->getCamera().begin();
	for (int i = 0; i < asteroids; i++)
		body[i].draw();
	player->draw();
	player->getCamera().end();
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
