#pragma once

#include "ofMain.h"
#include <GLFW/glfw3.h>

#include "MyCustomCamera.h"
#include "GameObject.h"
#include "PlayerGameObject.h"
#include "CheckpointGameObject.h"
#include "EnemyGameObject.h"
#include "timer.h"
#include "redBloodCell.h"
#include "ParticleSystem.h"
#include "screenSpaceEffect.h"
#include "TextBox.h"
#include "EnemySpawner.h"
#include "BoneSpikeGameObject.h"
#include "BoneSpikeSpawner.h"
#include "InteractableObject.h"
#include "BloodStreamCylinder.h"
#include "BloodStreamCylinderCollection.h"
#include "LSystem.h"

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
	void setupLSystems();
	static GLFWwindow* getGLFW(void);
	void setRawMouseCapture(bool on);
	void recenterCursorToWindowCenter(void);
	void handleCheckpointCollision(CheckpointGameObject* checkpoint);
	void spawnInfectedPS(const glm::vec3& position);
	void createSpawnPortal(const glm::vec3& pos, const glm::quat& orientation);
	void spawnEnemiesAfterInfect(InteractableObject* interact_obj);
	RedBloodCell* createRedBloodCellEnemy(const glm::vec3& position, const glm::vec3& velocity);
	glm::vec3 getPortalSpawnDirection(const glm::quat& portalOrientation) const;
	void triggerSpawnPortalBurst(ParticleSystem* portalPs);
	void updateSpawnPortalBursts(float deltaTime);
	ofMesh createBoneMesh(float radius, float height);

	// collidable object creation
	void createWalls();

	// blood stream helper functions
	void createWallsSection1();
	void createWallsSection2();
	void createWallsSection3();
	void createWallsSection4();
	void createVeins();
	void createBloodStreamCylinders();
	void createBloodStreamLookout();
	void createBloodSpawnPortals();
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
	void createBoneMarrowScenery();

private:

	// enemy spawning from portal helper
	struct PortalSpawnBurst {
		ParticleSystem* portalPs;
		std::vector<RedBloodCell*> enemies;
		Timer despawnTimer;
		Timer burstTimer;
		int burstsSpawned;
	};
	ofMesh rbc_mesh;
	void spawnPortalBurstEnemies(PortalSpawnBurst& burst); // gotta define this afterwards

	// game object collections
	PlayerGameObject* player;
	vector<CheckpointGameObject*> checkpoint_vec;
	vector<GameObject*> wall_objects_vec;
	vector<InteractableObject*> interactables_vec;
	vector<ParticleSystem*> infection_ps_vec;
	vector<ParticleSystem*> spawn_portal_ps_vec;
	vector<PortalSpawnBurst> portal_spawn_bursts;
	vector<BloodStreamCylinderCollection*> cylinder_collections_vec;
	vector<LSystem*> lsys;
	vector<GameObject*> bone_marrow_scenery;

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
	ofMesh taller_bone;
	ofMesh tall_bone;
	ofMesh large_bone;
	ofMesh small_bone;

	// shader
	ofShader* lightingShader;
	ofShader* skyBoxShader;

	// main-menu fonts, geometry, texture
	ofTrueTypeFont menu_button_font;
	ofTrueTypeFont menu_title_font;
	ofTrueTypeFont menu_caption_font;
	ofTrueTypeFont game_over_font;
	ofTrueTypeFont hud_font;

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
	ofImage l_sys_tex;
	ofImage bone_tex;

	// priority renders: light source, skybox, use texture flag
	glm::vec3 light_pos;
	float light_orbit_angle;
	ofIcoSpherePrimitive lightSphere, skySphere;
	bool bUseTexture = true;

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

	bool gameStarted;

	// ----------------- Constants -----------------
	const unsigned short int GAME_FPS = 144;

	const string TITLE_TEXT = "Triple Sicks";
	const string CAPTION_TEXT = "COMP3501 Course Project by Aidan, Evan, Zander";
	const string START_TEXT = "Start Game";
	const string QUIT_TEXT = "Quit Game";
	const string INTERACT_TEXT = "Press ' f ' to Infect.";
	const string GAME_OVER_TEXT = "GAME OVER!";
	const string GAME_WON_TEXT = "YOU WIN! Congrats!";

	// interacting thru a wall is an intended speedrun strat (we are not lazy)
	const float INTERACT_RANGE = 150.0f;

	const float LIGHT_HEIGHT = 1500.0f;
	const float LIGHT_ORBIT_RADIUS = 4000.0f;
	const float LIGHT_ORBIT_SPEED = 0.25f;
	const glm::vec3 BLOODSTREAM_GROUND_CENTER = glm::vec3(-1850.0f, -50.0f, -100.0f);
	const glm::vec3 BONE_MARROW_GROUND_CENTER = glm::vec3(13000.0f, -50.0f, -500.0f);

	const float MUSIC_VOL = 0.2f;
	const float SFX_VOL = 0.4f;

	const float PORTAL_ENEMY_LIFETIME = 6.0f;
	const float PORTAL_ENEMY_SPEED = 250.0f;
	const float PORTAL_ENEMY_YAW_SPREAD = glm::radians(20.0f);
	const float PORTAL_ENEMY_PITCH_SPREAD = glm::radians(40.0f);
	const int PORTAL_ENEMY_COUNT = 1;
	const int PORTAL_BURST_COUNT = 16;
	const float PORTAL_BURST_INTERVAL = 0.25f;

	const float LSYS_CULL_DIST = 3500.0f;
	const float LSYS_CULL_DIST_SQ = LSYS_CULL_DIST * LSYS_CULL_DIST;

	const float BONE_SPIKE_MINIGAME_DURATION = 45.0f;

	const bool CREATE_CEILING = true;
	// removing the ceiling helps with placing geometry into the game world
	// however, this should always be true for real builds of the game, i.e. debug only 

};
