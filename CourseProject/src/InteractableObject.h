#pragma once

#include "GameObject.h"

class InteractableObject : public GameObject {

public:

	InteractableObject(const ofMesh& mesh, const glm::vec3& position, float scale, int sp_index);
	void draw(ofShader* lightingShader) override;

	inline void setSpawnPortalIndex(int index) { spawn_portal_index = index; }
	inline const int getSpawnPortalIndex(void) { return spawn_portal_index; }

private:

	int spawn_portal_index;

};

