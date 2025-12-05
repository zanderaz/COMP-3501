#include "ofApp.h"

// Universal game-state setup function
void ofApp::setup() {

	// openframework project specific
	ofSetWindowTitle("Course Project - Triple Sicks");
	ofSetFrameRate(GAME_FPS); // gets overriden with vsync for whatever reason
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
	show_interact_tip = false;
	veins_infected_count = 0;
	marrow_infected_count = 0;
	game_state = 0; // start on main menu

	// helpers for all shaders and textures
	setupShaders();
	setupTextures();

	// init sounds
	try {
		setupSFX();
	}
	catch (...) {
		ofLogError() << "Sound files could not be loaded. Ensure bin/data/sfx exists and is populated.";
	}

	// camera settings
	cam.setNearClip(1.0f);
	cam.setFarClip(5000.0f);
	cam.setFov(90.0f);

	// setup meshes
	skySphere.setRadius(1000);
	skySphere.setResolution(6);
	power_up_mesh.setRadius(20);
	power_up_mesh.setResolution(10);
	empty_mesh.setRadius(0);
	empty_mesh.setResolution(0);

	// create the player
	player = new PlayerGameObject(empty_mesh.getMesh(), glm::vec3(0), 1.0f, cam);

	// init light position to above the player
	light_pos = player->getPosition(); 
	light_pos.y = LIGHT_HEIGHT;

	// example objs (FOR TESTING PURPOSES, NOT USEFUL AT ALL RN)
	/*
	power_up_vec.push_back(new PowerUpObject(power_up_mesh.getMesh(), glm::vec3(200, 0, 0), 1.f));
	power_up_vec.push_back(new PowerUpObject(power_up_mesh.getMesh(), glm::vec3(-200, 0, 0), 1.f));
	power_up_vec.push_back(new PowerUpObject(power_up_mesh.getMesh(), glm::vec3(0, 200, 0), 1.f));
	power_up_vec.push_back(new PowerUpObject(power_up_mesh.getMesh(), glm::vec3(0, -200, 0), 1.f));
	power_up_vec.push_back(new PowerUpObject(power_up_mesh.getMesh(), glm::vec3(0, 0, 200), 1.f));
	power_up_vec.push_back(new PowerUpObject(power_up_mesh.getMesh(), glm::vec3(0, 0, -200), 1.f));
	*/

	// setup red blood cell particle system
	rbc = new ParticleSystem(player->getCamera(), 25);
	rbc->loadShader("shader/rbcparticle.vert", "shader/rbcparticle.frag", "shader/rbcparticle.geom");
	rbc->loadImage("images/redbloodcell.jpg");
	rbc->setupRbcParticles();
	ofMesh mesh = ofMesh::sphere(25, 100);
	redBloodCell = new RedBloodCell(rbc, mesh, glm::vec3(20, 200, -20), 1.0f);

	// setup SSE handler
	screenSpaceEffect.setup(ofGetWidth(), ofGetHeight());

	// helper for font and text elements
	setupTextElements();

	// helper for interactables
	setupInteractableObjects();

	// create collidable geometry, store in player so collision resolving works properly
	setupDynamicWalls();
	createWalls();
	player->setWalls(&wall_objects_vec);
	
	// make bullet hell checkpoint but have it not be visible or collidable until the user completes the bullet hell
	bulletHellCheckpoint = new CheckpointGameObject(power_up_mesh.getMesh(), glm::vec3(-3500, -10, 950), 1.f, glm::vec3(15000, 0, 0));
	bulletHellCheckpoint->setVisible(false);
	bulletHellCheckpoint->setCollidable(false);
	checkpoint_vec.push_back(bulletHellCheckpoint);

	// bullet hell spawn stuff
	bulletHellEnemyMesh.set(30, 100);
	enemySpawner.setup(&bulletHellEnemyMesh.getMesh());
	bloodBulletHellActive = false;
	isBulletHellComplete = false;

	// bone marrow event stuff
	boneSpikeMinigameActive = false;
	isBoneMarrowComplete = false;
	boneSpikeSpawner.setup(player);

	// spawn portals
	createBloodSpawnPortals();

	// play main menu music
	menu_music.play();
}


// Sets up gameplay specific elements
void ofApp::setupGameplayGameState(void) {

	ofBackground(80);
	menu_music.stop();
	gameplay_music.play();

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

	for (InteractableObject* obj : interactables_vec) {
		delete obj;
	}
	interactables_vec.clear();

	for (ParticleSystem* ps : infection_ps_vec) {
		delete ps;
	}
	infection_ps_vec.clear();

	for (ParticleSystem* ps : spawn_portal_ps_vec) {
		delete ps;
	}
	spawn_portal_ps_vec.clear();

	enemySpawner.clearEnemies();
	boneSpikeSpawner.clearSpikes();

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

		// check for game over beforehand
		if (player->getHealth() <= 0) {
			gameOverTimer.Start(5.0f);
			game_state = 2;
		}

		else {

			/*** PLAYER HANDLING ***/

			player->update(delta_time);

			/*** ENEMY HANDLING (OLD) ***/

			// updates for opps

			/*
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
					player->setHealth(player->getHealth() - 1);
					// add SSE or other indicator that damage occurred

					// clean up enemy
					delete enemy;
					opposition_vec.erase(opposition_vec.begin() + i);
				}
			}
			*/

			// blood bullet hell stuff (all collision detection and deletion and allat is handled in the class by passing in the player)
			if (bloodstream) {
				if (bloodBulletHellActive) {
					enemySpawner.update(delta_time, player);
					// end after certain amount of time
					if (bloodBulletHellTimer.FinishedAndStop()) {
						endBloodBulletHell();
						// redundant but yea
						if (player->getHealth() > 0) {
							bulletHellComplete();
						}
					}
				}
			}

			if (boneMarrow) {
				// update bone spike minigame
				if (boneSpikeMinigameActive) {
					boneSpikeSpawner.update(delta_time, player);

					// check if minigame is complete
					if (boneSpikeSpawner.isComplete()) {
						boneSpikeMinigameActive = false;
						boneSpikeSpawner.stopMinigame();
						isBoneMarrowComplete = true;

						// end of the game - show victory screen
						gameOverTimer.Start(10.0f);
						game_state = 3; // game won state
					}
				}
			}

			/*** CHECKPOINT HANDLING ***/

			for (int i = 0; i < checkpoint_vec.size(); ++i) {
				CheckpointGameObject* checkpoint = checkpoint_vec[i];

				// Check for collision with checkpoint
				float dist = glm::distance(player->getPosition(), checkpoint->getPosition());
				if (dist <= player->getRadius() + checkpoint->getRadius() && checkpoint->isCollidable()) {

					// Teleport player and change environment
					handleCheckpointCollision(checkpoint);

					// Remove the checkpoint
					delete checkpoint;
					checkpoint_vec.erase(checkpoint_vec.begin() + i);
					break; // Exit loop since we modified the vector
				}
			}

			/*** INTERACTABLE HANDLING ***/

			// check if player is in interact range
			bool player_in_range = false;
			for (InteractableObject* interact_obj : interactables_vec) {
				if (glm::distance(interact_obj->getPosition(), player->getPosition()) < INTERACT_RANGE) {
					player_in_range = true;
				}
			}
			if (player_in_range) show_interact_tip = true;
			else show_interact_tip = false;

			/*** PARTICLE SYSTEM HANDLING ***/

			redBloodCell->update(delta_time); // will update particle system stored inside
			for (ParticleSystem* i_ps : infection_ps_vec) {
				i_ps->update();
			}
			for (ParticleSystem* sp_ps : spawn_portal_ps_vec) {
				sp_ps->update();
			}

			/*** LIGHT SOURCE HANDLING ***/

			glm::vec3 targetPos = player->getPosition();
			targetPos.y = LIGHT_HEIGHT; // ensure light is above player
			float lerpSpeed = 0.5f * delta_time;
			light_pos = glm::mix(light_pos, targetPos, lerpSpeed);
			lightSphere.setPosition(light_pos);

			/*** SCREEN SPACE EFFECT HANDLING ***/

			screenSpaceEffect.setInBloodstream(bloodstream);
			screenSpaceEffect.setSpeedBoostActive(player->isSpeedBoostOn());

		}
	}

	// -------------------- GAME OVER GAME STATE ---------------------------
	else if (game_state == 2) {
		if (gameOverTimer.Finished()) {
			ofExit();
		}
	}

	// -------------------- GAME WON GAME STATE ---------------------------
	else if (game_state == 3) {
		if (gameOverTimer.Finished()) {
			ofExit();
		}
	}

}


//================================================================================================
//                                  DRAWING / RENDERING RELATED 
//================================================================================================
void ofApp::draw() {

	// -------------------- MAIN MENU GAME STATE ---------------------------
	if (game_state == 0) {

		ofSetColor(255, 255, 255);
		
		// background image for main menu, texture is changed dynamically using the menu shader
		menu_shader.begin();
		menu_shader.setUniform1f("u_time", ofGetElapsedTimef());
		if (menu_texture.isAllocated()) {
			menu_texture.draw(0, 0, ofGetWidth(), ofGetHeight());
		}
		menu_shader.end();

		// setup button rectangle positions
		float center_x = ofGetWidth() / 2.0f;
		float center_y = ofGetHeight() / 2.0f;

		// title specific
		ofRectangle title_bounds = menu_title_font.getStringBoundingBox(TITLE_TEXT, 0, 0);
		float title_x = center_x - (title_bounds.width / 2.0f);
		float title_y = center_y - 100; // top
		menu_title_font.drawString(TITLE_TEXT, title_x, title_y);

		// start button specific
		ofRectangle start_bounds = menu_button_font.getStringBoundingBox(START_TEXT, 0, 0);
		float start_x = center_x - (start_bounds.width / 2.0f);
		float start_y = center_y + 150; // middle-bottom
		start_button_rect = menu_button_font.getStringBoundingBox(START_TEXT, start_x, start_y);

		// quit button specific
		ofRectangle quit_bounds = menu_button_font.getStringBoundingBox(QUIT_TEXT, 0, 0);
		float quit_x = center_x - (quit_bounds.width / 2.0f);
		float quit_y = center_y + 200; // just under start
		quit_button_rect = menu_button_font.getStringBoundingBox(QUIT_TEXT, quit_x, quit_y);

		// caption specific
		ofRectangle caption_bounds = menu_caption_font.getStringBoundingBox(CAPTION_TEXT, 0, 0);
		float caption_x = center_x - (caption_bounds.width / 2.0f);
		float caption_y = ofGetHeight() - 50; // bottom
		menu_caption_font.drawString(CAPTION_TEXT, caption_x, caption_y);

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

		screenSpaceEffect.begin();
		ofEnableDepthTest();
		player->getCamera().begin();

		// Draw skybox first
		glDepthMask(GL_FALSE);
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

		lightingShader->begin();

		// setup lighting shader attributes
		if (bUseTexture) {
			lightingShader->setUniform1i("useTexture", 1);
			lightingShader->setUniformTexture("tex0", texture, 0);
		}
		else {
			lightingShader->setUniform1i("useTexture", 0);
		}
		lightingShader->setUniformMatrix4f("viewMatrix", player->getCamera().getModelViewMatrix());
		lightingShader->setUniformMatrix4f("modelViewProjectionMatrix", player->getCamera().getModelViewProjectionMatrix());
		lightingShader->setUniformMatrix4f("projectionMatrix", player->getCamera().getProjectionMatrix());

		lightingShader->setUniform3f("lightColor", glm::vec3(1, 1, 1));
		lightingShader->setUniform1i("specularPower", 128);
		if (bloodstream) {
			lightingShader->setUniform3f("emissionColor", glm::vec3(1.0f, 0.7f, 0.7f));
		}
		else if (boneMarrow) {
			lightingShader->setUniform3f("emissionColor", glm::vec3(0.8f, 0.9f, 1.0f));
		}
		glm::mat4 view = player->getCamera().getModelViewMatrix();
		glm::vec3 viewLight = glm::vec3(view * glm::vec4(light_pos, 1.0));
		lightingShader->setUniform3f("lightPos", viewLight);
		glm::vec3 camPos = player->getCamera().getPosition();
		glm::vec3 viewCam = glm::vec3(view * glm::vec4(camPos, 1.0));
		lightingShader->setUniform3f("viewPos", viewCam);
		
		// draw the light source sphere - needs additional uniforms (normally set in gameobject class)
		lightingShader->setUniformMatrix4f("worldMatrix", lightSphere.getGlobalTransformMatrix());
		lightingShader->setUniform3f("objectColor", glm::vec3(1.0, 0.9, 0.2));
		lightingShader->setUniform1i("isLight", true);
		lightingShader->setUniform3f("emissionColor", glm::vec3(1, 1, 0.4));
		lightSphere.draw();

		// start drawing game objects
		player->draw(lightingShader);
		for (int i = 0; i < opposition_vec.size(); ++i) {
			opposition_vec[i]->draw(lightingShader);
		}

		// keep order like this bc its kinda odd
		enemySpawner.draw(lightingShader);

		if (boneMarrow && boneSpikeMinigameActive) {
			boneSpikeSpawner.draw(lightingShader);

			/*
			// Draw minigame timer in HUD
			if (boneSpikeSpawner.isActive()) {
				float timeLeft = BONE_SPIKE_MINIGAME_DURATION - boneSpikeSpawner.getMinigameTimer().getElapsed();
				if (timeLeft < 0) timeLeft = 0;

				ofSetColor(255, 255, 0);
				ofDrawBitmapString("Spike Minigame: " + to_string((int)timeLeft) + "s", glm::vec2(30, 130));
			}
			*/
		}

		for (int i = 0; i < power_up_vec.size(); ++i) {
			power_up_vec[i]->draw(lightingShader);
		}
		for (int i = 0; i < checkpoint_vec.size(); ++i) {
			checkpoint_vec[i]->draw(lightingShader);
		}

		// interactable drawing, no texture
		lightingShader->setUniform1i("useTexture", 0);
		for (InteractableObject* interact_obj : interactables_vec) {
			interact_obj->draw(lightingShader);
		}
		if (bUseTexture) lightingShader->setUniform1i("useTexture", 1);

		// different texture for walls
		if (bUseTexture && bloodstream) {
			lightingShader->setUniformTexture("tex0", bloodstreamWallTexture, 0);
		}
		else if (bUseTexture && boneMarrow) {
			lightingShader->setUniformTexture("tex0", boneMarrowWallTexture, 0);
		}
		for (GameObject* wall : wall_objects_vec) {
			wall->draw(lightingShader);
		}

		// draw particle system related objects
		redBloodCell->draw(lightingShader);
		for (ParticleSystem* ps : infection_ps_vec) {
			ps->draw();
		}
		for (ParticleSystem* sp_ps : spawn_portal_ps_vec) {
			sp_ps->draw();
		}

		lightingShader->end();

		player->getCamera().end();
		ofDisableDepthTest();
		screenSpaceEffect.end();

		screenSpaceEffect.draw();

		// HUD elements

		textBox.draw();
		ofSetColor(255);

		// press F to interact tip
		if (show_interact_tip) {
			float center_x = ofGetWidth() / 2.0f;
			float center_y = ofGetHeight() / 2.0f;
			ofRectangle interact_bounds = dialog_font.getStringBoundingBox(INTERACT_TEXT, 0, 0);
			float caption_x = center_x - (interact_bounds.width / 2.0f);
			float caption_y = center_y + 200;
			dialog_font.drawString(INTERACT_TEXT, caption_x, caption_y);
		}

		float padding = 30.0f;

		// health
		string healthStr = "Health: " + std::to_string(player->getHealth());
		float healthX = padding;
		float healthY = ofGetHeight() - padding;
		hud_font.drawString(healthStr, healthX, healthY);

		// speed boost
		string speedStr;
		if (player->isSpeedBoostReady()) speedStr = "Speed Boost: Ready!";
		else speedStr = "Speed Boost: On Cooldown.";
		float speedX = padding;
		float speedY = ofGetHeight() - (padding * 2);
		hud_font.drawString(speedStr, speedX, speedY);

		// veins/marrow infected
		string infectionStr = "";
		if (bloodstream) infectionStr = "Veins Infected: " + std::to_string(veins_infected_count) + " / 4";
		else if (boneMarrow) infectionStr = "Marrow Infected: " + std::to_string(marrow_infected_count) + " / 9";
		ofRectangle bounds = hud_font.getStringBoundingBox(infectionStr, 0, 0);
		float infectX = ofGetWidth() - bounds.width - padding;
		float infectY = ofGetHeight() - padding;
		hud_font.drawString(infectionStr, infectX, infectY);
		
		// DEBUG: minecraft f3 menu with developer info
		ofDrawBitmapString("FPS: " + to_string(ofGetFrameRate()), glm::vec2(30, 30));
		ofDrawBitmapString("X-pos: " + to_string(player->getPosition().x), glm::vec2(30, 50));
		ofDrawBitmapString("Y-pos: " + to_string(player->getPosition().y), glm::vec2(30, 60));
		ofDrawBitmapString("Z-pos: " + to_string(player->getPosition().z), glm::vec2(30, 70));

	}

	// -------------------- GAME OVER GAME STATE ---------------------------
	else if (game_state == 2) {
		if (!gameOverTimer.Finished()) {
			ofRectangle title_bounds = game_over_font.getStringBoundingBox(GAME_OVER_TEXT, 0, 0);
			float title_x = ofGetWidth() / 2.0f - (title_bounds.width / 2.0f);
			float title_y = ofGetHeight() / 2.0f;
			game_over_font.drawString(GAME_OVER_TEXT, title_x, title_y);
		}
	}

	// -------------------- GAME WON GAME STATE ---------------------------
	else if (game_state == 3) {
		ofRectangle title_bounds = game_over_font.getStringBoundingBox(GAME_WON_TEXT, 0, 0);
		float title_x = ofGetWidth() / 2.0f - (title_bounds.width / 2.0f);
		float title_y = ofGetHeight() / 2.0f;
		game_over_font.drawString(GAME_WON_TEXT, title_x, title_y);
	}
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
          
	// --------------------- gameplay related keybinds --------------------------
	if (game_state == 1) {

		// player speed boost
		if (key == 'e' || key == 'E') {
			if (player->isSpeedBoostReady()) {
				player->activateSpeedBoost();
				speed_boost_sound.play();
			}
		}

		// interact key
		if (key == 'f' || key == 'F') {
			if (show_interact_tip) {

				// check which interactable needs to get infected and removed
				for (int i = 0; i < interactables_vec.size(); ++i) {
					if (glm::distance(interactables_vec[i]->getPosition(), player->getPosition()) < INTERACT_RANGE) {
						InteractableObject* interactable = interactables_vec[i]; // found vein to infect
						
						if (bloodstream) {
							veins_infected_count++;
							updateBulletHellWall();
						}

						else if (boneMarrow) {
							marrow_infected_count++;
							updateBoneMarrowBlockingWalls();
							if (marrow_infected_count == 9 && !boneSpikeMinigameActive) {
								boneSpikeMinigameActive = true;
								boneSpikeSpawner.startMinigame(BONE_SPIKE_MINIGAME_DURATION);

								// Play sound
								room_event_start.play();

								// Display message
								textBox.setup("Survive for the incoming spikes for 45 seconds!", &dialog_font, 500.0f);
								textBox.setSize(500, 120);
								textBox.showTemporarily(5.0f);
							}
						}

						infect_sound.play();
						spawnInfectedPS(interactable->getPosition());

						spawnEnemiesAfterInfect(interactable);

						interactables_vec.erase(interactables_vec.begin() + i);
						delete interactable;
						break;
					}
				}
			}
		}
	}

	// --------------------- universal keybinds --------------------------

	// mute the music (maybe do sfx too but at that point just set vol mixer to 0)
	if (key == 'm' || key == 'M') {
		is_muted = !is_muted;
		if (is_muted) {
			menu_music.setVolume(0.0f);
			gameplay_music.setVolume(0.0f);
		}
		else {
			menu_music.setVolume(MUSIC_VOL);
			gameplay_music.setVolume(MUSIC_VOL);
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

	/*
	if (key == 'g' || key == 'G') {
		if (!bloodBulletHellActive) {
			startBloodBulletHell(30.0f);
		}
	}
	*/

	if (key == 'l' || key == 'L') {
		player->setPosition(glm::vec3(15000, 0, 0));
		boneMarrow = !boneMarrow;
		marrow_infected_count = 4;
		updateBoneMarrowBlockingWalls();
		marrow_infected_count = 8;
		updateBoneMarrowBlockingWalls();
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

// sound setup helper function
void ofApp::setupSFX(void) {

	// music
	menu_music.load("sfx/menu_music.mp3");
	menu_music.setLoop(true);
	menu_music.setVolume(MUSIC_VOL);

	gameplay_music.load("sfx/bg_music.mp3");
	gameplay_music.setLoop(true);
	gameplay_music.setVolume(MUSIC_VOL);

	// sounds
	infect_sound.load("sfx/vein_infect.wav");
	infect_sound.setLoop(false);
	infect_sound.setVolume(SFX_VOL);

	speed_boost_sound.load("sfx/speed_boost.wav");
	speed_boost_sound.setLoop(false);
	speed_boost_sound.setVolume(SFX_VOL);

	room_event_start.load("sfx/room_event_start.wav");
	room_event_start.setLoop(false);
	room_event_start.setVolume(SFX_VOL);

	room_event_end.load("sfx/room_event_end.wav");
	room_event_end.setLoop(false);
	room_event_end.setVolume(SFX_VOL);

	checkpoint_teleport.load("sfx/teleport.wav");
	checkpoint_teleport.setLoop(false);
	checkpoint_teleport.setVolume(SFX_VOL);
}

void ofApp::setupShaders() {
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

	// setup the main menu's dynamic texture
	menu_shader.load("shader/menu_bg");
	if (!menu_shader.isLoaded()) {
		ofLogError() << "Menu Shader failed to load!";
	}
}

void ofApp::setupTextures() {
	// init textures
	ofDisableArbTex();
	texture.load("images/DOG.png");
	skyTexture.load("images/blood.jpg");
	skyTexture.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
	bloodstreamWallTexture.load("images/bloodold.jpeg");
	boneMarrowWallTexture.load("images/bonemarrow.jpg");
}

void ofApp::setupTextElements() {
	// setup all text elements
	try {
		menu_button_font.load("fonts/ArialMedium.ttf", 32);
		menu_title_font.load("fonts/PapyrusBold.ttf", 96);
		menu_caption_font.load("fonts/ArialMedium.ttf", 14);
		game_over_font.load("fonts/ArialMedium.ttf", 72);
		menu_texture.load("images/menu_bg.png");
		dialog_font.load("fonts/ArialMedium.ttf", 24);
		hud_font.load("fonts/ArialMedium.ttf", 20);
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
	textBox.setPosition((ofGetWidth() / 2.0f) - 250, ofGetHeight() - 150);
	textBox.setBorderWidth(3.0f);
	showTextBox = false;
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

	// in bloodstream, going to bone marrow
	if (bloodstream && isBulletHellComplete) {

		player->setPosition(checkpoint->getTeleportPosition());
		bloodstream = false;
		boneMarrow = true;
		skyTexture.load("images/DOG.png");
		checkpoint_teleport.play();

		ofLog() << "Teleported to Bone Marrow environment";

	}
}

// helper for spawning in infected particles on an infection interactable
void ofApp::spawnInfectedPS(const glm::vec3& position) {

	// setup infected particle system and place into storage
	ParticleSystem* infected_ps = new ParticleSystem(player->getCamera(), 15);
	infected_ps->loadShader("shader/biohazardparticle.vert", "shader/biohazardparticle.frag", "shader/biohazardparticle.geom");
	infected_ps->loadImage("images/biohazard.png");
	infected_ps->setupInfectionParticles();
	infected_ps->setPosition(position);
	infection_ps_vec.push_back(infected_ps);

}

// creates an invisible spawn portal at a given position and orientation, adds to spawn_portal_ps_vec
void ofApp::createSpawnPortal(const glm::vec3& pos, const glm::quat& orientation) {

	ParticleSystem* new_sp_ps = new ParticleSystem(player->getCamera(), 250);
	new_sp_ps->loadShader("shader/portal_particle.vert", "shader/portal_particle.frag", "shader/portal_particle.geom");
	new_sp_ps->loadImage("images/smoketex.png");
	new_sp_ps->setupSpawnPortalParticles();
	new_sp_ps->setPosition(pos);
	new_sp_ps->setOrientation(orientation);
	new_sp_ps->setVisbility(false);
	spawn_portal_ps_vec.push_back(new_sp_ps);

}

// find out which portal to spawn, make it visible, spawn enemies out of it
void ofApp::spawnEnemiesAfterInfect(InteractableObject* interact_obj) {
	
	// early exit for final room spawning infects
	if (bloodstream && veins_infected_count >= 4) return;
	if (boneMarrow && marrow_infected_count >= 9) return;

	ParticleSystem* sp_ps = nullptr;

	// find portal, ensure we don't go OOB
	try {
		sp_ps = spawn_portal_ps_vec[interact_obj->getSpawnPortalIndex()];
	}
	catch (...) {
		ofLog() << "Spawn Portal Logic had an index go out of bounds, go investigate!!";
	}
	
	// operations
	if (sp_ps != nullptr) {
		sp_ps->setVisbility(true);
	}
}

// create walls for the play area, as well as any other collidable objects 
void ofApp::createWalls() {

	// bloodstream
	createWallsSection1();
	createWallsSection2();
	createWallsSection3();
	createWallsSection4();
	createVeins();
	createBloodStreamLookout();

	// bone marrow
	createWallsSection5();
	createWallsSection6();
	createWallsSection7();
	createBoneMarrowLookout();

	/*
	ofLog() << "Created walls: " << wall_objects_vec.size() << " objects";
	ofLog() << "Player starts at position: " << player->getPosition();
	*/
}

void ofApp::setupDynamicWalls() {
	// setup the wall (door) to the bullet hell room
	ofBoxPrimitive bulletHellWallMesh;
	bulletHellWallMesh.set(300, 300, 20);
	bulletHellWall = new GameObject(bulletHellWallMesh.getMesh(), glm::vec3(-2980, 300 / 2 - 50, 975), 1.0f);
	bulletHellWall->setOrientation(glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)));
	wall_objects_vec.push_back(bulletHellWall);

	// to block certain spots of bone marrow (until infected all necessary objects)
	ofBoxPrimitive boneMarrowWallMesh;
	boneMarrowWallMesh.set(300, 300, 20);
	boneMarrowBlockingWall1 = new GameObject(boneMarrowWallMesh.getMesh(), glm::vec3(12700, 300 / 2 - 50, 450), 1.0f);
	boneMarrowBlockingWall1->setOrientation(glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)));
	wall_objects_vec.push_back(boneMarrowBlockingWall1);

	boneMarrowWallMesh.set(265, 300, 20);
	boneMarrowBlockingWall2 = new GameObject(boneMarrowWallMesh.getMesh(), glm::vec3(11557.5, 300 / 2 - 50, -1050), 1.0f);
	wall_objects_vec.push_back(boneMarrowBlockingWall2);
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
	floorMesh.set(7000, 5, 6000);
	GameObject* floor = new GameObject(floorMesh.getMesh(), glm::vec3(-1850, -50, -100), 1.0f);
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
	ceilingMesh.set(5000, 5, 2200);
	GameObject* ceiling = new GameObject(ceilingMesh.getMesh(), glm::vec3(-1500, wallHeight - 50, 700), 1.0f);
	ceiling->setVisible(false);
	wall_objects_vec.push_back(ceiling);

}

// starts at hallways out of first room
void ofApp::createWallsSection2() {
	
	// wall dimensions
	float wallThickness = 20.0f;
	float wallHeight = 300.0f;

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

	sideWallMesh.set(250, wallHeight, wallThickness);
	GameObject* sideWall8 = new GameObject(sideWallMesh.getMesh(), glm::vec3(-2250, wallHeight / 2 - 50, 250), 1.0f);
	wall_objects_vec.push_back(sideWall8);
}

// boss room
void ofApp::createWallsSection4() {

	// wall dimensions
	float wallThickness = 20.0f;
	float wallHeight = 300.0f;

	// entrance split walls
	ofBoxPrimitive entranceWalls;
	entranceWalls.set(600, wallHeight, wallThickness);
	GameObject* entranceWall1 = new GameObject(entranceWalls.getMesh(), glm::vec3(-2980, wallHeight / 2 - 50, 530), 1.0f);
	entranceWall1->setOrientation(glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)));
	wall_objects_vec.push_back(entranceWall1);

	GameObject* entranceWall2 = new GameObject(entranceWalls.getMesh(), glm::vec3(-2980, wallHeight / 2 - 50, 1425), 1.0f);
	entranceWall2->setOrientation(glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)));
	wall_objects_vec.push_back(entranceWall2);

	// side walls
	ofBoxPrimitive sideWallMesh;
	sideWallMesh.set(1000, wallHeight, wallThickness);
	GameObject* sideWall1 = new GameObject(sideWallMesh.getMesh(), glm::vec3(-3480, wallHeight / 2 - 50, 1700), 1.0f);
	wall_objects_vec.push_back(sideWall1);

	GameObject* sideWall2 = new GameObject(sideWallMesh.getMesh(), glm::vec3(-3480, wallHeight / 2 - 50, 225), 1.0f);
	wall_objects_vec.push_back(sideWall2);

	// back wall
	ofBoxPrimitive backWallMesh;
	backWallMesh.set(1600, wallHeight, wallThickness);
	GameObject* backWall = new GameObject(backWallMesh.getMesh(), glm::vec3(-3980, wallHeight / 2 - 50, 955), 1.0f);
	backWall->setOrientation(glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)));
	wall_objects_vec.push_back(backWall);

	/*
	ofBoxPrimitive obstacle1Mesh;
	obstacle1Mesh.set(50, 50, 50);
	GameObject* obstacle1 = new GameObject(obstacle1Mesh.getMesh(), glm::vec3(-3040, - 25, 290), 1.0f);
	wall_objects_vec.push_back(obstacle1);
	*/
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

	// fourth vein, causes blood bullet hell to start -3950 950
	ofCylinderPrimitive vein_mesh4;
	vein_mesh4.set(radius, 300.0f);
	vein_mesh4.setResolution(18, 2);
	glm::vec3 vein4_pos(-3975, 100, 950);
	GameObject* vein4 = new GameObject(vein_mesh4.getMesh(), vein4_pos, 1.0f);
	//vein4->setOrientation(glm::angleAxis(glm::radians(90.0f), glm::vec3(1, 0, 0)));
	wall_objects_vec.push_back(vein4);

	// bone marrow
	ofCylinderPrimitive vein_mesh5;
	vein_mesh5.set(radius, 320.0f);
	vein_mesh5.setResolution(18, 2);
	glm::vec3 vein5_pos(13325, 100, 1025);
	GameObject* vein5 = new GameObject(vein_mesh5.getMesh(), vein5_pos, 1.0f);
	//vein4->setOrientation(glm::angleAxis(glm::radians(90.0f), glm::vec3(1, 0, 0)));
	wall_objects_vec.push_back(vein5);

	ofCylinderPrimitive vein_mesh6;
	vein_mesh6.set(radius, 285.0f);
	vein_mesh6.setResolution(18, 2);
	glm::vec3 vein6_pos(13465, 100, 675);
	GameObject* vein6 = new GameObject(vein_mesh6.getMesh(), vein6_pos, 1.0f);
	vein6->setOrientation(glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 0, 1)));
	wall_objects_vec.push_back(vein6);

	ofCylinderPrimitive vein_mesh7;
	vein_mesh7.set(radius, 285.0f);
	vein_mesh7.setResolution(18, 2);
	glm::vec3 vein7_pos(13160, 100, -100);
	GameObject* vein7 = new GameObject(vein_mesh7.getMesh(), vein7_pos, 1.0f);
	vein7->setOrientation(glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 0, 1)));
	wall_objects_vec.push_back(vein7);

	ofCylinderPrimitive vein_mesh8;
	vein_mesh8.set(radius, 300.0f);
	vein_mesh8.setResolution(18, 2);
	glm::vec3 vein8_pos(10800, 100, -185);
	GameObject* vein8 = new GameObject(vein_mesh8.getMesh(), vein8_pos, 1.0f);
	wall_objects_vec.push_back(vein8);

	ofCylinderPrimitive vein_mesh9;
	vein_mesh9.set(radius, 300.0f);
	vein_mesh9.setResolution(18, 2);
	glm::vec3 vein9_pos(11200, 100, 35);
	GameObject* vein9 = new GameObject(vein_mesh9.getMesh(), vein9_pos, 1.0f);
	wall_objects_vec.push_back(vein9);

	ofCylinderPrimitive vein_mesh10;
	vein_mesh10.set(radius, 300.0f);
	vein_mesh10.setResolution(18, 2);
	glm::vec3 vein10_pos(12700, 100, 700);
	GameObject* vein10 = new GameObject(vein_mesh10.getMesh(), vein10_pos, 1.0f);
	wall_objects_vec.push_back(vein10);

	ofCylinderPrimitive vein_mesh11;
	vein_mesh11.set(radius, 300.0f);
	vein_mesh11.setResolution(18, 2);
	glm::vec3 vein11_pos(11925, 100, 120);
	GameObject* vein11 = new GameObject(vein_mesh11.getMesh(), vein11_pos, 1.0f);
	wall_objects_vec.push_back(vein11);

	ofCylinderPrimitive vein_mesh12;
	vein_mesh12.set(radius, 300.0f);
	vein_mesh12.setResolution(18, 2);
	glm::vec3 vein12_pos(11550, 100, -2085);
	GameObject* vein12 = new GameObject(vein_mesh12.getMesh(), vein12_pos, 1.0f);
	wall_objects_vec.push_back(vein12);
}

// helper function to create interactable objects on the veins and bone marrow
void ofApp::setupInteractableObjects() {

	// bloodstream
	glm::vec3 infect1_pos(600, 50, 130);
	glm::vec3 infect2_pos(-830, 70, 1350);
	glm::vec3 infect3_pos(-2250, 45, 425);
	glm::vec3 infect4_pos(-3950, 100, 950);

	// bone marrow
	glm::vec3 infect5_pos(13295, 150, 1035);
	glm::vec3 infect6_pos(13340, 60, 985);
	glm::vec3 infect7_pos(13465, 100, 710);
	glm::vec3 infect8_pos(13160, 100, -50);
	glm::vec3 infect9_pos(10800, 100, -150);
	glm::vec3 infect10_pos(11200, 100, 0);
	glm::vec3 infect11_pos(12665, 100, 700);
	glm::vec3 infect12_pos(11890, 100, 125);
	glm::vec3 infect13_pos(11550, 100, -2055);


	// bloodstream interactables
	InteractableObject* interact_obj1 = new InteractableObject(power_up_mesh.getMesh(), infect1_pos, 1.f, 0);
	interact_obj1->setColour(glm::vec3(1.0f, 0.9f, 0.4f));
	interactables_vec.push_back(interact_obj1);

	InteractableObject* interact_obj2 = new InteractableObject(power_up_mesh.getMesh(), infect2_pos, 1.f, 1);
	interact_obj2->setColour(glm::vec3(1.0f, 0.9f, 0.4f));
	interactables_vec.push_back(interact_obj2);

	InteractableObject* interact_obj3 = new InteractableObject(power_up_mesh.getMesh(), infect3_pos, 1.f, 2);
	interact_obj3->setColour(glm::vec3(1.0f, 0.9f, 0.4f));
	interactables_vec.push_back(interact_obj3);

	InteractableObject* interact_obj4 = new InteractableObject(power_up_mesh.getMesh(), infect4_pos, 1.f, -1); // starts bullet hell
	interact_obj4->setColour(glm::vec3(1.0f, 0.9f, 0.4f));
	interactables_vec.push_back(interact_obj4);

	// bone marrow interactables
	InteractableObject* interact_obj5 = new InteractableObject(power_up_mesh.getMesh(), infect5_pos, 1.f, 3);
	interact_obj5->setColour(glm::vec3(1.0f, 0.3f, 0.2f));
	interactables_vec.push_back(interact_obj5);

	InteractableObject* interact_obj6 = new InteractableObject(power_up_mesh.getMesh(), infect6_pos, 1.f, 4);
	interact_obj6->setColour(glm::vec3(1.0f, 0.3f, 0.2f));
	interactables_vec.push_back(interact_obj6);

	InteractableObject* interact_obj7 = new InteractableObject(power_up_mesh.getMesh(), infect7_pos, 1.f, 5);
	interact_obj7->setColour(glm::vec3(1.0f, 0.3f, 0.2f));
	interactables_vec.push_back(interact_obj7);

	InteractableObject* interact_obj8 = new InteractableObject(power_up_mesh.getMesh(), infect8_pos, 1.f, 6);
	interact_obj8->setColour(glm::vec3(1.0f, 0.3f, 0.2f));
	interactables_vec.push_back(interact_obj8);

	InteractableObject* interact_obj9 = new InteractableObject(power_up_mesh.getMesh(), infect9_pos, 1.f, 7);
	interact_obj9->setColour(glm::vec3(1.0f, 0.3f, 0.2f));
	interactables_vec.push_back(interact_obj9);

	InteractableObject* interact_obj10 = new InteractableObject(power_up_mesh.getMesh(), infect10_pos, 1.f, 8);
	interact_obj10->setColour(glm::vec3(1.0f, 0.3f, 0.2f));
	interactables_vec.push_back(interact_obj10);

	InteractableObject* interact_obj11 = new InteractableObject(power_up_mesh.getMesh(), infect11_pos, 1.f, 9);
	interact_obj11->setColour(glm::vec3(1.0f, 0.3f, 0.2f));
	interactables_vec.push_back(interact_obj11);

	InteractableObject* interact_obj12 = new InteractableObject(power_up_mesh.getMesh(), infect12_pos, 1.f, 10);
	interact_obj12->setColour(glm::vec3(1.0f, 0.3f, 0.2f));
	interactables_vec.push_back(interact_obj12);

	InteractableObject* interact_obj13 = new InteractableObject(power_up_mesh.getMesh(), infect13_pos, 1.f, -1); // starts bone minigame
	interact_obj13->setColour(glm::vec3(1.0f, 0.3f, 0.2f));
	interactables_vec.push_back(interact_obj13);
}

// create the lookout point down the right side diagonal hallway
void ofApp::createBloodStreamLookout() {

	float wall_thickness = 20.0f;
	float wall_height = 200.0f;
	ofBoxPrimitive reused_mesh;

	// inner
	reused_mesh.set(240, wall_height + 40, wall_thickness);
	GameObject* upward_wall1 = new GameObject(reused_mesh.getMesh(), glm::vec3(-1830, 280, -400), 1.0f);
	wall_objects_vec.push_back(upward_wall1);

	// outer wall, tallest one
	reused_mesh.set(240, wall_height * 3 + 50, wall_thickness);
	GameObject* upward_wall2 = new GameObject(reused_mesh.getMesh(), glm::vec3(-1870, 280, -520), 1.0f);
	wall_objects_vec.push_back(upward_wall2);

	// left wall
	reused_mesh.set(wall_thickness, wall_height * 2, 125);
	GameObject* upward_wall3 = new GameObject(reused_mesh.getMesh(), glm::vec3(-1967, 400, -460), 1.0f);
	upward_wall3->setOrientation(glm::angleAxis(glm::radians(20.0f), glm::vec3(0, 1, 0)));
	wall_objects_vec.push_back(upward_wall3);

	// right wall
	reused_mesh.set(wall_thickness, wall_height * 2, 125);
	GameObject* upward_wall4 = new GameObject(reused_mesh.getMesh(), glm::vec3(-1738, 400, -457), 1.0f);
	upward_wall4->setOrientation(glm::angleAxis(glm::radians(20.0f), glm::vec3(0, 1, 0)));
	wall_objects_vec.push_back(upward_wall4);

	// balcony floor
	reused_mesh.set(240, wall_thickness, 150);
	GameObject* balcony_floor = new GameObject(reused_mesh.getMesh(), glm::vec3(-1830, 395, -325), 1.0f);
	wall_objects_vec.push_back(balcony_floor);

	// balcony roof
	reused_mesh.set(260, wall_thickness / 2, 280);
	GameObject* balcony_roof = new GameObject(reused_mesh.getMesh(), glm::vec3(-1840, 590, -390), 1.0f);
	wall_objects_vec.push_back(balcony_roof);

	// invisible barrier, front of lookout
	reused_mesh.set(240, wall_height, wall_thickness);
	GameObject* invis_front = new GameObject(reused_mesh.getMesh(), glm::vec3(-1830, 500, -250), 1.0f);
	invis_front->setVisible(false);
	wall_objects_vec.push_back(invis_front);

	// invisible barrier, left of lookout
	reused_mesh.set(wall_thickness, wall_height, 160);
	GameObject* invis_left = new GameObject(reused_mesh.getMesh(), glm::vec3(-1710, 500, -325), 1.0f);
	invis_left->setVisible(false);
	wall_objects_vec.push_back(invis_left);

	// invisible barrier, right of lookout
	reused_mesh.set(wall_thickness, wall_height, 160);
	GameObject* invis_right = new GameObject(reused_mesh.getMesh(), glm::vec3(-1950, 500, -325), 1.0f);
	invis_right->setVisible(false);
	wall_objects_vec.push_back(invis_right);

}

// initialize the enemy spawn portals for the blood stream
void ofApp::createBloodSpawnPortals() {
	glm::vec3 reused_pos;
	glm::quat reused_quat;

	// vein 1
	reused_pos = glm::vec3(400, 80, 750);
	reused_quat = glm::angleAxis(glm::radians(-20.0f), glm::vec3(0, 1, 0));
	createSpawnPortal(reused_pos, reused_quat);

	// vein 2
	reused_pos = glm::vec3(-1400, 100, 1175);
	reused_quat = glm::angleAxis(glm::radians(-110.0f), glm::vec3(0, 1, 0));
	createSpawnPortal(reused_pos, reused_quat);

	// vein 3
	reused_pos = glm::vec3(-2250, 120, 1080);
	reused_quat = glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0));
	createSpawnPortal(reused_pos, reused_quat);

	// marrow 1
	reused_pos = glm::vec3(13175, 100, 290);
	reused_quat = glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0));
	createSpawnPortal(reused_pos, reused_quat);

	// marrow 2
	reused_pos = glm::vec3(13330, 80, 1450);
	reused_quat = glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0));
	createSpawnPortal(reused_pos, reused_quat);

	// marrow 3
	reused_pos = glm::vec3(13540, 120, 1400);
	reused_quat = glm::angleAxis(glm::radians(20.0f), glm::vec3(0, 1, 0));
	createSpawnPortal(reused_pos, reused_quat);

	// marrow 4
	reused_pos = glm::vec3(13175, 120, 600);
	reused_quat = glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0));
	createSpawnPortal(reused_pos, reused_quat);

	// marrow 5
	reused_pos = glm::vec3(11600, 90, -100);
	reused_quat = glm::angleAxis(glm::radians(80.0f), glm::vec3(0, 1, 0));
	createSpawnPortal(reused_pos, reused_quat);

	// marrow 6
	reused_pos = glm::vec3(10730, 120, -40);
	reused_quat = glm::angleAxis(glm::radians(-80.0f), glm::vec3(0, 1, 0));
	createSpawnPortal(reused_pos, reused_quat);

	// marrow 7
	reused_pos = glm::vec3(12220, 100, 350);
	reused_quat = glm::angleAxis(glm::radians(-135.0f), glm::vec3(0, 1, 0));
	createSpawnPortal(reused_pos, reused_quat);

	// marrow 8
	reused_pos = glm::vec3(11500, 110, -275);
	reused_quat = glm::angleAxis(glm::radians(-150.0f), glm::vec3(0, 1, 0));
	createSpawnPortal(reused_pos, reused_quat);

}

// functions to start/end blood bullet hell
void ofApp::startBloodBulletHell(float duration) {
	bloodBulletHellActive = true;
	bloodBulletHellTimer.Start(duration);
	enemySpawner.startSpawning(0.25f);
	room_event_start.play();
	//ofLog() << "Bullet Hell Start";
}

void ofApp::endBloodBulletHell() {
	bloodBulletHellActive = false;
	enemySpawner.stopSpawning();
	enemySpawner.clearEnemies();
	//ofLog() << "Bullet Hell End";
}

// updates the wall going into the bullet hell room
void ofApp::updateBulletHellWall() {
	if (!bulletHellWall) return;

	// if 3 veins infected, hide the wall and make sure the player cant collide with it
	if (veins_infected_count == 3) {
		bulletHellWall->setVisible(false);
		bulletHellWall->setCollidable(false);
	}
	// if 4 veins infected (the last one is in the bullet hell room), restore the wall
	else if (veins_infected_count == 4) {
		bulletHellWall->setVisible(true);
		bulletHellWall->setCollidable(true);

		// also automatically start the bullet hell
		if (!bloodBulletHellActive) {
			startBloodBulletHell(30.0f);
		}
	}
}

// handle when the player completes the bullet hell
void ofApp::bulletHellComplete() {
	isBulletHellComplete = true;
	room_event_end.play();

	// display some text here
	bulletHellCheckpoint->setVisible(true);
	bulletHellCheckpoint->setCollidable(true);
}

void ofApp::updateBoneMarrowBlockingWalls() {
	if (marrow_infected_count == 4) {
		boneMarrowBlockingWall1->setCollidable(false);
		boneMarrowBlockingWall1->setVisible(false);
	}

	else if (marrow_infected_count == 8) {
		boneMarrowBlockingWall2->setCollidable(false);
		boneMarrowBlockingWall2->setVisible(false);
	}

	else if (marrow_infected_count == 9) {
		boneMarrowBlockingWall2->setCollidable(true);
		boneMarrowBlockingWall2->setVisible(true);
	}
}

// start of bone marrow area
void ofApp::createWallsSection5() {

	// wall dimensions
	float wallThickness = 20.0f;
	float wallHeight = 300.0f;

	// floor
	ofBoxPrimitive floorMesh;
	floorMesh.set(8000, 5, 8000);
	GameObject* floor = new GameObject(floorMesh.getMesh(), glm::vec3(13000, -50, -500), 1.0f);
	wall_objects_vec.push_back(floor);

	// ceiling (big on right side, smaller ver on left so that the lookout still works
	ofBoxPrimitive ceilingMesh;
	ceilingMesh.set(5000, 5, 4000);
	GameObject* ceiling1 = new GameObject(ceilingMesh.getMesh(), glm::vec3(13000, wallHeight - 50, -1100), 1.0f);
	ceiling1->setVisible(false);
	wall_objects_vec.push_back(ceiling1);

	ceilingMesh.set(2000, 5, 1000);
	GameObject* ceiling2 = new GameObject(ceilingMesh.getMesh(), glm::vec3(14000, wallHeight - 50, 1100), 1.0f);
	ceiling2->setVisible(false);
	wall_objects_vec.push_back(ceiling2);

	// starting area

	ofBoxPrimitive startingWallMesh;
	startingWallMesh.set(400, wallHeight, wallThickness);
	GameObject* startingWall1 = new GameObject(startingWallMesh.getMesh(), glm::vec3(15200, wallHeight / 2 - 50, 0), 1.0f);
	startingWall1->setOrientation(glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)));
	wall_objects_vec.push_back(startingWall1);

	startingWallMesh.set(600, wallHeight, wallThickness);
	GameObject* startingWall2 = new GameObject(startingWallMesh.getMesh(), glm::vec3(14900, wallHeight / 2 - 50, 200), 1.0f);
	wall_objects_vec.push_back(startingWall2);

	GameObject* startingWall3 = new GameObject(startingWallMesh.getMesh(), glm::vec3(14900, wallHeight / 2 - 50, -200), 1.0f);
	wall_objects_vec.push_back(startingWall3);

	ofBoxPrimitive firstHallwayMesh;
	firstHallwayMesh.set(1200, wallHeight, wallThickness);
	GameObject* firstHallwayWall1 = new GameObject(firstHallwayMesh.getMesh(), glm::vec3(14300, wallHeight / 2 - 50, 0), 1.0f);
	firstHallwayWall1->setOrientation(glm::angleAxis(glm::radians(30.0f), glm::vec3(0, 1, 0)));
	wall_objects_vec.push_back(firstHallwayWall1);

	firstHallwayMesh.set(800, wallHeight, wallThickness);
	GameObject* firstHallwayWall2 = new GameObject(firstHallwayMesh.getMesh(), glm::vec3(14260, wallHeight / 2 - 50, 400), 1.0f);
	firstHallwayWall2->setOrientation(glm::angleAxis(glm::radians(30.0f), glm::vec3(0, 1, 0)));
	wall_objects_vec.push_back(firstHallwayWall2);

	// junction after first hallway
	ofBoxPrimitive junctionMesh;
	
	junctionMesh.set(465, wallHeight, wallThickness);
	GameObject* junctionWall1 = new GameObject(junctionMesh.getMesh(), glm::vec3(13550, wallHeight / 2 - 50, 300), 1.0f);
	wall_objects_vec.push_back(junctionWall1);

	junctionMesh.set(600, wallHeight, wallThickness);
	GameObject* junctionWall2 = new GameObject(junctionMesh.getMesh(), glm::vec3(13625, wallHeight / 2 - 50, 600), 1.0f);
	wall_objects_vec.push_back(junctionWall2);

	// wall that blocks entrance to second part, has own variable so this isn't needed anymore
	/*
	junctionMesh.set(300, wallHeight, wallThickness);
	GameObject* junctionWall3 = new GameObject(junctionMesh.getMesh(), glm::vec3(12700, wallHeight / 2 - 50, 450), 1.0f);
	junctionWall3->setOrientation(glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)));
	wall_objects_vec.push_back(junctionWall3);
	*/

	junctionMesh.set(300, wallHeight, wallThickness);
	GameObject* junctionWall5 = new GameObject(junctionMesh.getMesh(), glm::vec3(12850, wallHeight / 2 - 50, 600), 1.0f);
	wall_objects_vec.push_back(junctionWall5);

	// right hallway
	junctionMesh.set(500, wallHeight, wallThickness);
	GameObject* junctionWall8 = new GameObject(junctionMesh.getMesh(), glm::vec3(13000, wallHeight / 2 - 50, 50), 1.0f);
	junctionWall8->setOrientation(glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)));
	wall_objects_vec.push_back(junctionWall8);

	GameObject* junctionWall9 = new GameObject(junctionMesh.getMesh(), glm::vec3(13320, wallHeight / 2 - 50, 50), 1.0f);
	junctionWall9->setOrientation(glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)));
	wall_objects_vec.push_back(junctionWall9);

	junctionMesh.set(320, wallHeight, wallThickness);
	GameObject* junctionWall10 = new GameObject(junctionMesh.getMesh(), glm::vec3(13160, wallHeight / 2 - 50, -200), 1.0f);
	//junctionWall10->setOrientation(glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)));
	wall_objects_vec.push_back(junctionWall10);

	// left hallway
	junctionMesh.set(900, wallHeight, wallThickness);
	GameObject* junctionWall11 = new GameObject(junctionMesh.getMesh(), glm::vec3(13000, wallHeight / 2 - 50, 1050), 1.0f);
	junctionWall11->setOrientation(glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)));
	wall_objects_vec.push_back(junctionWall11);

	junctionMesh.set(600, wallHeight, wallThickness);
	GameObject* junctionWall12 = new GameObject(junctionMesh.getMesh(), glm::vec3(13320, wallHeight / 2 - 50, 900), 1.0f);
	junctionWall12->setOrientation(glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)));
	wall_objects_vec.push_back(junctionWall12);
	
	// area after left hallway
	junctionMesh.set(620, wallHeight, wallThickness);
	GameObject* junctionWall13 = new GameObject(junctionMesh.getMesh(), glm::vec3(13310, wallHeight / 2 - 50, 1500), 1.0f);
	//junctionWall13->setOrientation(glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)));
	wall_objects_vec.push_back(junctionWall13);

	junctionMesh.set(900, wallHeight, wallThickness);
	GameObject* junctionWall14 = new GameObject(junctionMesh.getMesh(), glm::vec3(13620, wallHeight / 2 - 50, 1050), 1.0f);
	junctionWall14->setOrientation(glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)));
	wall_objects_vec.push_back(junctionWall14);
}

// after first blocking wall
void ofApp::createWallsSection6() {
	// wall dimensions
	float wallThickness = 20.0f;
	float wallHeight = 300.0f;

	// first hallway past first blocking wall
	ofBoxPrimitive firstHallwayMesh;
	firstHallwayMesh.set(900, wallHeight, wallThickness);
	GameObject* firstHallwayWall1 = new GameObject(firstHallwayMesh.getMesh(), glm::vec3(12550, wallHeight / 2 - 50, 300), 1.0f);
	wall_objects_vec.push_back(firstHallwayWall1);

	firstHallwayMesh.set(300, wallHeight, wallThickness);
	GameObject* firstHallwayWall2 = new GameObject(firstHallwayMesh.getMesh(), glm::vec3(12250, wallHeight / 2 - 50, 600), 1.0f);
	wall_objects_vec.push_back(firstHallwayWall2);

	// first left room in hallway
	GameObject* firstHallwayWall3 = new GameObject(firstHallwayMesh.getMesh(), glm::vec3(12400, wallHeight / 2 - 50, 750), 1.0f);
	firstHallwayWall3->setOrientation(glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)));
	wall_objects_vec.push_back(firstHallwayWall3);

	GameObject* firstHallwayWall4 = new GameObject(firstHallwayMesh.getMesh(), glm::vec3(12700, wallHeight / 2 - 50, 750), 1.0f);
	firstHallwayWall4->setOrientation(glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)));
	wall_objects_vec.push_back(firstHallwayWall4);

	ofBoxPrimitive diagonalHallwayMesh;
	diagonalHallwayMesh.set(600, wallHeight, wallThickness);
	GameObject* diagonalHallwayWall1 = new GameObject(diagonalHallwayMesh.getMesh(), glm::vec3(11900, wallHeight / 2 - 50, 90), 1.0f);
	diagonalHallwayWall1->setOrientation(glm::angleAxis(glm::radians(135.0f), glm::vec3(0, 1, 0)));
	wall_objects_vec.push_back(diagonalHallwayWall1);

	diagonalHallwayMesh.set(800, wallHeight, wallThickness);
	GameObject* diagonalHallwayWall2 = new GameObject(diagonalHallwayMesh.getMesh(), glm::vec3(11820, wallHeight / 2 - 50, 320), 1.0f);
	diagonalHallwayWall2->setOrientation(glm::angleAxis(glm::radians(135.0f), glm::vec3(0, 1, 0)));
	wall_objects_vec.push_back(diagonalHallwayWall2);

	// junction
	ofBoxPrimitive junctionMesh;
	junctionMesh.set(830, wallHeight, wallThickness);
	GameObject* junctionWall1 = new GameObject(junctionMesh.getMesh(), glm::vec3(11425, wallHeight / 2 - 50, -630), 1.0f);
	junctionWall1->setOrientation(glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)));
	wall_objects_vec.push_back(junctionWall1);

	junctionMesh.set(800, wallHeight, wallThickness);
	GameObject* junctionWall2 = new GameObject(junctionMesh.getMesh(), glm::vec3(11025, wallHeight / 2 - 50, -215), 1.0f);
	wall_objects_vec.push_back(junctionWall2);

	junctionMesh.set(925, wallHeight, wallThickness);
	GameObject* junctionWall3 = new GameObject(junctionMesh.getMesh(), glm::vec3(11690, wallHeight / 2 - 50, -580), 1.0f);
	junctionWall3->setOrientation(glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)));
	wall_objects_vec.push_back(junctionWall3);

	junctionMesh.set(1000, wallHeight, wallThickness);
	GameObject* junctionWall4 = new GameObject(junctionMesh.getMesh(), glm::vec3(11050, wallHeight / 2 - 50, 45), 1.0f);
	wall_objects_vec.push_back(junctionWall4);

	// left hallway
	junctionMesh.set(300, wallHeight, wallThickness);
	GameObject* leftHallwayBackWall = new GameObject(junctionMesh.getMesh(), glm::vec3(10675, wallHeight / 2 - 50, -65), 1.0f);
	leftHallwayBackWall->setOrientation(glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)));
	wall_objects_vec.push_back(leftHallwayBackWall);
}

// final area
void ofApp::createWallsSection7() {
	// wall dimensions
	float wallThickness = 20.0f;
	float wallHeight = 300.0f;

	ofBoxPrimitive finalAreaWallMesh;
	finalAreaWallMesh.set(600, wallHeight, wallThickness);
	GameObject* finalAreaWall1 = new GameObject(finalAreaWallMesh.getMesh(), glm::vec3(11990, wallHeight / 2 - 50, -1050), 1.0f);
	wall_objects_vec.push_back(finalAreaWall1);

	GameObject* finalAreaWall2 = new GameObject(finalAreaWallMesh.getMesh(), glm::vec3(11125, wallHeight / 2 - 50, -1050), 1.0f);
	wall_objects_vec.push_back(finalAreaWall2);

	finalAreaWallMesh.set(1050, wallHeight, wallThickness);
	GameObject* finalAreaWall3 = new GameObject(finalAreaWallMesh.getMesh(), glm::vec3(11025, wallHeight / 2 - 50, -1580), 1.0f);
	finalAreaWall3->setOrientation(glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)));
	wall_objects_vec.push_back(finalAreaWall3);

	GameObject* finalAreaWall4 = new GameObject(finalAreaWallMesh.getMesh(), glm::vec3(12075, wallHeight / 2 - 50, -1580), 1.0f);
	finalAreaWall4->setOrientation(glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)));
	wall_objects_vec.push_back(finalAreaWall4);

	finalAreaWallMesh.set(1125, wallHeight, wallThickness);
	GameObject* finalAreaWall5 = new GameObject(finalAreaWallMesh.getMesh(), glm::vec3(11500, wallHeight / 2 - 50, -2100), 1.0f);
	wall_objects_vec.push_back(finalAreaWall5);
}

// create the lookout zone for the bone marrow area
void ofApp::createBoneMarrowLookout() {
	float wall_thickness = 10.0f;
	float wall_height = 200.0f;
	ofBoxPrimitive reused_mesh;

	// inner
	reused_mesh.set(300, wall_height + 40, wall_thickness);
	GameObject* upward_wall1 = new GameObject(reused_mesh.getMesh(), glm::vec3(12550, 280, 900), 1.0f);
	wall_objects_vec.push_back(upward_wall1);

	// outer wall, tallest one
	reused_mesh.set(300, wall_height * 3 + 50, wall_thickness);
	GameObject* upward_wall2 = new GameObject(reused_mesh.getMesh(), glm::vec3(12550, 280, 1200), 1.0f);
	wall_objects_vec.push_back(upward_wall2);

	// left wall
	reused_mesh.set(wall_thickness, wall_height * 3 + 50, 300);
	GameObject* upward_wall3 = new GameObject(reused_mesh.getMesh(), glm::vec3(12700, 280, 1050), 1.0f);
	wall_objects_vec.push_back(upward_wall3);

	// right wall
	reused_mesh.set(wall_thickness, wall_height * 3 + 50, 300);
	GameObject* upward_wall4 = new GameObject(reused_mesh.getMesh(), glm::vec3(12400, 280, 1050), 1.0f);
	wall_objects_vec.push_back(upward_wall4);

	// balcony floor
	reused_mesh.set(300, wall_thickness, 150);
	GameObject* balcony_floor = new GameObject(reused_mesh.getMesh(), glm::vec3(12550, 395, 825), 1.0f);
	wall_objects_vec.push_back(balcony_floor);

	// balcony roof
	reused_mesh.set(300, wall_thickness / 2, 450);
	GameObject* balcony_roof = new GameObject(reused_mesh.getMesh(), glm::vec3(12550, 590, 975), 1.0f);
	wall_objects_vec.push_back(balcony_roof);

	// invisible barrier, front of lookout
	reused_mesh.set(300, wall_height, wall_thickness / 2);
	GameObject* invis_front = new GameObject(reused_mesh.getMesh(), glm::vec3(12550, 500, 750), 1.0f);
	invis_front->setVisible(false);
	wall_objects_vec.push_back(invis_front);

	// invisible barrier, left of lookout
	reused_mesh.set(wall_thickness / 2, wall_height, 150);
	GameObject* invis_left = new GameObject(reused_mesh.getMesh(), glm::vec3(12400, 500, 825), 1.0f);
	invis_left->setVisible(false);
	wall_objects_vec.push_back(invis_left);

	// invisible barrier, right of lookout
	reused_mesh.set(wall_thickness / 2, wall_height, 150);
	GameObject* invis_right = new GameObject(reused_mesh.getMesh(), glm::vec3(12700, 500, 825), 1.0f);
	invis_right->setVisible(false);
	wall_objects_vec.push_back(invis_right);

}
