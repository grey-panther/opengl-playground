#pragma once

#include <glm/mat4x4.hpp>


class FreeMotionCamera
{
private:
	static constexpr float FOV_DEG_MIN = 1.f;
	static constexpr float FOV_DEG_MAX = 45.f;
	static constexpr float CAMERA_MOVEMENT_SPEED = 10.f;
	static constexpr float CAMERA_ROTATION_SENSITIVITY = 0.2f;
	static constexpr float CAMERA_ZOOM_SENSITIVITY = 0.5f;
	static constexpr glm::vec3 WORLD_UP_DIRECTION = {0.f, 1.f, 0.f};

public:
	[[nodiscard]]
	glm::mat4 getViewMatrix() const;

	[[nodiscard]]
	float getFovYRadians() const;

	void update(float dt);

	/**
	 * @brief Process the position change input.
	 * @param axisX (axisX < 0) - moving left; (axisX > 0) - moving right; The value is clamped in [-1; 1].
	 * @param axisY (axisY < 0) - moving backward; (axisY > 0) - moving forward; The value is clamped in [-1; 1].
	 */
	void processMovementInput(float axisX, float axisY);

	void processRotationInput(float diffX, float diffY);

	/**
	 * @brief Zoom the camera view through changing its angle of field of view.
	 * @param diff (diff > 0) - zoom in; (diff < 0) - zoom out.
	 */
	void processZoomInput(float diff);

private:
	glm::vec3 _position = {0.f, 0.f, 1.f};
	glm::vec3 _lookDirection = {0.f, 0.f, -1.f};
	glm::vec3 _movementDirection = {0.f, 0.f, 0.f};
	float _yawDegrees = -90.f;
	float _pitchDegrees = 0.f;
	float _fovDegrees = FOV_DEG_MAX;
};
