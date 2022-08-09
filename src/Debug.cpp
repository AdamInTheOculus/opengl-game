#include "Debug.h"

void Debug::printMatrix(const glm::mat4 &matrix) {
    const float *pSource = (const float*)glm::value_ptr(matrix);
    for (int i = 0; i < 16; i += 4) {
        DEBUG_LOG("[%.2f   %0.2f   %0.2f   %0.2f]", pSource[i], pSource[i + 1], pSource[i + 2], pSource[i + 3]);
    }
}

void Debug::printMatrix(const glm::mat4 &matrix, const char* title) {
    DEBUG_LOG("%s", title);
    const float *pSource = (const float*)glm::value_ptr(matrix);
    for (int i = 0; i < 16; i += 4) {
        DEBUG_LOG("[%.2f   %0.2f   %0.2f   %0.2f]", pSource[i], pSource[i + 1], pSource[i + 2], pSource[i + 3]);
    }
    DEBUG_LOG("---");
}

void Debug::printVector(const glm::vec3 &vector) {
    const float *pSource = (const float*)glm::value_ptr(vector);
    DEBUG_LOG("[%.2f   %0.2f   %0.2f]", pSource[0], pSource[1], pSource[2]);
}

void Debug::printVector(const glm::vec4 &vector) {
    const float *pSource = (const float*)glm::value_ptr(vector);
    DEBUG_LOG("[%.2f   %0.2f   %0.2f   %0.2f]", pSource[0], pSource[1], pSource[2], pSource[3]);
}

void Debug::printVector(const glm::vec3 &vector, const char* title) {
    DEBUG_LOG("%s", title);
    const float *pSource = (const float*)glm::value_ptr(vector);
    DEBUG_LOG("[%.2f   %0.2f   %0.2f]", pSource[0], pSource[1], pSource[2]);
    DEBUG_LOG("---");
}

void Debug::printVector(const glm::vec4 &vector, const char* title) {
    DEBUG_LOG("%s", title);
    const float *pSource = (const float*)glm::value_ptr(vector);
    DEBUG_LOG("[%.2f   %0.2f   %0.2f   %0.2f]", pSource[0], pSource[1], pSource[2], pSource[3]);
    DEBUG_LOG("---");
}