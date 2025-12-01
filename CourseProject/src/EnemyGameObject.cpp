#include "EnemyGameObject.h"

// constructor, does the same shit as the base class, however might need a different mesh down the road so yeah
EnemyGameObject::EnemyGameObject(const ofMesh& mesh, const glm::vec3& position, float scale)
	: GameObject(mesh, position, scale) {
	radius = 15;
	colour = glm::vec3(1.0f, 0.5f, 0.5f);
	enemy_speed = ENEMY_BASE_SPEED + ofRandom(0, 25.f); // variable speed
}


// specialized update function that moves the enemy along its direction vec
void EnemyGameObject::update(float dt) {
	
	// get forward direction, should point towards the player, then move
	//glm::vec3 forward = orientation * glm::vec3(0.0f, 0.0f, -1.0f);
	//position += forward * enemy_speed * dt;
	glm::vec3 newPos = getPosition() + velocity * dt;
	setPosition(newPos);

	// Optional: Face movement direction
	if (glm::length(velocity) > 0.1f) {
		glm::quat rotation = glm::quatLookAt(glm::normalize(velocity), glm::vec3(0, 1, 0));
		setOrientation(rotation);
	}
	
}

void EnemyGameObject::faceTowards(const glm::vec3& point_to_face) {

	// set orientation to face towards player
	glm::vec3 dir = glm::normalize(point_to_face - position);

	// if parallel, we just use identity quaternion
	if (glm::length(dir) < 0.0001f) {
		orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	}

	// else, update orientation based on standard forward dir 
	else {
		glm::vec3 forward = glm::vec3(0.0f, 0.0f, -1.0f);
		orientation = glm::rotation(forward, dir);
	}
}

// draw with appropriate scale, position, colour, and mesh
void EnemyGameObject::draw(ofShader* lightingShader) {
	GameObject::draw(lightingShader);
}

