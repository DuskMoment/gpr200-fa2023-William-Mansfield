#pragma once
#include "procGen.h"
namespace wm
{
	ew::MeshData createPlane(float size, int subdivisions)
	{
		ew::MeshData plane;
		float width = subdivisions;
		float height = size/width;
		//vertex
		for (int row = 0; row <= subdivisions; row++)
		{
			for (int col = 0; col <= subdivisions; col++)
			{
				float colSub = static_cast<float>(col) / subdivisions;
				float rowSub = static_cast<float>(row) / subdivisions;
				ew::Vertex vertex;
				vertex.pos.x = width * colSub;
				vertex.pos.y = 0;
				vertex.pos.z = -height * rowSub;

				//normals
				vertex.normal = ew::Vec3(0, 1, 0);
				//UVs
				vertex.uv = ew::Vec2(colSub, rowSub);

				plane.vertices.push_back(vertex);
			}
		}
		//indecies
		float columbs = subdivisions + 1;
		for (int row = 0; row < subdivisions; row++)
		{
			for (int col = 0; col < subdivisions; col++)
			{
	
				unsigned int start = row * columbs + col;
				//bottom triangle
				plane.indices.push_back(start);
				plane.indices.push_back(start + 1);
				plane.indices.push_back(start + columbs + 1);
				//bottom triangle
				plane.indices.push_back(start);
				plane.indices.push_back(start + columbs + 1);
				plane.indices.push_back(start + columbs);

			}
		}
		return plane;
	};

	ew::MeshData createCylinder(float height, float raidus, int numSegemnts)
	{
		ew::MeshData cylinder;
		float topY = height/2; // y = 0 centered
		float bottomY = -topY;
		//top center
		cylinder.vertices.push_back(ew::Vertex{ 0,topY,0 });

		//top ring 
		float step = 2 / ew::PI;
		for (int i = 0; i <= numSegemnts; i++)
		{
			float theta = i + step;
			ew::Vertex vertex;
			vertex.pos.x = cos(theta) * raidus;
			vertex.pos.y = height / 2;
			vertex.pos.z = sin(theta) * raidus;
		}


	};
}