
#ifndef CHECKPOINT_GAME_OBJECT_H_
#define CHECKPOINT_GAME_OBJECT_H_
#include "GameObject.h"
#include <glm/glm.hpp>


class CheckpointGameObject : public GameObject {

	public:
		CheckpointGameObject(const glm::vec3& Position, float radius, int checkNum);



		const float getRad() const { return radius; }
		const int getCheckNum() const { return checkNum; }

	private:
		float radius;
		int checkNum;

};
#endif