#include "LSystem.h"

LSystem::LSystem(const glm::vec3& pos) : GameObject(ofMesh(), pos, 1.0f) {
    baseAngle = 35.0;
    baseLength = 27.5;

    this->colour = glm::vec3(0.1f, 0.2f, 0.6f);

    lsysString = "X";

    // generate L-System String
    for (int i = 0; i < INTERATION_COUNT; i++) {
        string nextString = "";
        for (char c : lsysString) {
            if (c == 'X') nextString += "F[+X][-X][&X][^X]FX";
            else if (c == 'F') nextString += "FF";
            else nextString += c;
        }
        lsysString = nextString;
    }

    // precompute random offsets
    for (char c : lsysString) {
        if (c == '+' || c == '-' || c == '&' || c == '^' || c == '\\' || c == '/')
            randomOffsets.push_back(ofRandom(-5.0f, 5.0f));
        else
            randomOffsets.push_back(0.0f);
    }

    generateAndSetMesh();
}

void LSystem::generateAndSetMesh() {
    ofMesh tempMesh;
    tempMesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);

    struct TurtleState {
        glm::mat4 transform;
        float len;
        float rad;
        int depth;
    };

    vector<TurtleState> stack;
    glm::mat4 currentTransform = glm::mat4(1.0f);

    float segLength = baseLength;
    float baseRadius = 8.0f;
    float taper = 0.5f;
    int stackDepth = 0;
    int index = 0;

    ofCylinderPrimitive cylPrimitive;
    cylPrimitive.setResolution(5, 1, 1);

    for (char c : lsysString) {
        float offset = (index < randomOffsets.size()) ? randomOffsets[index] : 0.0f;
        index++;

        switch (c) {
        case 'F': {
            float radius = baseRadius * pow(taper, stackDepth);
            cylPrimitive.set(radius, segLength);

            // Get mesh with caps now
            ofMesh segmentMesh = cylPrimitive.getMesh();

            glm::mat4 modelMatrix = currentTransform;
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0, segLength * 0.5f, 0));

            for (int i = 0; i < segmentMesh.getNumVertices(); i++) {
                glm::vec3 v = segmentMesh.getVertex(i);
                glm::vec4 v4 = modelMatrix * glm::vec4(v, 1.0f);
                segmentMesh.setVertex(i, glm::vec3(v4));
            }

            tempMesh.append(segmentMesh);
            currentTransform = glm::translate(currentTransform, glm::vec3(0, segLength, 0));
            break;
        }
        case '+': currentTransform = glm::rotate(currentTransform, glm::radians(baseAngle + offset), glm::vec3(0, 0, 1)); break;
        case '-': currentTransform = glm::rotate(currentTransform, glm::radians(-baseAngle + offset), glm::vec3(0, 0, 1)); break;
        case '&': currentTransform = glm::rotate(currentTransform, glm::radians(baseAngle + offset), glm::vec3(1, 0, 0)); break;
        case '^': currentTransform = glm::rotate(currentTransform, glm::radians(-baseAngle + offset), glm::vec3(1, 0, 0)); break;
        case '\\': currentTransform = glm::rotate(currentTransform, glm::radians(baseAngle + offset), glm::vec3(0, 1, 0)); break;
        case '/': currentTransform = glm::rotate(currentTransform, glm::radians(-baseAngle + offset), glm::vec3(0, 1, 0)); break;
        case '[':
            stack.push_back({ currentTransform, segLength, baseRadius, stackDepth });
            segLength *= 0.8f; baseRadius *= 0.8f; stackDepth++;
            break;
        case ']':
            if (!stack.empty()) {
                TurtleState s = stack.back(); stack.pop_back();
                currentTransform = s.transform; segLength = s.len; baseRadius = s.rad; stackDepth = s.depth;
            }
            break;
        }
    }

    this->setMesh(tempMesh);
}
