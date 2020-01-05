﻿using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using LibSWBF2.Logging;
using LibSWBF2.Wrappers;



namespace LibSWBF2.NET.Test
{
    class ModelsSegmentsTest
    {
        static int Main(string[] args)
        {            
            TestBench.StartLogging(ELogType.Warning);

            Level level = TestBench.LoadAndTrackLVL(args[0]);
            if (level == null) return -1;
            
            Model[] models = level.GetModels();


            int j = 0;
            foreach (Model model in models)
            {   
                Console.WriteLine("\n" + model.Name + ": ");

                if (model.IsSkeletalMesh || model.HasNonTrivialHierarchy)
                {
                    Console.WriteLine("\tSkeleton: ");
                    
                    Bone[] bones = model.GetSkeleton();

                    for (int k = 0; k < bones.Length; k++)
                    {
                        Console.WriteLine("\t\tName: {0} Parent: {1}", bones[k].name, bones[k].parentName);
                    }
                }

                Segment[] segments = model.GetSegments(); 
                int i = 0;
                foreach (Segment seg in segments)
                {
                    Console.WriteLine("\tSegment " + i++ + ": ");
                    float[] vBuf = seg.GetVertexBuffer();                        
                    string texName = seg.GetMaterialTexName();

                    VertexWeight[] weights = seg.GetVertexWeights();

                    Console.WriteLine("\t\tTopology: {0}", seg.GetTopology());


                    float[] buffer = seg.GetNormalsBuffer();
                    Console.WriteLine("\t\tNum positions: {0}, Num normals: {1}", vBuf.Length/3, buffer.Length/3);

                    if (model.IsSkeletalMesh)
                    {
                        Console.WriteLine("\t\t{0} weights ---- {1} vertices.", weights.Length, seg.GetVertexBufferLength());
                    }

                    if (model.HasNonTrivialHierarchy)
                    {
                        Console.WriteLine("\t\tSegment belongs to bone: {0}", seg.GetBone());
                    }

                    
                    Console.Write("\n\t\tTexture: " + texName);
                    Texture tex = level.GetTexture(texName);
                    if (tex != null)
                    {
                        Console.WriteLine(" Height: {0} Width: {1}", tex.height, tex.width);
                    }
                }
            } 

            TestBench.StopLogging();
            
            return 0;
        }
    }
}
