#ifndef ASTROID_GAME_OBJECT_H_
#define ASTROID_GAME_OBJECT_H_
#include "GameObject.h"
#include <glm/glm.hpp>


class AstroidGameObject : public GameObject {

public:
	AstroidGameObject(const glm::vec3& Position, float scale);

	void draw() override;


	
	inline const float getLength() const { return length; }

	inline void setLength(float len) { length = len; }

private:
	float length;
	int checkNum;

};
#endif