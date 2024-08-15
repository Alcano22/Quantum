#pragma once

#include <glm/glm.hpp>
#include <glm/ext/scalar_constants.hpp>

#include <vector>
#include <stdint.h>

namespace Quantum
{
	inline void GenerateCube(std::vector<float>& vertices, std::vector<uint32_t>& indices)
	{
		vertices.clear();
		indices.clear();
		
		constexpr float halfSize = 0.5f;
		const std::vector<float> positions =
		{
			// positions						normals
			-halfSize, -halfSize, -halfSize,	0.0f,  0.0f, -1.0f, // Back face
			 halfSize, -halfSize, -halfSize,    0.0f,  0.0f, -1.0f,
			 halfSize,  halfSize, -halfSize,    0.0f,  0.0f, -1.0f,
			-halfSize,  halfSize, -halfSize,    0.0f,  0.0f, -1.0f,
		
			-halfSize, -halfSize,  halfSize,    0.0f,  0.0f,  1.0f, // Front face
			 halfSize, -halfSize,  halfSize,    0.0f,  0.0f,  1.0f,
			 halfSize,  halfSize,  halfSize,    0.0f,  0.0f,  1.0f,
			-halfSize,  halfSize,  halfSize,    0.0f,  0.0f,  1.0f,
		
			-halfSize,  halfSize,  halfSize, -  1.0f,  0.0f,  0.0f, // Left face
			-halfSize,  halfSize, -halfSize, -  1.0f,  0.0f,  0.0f,
			-halfSize, -halfSize, -halfSize, -  1.0f,  0.0f,  0.0f,
			-halfSize, -halfSize,  halfSize, -  1.0f,  0.0f,  0.0f,
		
			 halfSize,  halfSize,  halfSize,    1.0f,  0.0f,  0.0f, // Right face
			 halfSize,  halfSize, -halfSize,    1.0f,  0.0f,  0.0f,
			 halfSize, -halfSize, -halfSize,    1.0f,  0.0f,  0.0f,
			 halfSize, -halfSize,  halfSize,    1.0f,  0.0f,  0.0f,
		
			-halfSize, -halfSize, -halfSize,    0.0f, -1.0f,  0.0f, // Bottom face
			 halfSize, -halfSize, -halfSize,    0.0f, -1.0f,  0.0f,
			 halfSize, -halfSize,  halfSize,    0.0f, -1.0f,  0.0f,
			-halfSize, -halfSize,  halfSize,    0.0f, -1.0f,  0.0f,
		
			-halfSize,  halfSize, -halfSize,    0.0f,  1.0f,  0.0f, // Top face
			 halfSize,  halfSize, -halfSize,    0.0f,  1.0f,  0.0f,
			 halfSize,  halfSize,  halfSize,    0.0f,  1.0f,  0.0f,
			-halfSize,  halfSize,  halfSize,    0.0f,  1.0f,  0.0f
		};

		vertices.insert(vertices.end(), positions.begin(), positions.end());

		const std::vector<uint32_t> cubeIndices =
		{
			0,  1,  2,		2,  3,  0,	// back face
			4,  5,  6,		6,  7,  4,	// front face
			8,  9,  10,		10, 11, 8,	// left face
			12, 13, 14,		14, 15, 12,	// right face
			16, 17, 18,		18, 19, 16,	// bottom face
			20, 21, 22,		22, 23, 20	// top face
		};

		indices.insert(indices.end(), cubeIndices.begin(), cubeIndices.end());
	}
	
	inline void GenerateSphere(const uint32_t numSectors, const uint32_t numStacks, std::vector<float>& vertices, std::vector<uint32_t>& indices)
	{
		vertices.clear();
		indices.clear();
		
		constexpr float radius = 0.5f;
		constexpr float lengthInv = 1.0f / radius;

		const float sectorStep = 2 * glm::pi<float>() / numSectors;
		const float stackStep = glm::pi<float>() / numStacks;

		for (uint32_t i = 0; i <= numStacks; i++)
		{
			const float stackAngle = glm::pi<float>() / 2.0f - i * stackStep;
			const float xy = radius * cosf(stackAngle);
			const float z = radius * sinf(stackAngle);

			for (uint32_t j = 0; j <= numSectors; j++)
			{
				const float sectorAngle = j * sectorStep;

				const float x = xy * cosf(sectorAngle);
				const float y = xy * sinf(sectorAngle);

				vertices.push_back(x);
				vertices.push_back(y);
				vertices.push_back(z);

				const float nx = x * lengthInv;
				const float ny = y * lengthInv;
				const float nz = z * lengthInv;

				vertices.push_back(nx);
				vertices.push_back(ny);
				vertices.push_back(nz);
			}
		}

		for (uint32_t i = 0; i < numStacks; i++)
		{
			uint32_t k1 = i * (numSectors + 1);
			uint32_t k2 = k1 + numSectors + 1;

			for (uint32_t j = 0; j < numSectors; j++, k1++, k2++)
			{
				if (i != 0)
				{
					indices.push_back(k1);
					indices.push_back(k2);
					indices.push_back(k1 + 1);
				}

				if (i != (numStacks - 1))
				{
					indices.push_back(k1 + 1);
					indices.push_back(k2);
					indices.push_back(k2 + 1);
				}
			}
		}
	}
}
