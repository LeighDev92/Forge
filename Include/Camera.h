#pragma once
#include "framework.h"

namespace Forge
{
	/**
	 * @brief Represents a view camera with position, orientation, field of view, and optional orbit controls.
	 */

	class FORGE_API Camera
	{
	public:
		Camera(FVector pos=FVector(), FRotator rot=FRotator(), float fov=90.f);

		/**
		 * @brief Retrieves the current position of the camera.
		 */
		FVector GetPosition() const;

		/**
		 * @brief Retrieves the current rotation of the camera.
		 */
		FRotator GetRotation() const;

		/**
		 * @brief Retrieves the current field of view of the camera.
		 */
		float GetFOV() const;

		/**
		 * @brief Retrieves the camera's forward vector.
		 */
		FVector GetForwardVector() const;

		/**
		 * @brief Retrieves the camera's right vector.
		 */
		FVector GetRightVector() const;

		/**
		 * @brief Retrieves the camera's up vector.
		 */
		FVector GetUpVector() const;

		/**
		 * @brief Retrieves the camera's orbit distance away from the orbit target.
		 */
		float GetOrbitDistance() const;

		/**
		 * @brief Sets the camera's position.
		 * @param pos Desired position.
		 */
		void SetPosition(FVector pos);
		
		/**
		 * @brief Sets the camera's rotation.
		 * @param rot Desired rotation.
		 */
		void SetRotation(FRotator rot);
		
		/**
		 * @brief Sets the camera's fov.
		 * @param fov Desired fov.
		 */
		void SetFOV(float fov);
		
		/**
		 * @brief Used to update the camera's position when the mouse moves
		 * @param deltaX mouse movement in the screen x direction.
		 * @param deltaY mouse movement in the screen y direction.
		 */
		void Orbit(int deltaX, int deltaY);
		
		/**
		 * @brief Adjusts the orbit distance of the camera, essentially updates the camera position towards/away from the orbit target
		 * @param amount the camera should increase or decrease the current zoom.
		 */
		void Zoom(int amount);
		
		/**
		 * @brief Orients the camera to face the direction of the target
		 * @param target Desired position to look at.
		 */
		void LookAt(const FVector& target);

		/**
		 * @brief Used to enable or disable orbitting.
		 * @param enable Input value.
		 */
		void SetOrbitTarget(bool enable);
		
		/**
		 * @brief Sets the position that the camera should orbit.
		 * @param position Input value.
		 */
		void SetOrbitTargetPosition(const FVector& position);

	private:
		FVector _position;
		FRotator _rotation;
		float _FOV;

		bool _shouldOrbitTarget;
		FVector _orbitTargetPosition;

		float _orbitDistance;

		/**
		 * @brief Updates orbit position to the proper location around the orbit target.
		 */
		void UpdateOrbitPosition();
	};
}
