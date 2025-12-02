#pragma once
#ifndef RED_BLOOD_CELL_H
#define RED_BLOOD_CELL_H

#include <ofMain.h>
#include <glm/glm.hpp>
#include "redBloodCellParticleSystem.h"
#include "GameObject.h"


class ParticleSystemHolder : public GameObject {
public:
	ParticleSystemHolder(ParticleSystem* rbc, const ofMesh& mesh, const glm::vec3& position, float scale);
	void update(float deltaTime) override;
	void draw(ofShader* lightingShader) override;
private:
	ParticleSystem* ps;
};


#endif