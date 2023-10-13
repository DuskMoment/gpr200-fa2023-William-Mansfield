#pragma once
#include "../ew/ewMath/mat4.h"
#include "../ew/ewMath/vec3.h"
#include "../ew/ewMath/ewMath.h"


namespace wm
{
	inline ew::Mat4 Identity()
	{
		return ew::Mat4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);

	};

	inline ew::Mat4 Sacle(ew::Vec3 s)
	{
		return ew::Mat4(
			s.x, 0, 0, 0,
			0,s.y, 0, 0,
			0, 0, s.z, 0,
			0, 0, 0, 1
		);
	};

	inline ew::Mat4 RotateX(float rad)
	{
		return ew::Mat4(
			1, 0, 0, 0,
			0, cos(rad), -sin(rad), 0,
			0, sin(rad), cos(rad), 0,
			0, 0, 0, 1
		);
	};
	inline ew::Mat4 RotateY(float rad)
	{
		return ew::Mat4(
			cos(rad), 0, sin(rad), 0,
			0, 1, 0, 0,
			-sin(rad), 0, cos(rad), 0,
			0, 0, 0, 1
		);
	};
	inline ew::Mat4 RotateZ(float rad)
	{
		return ew::Mat4(
			cos(rad), -sin(rad), 0, 0,
			sin(rad), cos(rad), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
			
	};
	inline ew::Mat4 Translate(ew::Vec3 t)
	{
		return ew::Mat4(
			1, 0, 0, t.x,
			0, 1, 0, t.y,
			0, 0, 1, t.z,
			0, 0, 0, 1
		);

	};

	inline ew::Mat4 LookAt(ew::Vec3 eye, ew::Vec3 target, ew::Vec3 up)
	{
		//get a vector to the target on the x axis
		ew::Vec3 f = ew::Normalize(eye-target);

		ew::Vec3 r = ew::Cross(up, f);
		r = ew::Normalize(r);

		ew::Vec3 u = ew::Cross(f, r);
		u = ew::Normalize(u);

		return ew::Mat4(
			r.x, r.y, r.z, -(ew::Dot(r,eye)),
			u.x, u.y, u.z, -(ew::Dot(u,eye)),
			f.x, f.y, f.z, -(ew::Dot(f,eye)),
			0, 0, 0, 1

		);

		
	};
	inline ew::Mat4 Orthographic(float height, float aspect, float near, float far)
	{
		float r = (height*aspect) / 2.0;
		float t = height / 2.0;
		float l = -r;
		float b = -t;

		return ew::Mat4(
			(2 / (r - l)), 0, 0, -(r + l) / (r - l),
			0, 2 / (t - b), 0, -(t + b) / (t - b),
			0, 0, -2 / (far - near), -(far + near) / (far - near),
			0, 0, 0, 1

		);


	};
	inline ew::Mat4 Perspective(float fov, float aspect, float near, float far)
	{
		fov = ew::Radians(fov);
		return ew::Mat4(
			1.0 /(tan(fov / 2) * aspect), 0.0, 0.0, 0.0,
			0.0, 1.0 / tan(fov / 2), 0.0, 0.0,
			0.0, 0.0, (near + far) / (near - far), (2 * far * near) / (near - far),
			0.0,0.0,-1.0,0.0
		);

	};

	struct Transform
	{
		ew::Vec3 position = ew::Vec3(0.0f, 0.0f, 0.0f);
		ew::Vec3 rotation = ew::Vec3(0.0f, 0.0f, 0.0f);
		ew::Vec3 scale = ew::Vec3(1.0f, 1.0f, 1.0f);
		ew::Mat4 getModelMatrix() const
		{
			return wm::Translate(position) * (RotateY(rotation.y) * RotateX(rotation.x) * RotateZ(rotation.z)) * Sacle(scale);
		}
	};

}