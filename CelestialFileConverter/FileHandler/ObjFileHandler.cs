using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CelestialFileConverter.FileObj;

namespace CelestialFileConverter.FileHandler
{
    class ObjFileHandler
    {

        private static Vector3 crossProduct(Vector3 a, Vector3 b)
        {

            Vector3 retVect = new Vector3();

            retVect.x = a.y * b.z - a.z * b.y;
            retVect.y = a.z * b.x - a.x * b.z;
            retVect.z = a.x * b.y - a.y * b.x;

            return retVect;
        }

        private static float dotProduct(Vector3 a, Vector3 b)
        {
            
            return a.x * b.x + a.y * b.y + a.z * b.z;

        }

        private static bool sameSide(Vector3 p1, Vector3 p2, Vector3 a, Vector3 b)
        {
            Vector3 cp1 = crossProduct(b - a, p1 - a);
            Vector3 cp2 = crossProduct(b - a, p2 - a);
            return dotProduct(cp1, cp2) >= 0;
        }

        private static bool isEar(uint p1, uint p2, uint p3, Face face, ObjObject obj)
        {

            bool isEar = true;
            Vector3 a = obj.Vertex[face.Node[p1].Vertex];
            Vector3 b = obj.Vertex[face.Node[p2].Vertex];
            Vector3 c = obj.Vertex[face.Node[p3].Vertex];

            for (int i=0;i<face.Nodes && isEar;i++)
            {

                if(i != p1 && i != p2 && i != p3)
                {
                    Vector3 p = obj.Vertex[face.Node[i].Vertex];
                    isEar = !(sameSide(p, a, b, c)
                        && sameSide(p, b, a, c)
                        && sameSide(p, c, a, b));

                }
            }

            return isEar;

        }

        private static Face[] trianguliza(Face face, ObjObject obj)
        {

            Face[] retFaces = new Face[face.Nodes - 3];
            uint faces = 0;
            uint node = 0;

            while (face.Nodes > 3)
            {

                uint right = node + 1;
                right %= face.Nodes;
                uint left = node == 0 ? face.Nodes - 1 : node - 1;
                bool ear = isEar(node, left, right, face, obj);

                if (ear)
                {

                    retFaces[faces] = new Face(face.FaceId);

                    retFaces[faces].AddNode(face.Node[node]);
                    retFaces[faces].AddNode(face.Node[right]);
                    retFaces[faces].AddNode(face.Node[left]);

                    face.RemoveNode(node);
                    faces++;

                }

                node++;
                node %= face.Nodes;

            }

            return retFaces;

        }

        public static ObjObject LoadObjFromText(string[] obj)
        {

            ObjObject retObj = new ObjObject();
            uint faceId = 0;

            for(uint i=0;i<obj.Length;i++)
            {

                string[] words = obj[i].Split(' ');

                if(words[0] == "v" || words[0] == "vt" || words[0] == "vn")
                {

                    Vector3 v = new Vector3();
                    v.x = float.Parse(words[1]);
                    v.y = float.Parse(words[2]);

                    if (words[0] != "vt")
                    {

                        v.z = float.Parse(words[3]);

                        if(words[0] == "v")
                        {

                            retObj.AddVertex(v);

                        }
                        else
                        {

                            retObj.AddNormalVertex(v);

                        }
                    }
                    else
                    {

                        retObj.AddTextureVertex(v);

                    }
                }
                else if(words[0] == "f")
                {

                    Face f = new Face(faceId);
                    faceId++;

                    for(uint k=1;k<words.Length;k++)
                    {

                        FaceNode node = new FaceNode();
                        string[] faceWords = words[k].Split('/');
                        int points = words[k].Length - words[k].Replace("/","").Length;

                        if(faceWords[0] != "")
                        {


                            node.Vertex = uint.Parse(faceWords[0])-1;

                        }

                        if (faceWords[1] != "")
                        {


                            node.TextureVertex = uint.Parse(faceWords[1])-1;

                        }

                        if (faceWords[2] != "")
                        {


                            node.NormalVertex = uint.Parse(faceWords[2])-1;

                        }

                        f.AddNode(node);

                    }

                    retObj.AddFace(f);

                    if(f.Nodes > 3)
                    {

                        Face[] faces = trianguliza(f, retObj);

                        for(int k=0;k<faces.Length;k++)
                        {

                            retObj.AddFace(faces[k]);

                        }
                    }

                }
            }

            return retObj;

        }
    }
}
