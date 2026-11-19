#include "Camera.h"

namespace
{
	const double PI = std::acos(-1.0);
}

namespace Forge
{
	Camera::Camera(FVector pos, FRotator rot, float fov)
	{
		_position = pos;
		_rotation = rot;
		_FOV = abs(fov);
		_shouldOrbitTarget = false;
	}

	FVector Camera::GetPosition() const
	{
		return _position;
	}

	FRotator Camera::GetRotation() const
	{
		return _rotation;
	}

	float Camera::GetFOV() const
	{
		return _FOV;
	}

	FVector Camera::GetForwardVector() const
	{
		float pitch = _rotation.Pitch * PI / 180.f;
		float yaw = _rotation.Yaw * PI / 180.f;

		return FVector(
			-sinf(yaw) * cosf(pitch),
			cosf(yaw) * cosf(pitch),
			sinf(pitch)
		).Normalized();
	}

	FVector Camera::GetRightVector() const
	{
		FVector worldUp(0.f, 0.f, 1.f);
		return FVector::Cross(GetForwardVector(), worldUp).Normalized();
	}

	FVector Camera::GetUpVector() const
	{
		return FVector::Cross(GetRightVector(), GetForwardVector()).Normalized();
	}

	float Camera::GetOrbitDistance() const
	{
		return _orbitDistance;
	}

	void Camera::SetPosition(FVector pos)
	{
		_position = pos;
	}

	void Camera::SetRotation(FRotator rot)
	{
		_rotation = rot;
	}

	void Camera::SetFOV(float fov)
	{
		_FOV = abs(fov);
	}

	void Camera::Orbit(int deltaX, int deltaY)
	{
		if (!_shouldOrbitTarget)
		{
			return;
		}

		constexpr float sensitivity = 0.3f;

		_rotation.Yaw -= deltaX * sensitivity;
		_rotation.Pitch -= deltaY * sensitivity;

		_rotation.Pitch = std::clamp(_rotation.Pitch, -89.f, 89.f);

		UpdateOrbitPosition();
	}

	void Camera::Zoom(int amount)
	{
		constexpr float minDistance = 5.f;
		constexpr float maxDistance = 30.f;

		FVector offset = GetPosition() - _orbitTargetPosition;

		float currentDistance = sqrtf(
			offset.X * offset.X +
			offset.Y * offset.Y +
			offset.Z * offset.Z
		);

		if (currentDistance <= 0.0001f)
			return;

		_orbitDistance = std::clamp(
			_orbitDistance - amount,
			minDistance,
			maxDistance
		);

		float scale = _orbitDistance / currentDistance;

		FVector newPosition = _orbitTargetPosition + offset * scale;

		SetPosition(newPosition);
	}

	void Camera::LookAt(const FVector& target)
	{
		FVector direction = (target - _position).Normalized();

		_rotation.Yaw = atan2f(-direction.X, direction.Y) * 180.f / PI;
		_rotation.Pitch = asinf(direction.Z) * 180.f / PI;
	}

	void Camera::SetOrbitTarget(bool enable)
	{
		_shouldOrbitTarget = enable;

		if (_shouldOrbitTarget)
		{
			LookAt(_orbitTargetPosition);

			FVector offset = _position - _orbitTargetPosition;

			float currentDistance = sqrtf(
				offset.X * offset.X +
				offset.Y * offset.Y +
				offset.Z * offset.Z
			);

			_orbitDistance = currentDistance;
			Orbit(0, 0);
		}
	}

	void Camera::SetOrbitTargetPosition(const FVector& position)
	{
		_orbitTargetPosition = position;
	}

	void Camera::UpdateOrbitPosition()
	{
		FVector offset = GetForwardVector() * -_orbitDistance;
		SetPosition(_orbitTargetPosition + offset);
	}
}