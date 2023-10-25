#pragma once
#include "procGen.h"
namespace wm
{
	ew::MeshData createPlane(float size, int subdivisions)
	{
		ew::MeshData plane;
		float width = size;
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
		float topY = height/2.0; // y = 0 centered
		float bottomY = -topY;
		//top center
		ew::Vertex top;
		top.pos = ew::Vec3(0, topY, 0);
		cylinder.vertices.push_back(top);

		//top ring 
		float thetaStep = ew::TAU/numSegemnts;
		for (int i = 0; i <= numSegemnts; i++)
		{
			float theta = i * thetaStep;
			ew::Vertex vertex;
			vertex.pos.x = cos(theta) * raidus;
			vertex.pos.y = topY;
			vertex.pos.z = sin(theta) * raidus;
			//normals
			ew::Vec3 normal = ew::Vec3(0 - vertex.pos.x, topY - vertex.pos.y, 0 - vertex.pos.z);

			vertex.normal = ew::Normalize(normal);

			cylinder.vertices.push_back(vertex);
		}
		//bottom ring
		for (int i = 0; i <= numSegemnts; i++)
		{
			float theta = i * thetaStep;
			ew::Vertex vertex;
			vertex.pos.x = (cos(theta) * raidus);
			vertex.pos.y = bottomY;
			vertex.pos.z = (sin(theta) * raidus);
			//normals
			ew::Vec3 normal = ew::Vec3(0 - vertex.pos.x, bottomY - vertex.pos.y,
				0 - vertex.pos.z );

			vertex.normal = ew::Normalize(normal);

			cylinder.vertices.push_back(vertex);
		}
		//bottom center
		ew::Vertex bottom;
		bottom.pos = ew::Vec3(0, bottomY, 0);
		cylinder.vertices.push_back(bottom);

		//cylinder cap
		int start = 1;
		int center = 0;
		int count = 0;
		for (int i = 0; i < numSegemnts; i++)
		{
			cylinder.indices.push_back(start + i);
			cylinder.indices.push_back(center);
			cylinder.indices.push_back(start + i + 1);
			count++;
		}
		//bottom indecies 
		//why is this plus 2?
		start = count+2;
		center = cylinder.vertices.size()-1;
		for (int i = 0; i < numSegemnts; i++)
		{
			cylinder.indices.push_back(start + i + 1);
			cylinder.indices.push_back(center);
			cylinder.indices.push_back(start + i);
	
		}
		//side indices
		int sideStart = 1;
		int columbs = numSegemnts + 1;
		for (int i = 0; i < columbs; i++)
		{
			start = sideStart + i;
			//triangle 1
			cylinder.indices.push_back(start);
			cylinder.indices.push_back(start + 1);
			cylinder.indices.push_back(start + columbs);
			//triangle 2
			cylinder.indices.push_back(start + columbs + 1);
			cylinder.indices.push_back(start + columbs);
			cylinder.indices.push_back(start + 1);
		}
		

		return cylinder;

	};
}