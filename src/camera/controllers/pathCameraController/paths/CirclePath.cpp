#include "CirclePath.h"

CirclePath::CirclePath(glm::vec3 circleCenter, float circleRadius, float pathSpeed) :
	circleCenter{ circleCenter }, circleRadius{ circleRadius }, 
	pathSpeed{ pathSpeed} {}

void CirclePath::update(glm::vec3& pos, double time) {

    time = pathSpeed*time;

	pos = circleRadius*glm::vec3(sin(time), 0.0f, cos(time));
	pos += glm::vec3 (circleCenter.x, circleCenter.y, circleCenter.z);
}