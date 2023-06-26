#pragma once

#include <glm/mat4x4.hpp>


enum class MovementDirection;


class FreeMotionCamera
{
private:
	static constexpr float FOV_DEG_MIN = 1.f;
	static constexpr float FOV_DEG_MAX = 45.f;
	static constexpr float CAMERA_MOVEMENT_SPEED = 0.1f;
	static constexpr float CAMERA_ROTATION_SENSITIVITY = 0.2f;
	static constexpr float CAMERA_ZOOM_SENSITIVITY = 0.5f;
	static constexpr glm::vec3 WORLD_UP_DIRECTION = {0.f, 1.f, 0.f};

public:
	[[nodiscard]]
	glm::mat4 getViewMatrix() const;

	[[nodiscard]]
	float getFovYRadians() const;

	void processMovementInput(MovementDirection direction);

	void processRotationInput(float diffX, float diffY);

	/**
	 * Zoom the camera view through changing its field of view angle.
	 * diff > 0 - zoom in.
	 * diff < 0 - zoom out.
	 */
	void processZoomInput(float diff);

private:
	glm::vec3 _cameraPosition = {0.f, 0.f, 1.f};
	glm::vec3 _cameraDirection = {0.f, 0.f, -1.f};
	float _yawDegrees = -90.f;
	float _pitchDegrees = 0.f;
	float _cameraFovDegrees = FOV_DEG_MAX;
};
