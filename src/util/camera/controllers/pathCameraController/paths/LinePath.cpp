#include "LinePath.h"

LinePath::LinePath(glm::vec3 startPoint, glm::vec3 endPoint, float pathSpeed) :
    startPoint{ startPoint }, endPoint{ endPoint }, pathSpeed{ pathSpeed } {}

void LinePath::update(glm::vec3& pos, double time) {

    pos = startPoint + ((endPoint - startPoint) * (float)fmod(((pathSpeed * time) / glm::distance(startPoint, endPoint)), 1.0) );
}