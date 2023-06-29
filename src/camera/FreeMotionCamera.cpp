#include "FreeMotionCamera.hpp"

#include <algorithm>

#include <glm/ext/matrix_transform.hpp>


glm::mat4 FreeMotionCamera::getViewMatrix() const
{
	return glm::lookAt(_cameraPosition, _cameraPosition + _cameraDirection, WORLD_UP_DIRECTION);
}


float FreeMotionCamera::getFovYRadians() const
{
	return glm::radians(_cameraFovDegrees);
}


void FreeMotionCamera::update(float dt)
{
	if (glm::length(_movementDirection) > 0.f) {
		_cameraPosition += CAMERA_MOVEMENT_SPEED * dt * _movementDirection;
		_movementDirection = glm::vec3(0.f);
	}
}


void FreeMotionCamera::processMovementInput(float axisX, float axisY)
{
	glm::vec3 dir = _cameraDirection * std::clamp(axisY, -1.f, 1.f);

	const glm::vec3 rightDir = glm::normalize(glm::cross(_cameraDirection, WORLD_UP_DIRECTION));
	dir += rightDir * std::clamp(axisX, -1.f, 1.f);

	if (glm::length(dir) <= std::numeric_limits<float>::epsilon()) {
		return;
	}

	_movementDirection = glm::normalize(dir);
}


void FreeMotionCamera::processRotationInput(float diffX, float diffY)
{
	_pitchDegrees = std::clamp(_pitchDegrees - diffY * CAMERA_ROTATION_SENSITIVITY, -89.f, 89.f);

	_yawDegrees += diffX * CAMERA_ROTATION_SENSITIVITY;
	while (_yawDegrees > 360.f) {
		_yawDegrees -= 360.f;
	}
	while (_yawDegrees < 0.f) {
		_yawDegrees += 360.f;
	}

	const float pitchCos = std::cos(glm::radians(_pitchDegrees));
	_cameraDirection.x = std::cos(glm::radians(_yawDegrees)) * pitchCos;
	_cameraDirection.y = std::sin(glm::radians(_pitchDegrees));
	_cameraDirection.z = std::sin(glm::radians(_yawDegrees)) * pitchCos;
	_cameraDirection = glm::normalize(_cameraDirection);
}


void FreeMotionCamera::processZoomInput(float diff)
{
	// The smaller the fov, the larger the zoom.
	// When the fov is decreased, the camera view is zoomed in.
	const float fovDiff = - diff * CAMERA_ZOOM_SENSITIVITY;
	_cameraFovDegrees = std::clamp(_cameraFovDegrees + fovDiff, FOV_DEG_MIN, FOV_DEG_MAX);
}
