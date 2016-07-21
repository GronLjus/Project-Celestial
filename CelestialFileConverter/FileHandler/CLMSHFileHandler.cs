using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CelestialFileConverter.ClMesh;
using System.IO;

namespace CelestialFileConverter.FileHandler
{
    class CLMSHFileHandler
    {

        public static void ExportMesh(string path, CLMSH meshOject)
        {

            FileStream fileStream = new FileStream(path, FileMode.OpenOrCreate);
            BinaryWriter writer = new BinaryWriter(fileStream);
            writer.Write((byte)0x0C);
            writer.Write((byte)0xE1);

            writer.Write(meshOject.Dimensions.x);
            writer.Write(meshOject.Dimensions.y);
            writer.Write(meshOject.Dimensions.z);

            writer.Write(meshOject.Vertices);
            writer.Write(meshOject.Indices);

            for(int i=0;i<meshOject.Vertices;i++)
            {

                writer.Write(meshOject.VertexBuffer[i].pos.x);
                writer.Write(meshOject.VertexBuffer[i].pos.y);
                writer.Write(meshOject.VertexBuffer[i].pos.z);

                writer.Write(meshOject.VertexBuffer[i].uv.x);
                writer.Write(meshOject.VertexBuffer[i].uv.y);

                writer.Write(meshOject.VertexBuffer[i].norm.x);
                writer.Write(meshOject.VertexBuffer[i].norm.y);
                writer.Write(meshOject.VertexBuffer[i].norm.z);

            }
            
            for(uint i=0;i<meshOject.Indices;i++)
            {

                writer.Write(meshOject.IndexBuffer[i]);

            }

            writer.Close();
            fileStream.Close();

        }
    }
}
