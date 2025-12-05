#pragma once
#ifndef RED_BLOOD_CELL_H
#define RED_BLOOD_CELL_H

#include <ofMain.h>
#include <glm/glm.hpp>
#include "ParticleSystem.h"
#include "GameObject.h"
#include "EnemyGameObject.h"


class RedBloodCell : public EnemyGameObject {
public:
	RedBloodCell(ParticleSystem* rbc, const ofMesh& mesh, const glm::vec3& position, float scale);
	~RedBloodCell();
	void update(float deltaTime) override;
	void draw(ofShader* lightingShader) override;
	void drawParticles();
private:
	ParticleSystem* ps;
};


#endif