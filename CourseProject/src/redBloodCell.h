#pragma once
#ifndef RED_BLOOD_CELL_H
#define RED_BLOOD_CELL_H

#include <ofMain.h>
#include <glm/glm.hpp>
#include "redBloodCellParticleSystem.h"
#include "GameObject.h"


class RedBloodCell : public GameObject {
public:
	RedBloodCell(RedBloodCellParticleSystem* rbc, const ofMesh& mesh, const glm::vec3& position, float scale);
	void update(float deltaTime) override;
	void draw(ofShader* lightingShader) override;
private:
	//ofMesh mesh;
	//glm::vec3 position;
	//glm::vec3 colour;
	RedBloodCellParticleSystem* rbc;
};


#endif