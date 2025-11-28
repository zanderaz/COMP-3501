#include "ofApp.h"

// Universal game-state setup function
void ofApp::setup() {

	// openframework project specific
	ofSetWindowTitle("Course Project - Triple Sicks");
	ofSetFrameRate(120); // gets overriden with vsync for whatever reason
	ofBackground(40);

	// initalize member vars
	time_elapsed = 0.f;
	mouse_capture_flag = false;
	mouse_sensitivity = 0.001f;
	last_x = 0.0, last_y = 0.0;
	is_first_mouse = true;

	game_state = 0; // start on main menu

	// camera frustum defaults
	cam.setNearClip(0.1f);
	cam.setFarClip(5000.0f);
	cam.setFov(90.0f);

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
		background_music.load("sfx/bg_music.mp3");
		background_music.setLoop(true);
		background_music.setVolume(0.01f);
	}
	catch (...) {
		ofLogError() << "Music file could not be loaded. Ensure bin/data/bg_music.mp3 exists.";
	}

	// setup main-menu elements
	try {
		menu_button_font.load("fonts/ArialMedium.ttf", 32);
		menu_title_font.load("fonts/PapyrusBold.ttf", 96);
		menu_caption_font.load("fonts/ArialMedium.ttf", 14);
		menu_background.load("images/menu_bg.jpg");
	}
	catch (...) {
		ofLogError() << "Menu elements could not be loaded. Please check bin/data/fonts and bin/data/images.";
	}
}


// Sets up gameplay specific elements
void ofApp::setupGameplayGameState(void) {

	ofBackground(80);
	background_music.play();

	// mouse related
	mouse_capture_flag = true;
	setRawMouseCapture(true);
	if (auto w = getGLFW()) {
		glfwGetCursorPos(w, &last_x, &last_y);
		is_first_mouse = false;
	}

}


// game destructor, ensure the game clears dynamically allocated memory correctly when exiting
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


//================================================================================================
//                                   UPDATING GAME LOGIC RELATED 
//================================================================================================
void ofApp::update() {

	// increment time
	float delta_time = ofGetLastFrameTime();
	time_elapsed += delta_time;

	// -------------------- MAIN MENU GAME STATE ---------------------------
	if (game_state == 0) {

	}

	// -------------------- GAMEPLAY GAME STATE ---------------------------
	else if (game_state == 1) {

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

	// -------------------- GAME OVER GAME STATE ---------------------------
	else if (game_state == 2) {
		// to-do
	}

	// -------------------- GAME WON GAME STATE ---------------------------
	else if (game_state == 3) {
		// to-do
	}
	
}


//================================================================================================
//                                  DRAWING / RENDERING RELATED 
//================================================================================================
void ofApp::draw() {

	// -------------------- MAIN MENU GAME STATE ---------------------------
	if (game_state == 0) {

		ofSetColor(255, 255, 255);
		
		// background image -> change to dynamic texture for bonus points later (if time permits)
		if (menu_background.isAllocated()) {
			menu_background.draw(0, 0, ofGetWidth(), ofGetHeight());
		}

		// setup button rectangle positions
		float center_x = ofGetWidth() / 2.0f;
		float center_y = ofGetHeight() / 2.0f;

		// title specific
		ofRectangle title_bounds = menu_title_font.getStringBoundingBox(TITLE_TEXT, 0, 0);
		float title_x = center_x - (title_bounds.width / 2.0f);
		float title_y = center_y - 100; // top
		menu_title_font.drawString(TITLE_TEXT, title_x, title_y);

		// caption specific
		ofRectangle caption_bounds = menu_caption_font.getStringBoundingBox(CAPTION_TEXT, 0, 0);
		float caption_x = center_x - (caption_bounds.width / 2.0f);
		float caption_y = center_y - 20; // middle-top
		menu_caption_font.drawString(CAPTION_TEXT, caption_x, caption_y);

		// start button specific
		ofRectangle start_bounds = menu_button_font.getStringBoundingBox(START_TEXT, 0, 0);
		float start_x = center_x - (start_bounds.width / 2.0f);
		float start_y = center_y + 150; // middle-bottom
		start_button_rect = menu_button_font.getStringBoundingBox(START_TEXT, start_x, start_y);

		// quit button specific
		ofRectangle quit_bounds = menu_button_font.getStringBoundingBox(QUIT_TEXT, 0, 0);
		float quit_x = center_x - (quit_bounds.width / 2.0f);
		float quit_y = center_y + 200; // bottom
		quit_button_rect = menu_button_font.getStringBoundingBox(QUIT_TEXT, quit_x, quit_y);

		// check if mouse is hovering over start, turn yellow if so
		if (start_button_rect.inside(ofGetMouseX(), ofGetMouseY())) {
			ofSetColor(255, 255, 0);
		}
		else {
			ofSetColor(255, 255, 255);
		}
		menu_button_font.drawString(START_TEXT, start_x, start_y);

		// check if mouse is hovering over quit, turn yellow if so
		if (quit_button_rect.inside(ofGetMouseX(), ofGetMouseY())) {
			ofSetColor(255, 255, 0);
		}
		else {
			ofSetColor(255, 255, 255);
		}
		menu_button_font.drawString(QUIT_TEXT, quit_x, quit_y);


	}

	// -------------------- GAMEPLAY GAME STATE ---------------------------
	else if (game_state == 1) {

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

	// -------------------- GAME OVER GAME STATE ---------------------------
	else if (game_state == 2) {
		// to-do
	}

	// -------------------- GAME WON GAME STATE ---------------------------
	else if (game_state == 3) {
		// to-do
	}
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}


//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}


//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

	// only make the mouse control the camera for gameplay
	if (game_state == 1) {

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

}


//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	mouseMoved(x, y);
}


//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

	// ----------- check if the player has clicked a menu option ----------
	if (game_state == 0) {

		if (button == OF_MOUSE_BUTTON_LEFT) {
			// handle start button
			if (start_button_rect.inside(x, y)) {
				setupGameplayGameState();
				game_state = 1;
			}
			// handle quit button
			if (quit_button_rect.inside(x, y)) {
				ofExit();
			}
		}
	}

	// ---------- toggle mouse capture when gameplay starts -----------
	else if (game_state == 1) {

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


// Get GLFW window helper method
GLFWwindow* ofApp::getGLFW() {
	auto base = ofGetWindowPtr();
	if (!base) return nullptr;
	auto glfwWin = dynamic_cast<ofAppGLFWWindow*>(base);
	return glfwWin ? glfwWin->getGLFWWindow() : nullptr;
}


// Raw mouse input toggle
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


// Reset mouse to center
void ofApp::recenterCursorToWindowCenter() {
	if (auto w = getGLFW()) {
		int ww = ofGetWindowWidth();
		int wh = ofGetWindowHeight();
		double cx = ww * 0.5, cy = wh * 0.5;
		glfwSetCursorPos(w, cx, cy);
		last_x = cx; last_y = cy;
	}
}
