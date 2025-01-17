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

	ew::MeshData createTorus(float innerRadius, float outerRadius, int sl, int st)
	{
		ew::MeshData torus;
		
		//ring radius (thick part of the dount)
		float phi = 0.0;
		float dp = ew::TAU / sl;
		//torus radius 
		float theta = 0.0;
		float dt = ew::TAU / st;
		

		for (int stack = 0; stack <= st; stack++)
		{
			theta = dt * stack;
			for (int slice = 0; slice <= sl; slice++)
			{
				ew::Vertex vertex;
				phi = dp * slice;
				vertex.pos.x = cos(theta) * (outerRadius + cos(phi) * innerRadius);
				vertex.pos.y = sin(theta) * (outerRadius + cos(phi) * innerRadius);
				vertex.pos.z = sin(phi) * innerRadius;

				//normals 
				//tangent for big circle
				float tx = -sin(theta);
				float ty = cos(theta);
				float tz = 0;
				ew::Vec3 tVector = ew::Vec3(tx, ty, tz);
				//tangent for small circle
				float sx = cos(theta) * (-sin(phi));
				float sy = sin(theta) * (-sin(phi));
				float sz = cos(phi);
				ew::Vec3 sVector = ew::Vec3(sx, sy, sz);
				//cross product
				vertex.normal = ew::Normalize(ew::Cross(tVector, sVector));

				//UV
				vertex.uv = ew::Vec2(slice / static_cast<float>(sl), stack / static_cast<float>(st));
				torus.vertices.push_back(vertex);

				
			}
		}

		//indices 
		sl = sl + 1;
		for (int stack = 0; stack < st; stack++)
		{

			for (int slice = 0; slice < sl-1; slice++)
			{

				unsigned int start = stack * sl + slice;

			
				//tri one // flip the slice and stack 
				torus.indices.push_back(start);
				torus.indices.push_back(start + 1 + sl);
				torus.indices.push_back(start + 1);
				
				//tri two
				torus.indices.push_back(start);
				torus.indices.push_back(start + sl);
				torus.indices.push_back(start + sl + 1);


			}
		}

		return torus;
	}

	// Making the randomly generated land - Isabel Rowland
	ew::MeshData createLand(float size, int subdivisions, int seed)
	{
		ew::MeshData plane;
		float width = size;
		float height = size;
		ir::PerlinNoise perlin = ir::PerlinNoise::PerlinNoise(seed); // Makes a perlin noise variable

		//vertex
		for (int row = 0; row <= subdivisions; row++)
		{
			for (int col = 0; col <= subdivisions; col++)
			{
				ew::Vertex vertex;
				float colSub = static_cast<float>(col) / subdivisions;
				float rowSub = static_cast<float>(row) / subdivisions;

				float n = perlin.noiseGen(row * 0.01, col * 0.01, seed); // generates the noise with a given seed
				n += 1.0; // setting the generated value from 0 to 1 instead of -1 to 1
				n *= 0.5;
				vertex.pos.y = n * 10; // making the y values bigger
				
				vertex.pos.x = width * colSub;
				
				vertex.pos.z = -height * rowSub;

				//normals
				vertex.normal = ew::Vec3(0, 1, 0);
				//UVs
				vertex.uv = ew::Vec2(colSub, rowSub);

				plane.vertices.push_back(vertex);
			}
		}

		// Setting normals - Isabel Rowland with math help from Will
		for (int row = 0; row <= subdivisions; row++) {
			for (int col = 0; col <= subdivisions; col++) {
				ew::Vec3 middle = plane.vertices[row * subdivisions + col].pos; // the point in question
				bool isRight, isUp, isLeft, isDown; // do all the vectors exist (is it on an edge or not)
				ew::Vec3 top, left, right, bottom; // the vertex positions above, below, to the left, and to the right of the point
				
				// Gathering the positional data of the vertices above, below, left, and right
				if (row != 0) {
					top = plane.vertices[row * subdivisions + col - subdivisions].pos;
					isUp = 1;
				}
				else {
					top = middle;
					isUp = 0;
				}
				if (col != 0) {
					left = plane.vertices[row * subdivisions + col - 1].pos;
					isLeft = 1;
				}
				else {
					left = middle;
					isLeft = 0;
				}
				if (col != subdivisions) {
					right = plane.vertices[row * subdivisions + col + 1].pos;
					isRight = 1;
				}
				else {
					right = middle;
					isRight = 0;
				}
				if (row != subdivisions) {
					bottom = plane.vertices[row * subdivisions + col + subdivisions].pos;
					isDown = 1;
				}
				else {
					bottom = middle;
					isDown = 0;
				}
				
				// Calculating the vectors from the middle to the other vertices
				ew::Vec3 rightVec = right - middle;
				ew::Vec3 upVec = top - middle;
				ew::Vec3 botVec = middle - bottom;
				ew::Vec3 leftVec = middle - left;
				ew::Vec3 normal;
				
				// Calculating the normals based on what exists
				if (isUp && isDown && isLeft && isRight) {
					upVec = upVec + botVec;
					rightVec = rightVec + leftVec;
					normal = ew::Normalize(ew::Cross(upVec, rightVec));
					
				}
				else if (!isUp) {
					botVec = bottom - middle;
					rightVec = rightVec + leftVec;
					ew::Vec3 normal = ew::Normalize(ew::Cross(botVec, rightVec));
				}
				else if (!isDown) {
					rightVec = rightVec + leftVec;
					normal = ew::Normalize(ew::Cross(upVec, rightVec));
				}
				else if (!isRight) {
					leftVec = left - middle;
					upVec = upVec + botVec;
					normal = ew::Normalize(ew::Cross(upVec, leftVec));
				}
				else if (!isLeft) {
					upVec = upVec + botVec;
					normal = ew::Normalize(ew::Cross(upVec, rightVec));
				}
				if (!isRight && !isUp) {
					botVec = bottom - middle;
					leftVec = left - middle;
					normal = ew::Normalize(ew::Cross(botVec, leftVec));
				}
				else if (!isRight && !isDown) {
					upVec = top - middle;
					leftVec = left - middle;
					normal = ew::Normalize(ew::Cross(upVec, leftVec));
				}
				else if (!isLeft && !isDown) {
					upVec = top - middle;
					rightVec = right - middle;
					normal = ew::Normalize(ew::Cross(upVec, rightVec));
				}
				else if (!isLeft && !isUp){
					botVec = bottom - middle;
					rightVec = right - middle;
					normal = ew::Normalize(ew::Cross(botVec, rightVec));
				}
				
				// Setting the normal at the point in question
				plane.vertices[row * subdivisions + col].normal = normal;
				

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
	
	
}