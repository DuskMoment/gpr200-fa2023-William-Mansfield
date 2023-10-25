#pragma once
#include "../ew/mesh.h"
namespace wm
{
	ew::MeshData createSphere(float radius, int numSegments);

	ew::MeshData createCylinder(float height, float raidus, int numSegemnts);

	ew::MeshData createPlane(float size, int subdivisions);

}