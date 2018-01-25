#include "rendering/Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace
{
}

namespace ye
{
    std::map<std::string, MeshData*> Mesh::s_resourceMap{};

    // TODO:
    void IndexedModel::CalculateNormals()
    {
        for (auto& vertice : m_vertices)
        {
            vertice.SetNormal(Vector3f(0, 0, 0));
        }

        for (uint32_t i = 0; i < m_indices.size(); ++i)
        {
            const int32_t i0 = m_indices[i][0];
            const int32_t i1 = m_indices[i][1];
            const int32_t i2 = m_indices[i][2];

            Vector3f v1(m_vertices[i1].GetPosition() - m_vertices[i0].GetPosition());
            Vector3f v2(m_vertices[i2].GetPosition() - m_vertices[i0].GetPosition());

            Vector3f normal(v1.Cross(v2).Normalized());

            m_vertices[i0].SetNormal(m_vertices[i0].GetNormal() + normal);
            m_vertices[i1].SetNormal(m_vertices[i1].GetNormal() + normal);
            m_vertices[i2].SetNormal(m_vertices[i2].GetNormal() + normal);
        }

        for (uint32_t i = 0; i < m_vertices.size(); ++i)
        {
            m_vertices[i].SetNormal(m_vertices[i].GetNormal().Normalized());
        }
    }

    void IndexedModel::CalculateTangents()
    {
        for (auto& vertice : m_vertices)
        {
            vertice.SetTangent(Vector3f(0, 0, 0));
        }

        for (auto& indice : m_indices)
        {
            const auto i0 = indice[0];
            const auto i1 = indice[1];
            const auto i2 = indice[2];

            Vector3f edge1(m_vertices[i1].GetPosition() - m_vertices[i0].GetPosition());
            Vector3f edge2(m_vertices[i2].GetPosition() - m_vertices[i0].GetPosition());

            const auto deltaU1 = m_vertices[i1].GetTexCoord().GetX() - m_vertices[i0].GetTexCoord().GetX();
            const auto deltaU2 = m_vertices[i2].GetTexCoord().GetX() - m_vertices[i0].GetTexCoord().GetX();
            const auto deltaV1 = m_vertices[i1].GetTexCoord().GetY() - m_vertices[i0].GetTexCoord().GetY();
            const auto deltaV2 = m_vertices[i2].GetTexCoord().GetY() - m_vertices[i0].GetTexCoord().GetY();

            const auto dividend = (deltaU1 * deltaV2 - deltaU2 * deltaV1);
            const auto f = dividend == 0.0f ? 0.0f : 1.0f / dividend;

            auto tangent = Vector3f(0, 0, 0);

            tangent.SetX(f * (deltaV2 * edge1.GetX() - deltaV1 * edge2.GetX()));
            tangent.SetY(f * (deltaV2 * edge1.GetY() - deltaV1 * edge2.GetY()));
            tangent.SetZ(f * (deltaV2 * edge1.GetZ() - deltaV1 * edge2.GetZ()));

            m_vertices[i0].SetTangent(m_vertices[i0].GetTangent() + tangent);
            m_vertices[i1].SetTangent(m_vertices[i1].GetTangent() + tangent);
            m_vertices[i2].SetTangent(m_vertices[i2].GetTangent() + tangent);
        }

        for (uint32_t i = 0; i < m_indices.size(); ++i)
        {
            m_vertices[i].SetTangent(m_vertices[i].GetTangent().Normalized());
        }
    }

    void IndexedModel::AddVertex(VertexData const& vertexData)
    {
        m_vertices.push_back(vertexData);
    }

    void IndexedModel::AddFace(Vector3u const vertexIndices)
    {
        m_indices.push_back(vertexIndices);
    }

    MeshData::MeshData(RenderingEngine const& renderer, IndexedModel const& model) :
        m_vertexBuffer{},
        m_indexBuffer{},
        m_drawCount{}
    {
        m_drawCount = model.GetIndices().size();

        D3D11_BUFFER_DESC vertexBufferDesc{};
        vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        vertexBufferDesc.ByteWidth = sizeof(VertexData) * model.GetVertices().size();
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vertexBufferDesc.CPUAccessFlags = 0;
        vertexBufferDesc.MiscFlags = 0;
        vertexBufferDesc.StructureByteStride = 0;

        D3D11_SUBRESOURCE_DATA vertexData{};
        vertexData.pSysMem = model.GetVertices().data();
        vertexData.SysMemPitch = 0;
        vertexData.SysMemSlicePitch = 0;

        HRESULT hResult = renderer.GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
        assert(SUCCEEDED(hResult));

        D3D11_BUFFER_DESC indexBufferDesc{};
        indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        indexBufferDesc.ByteWidth = sizeof(Vector3u) * model.GetIndices().size();
        indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        indexBufferDesc.CPUAccessFlags = 0;
        indexBufferDesc.MiscFlags = 0;
        indexBufferDesc.StructureByteStride = 0;

        D3D11_SUBRESOURCE_DATA indexData{};
        indexData.pSysMem = model.GetIndices().data();
        indexData.SysMemPitch = 0;
        indexData.SysMemSlicePitch = 0;

        hResult = renderer.GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
        assert(SUCCEEDED(hResult));
    }

    MeshData::~MeshData()
    {
        if (m_vertexBuffer) m_vertexBuffer->Release();
        if (m_indexBuffer) m_indexBuffer->Release();
    }

    void MeshData::Draw(RenderingEngine const& renderer) const
    {
        uint32_t stride = sizeof(VertexData);
        uint32_t offset = 0;

        renderer.GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

        renderer.GetDeviceContext()->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

        renderer.GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        renderer.GetDeviceContext()->DrawIndexed(m_drawCount, 0, 0);
    }

    Mesh::Mesh(RenderingEngine const& renderer, std::string const& fileName) :
        m_fileName{fileName},
        m_meshData{}
    {
        std::map<std::string, MeshData*>::const_iterator it = s_resourceMap.find(fileName);
        if (it != s_resourceMap.end())
        {
            m_meshData = it->second;
            m_meshData->AddReference();
        }
        else
        {
            Assimp::Importer importer{};

            auto const scene = importer.ReadFile(m_fileName.c_str(),
                                                 aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                                                 aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
            assert(scene);

            auto model = scene->mMeshes[0];

            std::vector<VertexData> vertexData{};
            std::vector<Vector3u> indices{};

            for (uint32_t i = 0; i < model->mNumVertices; ++i)
            {
                aiVector3D const& position = model->mVertices[i];
                aiVector3D const& texCoord = model->HasTextureCoords(0)
                                                 ? model->mTextureCoords[0][i]
                                                 : aiVector3D(0.0f, 0.0f, 0.0f);
                aiVector3D const& normal = model->mNormals[i];
                aiVector3D const& tangent = model->mTangents[i];
                aiVector3D const& binormal = model->mBitangents[i];

                vertexData.emplace_back(Vector3f(position.x, position.y, position.z),
                                        Vector2f(texCoord.x, texCoord.y),
                                        Vector3f(normal.x, normal.y, normal.z),
                                        Vector3f(tangent.x, tangent.y, tangent.z),
                                        Vector3f(binormal.x, binormal.y, binormal.z));
            }

            // TODO:
            for (uint32_t i = 0; i < model->mNumFaces; ++i)
            {
                aiFace const& face = model->mFaces[i];
                assert(face.mNumIndices == 3);

                indices.emplace_back(Vector3u(face.mIndices[0], face.mIndices[1], face.mIndices[2]));
            }

            m_meshData = new MeshData(renderer, IndexedModel(indices, vertexData));
            s_resourceMap.insert(std::pair<std::string, MeshData*>(fileName, m_meshData));
        }
    }

    Mesh::Mesh(RenderingEngine const& renderer, std::string const& meshName, IndexedModel const& model) :
        m_fileName{meshName},
        m_meshData{}
    {
        const std::map<std::string, MeshData*>::const_iterator it = s_resourceMap.find(meshName);
        const auto result = it != s_resourceMap.end();
        assert(result);

        m_meshData = new MeshData(renderer, model);
        s_resourceMap.insert(std::pair<std::string, MeshData*>(meshName, m_meshData));
    }

    Mesh::Mesh(Mesh const& other) :
        m_fileName{other.m_fileName},
        m_meshData{other.m_meshData}
    {
        m_meshData->AddReference();
    }

    Mesh::~Mesh()
    {
        if (m_meshData && m_meshData->RemoveReference())
        {
            if (m_fileName.length() > 0)
            {
                s_resourceMap.erase(m_fileName);
            }

            delete m_meshData;
        }
    }

    void Mesh::Draw(RenderingEngine const& renderer) const
    {
        m_meshData->Draw(renderer);
    }
}
