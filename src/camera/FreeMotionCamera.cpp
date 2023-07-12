#include "FreeMotionCamera.hpp"

#include <algorithm>

#include <glm/ext/matrix_transform.hpp>


glm::mat4 FreeMotionCamera::getViewMatrix() const
{
	// return glm::lookAt(_position, _position + _lookDirection, WORLD_UP_DIRECTION);
	return calculateViewMatrix(_position, _position + _lookDirection, WORLD_UP_DIRECTION);
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


glm::mat4 FreeMotionCamera::calculateViewMatrix(
		const glm::vec3& cameraPos,
		const glm::vec3& lookAtPos,
		const glm::vec3& worldUpDirection
)
{
	// It's supposed to work like glm::lookAt() function for right-handed coordinate system.
	// The custom implementation is just for practice.

	// i, j, k - basis vectors of new coordinate system (view space).
	const glm::vec3 k = glm::normalize((cameraPos - lookAtPos));
	const glm::vec3 i = glm::normalize(glm::cross(worldUpDirection, k));
	const glm::vec3 j = glm::normalize(glm::cross(k, i));

	// GLSL matrix has column-major order.
	auto m = glm::mat4(1);
	m[0][0] = i.x;	m[1][0] = i.y;	m[2][0] = i.z;	m[3][0] = glm::dot(i, -cameraPos);	// The first row.
	m[0][1] = j.x;	m[1][1] = j.y;	m[2][1] = j.z;	m[3][1] = glm::dot(j, -cameraPos);	// The second row.
	m[0][2] = k.x;	m[1][2] = k.y;	m[2][2] = k.z;	m[3][2] = glm::dot(k, -cameraPos);	// The third row.

	return m;
}
