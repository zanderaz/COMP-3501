
#ifndef CHECKPOINT_GAME_OBJECT_H_
#define CHECKPOINT_GAME_OBJECT_H_
#include "GameObject.h"
#include <glm/glm.hpp>


class CheckpointGameObject : public GameObject {

	public:
		CheckpointGameObject(const glm::vec3& Position, float scale);

		void draw() override;



		inline const float getRadius() const { return radius; }
		inline const int getCheckNum() const { return checkNum; }

		inline void setRadius(float rad) { radius = rad; }
		inline void setCheckNum(int cNum) { checkNum = cNum; }

	private:
		float radius;
		int checkNum;

};
#endif