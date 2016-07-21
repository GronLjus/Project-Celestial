using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CelestialFileConverter.FileObj
{
    class FaceNode
    {
        public FaceNode()
        {

            v = 0;
            vt = 0;
            vn = 0;
        }

        private uint v;
        public uint Vertex
        {

            get { return v; }
            set { v = value; }

        }

        private uint vt;
        public uint TextureVertex
        {

            get { return vt; }
            set { vt = value; }

        }

        private uint vn;
        public uint NormalVertex
        {

            get { return vn; }
            set { vn = value; }
        }
    }

    class Face
    {

        private uint nodeAmounts;

        public uint Nodes
        {

            get { return nodeAmounts; }

        }

        private uint id;

        public uint FaceId
        {

            get { return id; }

        }

        private FaceNode[] nodes;

        public FaceNode[] Node
        {

            get{ return nodes; }

        }

        public Face(uint id)
        {

            nodes = new FaceNode[128];
            nodeAmounts = 0;
            this.id = id;

        }

        public void AddNode(FaceNode node)
        {

            if(nodeAmounts >= nodes.Length)
            {

                Array.Resize<FaceNode>(ref nodes, nodes.Length + 128);
            }

            nodes[nodeAmounts] = node;
            nodeAmounts++;

        }
        
        public void RemoveNode(uint index)
        {

            for(uint i=index+1;i<nodeAmounts;i++)
            {

                nodes[i - 1] = nodes[i];

            }

            nodeAmounts--;

        }
    }
}
