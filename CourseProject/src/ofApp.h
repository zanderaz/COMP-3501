#pragma once

#include "ofMain.h"
#include <GLFW/glfw3.h>

#include "MyCustomCamera.h"
#include "GameObject.h"
#include "PlayerGameObject.h"
#include "CheckpointGameObject.h"
#include "EnemyGameObject.h"
#include "PowerUpObject.h"
#include "timer.h"


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

	static GLFWwindow* getGLFW(void);
	void setRawMouseCapture(bool on);
	void recenterCursorToWindowCenter(void);

private:

	MyCustomCamera cam;

	// mouse-look related
	bool mouse_capture_flag;
	float mouse_sensitivity; // radians per pixel
	double last_x;
	double last_y;
	bool is_first_mouse;

	// game object collections
	PlayerGameObject* player;
	vector<EnemyGameObject*> opposition_vec;
	vector<CheckpointGameObject*> checkpoint_vec;
	vector<PowerUpObject*> power_up_vec;
  
	// track time
	float time_elapsed;

	// sound
	ofSoundPlayer background_music;

	// meshes
	ofSpherePrimitive power_up_mesh;
	ofSpherePrimitive player_mesh;
	ofPlanePrimitive alignment_check;

};
