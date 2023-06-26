#include "FreeMotionCamera.hpp"

#include "camera/MovementDirection.hpp"

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


void FreeMotionCamera::processMovementInput(MovementDirection direction)
{
	if (direction == MovementDirection::NONE) {
		return;
	}

	if (direction == MovementDirection::FORWARD) {
		_cameraPosition += _cameraDirection * CAMERA_MOVEMENT_SPEED;
	}
	else if (direction == MovementDirection::BACKWARD) {
		_cameraPosition -= _cameraDirection * CAMERA_MOVEMENT_SPEED;
	}
	else if (direction == MovementDirection::LEFT) {
		const glm::vec3 rightDir = glm::cross(_cameraDirection, WORLD_UP_DIRECTION);
		_cameraPosition -= rightDir * CAMERA_MOVEMENT_SPEED;
	}
	else if (direction == MovementDirection::RIGHT) {
		const glm::vec3 rightDir = glm::cross(_cameraDirection, WORLD_UP_DIRECTION);
		_cameraPosition += rightDir * CAMERA_MOVEMENT_SPEED;
	}
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
