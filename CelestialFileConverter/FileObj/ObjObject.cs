using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CelestialFileConverter.FileObj
{
    struct Vector3
    {

        public float x;
        public float y;
        public float z;

        public static Vector3 operator -(Vector3 c1, Vector3 c2)
        {

            Vector3 ret = new Vector3();
            ret.x = c1.x - c2.x;
            ret.y = c1.y - c2.y;
            ret.z = c1.z - c2.z;
            return ret;

        }

        public static Vector3 operator +(Vector3 c1, Vector3 c2)
        {

            Vector3 ret = new Vector3();
            ret.x = c1.x + c2.x;
            ret.y = c1.y + c2.y;
            ret.z = c1.z + c2.z;
            return ret;

        }
    }

    struct Vector2
    {

        public float x;
        public float y;

    }

    class ObjObject
    {
        
        #region properties

        private uint vAmounts = 0;
        public uint Vertices
        {

            get { return vAmounts; }

        }

        private uint vtAmounts = 0;
        public uint TextureVertices
        {

            get { return vtAmounts;}

        }

        private uint vnAmounts = 0;
        public uint NormalVertices
        {

            get { return vnAmounts; }

        }

        private uint faces = 0;
        public uint Faces
        {

            get { return faces; }

        }
        
        private Vector3[] positionVertices;
        public Vector3[] Vertex
        {

            get { return positionVertices; }

        }

        private Vector2[] textureVertices;
        public Vector2[] TextureVertex
        {

            get { return textureVertices; }

        }

        private Vector3[] normalVertices;
        public Vector3[] NormalVertex
        {

            get { return normalVertices; }

        }

        private Face[] objFaces;
        public Face[] Face
        {

            get { return objFaces; }

        }
        #endregion


        public ObjObject()
        {

            positionVertices = new Vector3[128];
            textureVertices = new Vector2[128];
            normalVertices = new Vector3[128];

            objFaces = new Face[128];

        }

        public void AddVertex(Vector3 v)
        {

            if(vAmounts >= positionVertices.Length)
            {

                Array.Resize<Vector3>(ref positionVertices, positionVertices.Length + 128);

            }

            positionVertices[vAmounts] = v;
            vAmounts++;

        }

        public void AddTextureVertex(Vector2 v)
        {

            if (vtAmounts >= textureVertices.Length)
            {

                Array.Resize<Vector2>(ref textureVertices, textureVertices.Length + 128);

            }

            textureVertices[vtAmounts] = v;
            vtAmounts++;

        }

        public void AddTextureVertex(Vector3 v)
        {

            if (vtAmounts >= textureVertices.Length)
            {

                Array.Resize<Vector2>(ref textureVertices, textureVertices.Length + 128);

            }

            Vector2 v2;
            v2.x = v.x;
            v2.y = v.y;
            textureVertices[vtAmounts] = v2;
            vtAmounts++;

        }

        public void AddNormalVertex(Vector3 v)
        {

            if (vnAmounts >= normalVertices.Length)
            {

                Array.Resize<Vector3>(ref normalVertices, normalVertices.Length + 128);

            }

            normalVertices[vnAmounts] = v;
            vnAmounts++;

        }

        public void AddFace(Face f)
        {

            if (faces >= objFaces.Length)
            {

                Array.Resize<Face>(ref objFaces, objFaces.Length + 128);

            }

            objFaces[faces] = f;
            faces++;

        }
    }
}
