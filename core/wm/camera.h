#pragma once
#include "../ew/ewMath/mat4.h"
#include "../ew/ewMath/vec3.h"
#include "transformations.h"

namespace wm
{
	struct Camera
	{
		ew::Vec3 position; // camera pos;
		ew::Vec3 target; // possition to look at
		float fov;
		float aspectRatio;
		float nearPlane;
		float farPlane;
		bool orthographic;
		float orthoSize;
		ew::Mat4 VeiwMatrix()
		{
			return wm::LookAt(position, target, ew::Vec3(0, 1.0, 0));
		};
		ew::Mat4 ProjectionMarix()
		{
			if (orthographic)
			{
				return Orthographic(orthoSize, aspectRatio, nearPlane, farPlane);
			}
			else
			{
				return Perspective(fov, aspectRatio, nearPlane, farPlane);
			}
			return Identity();
		};


	};
	struct CameraControls
	{
		double prevMouseX, prevMouseY;
		float yaw = 0;
		float pitch = 0;
		float mouseSensitivity = 0.1f;
		bool firstMouse = true;
		float moveSpeed = 5.0f;
	};
}