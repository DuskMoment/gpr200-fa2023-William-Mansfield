#pragma once
#include "../ew/mesh.h"
#include"../ew/ewMath/ewMath.h"
namespace wm
{
	ew::MeshData createSphere(float radius, int numSegments);

	ew::MeshData createCylinder(float height, float radius, int numSegemnts);

	ew::MeshData createPlane(float size, int subdivisions);

	void createCylinderVertices(float yPos, ew::MeshData& mesh, float radius, int numSegemnts, bool isTop);

	ew::MeshData createTorus(float innerRadius, float outerRadius, int sl, int st);


}