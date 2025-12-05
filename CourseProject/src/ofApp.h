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
#include "ParticleSystem.h"
#include "screenSpaceEffect.h"
#include "TextBox.h"
#include "EnemySpawner.h"
#include "BoneSpikeGameObject.h"
#include "BoneSpikeSpawner.h"

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
	void setupSFX(void);
	void setupShaders();
	void setupTextures();
	void setupTextElements();
	void setupInteractableObjects();
	void setupDynamicWalls();
	static GLFWwindow* getGLFW(void);
	void setRawMouseCapture(bool on);
	void recenterCursorToWindowCenter(void);
	void handleCheckpointCollision(CheckpointGameObject* checkpoint);
	void spawnInfectedPS(const glm::vec3& position);


	// collidable object creation
	void createWalls();

	// blood stream helper functions
	void createWallsSection1();
	void createWallsSection2();
	void createWallsSection3();
	void createWallsSection4();
	void createVeins();
	void createBloodStreamLookout();
	void startBloodBulletHell(float duration = 30.0f);
	void endBloodBulletHell();
	void updateBulletHellWall();
	void bulletHellComplete();

	// bone marrow helper functions
	void updateBoneMarrowBlockingWalls();
	void createWallsSection5();
	void createWallsSection6();
	void createWallsSection7();
	void createBoneMarrowLookout();


private:

	// game object collections
	PlayerGameObject* player;
	vector<EnemyGameObject*> opposition_vec;
	vector<CheckpointGameObject*> checkpoint_vec;
	vector<PowerUpObject*> power_up_vec;
	vector<GameObject*> wall_objects_vec;
	vector<GameObject*> interactables_vec;
	vector<ParticleSystem*> infection_ps_vec;
	vector<ParticleSystem*> spawn_portal_ps_vec;

	// mouse-look and camera related
	MyCustomCamera cam;
	bool mouse_capture_flag;
	float mouse_sensitivity; // radians per pixel
	double last_x;
	double last_y;
	bool is_first_mouse;
  
	// track time
	float time_elapsed;

	// music
	ofSoundPlayer menu_music;
	ofSoundPlayer gameplay_music;
	// sfx
	ofSoundPlayer infect_sound;
	ofSoundPlayer speed_boost_sound;
	ofSoundPlayer room_event_start;
	ofSoundPlayer room_event_end;
	ofSoundPlayer checkpoint_teleport;

	// re-used meshes
	ofSpherePrimitive power_up_mesh;
	ofSpherePrimitive empty_mesh;
	ofSpherePrimitive bulletHellEnemyMesh;

	// shader
	ofShader* lightingShader;
	ofShader* skyBoxShader;

	// main-menu fonts, geometry, texture
	ofTrueTypeFont menu_button_font;
	ofTrueTypeFont menu_title_font;
	ofTrueTypeFont menu_caption_font;
	ofTrueTypeFont game_over_font;

	ofRectangle start_button_rect;
	ofRectangle quit_button_rect;

	ofImage menu_texture;
	ofShader menu_shader;

	// game state indicators
	unsigned short int game_state; // 0 = main menu, 1 = gameplay, 2 = game over, 3 = game won 
	bool is_muted;
	unsigned short int veins_infected_count, marrow_infected_count;

	// textures
	ofImage texture, skyTexture, bloodstreamWallTexture, boneMarrowWallTexture, redBloodCellTexture;

	// priority renders: light source, skybox, use texture flag
	glm::vec3 light_pos;
	ofIcoSpherePrimitive lightSphere, skySphere;
	bool bUseTexture = true;

	// particle systems
	ParticleSystem* rbc;
	RedBloodCell* redBloodCell;

	// SSE related
	ScreenSpaceEffect screenSpaceEffect;
	bool bloodstream, boneMarrow;

	// in game text and icons
	ofTrueTypeFont dialog_font;
	TextBox textBox;
	bool showTextBox;
	bool show_interact_tip;

	// bullet hell stuff
	EnemySpawner enemySpawner;
	bool bloodBulletHellActive;
	bool isBulletHellComplete;
	Timer bloodBulletHellTimer;
	GameObject* bulletHellWall;
	CheckpointGameObject* bulletHellCheckpoint;

	// bone marrow
	GameObject* boneMarrowBlockingWall1;
	GameObject* boneMarrowBlockingWall2;
	BoneSpikeSpawner boneSpikeSpawner;
	bool boneSpikeMinigameActive;
	Timer boneSpikeMinigameTimer;
	bool isBoneMarrowComplete;

	// game over timer
	Timer gameOverTimer;

	// ----------------- Constants -----------------
	const unsigned short int GAME_FPS = 144;

	const string TITLE_TEXT = "Triple Sicks";
	const string CAPTION_TEXT = "COMP3501 Course Project by Aidan, Evan, Zander";
	const string START_TEXT = "Start Game";
	const string QUIT_TEXT = "Quit Game";
	const string INTERACT_TEXT = "Press ' f ' to Infect.";
	const string GAME_OVER_TEXT = "GAME OVER!";
	const string GAME_WON_TEXT = "YOU WIN! Congrats!";

	const float INTERACT_RANGE = 100.0f;
	const float LIGHT_HEIGHT = 620.0f;

	const float MUSIC_VOL = 0.2f;
	const float SFX_VOL = 0.4f;

	const float BONE_SPIKE_MINIGAME_DURATION = 45.0f;

};
