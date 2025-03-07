﻿using System;
using System.Runtime.InteropServices;

namespace LibSWBF2
{
    using Enums;

    internal static class APIWrapper
    {
#if WIN32
        const string LIB_NAME = "LibSWBF2";
#else
        const string LIB_NAME = "SWBF2";
#endif

	// Hash Lookup //

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool Hashing_Lookup(uint hash, ref IntPtr str);

	// Memory //

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static unsafe extern void Memory_Blit(void *dest, void *src, int numBytes);

	// XXX Below this line

        // FileReader // 

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr FileReader_FromFile([MarshalAs(UnmanagedType.LPStr)] string path, bool UseMemoryMapping);
        
        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern void FileReader_Delete(IntPtr readerPtr);


        // Logging //

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void LogCallback(IntPtr LoggerEntryPtr);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool LOG_GetNextLog(out IntPtr msg, out Logging.ELogType level, out uint line, out IntPtr file);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern void LOG_SetLogfileLevel(Logging.ELogType LogfileLevel);


        // Container //

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr Container_Initialize();  

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern ushort Container_AddLevel(IntPtr container, [MarshalAs(UnmanagedType.LPStr)] string path);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern ushort Container_AddLevelFiltered(IntPtr container, [MarshalAs(UnmanagedType.LPStr)] string path, IntPtr[] subLVLs, uint subLVLCount);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern void Container_FreeAll(IntPtr container, [MarshalAs(UnmanagedType.U1)] bool force);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern float Container_GetProgress(IntPtr container, uint handle);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern float Container_GetOverallProgress(IntPtr container);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr Container_GetLevel(IntPtr container, uint handle);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern void Container_GetLoadedLevels(IntPtr container, out IntPtr handles, out ushort handleCount);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr Container_GetWrapper(IntPtr container, uint type, [MarshalAs(UnmanagedType.LPStr)] string name);        

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr Container_GetWrapperFNV(IntPtr container, uint type, uint name);        

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr Container_GetConfig(IntPtr container, uint type, uint nameHash);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern void Container_LoadLevels(IntPtr container);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool Container_IsDone(IntPtr container);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool Container_Delete(IntPtr container);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte Container_GetStatus(IntPtr container, uint handle);


        // AudioStreamer //

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool AudioStreamer_SetStreamAndSegment(IntPtr streamerptr, uint StreamID, uint SegmentID);
         
        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool AudioStreamer_AddStream(IntPtr streamerptr, uint StreamID);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr AudioStreamer_GetSoundStream(IntPtr streamerptr, uint StreamID);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool AudioStreamer_SetSampleSinks(IntPtr ptr, int Length, IntPtr Sink0, IntPtr Sink1, IntPtr Sink2, IntPtr Sink3);    

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern int AudioStreamer_FetchAndDecodeSamples(IntPtr ptr, int NumSamples);        

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr AudioStreamer_FromFile([MarshalAs(UnmanagedType.LPStr)] string path);
        
        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool AudioStreamer_Delete(IntPtr streamerptr);


        // Level //

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr Level_FromFile([MarshalAs(UnmanagedType.LPStr)] string path);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr Level_FromStream(IntPtr fileReaderPtr);        

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr Level_FindAndIndexSoundStream(IntPtr lvlPtr, IntPtr streamPtr, uint StreamName);


        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern void Level_Destroy(IntPtr level);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool Level_IsWorldLevel(IntPtr level);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr Level_GetName(IntPtr level);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr Level_GetPath(IntPtr level);
        

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr Level_GetConfig(IntPtr level, EConfigType cfgType, uint hash);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr Level_GetConfigs(IntPtr level, EConfigType cfgType, out int numConfigs);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr Level_GetWrapper(IntPtr level, uint type, string name);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr Level_GetWrapperFNV(IntPtr level, uint type, uint name);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr Level_GetWrappers(IntPtr level, uint type, out uint num, out uint inc);

        //Terrain

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool Terrain_FetchSimpleFields(IntPtr ter, out int numTexes, out IntPtr texNames,
                                                            out float heightUpper, out float heightLower, 
                                                            out uint numVerts, out IntPtr vBuf,
                                                            out uint numNormals, out IntPtr nBuf,
                                                            out uint numUVs, out IntPtr uvBuf);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern void Terrain_GetIndicies(IntPtr terrain, out uint numIndicies, out IntPtr indices);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern void Terrain_GetHeightMap(IntPtr terrain, out uint dim, out uint dimScale, out IntPtr heightData);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern void Terrain_GetBlendMap(IntPtr terrain, out uint edgeLength, out uint numLayers, out IntPtr data);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern void Terrain_GetIndexBuffer(IntPtr terPtr, out IntPtr buf, out uint numInds);



        // Model //

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool Model_FetchSimpleFields(IntPtr model, out IntPtr name,
                                                            [MarshalAs(UnmanagedType.U1)] out bool skinned,
                                                            [MarshalAs(UnmanagedType.U1)] out bool skelBroken,
                                                            out IntPtr segArr, out int segCount, out int segInc,
                                                            out IntPtr boneArr, out int boneCount, out int boneInc,
                                                            out IntPtr collMeshPtr);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern void Model_GetPrimitivesMasked(IntPtr NativeInstance, ECollisionMaskFlags mask,
                                                            out int numPrims, out IntPtr ptr);


        // Bone //

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]    
        public static extern void Bone_FetchAllFields(IntPtr NativeInstance, out IntPtr namePtr, out IntPtr parentNamePtr, out IntPtr loc, out IntPtr rot);


        // Segment //

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool Segment_FetchAllFields(IntPtr seg, [MarshalAs(UnmanagedType.U1)] out bool pretx, out IntPtr boneName, out IntPtr tag,
                                                        out uint numVerts, out IntPtr pBuf, out IntPtr nBuf, out IntPtr uvBuf,
                                                        out uint numVWs, out IntPtr vwBuf,
                                                        out int topo, out uint numInds, out IntPtr iBuf,
                                                        out IntPtr mat);

         // World //

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool World_FetchAllFields(IntPtr world, out IntPtr nameOut, out IntPtr skyNameOut, out IntPtr terrPtr);


        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool World_GetChildrenList(IntPtr world, byte listID, out IntPtr listPtr, out int listCount, out int wrapperSize);



        // World Animation //
        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool WorldAnim_FetchAllFields(IntPtr worldAnim, out bool loop, out bool localT, out IntPtr namePtr);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern void WorldAnim_GetAnimKeys(IntPtr worldAnim, out IntPtr keyBuff, out int numKeys, bool IsRotation);


        // World Animation Group //
        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool WorldAnimGroup_FetchAllFields(IntPtr group, out bool bool0, out bool bool1, out IntPtr namePtr);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern void WorldAnimGroup_GetAnimInstPairs(IntPtr group, out IntPtr animNames, out IntPtr instNames, out int numPairs);

        // World Animation Group //
        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool WorldAnimHier_FetchAllFields(IntPtr hier, out IntPtr rootPtr, out IntPtr children, out int numChildren);


        // Region // 

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool Region_FetchAllFields(IntPtr reg, out IntPtr size, 
                                                        out IntPtr pos, out IntPtr rot,
                                                        out IntPtr name, out IntPtr type); 

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern void Region_GetProperties(IntPtr reg, out IntPtr hashBuffer, out IntPtr valueBuffer, out int count);      
        
        // Barrier //

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]        
        public static extern IntPtr Barrier_GetFieldPtr(IntPtr bar, byte fieldID);    


        // HintNode //

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]        
        public static extern IntPtr HintNode_GetFieldPtr(IntPtr hnt, byte fieldID);  
        
        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]        
        public static extern void HintNode_GetProperties(IntPtr hnt, out IntPtr hashBuffer, out IntPtr valueBuffer, out int count);


        // Script //

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr Script_GetName(IntPtr script);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool Script_GetData(IntPtr script, out IntPtr data, out uint size);

        // Instance //

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool Instance_FetchSimpleFields(IntPtr inst, out IntPtr name, out IntPtr rot, out IntPtr pos, out IntPtr ecName, out IntPtr ec);
        
        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool Instance_GetPropertyFromName(IntPtr inst, [MarshalAs(UnmanagedType.LPStr)] string propName, out IntPtr value);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool Instance_GetPropertyFromHash(IntPtr inst, uint hashedPropName, out IntPtr value);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool Instance_GetPropertiesFromName(IntPtr inst, [MarshalAs(UnmanagedType.LPStr)]string propName, out IntPtr values, out uint count);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool Instance_GetPropertiesFromHash(IntPtr inst, uint hashedPropName, out IntPtr values, out uint count);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern void Instance_GetOverriddenProperties(IntPtr ec, out IntPtr hashBuffer, out IntPtr valueBuffer, out int count);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern void EntityClass_GetAllProperties(IntPtr ec, out IntPtr hashBuffer, out IntPtr valueBuffer, out int count);


        // Light //

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr Light_GetAllFields(IntPtr lightPtr,   out IntPtr rotPtr,
                                                    out IntPtr posPtr, out uint lightType, 
                                                    out IntPtr colPtr, out float range,
                                                    out IntPtr conePtr);


        // Texture //

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool Texture_FetchAllFields(IntPtr tex, out ushort width, out ushort height, out IntPtr bufOut, out IntPtr nameOut);      


        // CollisionMesh //

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool CollisionMesh_FetchAllFields(IntPtr cmPtr, out uint iCount, out IntPtr iBuf,
                                                                out uint vCount, out IntPtr vBuf, out ECollisionMaskFlags maskFlags,
                                                                out IntPtr namePtr, out IntPtr nodeNamePtr);


        // CollisionPrimitive //

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern void CollisionPrimitive_FetchAllFields(IntPtr nativePtr,
                                                    out float f1, out float f2, out float f3,
                                                    out IntPtr name, out IntPtr parentName,
                                                    out ECollisionMaskFlags maskFlags, out ECollisionPrimitiveType primitiveType,
                                                    out IntPtr pos, out IntPtr rot);
   

        // AnimationBank // 

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool AnimationBank_GetCurve(IntPtr setPtr, uint animCRC, uint boneCRC, uint comp, 
                                                        out IntPtr indicesBuffer, out IntPtr valuesBuffer, out int numKeys);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr AnimationBank_GetAnimationCRCs(IntPtr setPtr, out int numCRCs);
        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr AnimationBank_GetBoneCRCs(IntPtr setPtr, uint animCRC, out int numCRCs);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool AnimationBank_GetAnimationMetadata(IntPtr setPtr, uint animCRC,
                                                        out int numFrames, out int numBones);


        // AnimationSkeleton //
        
        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr AnimationSkeleton_GetName(IntPtr skelPtr);
        
        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool AnimationSkeleton_GetJoints(IntPtr skelPtr, out int numJoints, out IntPtr jointBuffer);



        // Sound //

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr Sound_GetName(IntPtr sound);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool Sound_GetData(IntPtr sound, out uint sampleRate, out uint sampleCount, out byte blockAlign, out IntPtr data);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern unsafe bool Sound_FillDataBuffer(IntPtr sound, void* buffer);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool Sound_FetchAllFields(IntPtr soundPtr, out uint format, 
                                                out int numChannels, out int sampleRate,
                                                out int numSamples, out uint alias, 
                                                out bool hasData, out uint name, out uint numBytes);



        // SoundStream //

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool SoundStream_FetchAllFields(
                IntPtr str, out uint nameOut, out bool hasDataOut,
                out uint formatOut, out uint numChannelsOut, 
                out uint numSubstreamsOut, out uint substreamInterleave);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool SoundStream_GetSound(IntPtr str, uint soundName, out IntPtr soundOut);
        
        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool SoundStream_GetSounds(IntPtr str, out IntPtr soundsOut, out uint numSounds, out uint soundInc);


        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern int SoundStream_SampleReadMethod(IntPtr str, IntPtr sBuf, int sBufLength, int numToRead, ESoundFormat format, out int numBytesRead, bool ReadSamples);
        
        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool SoundStream_SetFileReader(IntPtr str, IntPtr readerPtr);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool SoundStream_SetStreamBuffer(IntPtr str, IntPtr bufferPtr, int NumBytes);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern int SoundStream_ReadBytesFromStream(IntPtr str, int NumBytes);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern int SoundStream_GetNumSamplesInBytes(IntPtr str, int NumBytes);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool SoundStream_SetSegment(IntPtr str, uint name);        

        
        // SoundBank //

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool SoundBank_FetchAllFields(IntPtr str, out uint name, out bool hasData, out uint format);
        
        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool SoundBank_GetSound(IntPtr str, uint soundName, out IntPtr soundOut);
        
        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool SoundBank_GetSounds(IntPtr str, out IntPtr soundsOut, out uint numSounds, out uint soundInc);





        // Localization //
        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr Localization_GetName(IntPtr local);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool Localization_GetLocalizedWideString(IntPtr local, IntPtr path, out IntPtr chars, out uint charCount);


        // EntityClass //
        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern void EntityClass_FetchAllFields(IntPtr ec, out IntPtr name, out byte classType, out IntPtr baseClass, out IntPtr baseClassName);
        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool EntityClass_GetPropertyFromName(IntPtr ec, [MarshalAs(UnmanagedType.LPStr)] string propName, out IntPtr value);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool EntityClass_GetPropertyFromHash(IntPtr ec, uint hashedPropName, out IntPtr value);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool EntityClass_GetPropertiesFromName(IntPtr ec, [MarshalAs(UnmanagedType.LPStr)] string propName, out IntPtr values, out uint count);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool EntityClass_GetPropertiesFromHash(IntPtr ec, uint hashedPropName, out IntPtr values, out uint count);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern void EntityClass_GetOverriddenProperties(IntPtr ec, out IntPtr hashBuffer, out IntPtr valueBuffer, out int count);


        // Material //

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]        
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool Material_FetchAllFields(IntPtr matPtr, out IntPtr specular,
                                out IntPtr diffuse, out IntPtr texNames, out int numTexes,
                                out IntPtr attachedLightName, out uint matFlags, out uint specExp,
                                out uint param1, out uint param2);





        // Config //

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]        
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool Config_FetchSimpleFields(IntPtr cfg, out uint name);


        // Config / Scope // 

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]        
        public static extern IntPtr ConfigScope_GetFields(IntPtr ptr, uint hash, bool isScope, out uint count);


        // Field //

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]        
        [return: MarshalAs(UnmanagedType.U1)]
        public static extern bool Field_FetchAllFields(IntPtr cfg, out IntPtr scop);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern uint Field_GetNameHash(IntPtr cfg);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern byte Field_GetNumValues(IntPtr cfg);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]        
        public static extern float Field_GetFloat(IntPtr cfg, byte index);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]        
        public static extern uint Field_GetUInt32(IntPtr cfg, byte index);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]        
        public static extern IntPtr Field_GetVec2(IntPtr cfg); 

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]        
        public static extern IntPtr Field_GetVec3(IntPtr cfg); 

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]        
        public static extern IntPtr Field_GetVec4(IntPtr cfg); 

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]        
        public static extern IntPtr Field_GetString(IntPtr cfg, byte index);

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr Field_GetName(IntPtr cfg);


        // Planning //

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool PlanSet_GetChildWrappers(IntPtr ps, byte id, out IntPtr listPtr, out int listSize, out int elSize);        
         

        // Hub //

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr Hub_GetFieldPtr(IntPtr hub, byte id, out int NumBytes);        
         

        // Connection //

        [DllImport(LIB_NAME, CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr Connection_GetFieldPtr(IntPtr con, byte id); 

    }
}
