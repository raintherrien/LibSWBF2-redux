﻿using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


using LibSWBF2.Logging;
using LibSWBF2.Wrappers;
using LibSWBF2.Types;

namespace LibSWBF2.NET.Test
{
    class InstancesTest
    {
        static int Main(string[] args)
        {
            TestBench.StartLogging(ELogType.Warning);

            Container container = TestBench.LoadAndTrackContainer(new List<string>(args), out List<Level> lvls);

            Level level = lvls[0];
            if (level == null)
            {
                TestBench.StopLogging();
                return -1;
            }
        
            World[] worlds = level.GetWorlds();

            int k = 0;
            foreach (World world in worlds)
            {
                Console.WriteLine("\n" + world.name);

                Instance[] instances = world.GetInstances(); 
                
                int i = 0;
                
                foreach (Instance instance in instances)
                {
                    var ec = container.FindWrapper<EntityClass>(instance.entityClassName);

                    if (ec == null)
                    {
                        continue;
                    }

                    string baseName = ec.GetBaseName();

                    Console.WriteLine("\tInstance " + i++ + ": ");
                    
                    string instName = instance.name;
                    Vector4 rot = instance.rotation;
                    Vector3 pos = instance.position;

                    Console.WriteLine("\t\t" + "Name: " + instName +
                                    "  Class: " + ec.name +
                                    "  Base Class: " + baseName + 
                                    "  Rotation: " + rot.ToString() +
                                    "  Position: " + pos.ToString());

                    Console.WriteLine("\t\tOverridden properties: ");
                    if (instance.GetOverriddenProperties(out uint[] props, out string[] values))
                    {
                        for (int j = 0; j < props.Length; j++)
                        {
                            Console.WriteLine("\t\t\tHash: {0}, Value: {1}", props[j], values[j]);
                        }
                    }                  
                }
            } 

            TestBench.StopLogging();
            return 1;
        }
    }
}
