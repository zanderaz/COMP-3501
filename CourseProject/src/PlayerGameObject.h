#pragma once
#ifndef PLAYER_GAME_OBJECT_H_
#define PLAYER_GAME_OBJECT_H_
#include <glm/glm.hpp>
#include "ofMain.h"
#include "GameObject.h"
#include "MyCustomCamera.h"
#include "timer.h"

class PlayerGameObject : public GameObject {

public:

	PlayerGameObject(const ofMesh& mesh, const glm::vec3& position, float scale, MyCustomCamera cam);

	// getters
	inline const float getRadius() const { return radius; }
	inline const glm::vec3& getVelocity() const { return velocity; }
	inline const int getHealth() const { return health; }
	inline MyCustomCamera& getCamera() { return cam; }
	inline Timer& getInvincibilityTimer() { return invincibilityTimer; }

	// setters
	inline void setRadius(float r) { radius = r; }
	inline void setVelocity(const glm::vec3& vel) { velocity = vel; }
	inline void setHealth(int h) { health = h; }
	
	// render and logic
	void draw(ofShader* lightingShader) override;
	void update(float delta_time) override;

	// rotation and movement
	void roll(float amt);
	void yaw(float amt);
	void pitch(float amt);
	void enforceUpright(void);

protected:

	// motion params
	glm::vec3 velocity;
	float h_accel;
	float h_drag;
	float h_max_speed;
	float v_accel;
	float v_drag;
	float v_max_speed;
	float gravity;

	// other
	float radius;
	int health;
	Timer invincibilityTimer;
	MyCustomCamera cam;

};
#endif


