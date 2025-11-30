#include "ofApp.h"

// Universal game-state setup function
void ofApp::setup() {

	// openframework project specific
	ofSetWindowTitle("Course Project - Triple Sicks");
	
	// init shaders
	lightingShader = new ofShader();
	skyBoxShader = new ofShader();

	lightingShader->load("shader/lighting");
	skyBoxShader->load("shader/skyBox");

	// test (sphere not in a game obj)
	sphere.setPosition(-150, 0, 10);
	skySphere.setRadius(1000);
	skySphere.setResolution(5);

	orbitRadius = 400;
	orbitSpeed = 0.3;
	orbitAngle = 0;

	// load textures
	ofDisableArbTex();
	texture.load("images/DOG.png");
	skyTexture.load("images/blood.jpeg");

	if (!lightingShader->isLoaded()) {
		ofLogError() << "Lighting Shader failed to load!";
		ofExit();
	}

	if (!skyBoxShader->isLoaded()) {
		ofLogError() << "Skybox shader Shader failed to load!";
		ofExit();
	}
	ofSetFrameRate(120); // gets overriden with vsync for whatever reason
	ofBackground(40);

	// initalize member vars
	time_elapsed = 0.f;
	mouse_capture_flag = false;
	mouse_sensitivity = 0.001f;
	last_x = 0.0, last_y = 0.0;
	is_first_mouse = true;
	// start in bloodstream
	bloodstream = true;
	boneMarrow = false;

	game_state = 0; // start on main menu

	// camera frustum defaults
	cam.setNearClip(1.0f);
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

	// checkpoint to tp to bonemarrow
	checkpoint_vec.push_back(new CheckpointGameObject(power_up_mesh.getMesh(), glm::vec3(-300, 100, 200), 1.f));

	// test for red blood cells
	rbc = new RedBloodCellParticleSystem(player->getCamera());
	rbc->setup(25);
	ofMesh mesh = ofMesh::sphere(25, 100);
	redBloodCell = new RedBloodCell(rbc, mesh, glm::vec3(20, 200, -20), 1.0f);
	sse = false;
	screenSpaceEffect.setup(ofGetWidth(), ofGetHeight());

	// test
	// opposition_vec.push_back(new EnemyGameObject(power_up_mesh.getMesh(), glm::vec3(500, -200, 0), 1.f));

	// setup sound
	try {
		background_music.load("sfx/bg_music.mp3");
		background_music.setLoop(true);
		background_music.setVolume(10.0f);
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
		// for text boxes
		dialog_font.load("fonts/ArialMedium.ttf", 24);
	}
	catch (...) {
		ofLogError() << "Menu elements could not be loaded. Please check bin/data/fonts and bin/data/images.";
	}

	// test for text box
	textBox.setup("Welcome to Triple Sicks! Use WASD to move, mouse to look around, and right-click to toggle mouse capture.", &dialog_font, 500.0f);
	textBox.setSize(550, 150);
	textBox.setPosition(ofGetWidth() / 2 - 250, ofGetHeight() - 200);
	textBox.setBackgroundColor(ofColor(0, 0, 0, 220));
	textBox.setTextColor(ofColor(255, 255, 0));
	textBox.setBorderColor(ofColor(255, 255, 255));
	textBox.setBorderWidth(3.0f);
	showTextBox = false;
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

		// test (light source)
		orbitAngle += orbitSpeed * ofGetLastFrameTime();

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

		/*** CHECKPOINT HANDLING ***/
		for (int i = 0; i < checkpoint_vec.size(); ++i) {
			CheckpointGameObject* checkpoint = checkpoint_vec[i];

			// Check for collision with checkpoint
			float dist = glm::distance(player->getPosition(), checkpoint->getPosition());
			if (dist <= player->getRadius() + checkpoint->getRadius()) {

				// Teleport player and change environment
				handleCheckpointCollision(checkpoint);

				// Remove the checkpoint (optional - if you want one-time use)
				delete checkpoint;
				checkpoint_vec.erase(checkpoint_vec.begin() + i);
				break; // Exit loop since we modified the vector
			}
		}

		// rbc movement test
		/*
		glm::vec3 dir(1, 0, 0);
		dir += 10 * delta_time;
		redBloodCell->setPosition(redBloodCell->getPosition() + dir);
		*/
		redBloodCell->update(delta_time);
	}

	// -------------------- GAME OVER GAME STATE ---------------------------
	else if (game_state == 2) {
		// to-do
	}

	// -------------------- GAME WON GAME STATE ---------------------------
	else if (game_state == 3) {
		// to-do
	}

	//test (make objects move for visualization and testing model/world matrix)
	/*
	for (int i = 0; i < power_up_vec.size(); i++) {
		glm::vec3 dir (1, 0, 0);
		dir += 10*delta_time;
		power_up_vec[i]->setPosition(power_up_vec[i]->getPosition() + dir);
	}
	*/
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

		// everything below (until fbo is ended) will get put in the frame buffer for screen-space effects
		if (sse) {
			screenSpaceEffect.setInBloodstream(bloodstream);
			screenSpaceEffect.begin();
		}

		ofEnableDepthTest();

		player->getCamera().begin();

		// test (skybox)
		skyBoxShader->begin();
		skyBoxShader->setUniformTexture("skyTexture", skyTexture, 0);
		skyBoxShader->setUniformMatrix4f("viewMatrix", player->getCamera().getModelViewMatrix());
		skyBoxShader->setUniformMatrix4f("projectionMatrix", player->getCamera().getProjectionMatrix());
		skyBoxShader->setUniformMatrix4f("modelViewProjectionMatrix", player->getCamera().getModelViewProjectionMatrix());
	
		// draw skybox
		ofPushMatrix();
		ofScale(1, 1, 1);
		skySphere.draw();
		ofPopMatrix();

		skyBoxShader->end();

		glDepthMask(GL_TRUE);

		ofSetColor(ofColor::sandyBrown);

		lightingShader->begin();

		if (bUseTexture) {
			lightingShader->setUniform1i("useTexture", 1);
			lightingShader->setUniformTexture("tex0", texture, 0);
		}
		else {
			lightingShader->setUniform1i("useTexture", 0);
		}

		sphere.setPosition(-50, 300, 10);

		//bUseTexture = false;

		// setup attributes

		lightingShader->setUniformMatrix4f("viewMatrix", player->getCamera().getModelViewMatrix());
		lightingShader->setUniformMatrix4f("modelViewProjectionMatrix", player->getCamera().getModelViewProjectionMatrix());
		lightingShader->setUniformMatrix4f("projectionMatrix", player->getCamera().getProjectionMatrix());
		lightingShader->setUniformMatrix4f("worldMatrix", sphere.getGlobalTransformMatrix());

		lightingShader->setUniform1i("isLight", false);
		lightingShader->setUniform3f("emissionColor", glm::vec3(0.0));

		
		if (bloodstream) {
			lightingShader->setUniform3f("emissionColor", glm::vec3(1.0f, 0.7f, 0.7f));
		}
		else if (boneMarrow) {
			lightingShader->setUniform3f("emissionColor", glm::vec3(0.8f, 0.9f, 1.0f));
		}

		// light and view position setup (might make the player have the light close to them
		glm::mat4 view = player->getCamera().getModelViewMatrix();
		glm::vec3 lightPos(100, 420, 100);

		lightPos.x = 100 + (orbitRadius * cos(orbitAngle));
		lightPos.z = 100 + (orbitRadius * sin(orbitAngle));
		lightSphere.setPosition(lightPos);
		glm::vec3 viewLight = glm::vec3(view * glm::vec4(lightPos, 1.0));

		glm::vec3 camPos = player->getCamera().getPosition();
		glm::vec3 viewCam = glm::vec3(view * glm::vec4(camPos, 1.0));

		lightingShader->setUniform3f("lightPos", viewLight);
		lightingShader->setUniform3f("viewPos", viewCam);


		//lightingShader->setUniform3f("lightPos", lightPos);
		//lightingShader->setUniform3f("viewPos", camPos);

		// test objects

		lightingShader->setUniform3f("lightColor", glm::vec3(1, 1, 1));

		lightingShader->setUniform3f("objectColor", glm::vec3(0.6, 0.6, 0.9));
		lightingShader->setUniform1i("specularPower", 128);

		sphere.getMesh().draw();

		lightingShader->setUniform1i("isLight", true);
		lightingShader->setUniform3f("objectColor", glm::vec3(1.0, 0.9, 0.2));
		lightingShader->setUniform3f("emissionColor", glm::vec3(1, 1, 0.4));
		lightingShader->setUniformMatrix4f("worldMatrix", lightSphere.getGlobalTransformMatrix());
		lightSphere.draw();

		player->draw(lightingShader);
		for (int i = 0; i < opposition_vec.size(); ++i) {
			opposition_vec[i]->draw(lightingShader);
		}
		for (int i = 0; i < power_up_vec.size(); ++i) {
			power_up_vec[i]->draw(lightingShader);
		}
		for (int i = 0; i < checkpoint_vec.size(); ++i) {
			checkpoint_vec[i]->draw(lightingShader);
		}

		// rbc
		redBloodCell->draw(lightingShader);

		// plane for testing
		//ofSetColor(100, 60, 250)
		//lightingShader->setUniformMatrix4f("worldMatrix", alignment_check.getGlobalTransformMatrix());
		//lightingShader->setUniform3f("objectColor", glm::vec3(0.6, 0.6, 0.9));
		//alignment_check.getMesh().draw();

		lightingShader->end();
		player->getCamera().end();
		ofDisableDepthTest();

		if (sse) {
			screenSpaceEffect.end();
			screenSpaceEffect.draw();
		}

		textBox.setVisible(showTextBox);
		textBox.draw();

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
	if (key == 't' || key == 'T') {
		bUseTexture = !bUseTexture;
	}
	if (key == 'e' || key == 'E') {
		sse = !sse;
	}
	if (key == 'h' || key == 'H') { // Help key
		showTextBox = !showTextBox;
		//textBox.toggle();
	}
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

void ofApp::handleCheckpointCollision(CheckpointGameObject* checkpoint) {

	if (bloodstream) {
		bloodstream = false;
		boneMarrow = true;

		// Change skybox texture
		//skyTexture.load("images/bone_marrow_texture.jpg");

		// Teleport player to new area
		player->setPosition(glm::vec3(0, 100, 0));

		/*
		orbitRadius = 200;
		orbitSpeed = 0.5f;
		*/

		ofLog() << "Teleported to Bone Marrow environment";

	}
}
