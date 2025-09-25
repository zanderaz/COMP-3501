#pragma once
#ifndef PLAYER_GAME_OBJECT_H_
#define PLAYER_GAME_OBJECT_H_
#include <glm/glm.hpp>
#include "ofMain.h"
#include "GameObject.h"
#include "MyCustomCamera.h"
class PlayerGameObject : public GameObject
{
public:
	PlayerGameObject(const glm::vec3& position, float scale, float movementSpeed, float rotationSpeed, MyCustomCamera cam);
	inline const glm::vec3& getVelocity() const { return velocity; }
	inline void setVelocity(glm::vec3& vel) { velocity = vel; }
	inline MyCustomCamera& getCamera() { return cam; }
	void draw() override;
	void update(float deltaTime) override;
	void roll(float amt);
	void yaw(float amt);
	void pitch(float amt);	

protected:
	float movementSpeed;
	float rotationSpeed;
	glm::vec3 velocity;
	MyCustomCamera cam;
};
#endif


