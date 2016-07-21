using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CelestialFileConverter.FileObj;

namespace CelestialFileConverter.ClMesh
{

    struct Vertex
    {

        public Vector3 pos;
        public Vector2 uv;
        public Vector3 norm;

    }

    class CLMSH
    {

        #region Properties
        private uint vertices;

        public uint Vertices
        {

            get { return vertices; }

        }

        private uint indices;

        public uint Indices
        {

            get { return indices; }

        }

        private Vertex[] vertexBuffer;

        public Vertex[] VertexBuffer
        {

            get { return vertexBuffer; }

        }

        private uint[] indexBuffer;

        public uint[] IndexBuffer
        {

            get { return indexBuffer; }

        }


        private Vector3 dimensions;

        public Vector3 Dimensions
        {

            get { return dimensions; }

        }
        #endregion

        public CLMSH(Vector3 dims)
        {

            indexBuffer = new uint[256];
            vertexBuffer = new Vertex[128];
            indices = 0;
            vertices = 0;
            dimensions = dims;

        }

        public void AddVertex(Vertex v)
        {

            if (vertices >= vertexBuffer.Length)
            {

                Array.Resize<Vertex>(ref vertexBuffer, vertexBuffer.Length + 128);

            }

            vertexBuffer[vertices] = v;
            vertices++;

        }

        public void AddIndex(uint i)
        {

            if (indices >= indexBuffer.Length)
            {

                Array.Resize<uint>(ref indexBuffer, indexBuffer.Length + 256);

            }

            indexBuffer[indices] = i;
            indices++;

        }
        
    }
}
