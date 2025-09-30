#pragma once

#include "ofMain.h"
#include "MyCustomCamera.h"
#include "GameObject.h"
#include "PlayerGameObject.h"
#include "EnemyGameObject.h"
#include "timer.h"
#include "CheckpointGameObject.h"

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void exit(void) override;
		
		int asteroids;
		
		ofNode body[500];

		MyCustomCamera cam;

		// game object collections
		PlayerGameObject* player;
		vector<EnemyGameObject*> opposition_vec;
		vector<CheckpointGameObject*> checkpoint_vec;
		vector<CheckpointGameObject*> precheckpoint;
  
		// track time
		float time_elapsed;

		// sound
		ofSoundPlayer background_music;

		// game over stuffs
		bool lostGameOver;
		bool wonGameOver;
		Timer gameOverTimer;
};
