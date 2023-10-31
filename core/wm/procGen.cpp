#pragma once
#include "procGen.h"
namespace wm
{
	ew::MeshData createPlane(float size, int subdivisions)
	{
		ew::MeshData plane;
		float width = size;
		float height = size;
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
		top.uv = ew::Vec2(0.5, 0.5);
		cylinder.vertices.push_back(top);

		//top ring 
		createCylinderVertices(topY, cylinder, radius, numSegemnts,false);

		//bottom ring
		createCylinderVertices(bottomY, cylinder, radius, numSegemnts,false);
		
		//second top ring
		createCylinderVertices(topY, cylinder, radius, numSegemnts, true);

		//second bottom ring
		createCylinderVertices(bottomY, cylinder, radius, numSegemnts, true);

		//bottom center
		ew::Vertex bottom;
		bottom.pos = ew::Vec3(0, bottomY, 0);
		bottom.normal = ew::Vec3(0, -1, 0);
		bottom.uv = ew::Vec2(0.5, 0.5);
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

	void createCylinderVertices(float yPos, ew::MeshData& mesh, float radius, int numSegemnts, bool isTop)
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
			if (isTop)
			{
				ew::Vec3 normal = ew::Vec3(0,1,0);
				vertex.normal = ew::Normalize(normal);
				ew::Vec2  uv = ew::Vec2(cos(theta), sin(theta));
				uv = (uv+1)/ 2.0 ;
				vertex.uv = uv;
			}
			else
			{
				ew::Vec3 normal = ew::Vec3(vertex.pos.x- 0 , vertex.pos.y - yPos,
					 vertex.pos.z - 0);
				vertex.normal = ew::Normalize(normal);
				if (yPos < 0)
				{
					vertex.uv.y = 0.0;
				}
				else
				{
					vertex.uv.y = 1.0;
				}

				vertex.uv.x = static_cast<float>(i)/numSegemnts;
				
				
			}
			

			mesh.vertices.push_back(vertex);

		}

	};

	ew::MeshData createSphere(float radius, int numSegments)
	{
		ew::MeshData sphere;
		float thetaStep = ew::TAU / numSegments;
		float phiSteps = ew::PI / numSegments;
		ew::Vertex vertex;
		//vertices
		for (int row = 0; row <= numSegments; row++)
		{
			float phi = row * phiSteps;

			for (int col = 0; col <= numSegments; col++)
			{
				float theta = col * thetaStep;
				vertex.pos.x = radius * sin(phi) * sin(theta);
				vertex.pos.y = radius * cos(phi);
				vertex.pos.z = radius * sin(phi) * cos(theta);

				vertex.normal = ew::Normalize(vertex.pos);

				vertex.uv = ew::Vec2((numSegments - col) / static_cast<float>(numSegments), (numSegments - row) / static_cast<float>(numSegments));
				//vertex.uv = (vertex.uv + 1) / 2.0;
				sphere.vertices.push_back(vertex);
				
			}
		}
		//indices
		//top cap
		int poleStart = 0;
		int sideStart = numSegments + 1;
		for (size_t i = 0; i < numSegments; i++)
		{
			sphere.indices.push_back(sideStart + i);
			sphere.indices.push_back(sideStart + i + 1);
			sphere.indices.push_back(poleStart + i);
		}

		
		int columbs = numSegments + 1;
		for (int row = 1; row < numSegments - 1; row++)
		{
			for (int col = 0; col < numSegments; col++)
			{
				int start = row * columbs + col;
				//triangle one
				sphere.indices.push_back(start);
				sphere.indices.push_back(start + columbs);
				sphere.indices.push_back(start + 1);
				
				//triangle two
				sphere.indices.push_back(start + columbs + 1);
				sphere.indices.push_back(start + 1);
				sphere.indices.push_back(start + columbs);
				
			}
		}
		//bottom
		poleStart = sphere.vertices.size()-numSegments;
		sideStart = poleStart - numSegments - 2 ;
		for (size_t i = 0; i < numSegments; i++)
		{
			sphere.indices.push_back(sideStart + i);
			sphere.indices.push_back(poleStart + i);
			sphere.indices.push_back(sideStart + i + 1);
		}
		return sphere;
	};
	
	
}