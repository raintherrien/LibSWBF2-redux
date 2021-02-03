#include "stdafx.h"
#include "API.h"
#include "InternalHelpers.h"
#include "Types/LibString.h"
#include "Chunks/MSH/MSH.h"

#include <string.h>
#include "Wrappers/Level.h"
#include "Container.h"
#include "Wrappers/Terrain.h"
#include "Wrappers/Segment.h"
#include "Wrappers/Config.h"

#include "Chunks/HeaderNames.h"


namespace LibSWBF2
{
#define CheckPtr(obj, ret) if (obj == nullptr) { LOG_ERROR("[API] Given Pointer was NULL!"); return ret; }

	// Memory //
	void Memory_Blit(void *dest, void *src, int numBytes)
	{
		memcpy(dest, src, numBytes);
	}

	// Logging //
	uint8_t LOG_GetNextLog(const char*& msg, ELogType& level, uint32_t& line, const char*& file)
	{
		static Logging::LoggerEntry current;
		bool hasLogEntry = Logging::Logger::GetNextLog(current);
		msg = current.m_Message.Buffer();
		level = current.m_Level;
		line = current.m_Line;
		file = current.m_File.Buffer();
		return hasLogEntry;
	}

	void LOG_SetLogfileLevel(ELogType LogfileLevel)
	{
		Logging::Logger::SetLogfileLevel(LogfileLevel);
	}


	// MSH //
	Chunks::MSH::MSH* MSH_Create()
	{
		return Chunks::MSH::MSH::Create();
	}

	uint8_t MSH_Delete(Chunks::MSH::MSH* msh)
	{
		CheckPtr(msh, false)
		LibSWBF2::Chunks::MSH::MSH::Destroy(msh);
		return true;
	}

	uint8_t MSH_ReadFromFile(Chunks::MSH::MSH* msh, const char* path)
	{
		CheckPtr(msh, false)
		return msh->ReadFromFile(path);
	}

	uint8_t MSH_WriteToFile(Chunks::MSH::MSH* msh, const char* path)
	{
		CheckPtr(msh, false)
		return msh->WriteToFile(path);
	}

	void STRP_CalcPolygons(Chunks::MSH::STRP* strp)
	{
		CheckPtr(strp,)
		strp->CalcPolygons();
	}

	EModelPurpose MODL_GetPurpose(Chunks::MSH::MODL* modl)
	{
		CheckPtr(modl, EModelPurpose::Miscellaneous)
		return modl->GetPurpose();
	}

	const Container* Container_Initialize()
	{
		return Container::Create();
	}

	uint32_t Container_AddLevel(Container* container, const char *path)
	{
		return (uint32_t) container -> AddLevel(path);
	}

	float_t Container_GetProgress(Container* container, uint32_t handleNum)
	{
		CheckPtr(container, -1.0f)
		return container -> GetLevelProgress((uint16_t) handleNum);
	}  

	const bool Container_IsDone(Container* container)
	{
		CheckPtr(container, false);
		return container -> IsDone();
	}

	const Level* Container_GetLevel(Container* container, uint32_t handleNum)
	{
		CheckPtr(container,nullptr);
		return container -> GetLevel((uint16_t) handleNum);
	}

	const void Container_LoadLevels(Container* container)
	{
		CheckPtr(container,);
		container -> StartLoading();
	}

	/*
	wrapperMap[typeof(Light)]         = 0;
    wrapperMap[typeof(Model)]         = 1;
    wrapperMap[typeof(Texture)]       = 2;
    wrapperMap[typeof(World)]         = 3;
    wrapperMap[typeof(EntityClass)]   = 4;
    wrapperMap[typeof(AnimationBank)] = 5;
	*/

	const void* Container_GetWrapper(Container* container, uint32_t type, const char *name)
	{
		switch (type)
		{
			case 0:
				//return static_cast<const void *>(container -> FindLight(name));
			case 1:
				return static_cast<const void *>(container -> FindModel(name));
			case 2:
				return static_cast<const void *>(container -> FindTexture(name));
			case 3:
				return static_cast<const void *>(container -> FindWorld(name));
			case 4:
				return static_cast<const void *>(container -> FindEntityClass(name));
			case 5:
				return static_cast<const void *>(container -> FindAnimationBank(name));
			default:
				return nullptr;
		}
	}


	const bool Container_Delete(Container* container)
	{
		CheckPtr(container,false);

		Container::Delete(container);
		return true;
	}



	// Wrappers
	Level* Level_FromFile(const char* path)
	{
		CheckPtr(path, nullptr);
		return Level::FromFile(path);
	}

	void Level_Destroy(Level* level)
	{
		CheckPtr(level,);
		Level::Destroy(level);
	}

	uint8_t Level_IsWorldLevel(const Level* level)
	{
		CheckPtr(level, false);
		return level->IsWorldLevel();
	}

	const EntityClass* Level_GetEntityClass(const Level* level, const char* name)
	{
		CheckPtr(level, nullptr);
		return level->GetEntityClass(name);		
	}


	const Model* Level_GetModel(const Level* level, const char* modelName)
	{
		CheckPtr(level, nullptr);
		return level->GetModel(modelName);
	}


	void Level_GetModels(const Level* level, const void*& modelArr, uint32_t& modelCount, int32_t& inc)
	{
		const List<Model>& models = level->GetModels();
		modelArr = (void *) models.GetArrayPtr();
		modelCount = (uint32_t) models.Size();
		inc = sizeof(Model);
	}


	void Level_GetEntityClasses(const Level* level, const void*& classArr, int32_t& classCount, int32_t& inc)
	{
		const List<EntityClass>& classes = level->GetEntityClasses();
		classArr = (void *) classes.GetArrayPtr();
		classCount = (int32_t) classes.Size();
		inc = sizeof(EntityClass);
	}


	const Texture* Level_GetTexture(const Level* level, const char* texName)
	{
		CheckPtr(level, nullptr);
		return level->GetTexture(texName);
	}



	void Level_GetTerrains(const Level* level, const Terrain**& terrainArr, uint32_t& terrainCount)
	{
		CheckPtr(level, );
		const List<Terrain>& terrains = level->GetTerrains();

		static List<const Terrain*> terrainPtrs;
		terrainPtrs.Clear();

		for (size_t i = 0; i < terrains.Size(); ++i)
		{
			terrainPtrs.Add(&terrains[i]);
		}

		terrainArr = terrainPtrs.GetArrayPtr();
		terrainCount = (uint32_t)terrainPtrs.Size();
	}


	void Level_GetWorlds(const Level* level, const World**& worldArr, uint32_t& worldCount)
	{
		CheckPtr(level, );
		const List<World>& worlds = level->GetWorlds();

		// since level->GetModels() just returns a reference to the actual list
		// member of level, which will persist even after this call ended, we can safely
		// provide the model addresses of the underlying buffer to the inquirer.
		// The inquirer of course is not allowed to alter the data!
		static List<const World*> worldPtrs;
		worldPtrs.Clear();

		for (size_t i = 0; i < worlds.Size(); ++i)
		{
			worldPtrs.Add(&worlds[i]);
		}

		worldArr = worldPtrs.GetArrayPtr();
		worldCount = (uint32_t) worldPtrs.Size();
	}


	/*
	void Level_GetLights(const Level* level, const Light**& LightArr, uint32_t& LightCount)
	{
		CheckPtr(level, );
		const List<Light>& Lights = level->GetLights();

		// since level->GetModels() just returns a reference to the actual list
		// member of level, which will persist even after this call ended, we can safely
		// provide the model addresses of the underlying buffer to the inquirer.
		// The inquirer of course is not allowed to alter the data!
		static List<const Light*> LightPtrs;
		LightPtrs.Clear();

		for (size_t i = 0; i < Lights.Size(); ++i)
		{
			LightPtrs.Add(&Lights[i]);
		}

		LightArr = LightPtrs.GetArrayPtr();
		LightCount = (uint32_t) LightPtrs.Size();
	}



	const Light* Level_GetLight(const Level* level, const char* lightName)
	{
		CheckPtr(level, nullptr);
		return level->GetLight(lightName);
	}
	

	
	bool Level_GetGlobalLighting(const Level* level, Vector3 *& topColor, Vector3 *& bottomColor, 
								const char*& light1Name, const char*& light2Name)
	{
		const auto* config = level -> GetGlobalLighting();
		static Vector3 topCol, bottomCol;
		static String name1, name2;

		if (config != nullptr)
		{	
			topColor    = config -> GetTopColor(topCol) ? &topCol : nullptr;
			bottomColor = config -> GetBottomColor(bottomCol) ? &bottomCol : nullptr;

			light1Name  = config -> GetLight1(name1) ? name1.Buffer() : "";
			light2Name  = config -> GetLight2(name2) ? name2.Buffer() : "";

			return true;
		}

		return false;
	}
	*/


	const AnimationBank* Level_GetAnimationBank(const Level* level, const char* setName)
	{
		CheckPtr(level, nullptr);
		return level -> GetAnimationBank(setName);
	}


	char *  Level_GetName(const Level* level)
	{
		static String cache;
		CheckPtr(level, nullptr);
		cache = level -> GetLevelName();
		return const_cast<char *>(cache.Buffer());
	}



	// Texture

	const bool Texture_GetMetadata(const Texture* tex, int32_t& width, int32_t& height, const char *name)
	{
		static String texName;
		CheckPtr(tex, false);

		texName = tex -> GetName();

		uint16_t w, h;
		ETextureFormat fmt;
		bool status = tex -> GetImageMetadata(w, h, fmt);

		width = (int32_t) w;
		height = (int32_t) h;

		name = texName.Buffer();

		return status;
	}

	const bool Texture_GetData(const Texture* tex, int32_t& width, int32_t& height, const uint8_t*& buffer)
	{
		static const uint8_t* imageData = nullptr;
    	delete imageData;
    	imageData = nullptr;

		if (tex == nullptr)
		{
			return false;
		}

		uint16_t w,h;
	    bool conversionStatus = tex -> GetImageData(ETextureFormat::R8_G8_B8_A8, 0, w, h, imageData);

	    width = (int32_t) w;
	    height = (int32_t) h;
    	
    	if (conversionStatus)
    		buffer = imageData;

    	return conversionStatus;
	}


	const uint8_t Texture_GetBytesRGBA(const Texture* tex, const uint8_t*& buffer)
	{
	   	static const uint8_t* imageData = nullptr;
    	delete imageData;
    	imageData = nullptr;

		CheckPtr(tex,false);

		uint16_t w,h;
	    bool conversionStatus = tex -> GetImageData(ETextureFormat::R8_G8_B8_A8, 0, w, h, imageData);
    	
    	if (conversionStatus)
    		buffer = imageData;

    	return conversionStatus;
	}


	const Config* Level_GetConfig(const Level* level, uint32_t header, uint32_t hash)
	{
		const Config *ptr = nullptr;

		switch (header)
		{
			case "lght"_m:
				ptr = level -> GetConfig<"lght"_m>(hash);
				break;
			case "fx__"_m:
				ptr = level -> GetConfig<"fx__"_m>(hash);
				break;
			case "sky_"_m:
				ptr = level -> GetConfig<"sky_"_m>(hash);
				break;
			case "bnd_"_m:
				ptr = level -> GetConfig<"bnd_"_m>(hash);
				break;
			case "prp_"_m:
				ptr = level -> GetConfig<"prp_"_m>(hash);
				break;
			case "path"_m:
				ptr = level -> GetConfig<"path"_m>(hash);
				break;
			case "comb"_m:
				ptr = level -> GetConfig<"comb"_m>(hash);
				break;
			default:
				break;
		}

		return ptr;
	}

	Config** Level_GetConfigs(const Level* level, uint32_t header, int32_t& numConfigs)
	{
		static List<Config *> configs;
		switch (header)
		{
			case "lght"_m:
				configs = level -> GetConfigs<"lght"_m>();
				break;
			case "fx__"_m:
				configs = level -> GetConfigs<"fx__"_m>();
				break;
			case "sky_"_m:
				configs = level -> GetConfigs<"sky_"_m>();
				break;
			case "bnd_"_m:
				configs = level -> GetConfigs<"bnd_"_m>();
				break;
			case "prp_"_m:
				configs = level -> GetConfigs<"prp_"_m>();
				break;
			case "path"_m:
				configs = level -> GetConfigs<"path"_m>();
				break;
			case "comb"_m:
				configs = level -> GetConfigs<"comb"_m>();
				break;
			default:
				break;
		}

		numConfigs = configs.Size();
		return configs.GetArrayPtr();
	}

	const char* ENUM_TopologyToString(ETopology topology)
	{
		static Types::String lastToString;
		lastToString = TopologyToString(topology);
		return lastToString.Buffer();
	}


	/*
		Terrain
	*/
	
	//Will eventually return pointers to Texture wrappers...
	const void Terrain_GetTexNames(const Terrain *tern, uint32_t& numTextures, const char**& namesOut)
	{
		static const char** nameStrings = nullptr;
		delete nameStrings; 

		numTextures = 0;
		CheckPtr(tern, );

        const List<String>& texNames = tern -> GetLayerTextures();
        numTextures = (uint32_t) texNames.Size();

    	nameStrings = new const char *[numTextures]; 
    	for (uint32_t i = 0; i < numTextures; i++)
        {
        	nameStrings[i] = texNames[i].Buffer(); 
        }

        namesOut = nameStrings;
	}


    const void Terrain_GetHeightMap(const Terrain *ter, uint32_t& dim, uint32_t& dimScale, float_t*& heightData)
    {
    	dim = 0;
    	CheckPtr(ter, );
    	ter -> GetHeightMap(dim, dimScale, heightData);
    }


	const void Terrain_GetBlendMap(const Terrain *ter, uint32_t& dim, uint32_t& numLayers, uint8_t*& data)
	{	
		dim = 0;
    	CheckPtr(ter, );
		ter -> GetBlendMap(dim, numLayers, data);
	}


	const void Terrain_GetHeightBounds(const Terrain *ter, float& floor, float& ceiling)
	{
    	CheckPtr(ter, );
		ter -> GetHeightBounds(floor, ceiling);
	}


	const void Terrain_GetVertexBuffer(const Terrain *terr, float_t*& positions, int32_t& numVerts)
	{
		static float_t *rawBuffer = nullptr;

		numVerts = 0;
		CheckPtr(terr,)

		delete rawBuffer;

		Vector3 *positionsBuf;
		uint32_t numBufVerts;

		terr -> GetVertexBuffer(numBufVerts, positionsBuf);

		rawBuffer = new float_t[numBufVerts * 3];

		numVerts = (int32_t) numBufVerts;

		for (int32_t i = 0; i < numVerts * 3; i+=3)
		{
			Vector3& cur = positionsBuf[i / 3];
			rawBuffer[i] = cur.m_X;
			rawBuffer[i+1] = cur.m_Y;
			rawBuffer[i+2] = cur.m_Z;
		}

		positions = rawBuffer;
	}


	const void Terrain_GetNormalsBuffer(const Terrain *ter, float_t*& normals, int32_t& numNormals)
	{
		static float_t *rawBuffer = nullptr;

		numNormals = 0;
		CheckPtr(ter,)

		delete rawBuffer;

		Vector3 *normalsBuf;
		uint32_t numBufVerts;

		ter -> GetNormalBuffer(numBufVerts, normalsBuf);

		rawBuffer = new float_t[numBufVerts * 3];

		numNormals = (int32_t) numBufVerts;

		for (int32_t i = 0; i < numNormals * 3; i+=3)
		{
			Vector3& cur = normalsBuf[i / 3];
			rawBuffer[i] = cur.m_X;
			rawBuffer[i+1] = cur.m_Y;
			rawBuffer[i+2] = cur.m_Z;
		}

		normals = rawBuffer;
	}

	

	const void Terrain_GetIndexBuffer(const Terrain *terr, uint32_t*& indicies, int32_t& numIndsOut)
	{
		numIndsOut = 0;
		CheckPtr(terr,)

		indicies = nullptr;
		uint32_t numInds = 0;

		terr -> GetIndexBuffer(ETopology::TriangleList, numInds, indicies);
		numIndsOut = numInds;
	}


	const char* Model_GetName(const Model* model)
	{
		static String nameString;
		CheckPtr(model, "");

		// model->GetName() returns a ref to the persistent member,
		// char buffers of String's are always null terminated, so we
		// can just return the buffer pointer.
		//const String& name = model->GetName();

		nameString = model -> GetName();
		return nameString.Buffer();
	}


	const void Model_GetSegments(const Model* model, const Segment**& segmentArr, uint32_t& segmentCount)
	{	
		CheckPtr(model, );
		const List<Segment>& segments = model->GetSegments();

		static List<const Segment*> segmentPtrs;
		segmentPtrs.Clear();

		for (size_t i = 0; i < segments.Size(); ++i)
		{
			segmentPtrs.Add(&segments[i]);
		}

		segmentArr = segmentPtrs.GetArrayPtr();
		segmentCount = (uint32_t)segmentPtrs.Size();
	}


	uint8_t Model_IsSkinnedMesh(const Model* model)
	{
		CheckPtr(model, false);
		return model->IsSkeletalMesh();
	}


	uint8_t Model_GetSkeleton(const Model* model, Bone*& boneArr, uint32_t& boneCount, int32_t& inc)
	{
		CheckPtr(model, false);

		static List<Bone> bones;
		if (!model->GetSkeleton(bones))
		{
			return false;
		}
		boneArr = bones.GetArrayPtr();
		boneCount = (uint32_t) bones.Size();
		inc = sizeof(Bone);
		return true;
	}


	const CollisionMesh* Model_GetCollisionMesh(const Model *model)
	{
		const CollisionMesh& mesh = model -> GetCollisionMesh();
		return &mesh;
	}


	const void Model_GetPrimitivesMasked(const Model* model, uint32_t mask, int& numPrims,
										CollisionPrimitive**& primArrayPtr)
	{
		static List<CollisionPrimitive> primsList;
		static List<CollisionPrimitive *> primPtrs;
		
		numPrims = 0;
		CheckPtr(model,);

		primsList = model -> GetCollisionPrimitives((ECollisionMaskFlags) mask);
		primPtrs.Clear();

		for (size_t i = 0; i < primsList.Size(); i++)
		{
			primPtrs.Add(&primsList[i]);
		}		

		primArrayPtr = primPtrs.GetArrayPtr();
		numPrims = (uint32_t) primPtrs.Size();		
	}


	uint8_t Model_IsSkeletonBroken(const Model* model)
	{
		CheckPtr(model,false);
		return model -> IsSkeletonBroken();
	}



	const void Bone_FetchAllFields(const Bone* bone, const char *&name,
									const char *& parentName, const Vector3*& loc,
									const Vector4*& rot)
	{
	    name = (bone -> m_BoneName).Buffer();
	    loc = &(bone -> m_Position);
		rot = &(bone -> m_Rotation);
		
		parentName = (bone -> m_Parent).Buffer();
	}



	const void CollisionMesh_GetIndexBuffer(const CollisionMesh *collMesh, uint32_t& count, uint16_t*& outBuffer)
	{
		collMesh -> GetIndexBuffer(ETopology::TriangleList, count, outBuffer);
	}
    

    const void CollisionMesh_GetVertexBuffer(const CollisionMesh *collMesh, uint32_t& count, float_t*& buffer)
    {
    	static float_t *tempBuffer = nullptr;
    	delete tempBuffer;

    	Vector3 *verts;
    	collMesh -> GetVertexBuffer(count, verts);

    	tempBuffer = new float_t[count * 3];

    	for (int i = 0; i < (int)count; i++)
    	{
    		Vector3& curVec = verts[i];

    		tempBuffer[3*i]     = curVec.m_X; 
    		tempBuffer[3*i + 1] = curVec.m_Y; 
    		tempBuffer[3*i + 2] = curVec.m_Z; 
    	}

    	buffer = tempBuffer;
    }


    //CollisionPrimitive

    const void CollisionPrimitive_FetchAllFields(CollisionPrimitive *primPtr,
                                            float_t& f1, float_t& f2, float_t& f3,
                                            const char *& namePtr, const char *& parentNamePtr,
                                            uint32_t& maskFlags, uint32_t& primitiveType,
                                            Vector3*& pos, Vector4*& rot)
    {
    	static String name, parentName;
    	static Vector3 posTemp;
    	static Vector4 rotTemp;

    	f1 = f2 = f3 = 0.0f;

    	name = primPtr -> GetName();
    	parentName = primPtr -> GetParentName();

    	namePtr = name.Buffer();
    	parentNamePtr = parentName.Buffer();

    	maskFlags = (uint32_t) primPtr -> GetMaskFlags();
    	primitiveType = (uint32_t) primPtr -> GetPrimitiveType();

    	rotTemp = primPtr -> GetRotation();
    	posTemp = primPtr -> GetPosition();

    	pos = &posTemp; 
    	rot = &rotTemp; 

    	switch (primitiveType)
    	{
    		case 1:
    			primPtr -> GetSphereRadius(f1);
    			return;
    		case 2:
    			primPtr -> GetCylinderDims(f1,f2);
    			return;
    		case 4:
    			primPtr -> GetCubeDims(f1,f2,f3);
    			return;
    		default:
    			return;
    	}
    }

    
    //Segment

    const uint32_t Segment_GetVertexBufferLength(const Segment* segment)
    {
    	Vector3 *verts;
    	uint32_t numVerts;
		segment -> GetVertexBuffer(numVerts, verts);
    	return numVerts;
    }


	const void Segment_GetVertexBuffer(const Segment* segment, uint32_t& numVerts, float_t*& vertBuffer)
	{
		Vector3 *verts;
		segment -> GetVertexBuffer(numVerts, verts);

		vertBuffer = new float[numVerts * 3];

		for (int i = 0; i < (int) numVerts; i++)
		{
			Vector3& curVec = verts[i];
			vertBuffer[i * 3] = curVec.m_X;
			vertBuffer[i * 3 + 1] = curVec.m_Y;
			vertBuffer[i * 3 + 2] = curVec.m_Z;
		}
	}


	const void Segment_GetNormalBuffer(const Segment* segment, uint32_t& numNormals, float_t*& normalsBuffer)	
	{
		Vector3 *normals;
		segment -> GetNormalBuffer(numNormals, normals);

		normalsBuffer = new float[numNormals * 3];

		for (int i = 0; i < (int) numNormals; i++)
		{
			Vector3& curVec = normals[i];
			normalsBuffer[i * 3] = curVec.m_X;
			normalsBuffer[i * 3 + 1] = curVec.m_Y;
			normalsBuffer[i * 3 + 2] = curVec.m_Z;
		}
	}


	const void Segment_GetUVBuffer(const Segment* segment, uint32_t& numUVs, float_t*& UVBuffer)
	{
		Vector2 *UVs;
		segment -> GetUVBuffer(numUVs, UVs);

		UVBuffer = new float[numUVs * 2];

		for (int i = 0; i < (int) numUVs; i++)
		{
			Vector2& curVec = UVs[i];
			UVBuffer[i * 2] = curVec.m_X;
			UVBuffer[i * 2 + 1] = curVec.m_Y;
		}
	}


	const void Segment_GetIndexBuffer(const Segment* segment, uint32_t& numInds, uint16_t*& indexBuffer)
	{
		uint16_t *indicies = nullptr;
		numInds = 0;
		CheckPtr(segment,);
		segment -> GetIndexBuffer(numInds, indexBuffer, ETopology::TriangleList);
	}



	const void Segment_GetVertexWeightsBuffer(const Segment* segment, int32_t& numVWs, VertexWeight*& vwBuffer)
	{
		uint32_t count;
		if (segment -> GetVertexWeights(count, vwBuffer))
		{
			numVWs = (int32_t) count;
		}
		else 
		{
			numVWs = 0;
			vwBuffer = nullptr;			
		}
	}

	const Material* Segment_GetMaterial(const Segment* segment)
	{
		return &(segment -> GetMaterial());
	}

	const int32_t Segment_GetTopology(const Segment* segment)
	{
		return (int32_t) segment -> GetTopology();
	}

	const char* Segment_GetBone(const Segment* segment)
	{
		static String boneName = "";
		CheckPtr(segment, "");

		boneName = segment -> GetBone();
		return boneName.Buffer();
	}

	const uint8_t Segment_IsPretransformed(const Segment* segment)
	{
		CheckPtr(segment, false);
		return segment -> IsPretransformed();
	}


    uint8_t Material_FetchAllFields(const Material* matPtr,  Vector3*& specular,
                    Vector3*& diffuse, char**& texPtrs, int32_t& numTexes,
                    char* attachedLightName, uint32_t& matFlags, uint32_t& specExp)
    {	
    	static Vector3 specCache, diffCache;
    	static char** texNamePtrsCache = new char*[4];
    	static String namesCache[4];
    	static char* attachedLightNameCache = nullptr;

    	CheckPtr(matPtr, false);

    	uint8_t count = 0;
    	while (count < 4 && matPtr -> GetTextureName(count, namesCache[count]))
    	{
    		texNamePtrsCache[count] = const_cast<char *>(namesCache[count].Buffer());
    		count++;
    	}
    	numTexes = count;
    	texPtrs = texNamePtrsCache;

    	specExp = matPtr -> GetSpecularExponent();
    	matFlags = (uint32_t) matPtr -> GetFlags();  

    	Color4u8 d = matPtr -> GetDiffuseColor();
    	Color4u8 s = matPtr -> GetSpecularColor();

    	diffCache = Vector3(d.m_Red,d.m_Green,d.m_Blue); 
    	specCache = Vector3(s.m_Red,s.m_Green,s.m_Blue);

    	diffuse = &diffCache;
    	specular = &specCache; 

    	attachedLightName = const_cast<char *>(matPtr -> GetAttachedLight().Buffer());

    	return true;
    }


	const char* ENUM_MaterialFlagsToString(EMaterialFlags flags)
	{
		static Types::String lastToString;
		lastToString = MaterialFlagsToString(flags);
		return lastToString.Buffer();
	}

	const char* ENUM_VBUFFlagsToString(EVBUFFlags flags)
	{
		static String lastToString;
		lastToString = VBUFFlagsToString(flags);
		return lastToString.Buffer();
	}

    const char* World_GetName(const World* world)
    {
		CheckPtr(world, "");
		const String& name = world->GetName();
		//char *buffer = new char[strlen(name.Buffer()) + 1]();
		//strcpy(buffer, name.Buffer());
		return name.Buffer();
    }

    const void World_GetInstances(const World* model, const Instance**& instanceArr, uint32_t& instCount)
    {
    	CheckPtr(model, );
		const List<Instance>& segments = model->GetInstances();

		static List<const Instance*> segmentPtrs;
		segmentPtrs.Clear();

		for (size_t i = 0; i < segments.Size(); ++i)
		{
			segmentPtrs.Add(&segments[i]);
		}

		instanceArr = segmentPtrs.GetArrayPtr();
		instCount = (uint32_t)segmentPtrs.Size();
    }

    const void* World_GetTerrain(const World* world)
    {
    	CheckPtr(world, nullptr);
    	return (void *) world -> GetTerrain();
    }

	const void World_GetRegions(const World* world, const void*& regArr, uint32_t& count)
	{
		List<regn *>& regionChunks = world -> p_World -> m_Regions;
		regArr = (void *) regionChunks.GetArrayPtr();
		count = regionChunks.Size();
	}


	const void Region_FetchAllFields(const void* reg, Vector3*& sizeOut, Vector3*& posOut, Vector4*& rotOut, char *&nameOut, char*& typeOut)
	{
		static Vector4 rotCache;
		regn *regPtr = (regn *) reg;

		sizeOut = &(regPtr -> p_Info -> p_SIZE -> m_Dimensions);

		posOut = &(regPtr -> p_Info -> p_XFRM -> m_Position);
		
		rotCache = MatrixToQuaternion(regPtr -> p_Info -> p_XFRM -> m_RotationMatrix);
		rotOut = &rotCache;

		nameOut = const_cast<char *>(regPtr -> p_Info -> p_Name -> m_Text.Buffer());
		typeOut = const_cast<char *>(regPtr -> p_Info -> p_Type -> m_Text.Buffer());
	}



    /*
    const uint8_t World_GetLights(const World* world, Light*& lightArr, int32_t& count, int32_t& inc)
    {
    	CheckPtr(world, false);

    	const List<Light>& lights = world -> GetLights();

    	lightArr = lights.GetArrayPtr();
    	count = lights.Size();
    	inc = sizeof(Light);

    	return true;
    }
    */


    

    // Wrappers - Instance
    const char* Instance_GetName(const Instance* instance)
    {
        CheckPtr(instance,"")

    	static String instanceName;
		instanceName = instance->GetName();
		return instanceName.Buffer();
    }

    const Vector4* Instance_GetRotation(const Instance* instance)
    {
    	static Vector4 tempVec;
    	tempVec = instance -> GetRotation();
    	return &tempVec;
    }

    const Vector3* Instance_GetPosition(const Instance* instance)
    {
        static Vector3 tempVec;
    	tempVec = instance -> GetPosition();
    	return &tempVec;
    }

    const char * Instance_GetEntityClassName(const Instance* instance)
    {
    	CheckPtr(instance,"")
    	static String ecName; 

		ecName = instance -> GetType();

		return ecName.Buffer();
    }


    const uint8_t Instance_GetOverriddenProperties(const Instance *ec, uint32_t*& hashesBuffer, char **& valuesBuffer, int32_t& count)
    {
    	CheckPtr(ec, false)
    	char **ptrsBuffer = nullptr;
    	static List<String> values;
    	static List<uint32_t> hashes;

    	delete ptrsBuffer;

    	if (ec -> GetOverriddenProperties(hashes, values))
    	{
    		hashesBuffer = hashes.GetArrayPtr();
    		count = values.Size();

    		ptrsBuffer = new char *[count];
    		for (int i = 0; i < count; i++)
    		{
    			ptrsBuffer[i] = const_cast<char *>(values[i].Buffer());
    		}

    		valuesBuffer = ptrsBuffer;
    		return true;
    	}
    	else 
    	{
    		ptrsBuffer = nullptr;
    		count = 0;
    		return false;
    	}
    }




    // EntityClass

    const char * EntityClass_GetProperty(const EntityClass *ec, const char *propName)
    {
    	CheckPtr(ec,"")
    	static String value; 

		if (ec -> GetProperty(propName, value))
		{
			return value.Buffer();
		}

		return "";
    }


    const char *EntityClass_GetName(const EntityClass *ec)
    {
    	CheckPtr(ec,"")
    	static String typeName;

    	typeName = ec -> GetTypeName();
    	return typeName.Buffer();     	
    }


    const char *EntityClass_GetBaseName(const EntityClass *ec)
    {
    	CheckPtr(ec,"")
    	static String baseName;

    	baseName = ec -> GetBaseName();
    	return baseName.Buffer(); 
    }


    uint8_t EntityClass_GetOverriddenProperties(const EntityClass *ec, uint32_t*& hashesBuffer, char **& valuesBuffer, int32_t& count)
    {
    	CheckPtr(ec, false)
    	char **ptrsBuffer = nullptr;
    	static List<String> values;
    	static List<uint32_t> hashes;

    	delete ptrsBuffer;

    	if (ec -> GetOverriddenProperties(hashes, values))
    	{
    		hashesBuffer = hashes.GetArrayPtr();
    		count = values.Size();

    		ptrsBuffer = new char *[count];
    		for (int i = 0; i < count; i++)
    		{
    			ptrsBuffer[i] = const_cast<char *>(values[i].Buffer());
    		}

    		valuesBuffer = ptrsBuffer;
    		return true;
    	}
    	else 
    	{
    		ptrsBuffer = nullptr;
    		count = 0;
    		return false;
    	}
    }




    // Vector

    const void Vector4_FromPtr(const Vector4* vec, float& x, float& y, float& z, float &w)
    {
    	if (vec == nullptr)
    	{
    		x = y = z = w = 0.0f;
    		return;
    	}
    	x = vec -> m_X;
    	y = vec -> m_Y;
    	z = vec -> m_Z;
       	w = vec -> m_W;
    } 

    const void Vector3_FromPtr(const Vector3* vec, float& x, float& y, float& z)
    {
    	if (vec == nullptr)
    	{
    		x = y = z = 0.0f;
    		return;
    	}
    	x = vec -> m_X;
    	y = vec -> m_Y;
    	z = vec -> m_Z;
    } 

    const void Vector2_FromPtr(const Vector2* vec, float& x, float& y)
    {
    	if (vec == nullptr)
    	{
    		x = y = 0.0f;
    		return;
    	}
    	x = vec -> m_X;
    	y = vec -> m_Y;
    } 


    /*
    const char* Light_GetAllFields(const Light* lightPtr, Vector4*& rotPtr,
                                    Vector3*& posPtr, uint32_t& lightType, 
                                    Vector3*& colPtr, float_t& range,
                                    Vector2*& conePtr)
    {
    	CheckPtr(lightPtr, nullptr);

    	static Vector3 lastPos, lastCol;
    	static Vector4 lastRot; 
    	static Vector2 lastCone(0,0);	
    	float_t inner=0,outer=0;

    	lastRot  = lightPtr -> GetRotation();
    	lastPos  = lightPtr -> GetPosition();
    	lastCol  = lightPtr -> GetColor();

    	lightPtr -> GetSpotAngles(inner,outer);
    	lastCone = Vector2(inner,outer);

    	lightType = (uint32_t) lightPtr -> GetType();
    	lightPtr -> GetRange(range);

    	rotPtr  = &lastRot;
    	colPtr  = &lastCol;
    	posPtr  = &lastPos;
    	conePtr = &lastCone;

    	const String& name = lightPtr -> GetName();
    	return name.Buffer();
    }
    */



	const bool AnimationBank_GetCurve(const AnimationBank* setPtr, uint32_t animCRC, uint32_t boneCRC, uint32_t comp, 
                                                    const uint16_t*& indicesBuffer, const float_t*& valuesBuffer, int& numKeys)
	{
		static List<uint16_t> indices;
		static List<float_t>  values;

		CheckPtr(setPtr, false);

		bool status = setPtr -> GetCurve(animCRC, boneCRC, comp, indices, values);

		if (status)
		{
			numKeys = values.Size();
			indicesBuffer = indices.GetArrayPtr();
			valuesBuffer  = values.GetArrayPtr();
		}

		return status;
	}


    const uint32_t* AnimationBank_GetAnimationCRCs(const AnimationBank* setPtr, int& numCRCs)
    {
    	static List<uint32_t> crcs;

    	numCRCs = 0;
    	CheckPtr(setPtr, nullptr);

    	crcs = setPtr -> GetAnimationNames();
    	
    	numCRCs = crcs.Size();
    	return crcs.GetArrayPtr();
    }
    

    const bool AnimationBank_GetAnimationMetadata(const AnimationBank* setPtr, uint32_t animCRC,
                                                    int32_t& numFrames, int32_t& numBones)
    {
		CheckPtr(setPtr, false);
		uint32_t frames, bones;
		bool status = setPtr -> GetAnimationMetadata(animCRC, frames, bones);
		
		numFrames = frames;
		numBones = bones;
		return status;
    }


    const uint32_t Config_GetName(const Config* cfg)
    {
    	return cfg -> m_Name;
    }

    const uint8_t Config_IsPropertySet(const Config* cfg, uint32_t hash)
    {
    	return cfg -> IsPropertySet(hash);
    }

	const float_t Config_GetFloat(const Config* cfg, uint32_t hash)
	{
		return cfg -> GetFloat(hash);
	}

	const Vector2* Config_GetVec2(const Config* cfg, uint32_t hash)
	{
		static Vector2 cache;
		cache = cfg -> GetVector2(hash);
		return &cache; 
	}

	const Vector3* Config_GetVec3(const Config* cfg, uint32_t hash)
	{
		static Vector3 cache;
		cache = cfg -> GetVector3(hash);
		return &cache; 
	}

	const Vector4* Config_GetVec4(const Config* cfg, uint32_t hash)
	{
		static Vector4 cache;
		cache = cfg -> GetVector4(hash);
		return &cache; 
	}

	const char* Config_GetString(const Config* cfg, uint32_t hash)
	{
		static String cache;
		cache = cfg -> GetString(hash);
		return cache.Buffer(); 
	}

	const char** Config_GetStrings(const Config* cfg, uint32_t hash, int32_t& count)
	{
		static List<String> cache;
		static char** cachePtrs = nullptr;
		delete cachePtrs;

		cache = cfg -> GetStrings(hash);

		count = cache.Size();

		cachePtrs = new char*[count];
		for (int i = 0; i < count; i++)
		{
			cachePtrs[i] = const_cast<char *>(cache[i].Buffer());
		}

		return const_cast<const char **>(cachePtrs);
	}


    const Config* Config_GetChildConfigs(const Config* cfg, uint32_t hash, int32_t& numConfigs, int32_t& inc)
    {
    	static List<Config> cache;
    	cache = cfg -> GetChildConfigs(hash);

    	inc = sizeof(Config);
    	numConfigs = cache.Size();
    	return cache.GetArrayPtr();
    }
}