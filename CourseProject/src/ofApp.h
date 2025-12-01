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
#include "redBloodCell.h"
#include "redBloodCellParticleSystem.h"
#include "screenSpaceEffect.h"
#include "TextBox.h"

class ofApp : public ofBaseApp {

public:

	// main game logic functions
	void setup();
	void setupGameplayGameState(void);
	void exit(void) override;
	void update();
	void draw();

	// built in OF functions 
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

	// helper functions
	static GLFWwindow* getGLFW(void);
	void setRawMouseCapture(bool on);
	void recenterCursorToWindowCenter(void);
	void handleCheckpointCollision(CheckpointGameObject* checkpoint);

private:

	// game object collections
	PlayerGameObject* player;
	vector<EnemyGameObject*> opposition_vec;
	vector<CheckpointGameObject*> checkpoint_vec;
	vector<PowerUpObject*> power_up_vec;
	vector<GameObject*> wall_objects_vec;

	// mouse-look and camera related
	MyCustomCamera cam;
	bool mouse_capture_flag;
	float mouse_sensitivity; // radians per pixel
	double last_x;
	double last_y;
	bool is_first_mouse;
  
	// track time
	float time_elapsed;

	// sound
	ofSoundPlayer background_music;

	// meshes
	ofSpherePrimitive power_up_mesh;
	ofSpherePrimitive player_mesh;
	ofPlanePrimitive alignment_check;

	// shader
	ofShader* lightingShader;
	ofShader* skyBoxShader;

	// main-menu fonts and geometry
	ofTrueTypeFont menu_button_font;
	ofTrueTypeFont menu_title_font;
	ofTrueTypeFont menu_caption_font;
	ofTrueTypeFont dialog_font;
	ofImage menu_background;
	ofRectangle start_button_rect;
	ofRectangle quit_button_rect;

	// game state indicators
	unsigned short int game_state; // 0 = main menu, 1 = gameplay, 2 = game over, 3 = game won 
	bool bloodstream, boneMarrow;
	bool is_muted;

	// textures
	ofImage texture, skyTexture, wallTexture;

	// test (objects, orbit for light source, boolean to toggle texture)
	ofIcoSpherePrimitive sphere, lightSphere, skySphere;
	float orbitSpeed, orbitRadius, orbitAngle;
	bool bUseTexture = true;
	RedBloodCellParticleSystem* rbc;
	RedBloodCell* redBloodCell;
	ScreenSpaceEffect screenSpaceEffect;

	TextBox textBox;
	bool showTextBox;

	void createWalls();
	void createWallsSection1();
	void createWallsSection2();
	void createWallsSection3();

	// ----------------- Constants -----------------
	const string TITLE_TEXT = "Triple Sicks";
	const string CAPTION_TEXT = "COMP3501 Course Project by Aidan, Evan, Zander";
	const string START_TEXT = "Start Game";
	const string QUIT_TEXT = "Quit Game";

	const float BG_MUSIC_VOL = 0.2f;

};
