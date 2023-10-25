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


	ew::MeshData createCylinder(float height, float radius, int numSegemnts)
	{
		ew::MeshData cylinder;
		float topY = height/2.0; // y = 0 centered
		float bottomY = -topY;
		//top center
		ew::Vertex top;
		top.pos = ew::Vec3(0, topY, 0);
		top.normal = ew::Vec3(0, 1, 0);
		cylinder.vertices.push_back(top);

		//top ring 
		createCylinderVertices(topY, cylinder, radius, numSegemnts,false);
		//second ring

		//bottom ring
		createCylinderVertices(bottomY, cylinder, radius, numSegemnts,false);
		
		//second top ring
		createCylinderVertices(topY, cylinder, radius, numSegemnts, true);

		//second borrom ring
		createCylinderVertices(bottomY, cylinder, radius, numSegemnts, true);

		//bottom center
		ew::Vertex bottom;
		bottom.pos = ew::Vec3(0, bottomY, 0);
		bottom.normal = ew::Vec3(0, -1, 0);
		cylinder.vertices.push_back(bottom);

		//cylinder cap
		//why doe these numbers work????
		
		int start = (numSegemnts * 2)+3;
		int center = 0;
		for (int i = 0; i < numSegemnts; i++)
		{
			cylinder.indices.push_back(start + i);
			cylinder.indices.push_back(center);
			cylinder.indices.push_back(start + i + 1);
			
		}
		//bottom indecies 
		//why do these numbers work?????
		start = (numSegemnts*3)+4;
		center = cylinder.vertices.size()-1;
		for (int i = 0; i < numSegemnts; i++)
		{
			cylinder.indices.push_back(start + i + 1);
			cylinder.indices.push_back(center);
			cylinder.indices.push_back(start + i);
	
		}
		////side indices
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

	void createCylinderVertices(float yPos, ew::MeshData& mesh, float radius, int numSegemnts, bool isHardEdge)
	{
		float thetaStep = ew::TAU / numSegemnts;
		for (int i = 0; i <= numSegemnts; i++)
		{
			float theta = i * thetaStep;
			ew::Vertex vertex;

			//vertexOne
			vertex.pos.x = cos(theta) * radius;
			vertex.pos.y = yPos;
			vertex.pos.z = sin(theta) * radius;


			//normals vertex one
			if (isHardEdge)
			{
				ew::Vec3 normal = ew::Vec3(0,1,0);
				vertex.normal = ew::Normalize(normal);
			}
			else
			{
				ew::Vec3 normal = ew::Vec3(0 - vertex.pos.x, yPos - vertex.pos.y,
					0 - vertex.pos.z);
				vertex.normal = ew::Normalize(normal);
			}
			

			mesh.vertices.push_back(vertex);

		}

	};
	
	
}