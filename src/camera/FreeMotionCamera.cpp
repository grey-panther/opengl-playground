#include "FreeMotionCamera.hpp"

#include <algorithm>

#include <glm/ext/matrix_transform.hpp>


glm::mat4 FreeMotionCamera::getViewMatrix() const
{
	return glm::lookAt(_position, _position + _lookDirection, WORLD_UP_DIRECTION);
}


float FreeMotionCamera::getFovYRadians() const
{
	return glm::radians(_fovDegrees);
}


void FreeMotionCamera::update(float dt)
{
	if (glm::length(_movementDirection) > 0.f) {
		_position += CAMERA_MOVEMENT_SPEED * dt * _movementDirection;
		_movementDirection = glm::vec3(0.f);
	}
}


void FreeMotionCamera::processMovementInput(float axisX, float axisY)
{
	glm::vec3 dir = _lookDirection * std::clamp(axisY, -1.f, 1.f);

	const glm::vec3 rightDir = glm::normalize(glm::cross(_lookDirection, WORLD_UP_DIRECTION));
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
	_lookDirection.x = std::cos(glm::radians(_yawDegrees)) * pitchCos;
	_lookDirection.y = std::sin(glm::radians(_pitchDegrees));
	_lookDirection.z = std::sin(glm::radians(_yawDegrees)) * pitchCos;
	_lookDirection = glm::normalize(_lookDirection);
}


void FreeMotionCamera::processZoomInput(float diff)
{
	// The smaller the fov, the larger the zoom.
	// When the fov is decreased, the camera view is zoomed in.
	const float fovDiff = - diff * CAMERA_ZOOM_SENSITIVITY;
	_fovDegrees = std::clamp(_fovDegrees + fovDiff, FOV_DEG_MIN, FOV_DEG_MAX);
}
