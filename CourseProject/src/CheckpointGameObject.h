#ifndef CHECKPOINT_GAME_OBJECT_H_
#define CHECKPOINT_GAME_OBJECT_H_

#include "GameObject.h"
#include <glm/glm.hpp>

class CheckpointGameObject : public GameObject {

	public:
		CheckpointGameObject(const ofMesh& mesh, const glm::vec3& Position, float scale, const glm::vec3& teleportPosition);
		void draw(ofShader* lightingShader) override;
		inline const float getRadius() const { return radius; }
		inline void setRadius(float rad) { radius = rad; }
		inline const glm::vec3& getTeleportPosition() const { return teleportPosition; }
		inline void setTeleportPosition(const glm::vec3& teleportPosition) { this->teleportPosition = teleportPosition; }
		
	private:
		float radius;
		glm::vec3 teleportPosition;

};

#endif