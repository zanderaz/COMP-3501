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
	is_muted = false;
	bloodstream = true; // start in bloodstream
	boneMarrow = false;
	game_state = 0; // start on main menu
	
	// init shaders
	lightingShader = new ofShader();
	skyBoxShader = new ofShader();
	lightingShader->load("shader/lighting");
	if (!lightingShader->isLoaded()) {
		ofLogError() << "Lighting Shader failed to load!";
		ofExit();
	}
	skyBoxShader->load("shader/skyBox");
	if (!skyBoxShader->isLoaded()) {
		ofLogError() << "Skybox shader Shader failed to load!";
		ofExit();
	}

	// init textures
	ofDisableArbTex();
	texture.load("images/DOG.png");
	skyTexture.load("images/blood.jpg");
	skyTexture.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
	wallTexture.load("images/bloodold.jpeg");

	// init sounds
	try {
		background_music.load("sfx/bg_music.mp3");
		background_music.setLoop(true);
		background_music.setVolume(BG_MUSIC_VOL);
	}
	catch (...) {
		ofLogError() << "Music file could not be loaded. Ensure bin/data/bg_music.mp3 exists.";
	}

	// camera settings
	cam.setNearClip(1.0f);
	cam.setFarClip(5000.0f);
	cam.setFov(90.0f);

	// test object related
	sphere.setPosition(-150, 0, 10);
	skySphere.setRadius(1000);
	skySphere.setResolution(6);
	orbitRadius = 400;
	orbitSpeed = 0.3;
	orbitAngle = 0;

	// setup meshes
	power_up_mesh.setRadius(20);
	power_up_mesh.setResolution(10);

	player_mesh.setRadius(0);
	player_mesh.setResolution(0);

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
	screenSpaceEffect.setup(ofGetWidth(), ofGetHeight());

	// test
	// opposition_vec.push_back(new EnemyGameObject(power_up_mesh.getMesh(), glm::vec3(500, -200, 0), 1.f));

	// setup all text elements
	try {
		menu_button_font.load("fonts/ArialMedium.ttf", 32);
		menu_title_font.load("fonts/PapyrusBold.ttf", 96);
		menu_caption_font.load("fonts/ArialMedium.ttf", 14);
		menu_background.load("images/menu_bg.jpg");
		dialog_font.load("fonts/ArialMedium.ttf", 24);
	}
	catch (...) {
		ofLogError() << "Text elements could not be loaded. Please check bin/data/fonts and bin/data/images.";
	}

	// setup the info text box
	textBox.setup("Welcome to Triple Sicks! Use WASD to move, mouse to look around, and right-click to toggle mouse capture.", &dialog_font, 500.0f);
	textBox.setSize(550, 150);
	textBox.setBackgroundColor(ofColor(0, 0, 0, 220));
	textBox.setTextColor(ofColor(255, 255, 0));
	textBox.setBorderColor(ofColor(255, 255, 255));
	textBox.setPosition(ofGetWidth() / 2 - 250, ofGetHeight() - 200);
	textBox.setBorderWidth(3.0f);
	showTextBox = false;

	// create collidable geometry, store in player so collision resolving works properly
	createWalls();
	player->setWalls(&wall_objects_vec);

	// setup the interactable objects

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

	// player should start looking at the path out of the first room
	player->yaw(PI);

}


// game destructor, ensure the game clears dynamically allocated memory correctly when exiting
void ofApp::exit(void) {

	delete player;
	delete lightingShader;
	delete skyBoxShader;
	delete rbc;
	delete redBloodCell;

	for (int i = 0; i < opposition_vec.size(); ++i) {
		delete opposition_vec[i];
	}
	opposition_vec.clear();

	for (int i = 0; i < checkpoint_vec.size(); ++i) {
		delete checkpoint_vec[i];
	}
	checkpoint_vec.clear();

	for (int i = 0; i < power_up_vec.size(); ++i) {
		delete power_up_vec[i];
	}
	power_up_vec.clear();

	for (GameObject* obj : wall_objects_vec) {
		delete obj;
	}
	wall_objects_vec.clear();

	for (GameObject* obj : interactable_objects_vec) {
		delete obj;
	}
	interactable_objects_vec.clear();

}


//================================================================================================
//                                   UPDATING GAME LOGIC RELATED 
//================================================================================================
void ofApp::update() {

	// increment time
	float delta_time = ofGetLastFrameTime();

	// prevents massive jumps if the window is dragged or the game freezes
	if (delta_time > 0.1f) {
		delta_time = 0.1f;
	}
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

				// Remove the checkpoint
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
		// idk if this will be necessary
	}

	// -------------------- GAME WON GAME STATE ---------------------------
	else if (game_state == 3) {
		// idk if this will be necessary
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
		screenSpaceEffect.setInBloodstream(bloodstream);
		screenSpaceEffect.setSpeedBoostActive(player->isSpeedBoostOn());

		screenSpaceEffect.begin();
		ofEnableDepthTest();
		player->getCamera().begin();

		// Draw skybox first
		skyBoxShader->begin();

		skyBoxShader->setUniformTexture("skyTexture", skyTexture, 0);
		skyBoxShader->setUniformMatrix4f("viewMatrix", player->getCamera().getModelViewMatrix());
		skyBoxShader->setUniformMatrix4f("projectionMatrix", player->getCamera().getProjectionMatrix());
		skyBoxShader->setUniformMatrix4f("modelViewProjectionMatrix", player->getCamera().getModelViewProjectionMatrix());

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

		// different texture for walls
		if (bUseTexture) {
			lightingShader->setUniformTexture("tex0", wallTexture, 0);
		}
		for (GameObject* wall : wall_objects_vec) {
			wall->draw(lightingShader);
		}

		// rbc
		redBloodCell->draw(lightingShader);

		lightingShader->end();

		player->getCamera().end();
		ofDisableDepthTest();
		screenSpaceEffect.end();

		screenSpaceEffect.draw();

		// HUD elements
		textBox.draw();

		// DEBUG: minecraft f3 menu with developer info
		ofSetColor(255);
		ofDrawBitmapString("FPS: " + to_string(ofGetFrameRate()), glm::vec2(30, 30));
		ofDrawBitmapString("X-pos: " + to_string(player->getPosition().x), glm::vec2(30, 50));
		ofDrawBitmapString("Y-pos: " + to_string(player->getPosition().y), glm::vec2(30, 60));
		ofDrawBitmapString("Z-pos: " + to_string(player->getPosition().z), glm::vec2(30, 70));

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

	// player speed boost
	if (key == 'e' || key == 'E') {
		if (player->isSpeedBoostReady()) {
			player->activateSpeedBoost();
		}
	}

	// mute game audio
	if (key == 'm' || key == 'M') {
		is_muted = !is_muted;
		if (is_muted) {
			background_music.setVolume(0.0);
		}
		else {
			background_music.setVolume(BG_MUSIC_VOL);
		}
	}

	// debug, keys for testing shit
	if (key == 't' || key == 'T') {
		bUseTexture = !bUseTexture;
	}
	if (key == 'b' || key == 'B') {
		bloodstream = !bloodstream;
	}
	if (key == 'h' || key == 'H') {
		textBox.showTemporarily(4.0f);
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

		// pitch is clamped within the function to prevent vector singularity (i.e. world up vector and player foward vector match up)
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
	screenSpaceEffect.setResolution(w, h);
	textBox.setPosition((ofGetWidth() / 2.0f) - 250, ofGetHeight() - 200);
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

// Helper method, handles what should happen when the player hits a checkpoint
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

// create walls for the play area, as well as any other collidable objects 
void ofApp::createWalls() {
	createWallsSection1();
	createWallsSection2();
	createWallsSection3();
	createVeins();

	ofLog() << "Created walls: " << wall_objects_vec.size() << " objects";
	ofLog() << "Player starts at position: " << player->getPosition();
}

// first room and exit
void ofApp::createWallsSection1() {

	// wall dimensions
	float wallThickness = 20.0f;
	float wallHeight = 300.0f;

	// room dimensions
	float roomSize = 800.0f;

	// hallway dimensions
	float hallwayLength = 400.0f;
	float hallwayWidth = 200.0f;

	// floor
	ofBoxPrimitive floorMesh;
	floorMesh.set(10000, 5, 10000);
	GameObject* floor = new GameObject(floorMesh.getMesh(), glm::vec3(0, -50, 0), 1.0f);
	wall_objects_vec.push_back(floor);

	// starting room

	ofBoxPrimitive backWallMesh;
	backWallMesh.set(roomSize, wallHeight, wallThickness);
	GameObject* backWall = new GameObject(backWallMesh.getMesh(), glm::vec3(0, wallHeight / 2 - 50, -roomSize / 2), 1.0f);
	wall_objects_vec.push_back(backWall);

	ofBoxPrimitive leftWallMesh;
	leftWallMesh.set(wallThickness, wallHeight, roomSize*1.5);
	GameObject* leftWall = new GameObject(leftWallMesh.getMesh(), glm::vec3(-roomSize / 2, wallHeight / 2 - 50, (200 - roomSize*0.5)), 1.0f);
	wall_objects_vec.push_back(leftWall);

	
	ofBoxPrimitive rightWallMesh;
	rightWallMesh.set(wallThickness, wallHeight, roomSize);
	GameObject* rightWall = new GameObject(rightWallMesh.getMesh(), glm::vec3(roomSize / 2, wallHeight / 2 - 50, 0), 1.0f);
	wall_objects_vec.push_back(rightWall);

	// hallway out of room

	ofBoxPrimitive leftHallWallMesh;
	leftHallWallMesh.set(wallThickness, wallHeight, hallwayLength/2);
	GameObject* leftHallWall = new GameObject(leftHallWallMesh.getMesh(), glm::vec3(-hallwayWidth / 2, wallHeight / 2 - 50, hallwayLength/2 + 100), 1.0f);
	wall_objects_vec.push_back(leftHallWall);

	ofBoxPrimitive rightHallWallMesh;
	rightHallWallMesh.set(wallThickness, wallHeight, hallwayLength/2);
	GameObject* rightHallWall = new GameObject(rightHallWallMesh.getMesh(), glm::vec3(hallwayWidth / 2, wallHeight / 2 - 50, hallwayLength/2 + 100), 1.0f);
	wall_objects_vec.push_back(rightHallWall);

	// front of room

	float frontWallSegmentWidth = (roomSize - hallwayWidth) / 2;

	/*
	ofBoxPrimitive frontWallLeftMesh;
	frontWallLeftMesh.set(frontWallSegmentWidth, wallHeight, wallThickness);
	GameObject* frontWallLeft = new GameObject(frontWallLeftMesh.getMesh(), glm::vec3(-(roomSize + hallwayWidth) / 4, wallHeight / 2 - 50, roomSize / 2), 1.0f);
	wall_objects_vec.push_back(frontWallLeft);
	*/
	
	ofBoxPrimitive frontWallRightMesh;
	frontWallRightMesh.set(frontWallSegmentWidth, wallHeight, wallThickness);
	GameObject* frontWallRight = new GameObject(frontWallRightMesh.getMesh(), glm::vec3((roomSize + hallwayWidth) / 4, wallHeight / 2 - 50, roomSize / 2), 1.0f);
	wall_objects_vec.push_back(frontWallRight);
	

	// big ceiling for now
	ofBoxPrimitive ceilingMesh;
	ceilingMesh.set(5000, 5, 5000);
	GameObject* ceiling = new GameObject(ceilingMesh.getMesh(), glm::vec3(0, wallHeight - 50, 0), 1.0f);
	ceiling->setVisible(false);
	wall_objects_vec.push_back(ceiling);

}

// starts at hallways out of first room
void ofApp::createWallsSection2() {
	
	// wall dimensions
	float wallThickness = 20.0f;
	float wallHeight = 300.0f;

	// hallway dimensions
	float hallwayLength = 400.0f;
	float hallwayWidth = 200.0f;

	// back wall out of first room
	ofBoxPrimitive frontWallMesh;
	frontWallMesh.set(1600, wallHeight, wallThickness);
	GameObject* frontWall = new GameObject(frontWallMesh.getMesh(), glm::vec3(0, wallHeight / 2 - 50, 800), 1.0f);
	wall_objects_vec.push_back(frontWall);

	// out of first room

	// left side (dead end)
	ofBoxPrimitive sideWallMesh;
	sideWallMesh.set(800, wallHeight, wallThickness);
	GameObject* leftSideWall = new GameObject(sideWallMesh.getMesh(), glm::vec3(800, wallHeight / 2 - 50, 400), 1.0f);
	leftSideWall->setOrientation(glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)));
	wall_objects_vec.push_back(leftSideWall);

	sideWallMesh.set(400, wallHeight, wallThickness);
	GameObject* deadEnd = new GameObject(sideWallMesh.getMesh(), glm::vec3(600, wallHeight / 2 - 50, 0), 1.0f);
	//deadEnd->setOrientation(glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)));
	wall_objects_vec.push_back(deadEnd);

	// right side
	sideWallMesh.set(1000, wallHeight, wallThickness);
	GameObject* rightSideWall = new GameObject(sideWallMesh.getMesh(), glm::vec3(-600, wallHeight / 2 - 50, 400), 1.0f);
	//rightSideWall->setOrientation(glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)));
	wall_objects_vec.push_back(rightSideWall);

}

// starts at diagonal hallway to the right of start
void ofApp::createWallsSection3() {
	
	float wallThickness = 20.0f;
	float wallHeight = 300.0f;

	// hallway dimensions
	float hallwayLength = 400.0f;
	float hallwayWidth = 200.0f;

	// diagonal hallway to the right of start
	ofBoxPrimitive diagonalWallMesh;
	diagonalWallMesh.set(525, wallHeight, wallThickness);
	GameObject* diagonalWall1 = new GameObject(diagonalWallMesh.getMesh(), glm::vec3(-980, wallHeight / 2 - 50, 985), 1.0f);
	diagonalWall1->setOrientation(glm::angleAxis(glm::radians(45.0f), glm::vec3(0, 1, 0)));
	wall_objects_vec.push_back(diagonalWall1);

	diagonalWallMesh.set(350, wallHeight, wallThickness);
	GameObject* diagonalWall2 = new GameObject(diagonalWallMesh.getMesh(), glm::vec3(-1405, wallHeight / 2 - 50, 705), 1.0f);
	diagonalWall2->setOrientation(glm::angleAxis(glm::radians(45.0f), glm::vec3(0, 1, 0)));
	wall_objects_vec.push_back(diagonalWall2);

	// side hallway before junction
	diagonalWallMesh.set(685, wallHeight, wallThickness);
	GameObject* diagonalWall3 = new GameObject(diagonalWallMesh.getMesh(), glm::vec3(-1530, wallHeight / 2 - 50, 345), 1.0f);
	diagonalWall3->setOrientation(glm::angleAxis(glm::radians(135.0f), glm::vec3(0, 1, 0)));
	wall_objects_vec.push_back(diagonalWall3);

	diagonalWallMesh.set(700, wallHeight, wallThickness);
	GameObject* diagonalWall4 = new GameObject(diagonalWallMesh.getMesh(), glm::vec3(-1355, wallHeight / 2 - 50, 155), 1.0f);
	diagonalWall4->setOrientation(glm::angleAxis(glm::radians(135.0f), glm::vec3(0, 1, 0)));
	wall_objects_vec.push_back(diagonalWall4);

	diagonalWallMesh.set(685, wallHeight, wallThickness);
	GameObject* diagonalWall5 = new GameObject(diagonalWallMesh.getMesh(), glm::vec3(-1875, wallHeight / 2 - 50, -200), 1.0f);
	diagonalWall5->setOrientation(glm::angleAxis(glm::radians(110.0f), glm::vec3(0, 1, 0)));
	wall_objects_vec.push_back(diagonalWall5);

	diagonalWallMesh.set(700, wallHeight, wallThickness);
	GameObject* diagonalWall6 = new GameObject(diagonalWallMesh.getMesh(), glm::vec3(-1715, wallHeight / 2 - 50, -410), 1.0f);
	diagonalWall6->setOrientation(glm::angleAxis(glm::radians(110.0f), glm::vec3(0, 1, 0)));
	wall_objects_vec.push_back(diagonalWall6);

	// junction
	ofBoxPrimitive junctionWallMesh;
	junctionWallMesh.set(1500, wallHeight, wallThickness);
	GameObject* junctionWall1 = new GameObject(junctionWallMesh.getMesh(), glm::vec3(-2225, wallHeight / 2 - 50, 1125), 1.0f);
	wall_objects_vec.push_back(junctionWall1);

	junctionWallMesh.set(1000, wallHeight, wallThickness);
	GameObject* junctionWall2 = new GameObject(junctionWallMesh.getMesh(), glm::vec3(-1475, wallHeight / 2 - 50, 1625), 1.0f);
	junctionWall2->setOrientation(glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)));
	wall_objects_vec.push_back(junctionWall2);
	
	// left side of junction
	ofBoxPrimitive sideWallMesh;

	sideWallMesh.set(475, wallHeight, wallThickness);
	GameObject* sideWall1 = new GameObject(sideWallMesh.getMesh(), glm::vec3(-925, wallHeight / 2 - 50, 1175), 1.0f);
	wall_objects_vec.push_back(sideWall1);

	sideWallMesh.set(775, wallHeight, wallThickness);
	GameObject* sideWall2 = new GameObject(sideWallMesh.getMesh(), glm::vec3(-1075, wallHeight / 2 - 50, 1525), 1.0f);
	wall_objects_vec.push_back(sideWall2);
	
	sideWallMesh.set(750, wallHeight, wallThickness);
	GameObject* sideWall3 = new GameObject(sideWallMesh.getMesh(), glm::vec3(-700, wallHeight / 2 - 50, 1425), 1.0f);
	sideWall3->setOrientation(glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)));
	wall_objects_vec.push_back(sideWall3);

	// right side of junction

	sideWallMesh.set(600, wallHeight, wallThickness);
	GameObject* sideWall4 = new GameObject(sideWallMesh.getMesh(), glm::vec3(-1825, wallHeight / 2 - 50, 825), 1.0f);
	wall_objects_vec.push_back(sideWall4);

	sideWallMesh.set(600, wallHeight, wallThickness);
	GameObject* sideWall5 = new GameObject(sideWallMesh.getMesh(), glm::vec3(-2675, wallHeight / 2 - 50, 825), 1.0f);
	wall_objects_vec.push_back(sideWall5);

	// right corridor out right side of junction

	sideWallMesh.set(600, wallHeight, wallThickness);
	GameObject* sideWall6 = new GameObject(sideWallMesh.getMesh(), glm::vec3(-2125, wallHeight / 2 - 50, 525), 1.0f);
	sideWall6->setOrientation(glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)));
	wall_objects_vec.push_back(sideWall6);

	sideWallMesh.set(600, wallHeight, wallThickness);
	GameObject* sideWall7 = new GameObject(sideWallMesh.getMesh(), glm::vec3(-2375, wallHeight / 2 - 50, 525), 1.0f);
	sideWall7->setOrientation(glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)));
	wall_objects_vec.push_back(sideWall7);

}

// create veins that can be infected
void ofApp::createVeins(void) {

	float radius = 40.0f;

	// first vein, to the left of the starting room
	ofCylinderPrimitive vein_mesh1;
	vein_mesh1.set(radius, 400.0f);
	vein_mesh1.setResolution(18, 2);
	glm::vec3 vein1_pos(600, 30, 100);
	GameObject* vein1 = new GameObject(vein_mesh1.getMesh(), vein1_pos, 1.0f);
	vein1->setOrientation(glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 0, 1)));
	wall_objects_vec.push_back(vein1);

	// second vein, midway through bloodstream
	ofCylinderPrimitive vein_mesh2;
	vein_mesh2.set(radius, 350.0f);
	vein_mesh2.setResolution(18, 2);
	glm::vec3 vein2_pos(-800, 75, 1350);
	GameObject* vein2 = new GameObject(vein_mesh2.getMesh(), vein2_pos, 1.0f);
	vein2->setOrientation(glm::angleAxis(glm::radians(90.0f), glm::vec3(1, 0, 0)));
	wall_objects_vec.push_back(vein2);

	// third vein, next to final door
	ofCylinderPrimitive vein_mesh3;
	vein_mesh3.set(radius, 300.0f);
	vein_mesh3.setResolution(18, 2);
	glm::vec3 vein3_pos(-2250, 25, 400);
	GameObject* vein3 = new GameObject(vein_mesh3.getMesh(), vein3_pos, 1.0f);
	vein3->setOrientation(glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 0, 1)));
	wall_objects_vec.push_back(vein3);

}
