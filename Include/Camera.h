#pragma once
#include "framework.h"

namespace Forge
{
	class FORGE_API Camera
	{
	public:
		Camera(FVector pos=FVector(), FRotator rot=FRotator(), float fov=90.f);

		FVector GetPosition() const;
		FRotator GetRotation() const;
		float GetFOV() const;
		FVector GetForwardVector() const;
		float GetOrbitDistance() const;

		void SetPosition(FVector pos);
		void SetRotation(FRotator rot);
		void SetFOV(float fov);
		void Orbit(int deltaX, int deltaY);
		void Zoom(int amount);

	private:
		FVector position;
		FRotator rotation;
		float FOV;

		FVector OrbitTarget = FVector(0.f, 0.f, 0.f);

		float OrbitDistance = 10.f;
		float OrbitYaw = 0.f;
		float OrbitPitch = 0.f;

		void UpdateOrbitPosition();
	};
}
