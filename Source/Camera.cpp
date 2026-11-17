#include "Camera.h"

namespace
{
	const double PI = std::acos(-1.0);
}

namespace Forge
{
	Camera::Camera(FVector pos, FRotator rot, float fov)
	{
		position = pos;
		rotation = rot;
		FOV = abs(fov);
	}

	FVector Camera::GetPosition() const
	{
		return position;
	}

	FRotator Camera::GetRotation() const
	{
		return rotation;
	}

	float Camera::GetFOV() const
	{
		return FOV;
	}

	FVector Camera::GetForwardVector() const
	{
		float pitch = rotation.Pitch * PI / 180.f;
		float yaw = rotation.Yaw * PI / 180.f;

		return FVector(
			-sinf(yaw) * cosf(pitch),
			cosf(yaw) * cosf(pitch),
			sinf(pitch)
		).Normalized();
	}

	float Camera::GetOrbitDistance() const
	{
		return OrbitDistance;
	}

	void Camera::SetPosition(FVector pos)
	{
		position = pos;
	}

	void Camera::SetRotation(FRotator rot)
	{
		rotation = rot;
	}
	void Camera::SetFOV(float fov)
	{
		FOV = abs(fov);
	}
	void Camera::Orbit(int deltaX, int deltaY)
	{
		constexpr float sensitivity = 0.3f;

		OrbitYaw -= deltaX * sensitivity;
		OrbitPitch += deltaY * sensitivity;

		OrbitPitch = std::clamp(OrbitPitch, -89.f, 89.f);

		UpdateOrbitPosition();
	}
	void Camera::Zoom(int amount)
	{
		FVector offset = GetPosition() - OrbitTarget;

		float currentDistance = sqrtf(
			offset.X * offset.X +
			offset.Y * offset.Y +
			offset.Z * offset.Z
		);

		if (currentDistance <= 0.0001f)
			return;

		OrbitDistance = std::clamp(
			OrbitDistance - amount,
			5.f,
			30.f
		);

		float scale = OrbitDistance / currentDistance;

		FVector newPosition = OrbitTarget + offset * scale;

		SetPosition(newPosition);
	}
	void Camera::UpdateOrbitPosition()
	{
		float yaw = OrbitYaw * PI / 180.f;
		float pitch = OrbitPitch * PI / 180.f;

		float cosPitch = cosf(pitch);

		FVector offset(
			OrbitDistance * cosPitch * sinf(yaw),
			-OrbitDistance * cosPitch * cosf(yaw),
			OrbitDistance * sinf(pitch)
		);

		SetPosition(OrbitTarget + offset);
	}
}