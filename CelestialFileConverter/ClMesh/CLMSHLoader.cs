using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CelestialFileConverter.FileObj;

namespace CelestialFileConverter.ClMesh
{
    class CLMSHLoader
    {

        private struct tempVertex
        {
            public tempVertex(Vector3 position, Vector3 normal, Vector2 uv, uint id, uint faceId)
            {

                this.id = id;
                this.position = position;
                this.normal = normal;
                this.uv = uv;
                maxFaces = 6;
                maxNormals = 6;
                faces = new uint[maxFaces];

                faceNormals = new uint[maxNormals];
                faceNormals[0] = faceId;
                nrFaces = 0;
                normals = 1;

            }

            public void AddFace(Vector3 normal, uint id, uint faceId)
            {

                if(nrFaces >= maxFaces)
                {

                    maxFaces += 6;
                    Array.Resize<uint>(ref faces, (int)maxFaces);

                }

                faces[nrFaces] = id;

                bool exists = false;

                for (uint i = 0; i < normals && !exists; i++)
                {

                    exists = faceNormals[i] == faceId;

                }

                if (!exists)
                {
                    
                    this.normal = this.normal + normal;

                    if(normals >= maxNormals)
                    {

                        maxNormals += 6;
                        Array.Resize<uint>(ref faceNormals, (int)maxNormals);

                    }

                    faceNormals[normals] = faceId;
                    normals++;

                }

                nrFaces++;

            }

            public uint id;
            public Vector3 position;
            public Vector2 uv;
            public Vector3 normal;
            public uint[] faces;
            public uint nrFaces;

            private uint maxFaces;
            private uint maxNormals;
            private uint normals;
            private uint[] faceNormals;
        }

        public static CLMSH ConvertOBJ(ObjObject obj)
        {

            tempVertex[] newVertices = new tempVertex[obj.Vertices];
            bool[] inited = new bool[obj.Vertices];
            uint indices = 256;
            uint[] indexBuffer = new uint[256];
            uint index = 0;

            Vector3 maxData = obj.Vertex[0];
            Vector3 minData = obj.Vertex[0];

            for(uint i=0;i<obj.Vertices;i++)
            {

                inited[i] = false;

                if(maxData.x < obj.Vertex[i].x)
                {

                    maxData.x = obj.Vertex[i].x;

                }

                if (maxData.y < obj.Vertex[i].y)
                {

                    maxData.y = obj.Vertex[i].y;

                }

                if (maxData.z < obj.Vertex[i].z)
                {

                    maxData.z = obj.Vertex[i].z;

                }

                if (minData.x > obj.Vertex[i].x)
                {

                    minData.x = obj.Vertex[i].x;

                }

                if (minData.y > obj.Vertex[i].y)
                {

                    minData.y = obj.Vertex[i].y;

                }

                if (minData.z > obj.Vertex[i].z)
                {

                    minData.z = obj.Vertex[i].z;

                }
            }

            Vector3 dims = new Vector3();
            dims.x = maxData.x - minData.x;
            dims.y = maxData.y - minData.y;
            dims.z = maxData.z - minData.z;

            for (uint i=0;i<obj.Faces;i++)
            {

                uint faceId = index / 3;
                Face faceHere = obj.Face[i];

                for(uint k=0;k<faceHere.Nodes;k++)
                {

                    FaceNode node = faceHere.Node[k];

                    if(!inited[node.Vertex])
                    {

                        inited[node.Vertex] = true;
                        newVertices[node.Vertex] = new tempVertex(obj.Vertex[node.Vertex], obj.NormalVertex[node.NormalVertex], obj.TextureVertex[node.TextureVertex], faceId, faceHere.FaceId);
                        newVertices[node.Vertex].position.x /= dims.x;
                        newVertices[node.Vertex].position.y /= dims.y;
                        newVertices[node.Vertex].position.z /= dims.z;

                    }

                    newVertices[node.Vertex].AddFace(obj.NormalVertex[node.NormalVertex], faceId, faceHere.FaceId);

                    if(index >= indices)
                    {

                        indices += 256;
                        Array.Resize<uint>(ref indexBuffer, (int)indices);

                    }

                    indexBuffer[index] = node.Vertex;
                    index++;

                }
            }

            CLMSH retVal = new CLMSH(dims);

            for(uint i=0;i<obj.Vertices;i++)
            {

                float normalLength =(float) Math.Sqrt(
                    Math.Pow(newVertices[i].normal.x , 2)+
                    Math.Pow(newVertices[i].normal.y, 2) +
                    Math.Pow(newVertices[i].normal.z, 2));

                newVertices[i].normal.x /= normalLength;
                newVertices[i].normal.y /= normalLength;
                newVertices[i].normal.z /= normalLength;

                Vertex v = new Vertex();
                v.pos = newVertices[i].position;
                v.uv = newVertices[i].uv;
                v.norm = newVertices[i].normal;
                retVal.AddVertex(v);

            }

            for(uint i=0;i<index;i++)
            {

                tempVertex tv = newVertices[indexBuffer[i]];
                uint faceHere = i / 3;
                uint next = faceHere * 3 + ((i + 1) % 3);
                retVal.AddIndex(indexBuffer[i]);
                bool foundNeighbour = false;
                uint nIndex = 0;

                for(uint k=0;k<tv.nrFaces && !foundNeighbour; k++)
                {

                    uint faceStart = tv.faces[k] * 3;

                    for(uint j=0;j<3 && !foundNeighbour;j++)
                    {

                        foundNeighbour = indexBuffer[faceStart + j] == indexBuffer[next] &&
                            tv.faces[k] != faceHere;

                        if(foundNeighbour)
                        {

                            for(int h=0;h<3;h++)
                            {

                                if(indexBuffer[faceStart+h] != indexBuffer[i] &&
                                    indexBuffer[faceStart+h] != indexBuffer[next])
                                {

                                    nIndex = indexBuffer[faceStart + h];

                                }
                            }
                        }
                    }
                }

                if (!foundNeighbour)
                {

                    nIndex = indexBuffer[i];

                }

                retVal.AddIndex(nIndex);

            }

            return retVal;

        }
    }
}
