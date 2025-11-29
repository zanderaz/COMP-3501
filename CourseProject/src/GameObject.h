#pragma once
#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include <glm/glm.hpp>
#include "ofMain.h"

class GameObject {

public:

	GameObject(const ofMesh& mesh, const glm::vec3& position, float scale);
	~GameObject() = default;
	
	// getters
	inline const glm::vec3& getPosition() const { return position; }
	inline const float getScale() const { return scale; }
	inline const glm::quat& getOrientation() const { return orientation; }
	inline const ofMesh& getMesh() const { return mesh; }
	inline const glm::vec3& getColour() const { return colour; }

	// setters
	inline void setPosition(const glm::vec3& pos) { position = pos; }
	inline void setScale(float s) { scale = s; }
	inline void setOrientation(const glm::quat& q) { orientation = q; }
	inline void setMesh(const ofMesh& m) { mesh = m; }
	inline void setColour(const glm::vec3& c) { colour = c; }
	
	// render and logic specific
	virtual void draw(ofShader* lightingShader);
	virtual void update(float deltaTime) {}
	glm::vec3 getqForward() { return orientation * ((-BASE_FORWARD)); }
	glm::vec3 getqSide() { return orientation * BASE_SIDE; }
	glm::vec3 getqUp() { return orientation * BASE_UP; }
	glm::mat4 getWorldMatrix();

protected:

	float scale;
	glm::vec3 position;
	glm::vec3 colour;
	glm::quat orientation;
	ofMesh mesh;
	glm::vec3 BASE_SIDE = glm::vec3(1, 0, 0);
	glm::vec3 BASE_UP = glm::vec3(0, 1, 0);
	glm::vec3 BASE_FORWARD = glm::vec3(0, 0, 1);

	
	ofImage texture;

};

#endif


