#pragma once
#ifndef PLAYER_GAME_OBJECT_H_
#define PLAYER_GAME_OBJECT_H_
#include <glm/glm.hpp>
#include "ofMain.h"
#include "GameObject.h"
#include "MyCustomCamera.h"
#include "timer.h"
class PlayerGameObject : public GameObject
{
public:
	PlayerGameObject(const glm::vec3& position, float scale, float acceleration, float rotationSpeed, MyCustomCamera cam);

	// getters
	inline const glm::vec3& getVelocity() const { return velocity; }
	inline const float getRadius() const { return radius; }
	inline const int getHealth() const { return health; }
	inline MyCustomCamera& getCamera() { return cam; }
	inline Timer& getInvincibilityTimer() { return invincibilityTimer; }

	// setters
	inline void setRadius(float r) { radius = r; }
	inline void setVelocity(const glm::vec3& vel) { velocity = vel; }
	inline void setHealth(int h) { health = h; }
	
	void draw() override;
	void update(float deltaTime) override;

	// rotation
	void roll(float amt);
	void yaw(float amt);
	void pitch(float amt);	

	// shrink/grow
	void changeSize(float factor);



protected:
	float acceleration;
	float rotationSpeed;
	float speed;
	float maxSpeed;
	float radius;
	float initRadius;
	float maxRadius;
	float minRadius;
	int health;
	Timer sizeChangeTimer;
	Timer invincibilityTimer;
	glm::vec3 velocity; // unused rn
	MyCustomCamera cam;
};
#endif


