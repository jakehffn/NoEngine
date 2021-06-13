#include "PathCameraController.h"

PathCameraController::PathCameraController(Clock* clock, Path* path) :
	clock{ clock }, path{ path } {}

void PathCameraController::update(glm::vec3& pos) {
	path->update(pos, clock->getCumulativeTime());
}