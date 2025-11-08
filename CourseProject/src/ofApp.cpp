#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetWindowTitle("Course Project - Triple Sicks");

	player = new PlayerGameObject(glm::vec3(0, 0, -10), 1.0f, 100.0f, 1.0f, cam);

	// initalize member vars
	time_elapsed = 0.f;
	lostGameOver = false;
	wonGameOver = false;
 
	// setup asteroids
	asteroids = 200;
	for (int i = 0; i < asteroids; i++) {
		astroid_vec.push_back(new AstroidGameObject(glm::vec3(ofRandom(-800, 800), ofRandom(-800, 800), ofRandom(-800, 800)), 1.f));
	}

	// create 8 checkpoints for the player to go thru
	checkpoint_vec.push_back(new CheckpointGameObject(glm::vec3(0.f, 0.f, -400.f), 1.f));
	checkpoint_vec.push_back(new CheckpointGameObject(glm::vec3(200.f, -150.f, -700.f), 1.f));
	checkpoint_vec.push_back(new CheckpointGameObject(glm::vec3(-400.f, 500.f, 300.f), 1.f));
	checkpoint_vec.push_back(new CheckpointGameObject(glm::vec3(-200.f, -500.f, -250.f), 1.f));
	checkpoint_vec.push_back(new CheckpointGameObject(glm::vec3(200.f, 300.f, 350.f), 1.f));
	checkpoint_vec.push_back(new CheckpointGameObject(glm::vec3(-700.f, -400.f, 50.f), 1.f));
	checkpoint_vec.push_back(new CheckpointGameObject(glm::vec3(100.f, 700.f, 250.f), 1.f));
	checkpoint_vec.push_back(new CheckpointGameObject(glm::vec3(0.f, 250.f, 0.f), 1.f));

	// make the non current checkpoints a different shape and colour
	for (int i = 1; i < checkpoint_vec.size(); i++) {
		checkpoint_vec[i]->setMesh(ofMesh::cylinder(15, 30, 20, 10, true, 2));
		checkpoint_vec[i]->setColour(glm::vec3(150.f, 150.f, 225.f));
	}

	// populate the game with opposition objects, 1 per checkpoint beacon
	opposition_vec.push_back(new EnemyGameObject(glm::vec3(0.f, 0.f, -400.f), 1.f));
	opposition_vec.push_back(new EnemyGameObject(glm::vec3(200.f, -150.f, -700.f), 1.f));
	opposition_vec.push_back(new EnemyGameObject(glm::vec3(-400.f, 500.f, 300.f), 1.f));
	opposition_vec.push_back(new EnemyGameObject(glm::vec3(-200.f, -500.f, -250.f), 1.f));
	opposition_vec.push_back(new EnemyGameObject(glm::vec3(200.f, 300.f, 350.f), 1.f));
	opposition_vec.push_back(new EnemyGameObject(glm::vec3(-700.f, -400.f, 50.f), 1.f));
	opposition_vec.push_back(new EnemyGameObject(glm::vec3(100.f, 700.f, 250.f), 1.f));
	opposition_vec.push_back(new EnemyGameObject(glm::vec3(0.f, 350.f, 0.f), 1.f)); // offset a lil, gets to spawn too fast

	// add power ups close to the beacons
	power_up_vec.push_back(new PowerUpObject(glm::vec3(0.f, 25.f, -475.f), 1.f));
	power_up_vec.push_back(new PowerUpObject(glm::vec3(200.f, -225.f, -675.f), 1.f));
	power_up_vec.push_back(new PowerUpObject(glm::vec3(-425.f, 425.f, 300.f), 1.f));
	power_up_vec.push_back(new PowerUpObject(glm::vec3(-225.f, -550.f, -275.f), 1.f));
	power_up_vec.push_back(new PowerUpObject(glm::vec3(175.f, 375.f, 350.f), 1.f));
	power_up_vec.push_back(new PowerUpObject(glm::vec3(-675.f, -350.f, 50.f), 1.f));
	power_up_vec.push_back(new PowerUpObject(glm::vec3(100.f, 625.f, 275.f), 1.f));
	power_up_vec.push_back(new PowerUpObject(glm::vec3(-25.f, 300.f, 75.f), 1.f));

	// add 5 scattered power ups
	power_up_vec.push_back(new PowerUpObject(glm::vec3(0.f, -25.f, -200.f), 1.f)); // nearby so the player can become faster than opps
	power_up_vec.push_back(new PowerUpObject(glm::vec3(600.f, -450.f, 500.f), 1.f));
	power_up_vec.push_back(new PowerUpObject(glm::vec3(-400.f, 750.f, 500.f), 1.f));
	power_up_vec.push_back(new PowerUpObject(glm::vec3(-650.f, 350.f, -550.f), 1.f));
	power_up_vec.push_back(new PowerUpObject(glm::vec3(450.f, 500.f, -475.f), 1.f));

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
	time_elapsed += delta_time;

	// handle game over state
	if (lostGameOver) {
		if (gameOverTimer.Finished()) {
			ofExit();
		}
	}

	// handle game won state
	else if (wonGameOver) {
		if (gameOverTimer.Finished()) {
			ofExit();
		}
	}

	// handle the standard gameplay loop
	else {

		/*** CHECKPOINT HANDLING ***/

		// see if they collected the checkpoint make it dissapear and change the next checkpoint to the correct shape and colour
		if (checkpoint_vec.size() > 0) {
			float dist = glm::distance(player->getPosition(), checkpoint_vec[0]->getPosition());
			if (dist <= player->getRadius() + checkpoint_vec[0]->getRadius()) {
				checkpoint_vec.erase(checkpoint_vec.begin() + 0);
				if (checkpoint_vec.size() > 0) {
					checkpoint_vec[0]->setMesh(ofMesh::sphere(15));
					checkpoint_vec[0]->setColour(glm::vec3(100.f, 255.f, 80.f));
				}
			}
		}

		// this case should only be entered once all checkpoints have been hit, switch to win-game state
		else {
			if (!wonGameOver) {
				wonGameOver = true;
				gameOverTimer.Start(5.0f);
			}
		}

		/*** PLAYER HANDLING ***/

		// check if player should be able to be hit
		if (player->getInvincibilityTimer().FinishedAndStop()) {
			player->setColour(glm::vec3(255.0f));
		}

		player->update(delta_time);

		/*** OPPOSITION (AKA ENEMY) HANDLING ***/

		// updates for opps
		for (int i = 0; i < opposition_vec.size(); ++i) {
			EnemyGameObject* enemy = opposition_vec[i];
			enemy->faceTowards(player->getPosition());
			enemy->update(delta_time);

			// check for collisions between an enemy and the player
			float dist = glm::distance(player->getPosition(), enemy->getPosition());
			if (dist <= player->getRadius() + enemy->getRadius() && !player->getInvincibilityTimer().IsRunning()) {
				
				// insta-kill, as per assignment spec
				player->setHealth(0);
				player->getInvincibilityTimer().Start(2.0f); // start timer where the player cannot be hit
				player->setColour(glm::vec3(255.0f, 0.0f, 50.0f)); // set colour to show that player has been hit
				lostGameOver = true;
				gameOverTimer.Start(5.0f);

				// clean up enemy
				delete enemy;
				opposition_vec.erase(opposition_vec.begin() + i);
			}
		}

		/*** ASTEROID HANDLING ***/

		// have the astroids damage the player when they collide with it (has same logic when collide with enemy)
		for (int i = 0; i < astroid_vec.size(); i++) {
			float dist = glm::distance(player->getPosition(), astroid_vec[i]->getPosition());
			if (dist <= player->getRadius() + (astroid_vec[i]->getLength()/1.5) && !player->getInvincibilityTimer().IsRunning()) {
				player->setHealth(player->getHealth() - 1);
				player->getInvincibilityTimer().Start(2.0f); // start timer where the player cannot be hit
				player->setColour(glm::vec3(255.0f, 0.0f, 50.0f)); // set colour to show that player has been hit
				
				if (player->getHealth() <= 0) {
					lostGameOver = true;
					gameOverTimer.Start(5.0f);
					break;
				}
			}
		}

		/*** POWER-UP HANDLING ***/

		// updates for power ups
		for (int i = 0; i < power_up_vec.size(); ++i) {
			PowerUpObject* power_up = power_up_vec[i];

			// check for collision between player and power up
			float dist = glm::distance(player->getPosition(), power_up->getPosition());
			if (dist <= player->getRadius() + power_up->getRadius()) {
				
				// handle collision
				delete power_up;
				power_up_vec.erase(power_up_vec.begin() + i);
				player->powerUpSpeedIncrease();
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	// draw world coordinate objects
	player->getCamera().begin();
	ofEnableDepthTest();

	for (int i = 0; i < astroid_vec.size(); ++i) {
		astroid_vec[i]->draw();
	}
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

	player->getCamera().end();
	ofDisableDepthTest();

	// reset colour, draw HUD elements
	ofSetColor(255, 255, 255);
	ofDrawBitmapString("Time Elapsed: " + ofToString(time_elapsed, 2), ofGetWidth() - 160, 30);
	ofDrawBitmapString("Player Health: " + ofToString(player->getHealth()), ofGetWidth() - 130, 70);
	ofDrawBitmapString("Player Scale: " + ofToString(player->getScale(), 2), ofGetWidth() - 145, 50);

	// game over text
	if (lostGameOver) {
		ofClear(255, 10, 10);
		ofDrawBitmapString("GAME OVER, OPPS CAUGHT U LACKIN TWIN!!!",
			ofGetWidth() / 2 - 160, ofGetHeight() / 2);
	}
	if (wonGameOver) {
		ofClear(10, 255, 10);
		ofDrawBitmapString("YOU WIN!!! U DID NOT GET CAUGHT LACKING!!!",
			ofGetWidth() / 2 - 160, ofGetHeight() / 2);
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

	for (int i = 0; i < opposition_vec.size(); ++i) {
		delete opposition_vec[i];
	}

	for (int i = 0; i < checkpoint_vec.size(); ++i) {
		delete checkpoint_vec[i];
	}

	for (int i = 0; i < astroid_vec.size(); ++i) {
		delete astroid_vec[i];
	}

	for (int i = 0; i < power_up_vec.size(); ++i) {
		delete power_up_vec[i];
	}
}
