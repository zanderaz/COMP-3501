#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	player = new PlayerGameObject(glm::vec3(0, 0, -10), 1.0f, 100.0f, 1.0f, cam);

	// initalize member vars
	time_elapsed = 0.f;

	// setup asteroids
	asteroids = 200;
	for (int i = 0; i < asteroids; i++) {
		astroid_vec.push_back(new AstroidGameObject(glm::vec3(ofRandom(-800, 800), ofRandom(-800, 800), ofRandom(-800, 800)), 1.f));
	}

	// populate the game with opposition objects (placeholder positions, change to beacons later)
	opposition_vec.push_back(new EnemyGameObject(glm::vec3(-99.f, 0.f, -99.f), 1.f));
	opposition_vec.push_back(new EnemyGameObject(glm::vec3(99.f, 0.f, -99.f), 1.f));
	opposition_vec.push_back(new EnemyGameObject(glm::vec3(99.f, 0.f, 99.f), 1.f));
	opposition_vec.push_back(new EnemyGameObject(glm::vec3(-99.f, 0.f, 99.f), 1.f));


	//creating the checkpoints
	checkpoint_vec.push_back(new CheckpointGameObject(glm::vec3(0.f, 0.f, -400.f), 1.f));
	checkpoint_vec.push_back(new CheckpointGameObject(glm::vec3(200.f, -150.f, -700.f), 1.f));
	checkpoint_vec.push_back(new CheckpointGameObject(glm::vec3(-400.f, 500.f, 300.f), 1.f));
	checkpoint_vec.push_back(new CheckpointGameObject(glm::vec3(-200.f, -500.f, -250.f), 1.f));
	checkpoint_vec.push_back(new CheckpointGameObject(glm::vec3(0.f, 0.f, 0.f), 1.f));
	//make the non current checkpoints a different shape and colour
	for (int i = 1; i < checkpoint_vec.size(); i++) {
		checkpoint_vec[i]->setMesh(ofMesh::cylinder(15, 30, 20, 10, true, 2));
		checkpoint_vec[i]->setColour(glm::vec3(100.f, 100.f, 255.f));
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

	lostGameOver = false; // game is not over!!!!1!111!
	wonGameOver = false;
}


//--------------------------------------------------------------
void ofApp::update() {

	float delta_time = ofGetLastFrameTime();

	// game over
	if (lostGameOver) {
		// cout << "GAME OVER" << endl;
		if (gameOverTimer.Finished()) {
			ofExit();
		}
	}
	if (wonGameOver) {
		// cout << "GAME OVER" << endl;
		if (gameOverTimer.Finished()) {
			ofExit();
		}
	}

	// game not over
	else {
		//see if they collected the checkpoint make it dissapear and change the next checkpoint to the correct shape and colour
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
		//if the user won
		else {
			if (!wonGameOver) {
				wonGameOver = true;
				gameOverTimer.Start(5.0f);
			}
		}


		// check if player should be able to be hit
		if (player->getInvincibilityTimer().FinishedAndStop()) {
			player->setColour(glm::vec3(255.0f));
		}

		player->update(delta_time);

		// updates for opps
		for (int i = 0; i < opposition_vec.size(); ++i) {
			EnemyGameObject* enemy = opposition_vec[i];
			enemy->faceTowards(player->getPosition());
			enemy->update(delta_time);

			// check for collisions between an enemy and the player
			float dist = glm::distance(player->getPosition(), enemy->getPosition());
			if (dist <= player->getRadius() + enemy->getRadius() && !player->getInvincibilityTimer().IsRunning()) {
				// cout << "Collision" << endl;
				player->setHealth(player->getHealth() - 1);

				delete enemy;
				opposition_vec.erase(opposition_vec.begin() + i);

				player->getInvincibilityTimer().Start(2.0f); // start timer where the player cannot be hit
				player->setColour(glm::vec3(255.0f, 0.0f, 50.0f)); // set colour to show that player has been hit
				// cout << "Health: " << player->getHealth() << endl;

				// trigger game over
				if (player->getHealth() <= 0) {
					lostGameOver = true;
					gameOverTimer.Start(5.0f);
					break;
				}
			}

		}
		//have the astroids damage the player when they collide with it (has same logic when collide with enemy)
		for (int i = 0; i < astroid_vec.size(); i++) {
			float dist = glm::distance(player->getPosition(), astroid_vec[i]->getPosition());
			if (dist <= player->getRadius() + (astroid_vec[i]->getLength()/2) && !player->getInvincibilityTimer().IsRunning()) {
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

		time_elapsed += ofGetLastFrameTime();
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	// draw world coordinate objects
	player->getCamera().begin();
	ofEnableDepthTest();

	//draw the astroids
	for (int i = 0; i < astroid_vec.size(); i++) {
		astroid_vec[i]->draw();
	}

	player->draw();

	for (int i = 0; i < opposition_vec.size(); ++i) {
		opposition_vec[i]->draw();
	}

	//draw all checkpoints
	for (int i = 0; i < checkpoint_vec.size(); i++) {
		checkpoint_vec[i]->draw();
	}

	

	player->getCamera().end();
	ofDisableDepthTest();

	// reset colour, draw HUD elements
	ofSetColor(255, 255, 255);
	ofDrawBitmapString("Time Elapsed: " + ofToString(time_elapsed), ofGetWidth() - 160, 30);
	ofDrawBitmapString("Player Health: " + ofToString(player->getHealth()), ofGetWidth() - 130, 70);
	ofDrawBitmapString("Player Scale: " + ofToString(player->getScale(), 2), ofGetWidth() - 145, 50);

	// game over text
	if (lostGameOver) {
		ofDrawBitmapString("GAME OVER, OPPS CAUGHT U LACKIN TWIN!! ", ofGetWidth()/2 - 160, ofGetHeight()/2);
	}
	if (wonGameOver) {
		ofDrawBitmapString("GAME OVER, U DID NOT GET CAUGHT LACKING ", ofGetWidth() / 2 - 160, ofGetHeight() / 2);
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

	// clean up opposition vec
	for (int i = 0; i < opposition_vec.size(); i++) {
		delete opposition_vec[i];
	}
}
