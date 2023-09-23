//------------------------------------------------------------------------------
//
// File Name:	PGEMesh.h
// Author(s):	Gabe Gramblicka
// Project:		Dreadit
// Course:      GAM200
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

#include <vector>

#include "PGE.h"

typedef class Mesh Mesh;

//------------------------------------------------------------------------------
// Classes:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

namespace PGE
{
	typedef class PGE_Mesh
	{
	public:
		PGE_Mesh();
		// This struct gives us an easy way to handle the vertex data
		typedef struct Vertex
		{
			glm::vec2 position;         // The position of the vertex relative to the mesh
			glm::vec2 textureCoord;     // The texture coordinate associated with this vertex
			glm::vec4 color;            // The color to be applied to this vertex

			// Constructor for ease of use
			Vertex(const glm::vec2& position_, const glm::vec2& uv_, const glm::vec4& color_) :
				position(position_), textureCoord(uv_), color(color_)
			{
			}
		}Vertex;

	private:

		// id for the mesh
		GLuint _meshID;

		// vertex buffer id
		GLuint _meshVertexBufferID;

		// vertex amount
		int _meshVertexCount;

		glm::vec2 _offsetIsDirty;

		// vector of vertices
		std::vector<Vertex> _vertices;

	public:


		std::vector<Vertex>& GetVertices();
		

		void SetOffset(const glm::vec2 uv);
		// Adds a vertex to the current vertex vector
		// Params:
		//     pos - vertex points
		//     uv  - offset for the 
		static void AddVertex(const glm::vec2& pos, const glm::vec2& uv, const glm::vec4& color);

		static void AddTriangle(const glm::vec2& position1, const glm::vec2& textureOffset1, const glm::vec4& color1,
								const glm::vec2& position2, const glm::vec2& textureOffset2, const glm::vec4& color2,
								const glm::vec2& position3, const glm::vec2& textureOffset3, const glm::vec4& color3);


		static void Start();

		static PGE_Mesh* End();

		// Renders the mesh
		// Params:
		//    mesh - current mesh to render
		static void Render(const PGE_Mesh* mesh);

		// Resets the mesh for the current frame
		static void Reset();


		static void Free(const PGE_Mesh* meshResource);

	}PGE_Mesh;
}
