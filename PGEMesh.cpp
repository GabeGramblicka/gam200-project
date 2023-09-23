//------------------------------------------------------------------------------
//
// File Name:	PGEMesh.cpp
// Author(s):	Gabe Gramblicka
// Project:     Dreadit
// Course:		GAM200
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "stdinclude.h"
#include "PGEMesh.h"

#include "Mesh.h"

using namespace PGE;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

PGE_Mesh* mesh; // This is global to hide implementation

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

PGE::PGE_Mesh::PGE_Mesh()
    : _meshID(0)
    , _meshVertexBufferID(0)
    , _meshVertexCount(0)
    , _offsetIsDirty(glm::vec2(0.0f, 0.0f))
{
    
}

std::vector<PGE_Mesh::Vertex>& PGE_Mesh::GetVertices()
{
    return _vertices;
}

void PGE::PGE_Mesh::AddVertex(const glm::vec2& pos, const glm::vec2& uv, const glm::vec4& color)
{
    mesh->_vertices.push_back(Vertex(pos, uv, color));
}

void PGE::PGE_Mesh::AddTriangle(const glm::vec2& position1, const glm::vec2& textureOffset1, const glm::vec4& color1,
                                const glm::vec2& position2, const glm::vec2& textureOffset2, const glm::vec4& color2,
                                const glm::vec2& position3, const glm::vec2& textureOffset3, const glm::vec4& color3)
{
    AddVertex(position1, textureOffset1, color1);
    AddVertex(position2, textureOffset2, color2);
    AddVertex(position3, textureOffset3, color3);
}

void PGE_Mesh::Start()
{
    mesh = new PGE_Mesh;
    // Generate the mesh
    glGenVertexArrays(1, &mesh->_meshID);
    PGE::CheckError();
    // Bind the mesh as the current OpenGL object
    glBindVertexArray(mesh->_meshID);
    PGE::CheckError();

    // Generate the vertex buffer
    glGenBuffers(1, &mesh->_meshVertexBufferID);
    PGE::CheckError();
    // Bind the vertex buffer as the current object
    glBindBuffer(GL_ARRAY_BUFFER, mesh->_meshVertexBufferID);
    PGE::CheckError();
}

PGE_Mesh* PGE_Mesh::End()
{
    // Assign the vertex data to the mesh
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mesh->_vertices.size(), 
                    &(mesh->_vertices[0].position.x), GL_STATIC_DRAW);
    PGE::CheckError();

    // These attribute layout values are assigned in the vertex shader
    GLint posAttribute = 0;
    GLint uvAttribute = 1;
    GLint colorAttribute = 2;

    // Set the position attribute (2 float values)
    glVertexAttribPointer(posAttribute, 2, GL_FLOAT, false, sizeof(Vertex), 0);
    PGE::CheckError();
    // Set the texture coordinate attribute (2 float values, offset by the size of a vec2)
    glVertexAttribPointer(uvAttribute, 2, GL_FLOAT, false, sizeof(Vertex), (void*)(sizeof(glm::vec2)));
    PGE::CheckError();
    // Set the color attribute (4 float values, offset by the size of 2 vec2s)
    glVertexAttribPointer(colorAttribute, 4, GL_FLOAT, false, sizeof(Vertex), (void*)(sizeof(glm::vec2) * 2));
    PGE::CheckError();

    // Enable the attributes
    glEnableVertexAttribArray(posAttribute);
    PGE::CheckError();
    glEnableVertexAttribArray(uvAttribute);
    PGE::CheckError();
    glEnableVertexAttribArray(colorAttribute);
    PGE::CheckError();

    // Unbind the vertex array to reset
    const GLuint unbindAll = 0;
    glBindVertexArray(unbindAll);
    PGE::CheckError();

    // Save the number of vertices used for the mesh
    mesh->_meshVertexCount = static_cast<int>(mesh->_vertices.size());

    return mesh;
}

void PGE_Mesh::Render(const PGE_Mesh* meshResource)
{
    glBindVertexArray(meshResource->_meshID);
    PGE::CheckError();

    glBindBuffer(GL_ARRAY_BUFFER, meshResource->_meshID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mesh->_vertices.size(),
        &(mesh->_vertices[0].position.x), GL_STATIC_DRAW);
    PGE::CheckError();

    // Draw the currently bound mesh
    glDrawArrays(GL_TRIANGLES, 0, meshResource->_meshVertexCount);
    PGE::CheckError();

    // Reset the mesh, shader, and texture
    glBindVertexArray(0);
    PGE::CheckError();
    glBindTexture(GL_TEXTURE_2D, 0);
    PGE::CheckError();
    glUseProgram(0);
    PGE::CheckError();
}

void PGE_Mesh::Reset()
{
    glBindVertexArray(0);
    CheckError();
}

void PGE_Mesh::SetOffset(const glm::vec2 uv)
{

    if (_offsetIsDirty == uv)
    {
        return;
    }
    else
    {
        _offsetIsDirty = uv;
    }

    for (auto& vertex : mesh->GetVertices())
    {
        //vertex.textureCoord.x += uv.x;
        vertex.textureCoord += uv;
    }
}

void PGE_Mesh::Free(const PGE_Mesh* meshResource)
{
    // Unload the vertex buffer
    glDeleteBuffers(1, &meshResource->_meshVertexBufferID);
    PGE::CheckError();
    // Unload the mesh
    glDeleteVertexArrays(1, &meshResource->_meshID);
    PGE::CheckError();

    delete meshResource;
    meshResource = nullptr;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

