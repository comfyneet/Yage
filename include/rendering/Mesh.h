#ifndef YAGE_RENDERING_MESH_H
#define YAGE_RENDERING_MESH_H

#include "core/Config.h"
#include "core/Math3D.h"
#include "core/NonCopyable.h"
#include "core/ReferenceCounter.h"
#include "rendering/RenderingEngine.h"

namespace ye
{
    class YE_API VertexData : public NonCopyable
    {
    public:
        VertexData() = default;

        VertexData(Vector3f const& position, Vector2f const& texCoord, Vector3f const& normal = Vector3f(),
                   Vector3f const& tangent = Vector3f(), Vector3f const& binormal = Vector3f()) :
            m_position{position},
            m_texCoord{texCoord},
            m_normal{normal},
            m_tangent{tangent},
            m_binormal{binormal}
        {
        }

        void SetPosition(Vector3f const& position) { m_position = position; }
        void SetPosition(const float x, const float y, const float z) { SetPosition(Vector3f(x, y, z)); }

        void SetTexCoord(Vector2f const& texCoord) { m_texCoord = texCoord; }
        void SetTexCoord(const float x, const float y) { SetTexCoord(Vector2f(x, y)); }

        void SetNormal(Vector3f const& normal) { m_normal = normal; }
        void SetNormal(const float x, const float y, const float z) { SetNormal(Vector3f(x, y, z)); }

        void SetTangent(Vector3f const& tangent) { m_tangent = tangent; }
        void SetTangent(const float x, const float y, const float z) { m_tangent = Vector3f(x, y, z); }

        void SetBinormal(Vector3f const& binormal) { m_binormal = binormal; }
        void SetBinormal(const float x, const float y, const float z) { SetNormal(Vector3f(x, y, z)); }

        Vector3f const& GetPosition() const { return m_position; }
        Vector2f const& GetTexCoord() const { return m_texCoord; }
        Vector3f const& GetNormal() const { return m_normal; }
        Vector3f const& GetTangent() const { return m_tangent; }
        Vector3f const& GetBinormal() const { return m_binormal; }

    private:
        Vector3f m_position;
        Vector2f m_texCoord;
        Vector3f m_normal;
        Vector3f m_tangent;
        Vector3f m_binormal;
    };

    class YE_API IndexedModel : public NonCopyable
    {
    public:
        IndexedModel() = default;

        IndexedModel(std::vector<Vector3u> const& indices, std::vector<VertexData> vertices) :
            m_indices{indices},
            m_vertices{vertices}
        {
        }

        void CalculateNormals();
        void CalculateTangents();

        void AddVertex(VertexData const& vertexData);

        void AddVertex(Vector3f const& position, Vector2f const& texCoord, Vector3f const& normal,
                       Vector3f const& tangent) { AddVertex(VertexData(position, texCoord, normal, tangent)); }

        void AddFace(Vector3u vertexIndices);

        void AddFace(const uint32_t vertexIndex0, const uint32_t vertexIndex1, const uint32_t vertexIndex2)
        {
            AddFace(Vector3u(vertexIndex0, vertexIndex1, vertexIndex2));
        }

        std::vector<Vector3u> const& GetIndices() const { return m_indices; }
        std::vector<VertexData> const& GetVertices() const { return m_vertices; }

    private:
        std::vector<Vector3u> m_indices;
        std::vector<VertexData> m_vertices;
    };

    class YE_API MeshData : public ReferenceCounter
    {
    public:
        MeshData(RenderingEngine const& renderer, IndexedModel const& model);
        virtual ~MeshData();

        void Draw(RenderingEngine const& renderer) const;

    private:
        MeshData(MeshData const& other) :
            m_vertexBuffer{},
            m_indexBuffer{},
            m_drawCount{}
        {
        }

        ID3D11Buffer* m_vertexBuffer;
        ID3D11Buffer* m_indexBuffer;
        int32_t m_drawCount;
    };

    class YE_API Mesh : public NonCopyable
    {
    public:
        explicit Mesh(RenderingEngine const& renderer, std::string const& fileName = "./resources/models/cube.obj");
        Mesh(RenderingEngine const& renderer, std::string const& meshName, IndexedModel const& model);
        Mesh(Mesh const& other);
        virtual ~Mesh();

        void Draw(RenderingEngine const& renderer) const;

    private:
        static std::map<std::string, MeshData*> s_resourceMap;

        std::string m_fileName;
        MeshData* m_meshData;
    };
}

#endif
