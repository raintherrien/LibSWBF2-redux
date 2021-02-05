﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

using LibSWBF2.Utils;
using LibSWBF2.Logging;
using LibSWBF2.Types;



namespace LibSWBF2.Wrappers
{
    public class Instance : NativeWrapper
    {
        public Instance(IntPtr instancePtr) : base(instancePtr){ SetPtr(instancePtr); }
        public Instance() : base(IntPtr.Zero) {}

        public string name = "";
        public Vector4 rotation;
        public Vector3 position;
        public string entityClassName = "";



        internal override void SetPtr(IntPtr instancePtr)
        {
            bool status = APIWrapper.Instance_FetchSimpleFields(instancePtr, out IntPtr namePtr, out IntPtr rot, out IntPtr pos, out IntPtr ecNamePtr);

            if (!status)
            {
                NativeInstance = IntPtr.Zero;
            }
            else 
            {
                NativeInstance = instancePtr;
                name = Marshal.PtrToStringAnsi(namePtr);
                entityClassName = Marshal.PtrToStringAnsi(ecNamePtr);
                rotation = new Vector4(rot);
                position = new Vector3(pos);
            }
        }


        public bool GetOverriddenProperties(out uint[] properties, out string[] values)
        {
            bool status = APIWrapper.Instance_GetOverriddenProperties(NativeInstance, out IntPtr props, out IntPtr vals, out int count);
            count = status ? count : 0;
            properties = MemUtils.IntPtrToArray<uint>(props, count);
            values = MemUtils.IntPtrToStringList(vals, count).ToArray();

            return status;
        }
    }
}
