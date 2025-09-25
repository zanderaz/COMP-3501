#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	player = new PlayerGameObject(glm::vec3(0, 0, -10), 1, 100.0f, 0.75f, cam);

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

	// populate the game with opposition objects (placeholder positions, change to beacons later)
	opposition_vec.push_back(new EnemyGameObject(glm::vec3(-99.f, 0.f, -99.f), 1.f));
	opposition_vec.push_back(new EnemyGameObject(glm::vec3(99.f, 0.f, -99.f), 1.f));
	opposition_vec.push_back(new EnemyGameObject(glm::vec3(99.f, 0.f, 99.f), 1.f));
	opposition_vec.push_back(new EnemyGameObject(glm::vec3(-99.f, 0.f, 99.f), 1.f));

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

	float delta_time = ofGetLastFrameTime();

	player->update(delta_time);

	for (int i = 0; i < opposition_vec.size(); ++i) {
		EnemyGameObject* enemy = opposition_vec[i];
		enemy->faceTowards(player->getPosition());
		enemy->update(delta_time);
	}

	time_elapsed += ofGetLastFrameTime();
}

//--------------------------------------------------------------
void ofApp::draw() {

	// draw world coordinate objects
	player->getCamera().begin();
	ofEnableDepthTest();

	for (int i = 0; i < asteroids; i++) {
		body[i].draw();
	}

	player->draw();

	for (int i = 0; i < opposition_vec.size(); ++i) {
		opposition_vec[i]->draw();
	}

	player->getCamera().end();
	ofDisableDepthTest();

	// reset color, draw HUD elements
	ofSetColor(255, 255, 255);
	ofDrawBitmapString("Time Elapsed: " + ofToString(time_elapsed), ofGetWidth() - 160, 30);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) { 

}

/*** Free all the dynamically allocated memory ***/
void ofApp::exit(void) {

	delete player;

	// clean up opposition vec
	for (int i = 0; i < opposition_vec.size(); i++) {
		delete opposition_vec[i];
	}
}
