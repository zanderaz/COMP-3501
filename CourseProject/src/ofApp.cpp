#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	// openframework project specific
	ofSetWindowTitle("Course Project - Triple Sicks");
	ofSetFrameRate(120);

	// initalize member vars
	time_elapsed = 0.f;
	mouse_capture_flag = true;
	mouse_sensitivity = 0.001f;
	last_x = 0.0, last_y = 0.0;
	is_first_mouse = true;

	// camera frustum defaults
	cam.setNearClip(0.1f);
	cam.setFarClip(5000.0f);
	cam.setFov(90.0f);

	// setup mouse capture
	if (mouse_capture_flag) {
		setRawMouseCapture(true);
		// seed lastX/lastY from GLFW
		if (auto w = getGLFW()) {
			glfwGetCursorPos(w, &last_x, &last_y);
			is_first_mouse = false;
		}
	}

	// setup meshes
	power_up_mesh.setRadius(20);
	power_up_mesh.setResolution(10);

	player_mesh.setRadius(0);
	player_mesh.setResolution(0);

	alignment_check.set(640, 480);
	alignment_check.setPosition(320, 240, 0);
	alignment_check.setResolution(2, 2);

	// create the player
	player = new PlayerGameObject(player_mesh.getMesh(), glm::vec3(0), 1.0f, cam);

	// example objs
	power_up_vec.push_back(new PowerUpObject(power_up_mesh.getMesh(), glm::vec3(200, 0, 0), 1.f));
	power_up_vec.push_back(new PowerUpObject(power_up_mesh.getMesh(), glm::vec3(-200, 0, 0), 1.f));
	power_up_vec.push_back(new PowerUpObject(power_up_mesh.getMesh(), glm::vec3(0, 200, 0), 1.f));
	power_up_vec.push_back(new PowerUpObject(power_up_mesh.getMesh(), glm::vec3(0, -200, 0), 1.f));
	power_up_vec.push_back(new PowerUpObject(power_up_mesh.getMesh(), glm::vec3(0, 0, 200), 1.f));
	power_up_vec.push_back(new PowerUpObject(power_up_mesh.getMesh(), glm::vec3(0, 0, -200), 1.f));

	// setup sound
	try {
		background_music.load("bg_music.mp3");
		background_music.setLoop(true);
		background_music.setVolume(0.01f);
		background_music.play();
	}
	catch (...) {
		cout << "Music file could not be loaded. Ensure bin/data/bg_music.mp3 exists." << endl;
	}

}


//--------------------------------------------------------------
void ofApp::update() {

	// increment time
	float delta_time = ofGetLastFrameTime();
	time_elapsed += delta_time;

	/*** PLAYER HANDLING ***/

	player->update(delta_time);

	// check if player should be able to be hit
	if (player->getInvincibilityTimer().FinishedAndStop()) {
		player->setColour(glm::vec3(255.0f));
	}

	/*** ENEMY HANDLING ***/

	// updates for opps
	for (int i = 0; i < opposition_vec.size(); ++i) {
		EnemyGameObject* enemy = opposition_vec[i];
		enemy->faceTowards(player->getPosition());
		enemy->update(delta_time);

		// check for collisions between an enemy and the player
		float dist = glm::distance(player->getPosition(), enemy->getPosition());
		if (dist <= player->getRadius() + enemy->getRadius() && !player->getInvincibilityTimer().IsRunning()) {
				
			// handle damage
			player->getInvincibilityTimer().Start(2.0f);
			player->setColour(glm::vec3(255.0f, 0.0f, 50.0f));

			// clean up enemy
			delete enemy;
			opposition_vec.erase(opposition_vec.begin() + i);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	// draw world coordinate objects
	player->getCamera().begin();
	ofEnableDepthTest();

	player->draw();
	for (int i = 0; i < opposition_vec.size(); ++i) {
		opposition_vec[i]->draw();
	}
	for (int i = 0; i < power_up_vec.size(); ++i) {
		power_up_vec[i]->draw();
	}
	for (int i = 0; i < checkpoint_vec.size(); ++i) {
		checkpoint_vec[i]->draw();
	}
	ofSetColor(100, 60, 250);
	alignment_check.getMesh().draw();

	ofDisableDepthTest();
	player->getCamera().end();

	// reset colour, draw HUD elements
	ofSetColor(255, 255, 255);
	ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate()), ofGetWidth() - 160, 20);
	ofDrawBitmapString("Time Elapsed: " + ofToString(time_elapsed, 2), ofGetWidth() - 160, 35);
	ofDrawBitmapString("Player Health: " + ofToString(player->getHealth()), ofGetWidth() - 160, 50);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
	if (!mouse_capture_flag || !player) return;

	double cursor_x = x, cursor_y = y;

	if (is_first_mouse) {
		last_x = cursor_x;
		last_y = cursor_y;
		is_first_mouse = false;
		return;
	}

	double delta_x = cursor_x - last_x;
	double delta_y = cursor_y - last_y;
	last_x = cursor_x; last_y = cursor_y;

	float yaw_amt = static_cast<float>(-delta_x) * mouse_sensitivity; // left -> +yaw
	float pitch_amt = static_cast<float>(-delta_y) * mouse_sensitivity; // up -> +pitch

	// LOWKEY WE NEED PROPER PITCH CLAMPING however I cannot figure it out so fuck it (js don't flick straight up/down)
	player->pitch(pitch_amt);
	player->yaw(yaw_amt);

	recenterCursorToWindowCenter();
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	mouseMoved(x, y);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

	// toggle mouse capture
	if (button == OF_MOUSE_BUTTON_RIGHT) {
		mouse_capture_flag = !mouse_capture_flag;
		setRawMouseCapture(mouse_capture_flag);

		// reset delta baseline so the first frame after toggle has no jump
		if (auto w = getGLFW()) {
			glfwGetCursorPos(w, &last_x, &last_y);
			is_first_mouse = false;
		}
	}
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

	for (int i = 0; i < opposition_vec.size(); ++i) {
		delete opposition_vec[i];
	}

	for (int i = 0; i < checkpoint_vec.size(); ++i) {
		delete checkpoint_vec[i];
	}

	for (int i = 0; i < power_up_vec.size(); ++i) {
		delete power_up_vec[i];
	}
}
 
/*** Get GLFW window helper method ***/
GLFWwindow* ofApp::getGLFW() {
	auto base = ofGetWindowPtr();
	if (!base) return nullptr;
	auto glfwWin = dynamic_cast<ofAppGLFWWindow*>(base);
	return glfwWin ? glfwWin->getGLFWWindow() : nullptr;
}

/*** Raw mouse input toggle ***/
void ofApp::setRawMouseCapture(bool on) {
	GLFWwindow* w = getGLFW();
	if (!w) return;

	if (on) {
		glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		if (glfwRawMouseMotionSupported())
			glfwSetInputMode(w, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE); // use raw input
		ofHideCursor();
	}
	else {
		if (glfwRawMouseMotionSupported())
			glfwSetInputMode(w, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
		glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		ofShowCursor();
	}
}

/*** Reset mouse to center ***/
void ofApp::recenterCursorToWindowCenter() {
	if (auto w = getGLFW()) {
		int ww = ofGetWindowWidth();
		int wh = ofGetWindowHeight();
		double cx = ww * 0.5, cy = wh * 0.5;
		glfwSetCursorPos(w, cx, cy);
		last_x = cx; last_y = cy;
	}
}