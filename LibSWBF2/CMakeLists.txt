cmake_minimum_required(VERSION 3.16)
cmake_policy(SET CMP0079 NEW)

project(LibSWBF2_Project)

# Use folders for generators that supports them (VS, XCode, ...)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# SETTINGS
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# produce relative code adresses, necessary for shared libs
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(LibSWBF2 SHARED)

set(GLM_DIR "${PROJECT_SOURCE_DIR}/../ThirdParty/glm")
set(FMT_DIR "${PROJECT_SOURCE_DIR}/../ThirdParty/fmt")

add_subdirectory(${GLM_DIR} "${CMAKE_CURRENT_BINARY_DIR}/glm")
add_subdirectory(${FMT_DIR} "${CMAKE_CURRENT_BINARY_DIR}/fmt")

target_include_directories(LibSWBF2 PRIVATE ${PROJECT_SOURCE_DIR} "${PROJECT_SOURCE_DIR}/IO")

target_compile_definitions(LibSWBF2 PRIVATE LOOKUP_CSV_PATH="${PROJECT_SOURCE_DIR}/../lookup.csv")

target_link_libraries(LibSWBF2 glm)
target_link_libraries(LibSWBF2 fmt)

target_precompile_headers(LibSWBF2 PRIVATE "$<$<COMPILE_LANGUAGE:CXX>:${PROJECT_SOURCE_DIR}/pch.h>")
target_compile_definitions(LibSWBF2 PUBLIC MEMORY_MAPPED_READER)

if (UNIX)
  target_compile_options(LibSWBF2 PUBLIC -Wall -Wextra -Wno-attributes -Wno-unused-variable -Wno-unused-parameter -Wno-sign-compare)
  #target_compile_options(LibSWBF2 PUBLIC -Wno-deprecated-volatile)

  # Per convention, in UNIX shared libraries start with a lower 
  # case "lib" prefix, which would result in "libLibSWBF2". 
  # To circumvent that, just change the file output name accordingly.
  set_target_properties(LibSWBF2 PROPERTIES OUTPUT_NAME "SWBF2")

  # Optionally build with GCC/Clang sanitizers
  option(BUILD_SAN "Build with sanitizers" OFF)
  if(BUILD_SAN)
    target_compile_options(LibSWBF2 PUBLIC -fsanitize=address,leak,undefined,pointer-compare,pointer-subtract -fstack-protector -g3 -O0)
    target_link_options   (LibSWBF2 PUBLIC -fsanitize=address,leak,undefined,pointer-compare,pointer-subtract -fstack-protector -g3 -O0)
  endif()
endif(UNIX)

if(MSVC)
  # Enable multi processor compilation for VS projects
  target_compile_options(LibSWBF2 PRIVATE "/MP /W4 /WX")
endif()

# Just for folder display in generated project
# TODO: There is probably a way to do this procedurally
source_group("Source" REGULAR_EXPRESSION "${PROJECT_SOURCE_DIR}/*")
source_group("Source/DirectX" REGULAR_EXPRESSION "${PROJECT_SOURCE_DIR}/DirectX/*")
source_group("Source/Logging" REGULAR_EXPRESSION "${PROJECT_SOURCE_DIR}/Logging/*")
source_group("Source/Types" REGULAR_EXPRESSION "${PROJECT_SOURCE_DIR}/Types/*")
source_group("Source/Wrappers" REGULAR_EXPRESSION "${PROJECT_SOURCE_DIR}/Wrappers/*")
source_group("Source/Chunks" REGULAR_EXPRESSION "${PROJECT_SOURCE_DIR}/Chunks/*")
source_group("Source/Chunks/LVL" REGULAR_EXPRESSION "${PROJECT_SOURCE_DIR}/Chunks/LVL/*")
source_group("Source/Chunks/LVL/coll" REGULAR_EXPRESSION "${PROJECT_SOURCE_DIR}/Chunks/LVL/coll/*")
source_group("Source/Chunks/LVL/common" REGULAR_EXPRESSION "${PROJECT_SOURCE_DIR}/Chunks/LVL/common/*")
source_group("Source/Chunks/LVL/config" REGULAR_EXPRESSION "${PROJECT_SOURCE_DIR}/Chunks/LVL/config/*")
source_group("Source/Chunks/LVL/gmod" REGULAR_EXPRESSION "${PROJECT_SOURCE_DIR}/Chunks/LVL/gmod/*")
source_group("Source/Chunks/LVL/Locl" REGULAR_EXPRESSION "${PROJECT_SOURCE_DIR}/Chunks/LVL/Locl/*")
source_group("Source/Chunks/LVL/modl" REGULAR_EXPRESSION "${PROJECT_SOURCE_DIR}/Chunks/LVL/modl/*")
source_group("Source/Chunks/LVL/prim" REGULAR_EXPRESSION "${PROJECT_SOURCE_DIR}/Chunks/LVL/prim/*")
source_group("Source/Chunks/LVL/scr_" REGULAR_EXPRESSION "${PROJECT_SOURCE_DIR}/Chunks/LVL/scr_/*")
source_group("Source/Chunks/LVL/skel" REGULAR_EXPRESSION "${PROJECT_SOURCE_DIR}/Chunks/LVL/skel/*")
source_group("Source/Chunks/LVL/sound" REGULAR_EXPRESSION "${PROJECT_SOURCE_DIR}/Chunks/LVL/sound/*")
source_group("Source/Chunks/LVL/tern" REGULAR_EXPRESSION "${PROJECT_SOURCE_DIR}/Chunks/LVL/tern/*")
source_group("Source/Chunks/LVL/tex_" REGULAR_EXPRESSION "${PROJECT_SOURCE_DIR}/Chunks/LVL/tex_/*")
source_group("Source/Chunks/LVL/wrld" REGULAR_EXPRESSION "${PROJECT_SOURCE_DIR}/Chunks/LVL/wrld/*")
source_group("Source/Chunks/LVL/zaa_" REGULAR_EXPRESSION "${PROJECT_SOURCE_DIR}/Chunks/LVL/zaa_/*")
source_group("Source/Chunks/LVL/zaf_" REGULAR_EXPRESSION "${PROJECT_SOURCE_DIR}/Chunks/LVL/zaf_/*")
source_group("Source/Chunks/MSH" REGULAR_EXPRESSION "${PROJECT_SOURCE_DIR}/Chunks/MSH/*")
source_group("Source/Chunks/TERR" REGULAR_EXPRESSION "${PROJECT_SOURCE_DIR}/Chunks/TERR/*")
source_group("Source/Chunks/LVL/plan" REGULAR_EXPRESSION "${PROJECT_SOURCE_DIR}/Chunks/LVL/plan/*")

target_sources(LibSWBF2 PRIVATE 

  # Root
  "${PROJECT_SOURCE_DIR}/API.cpp"
  "${PROJECT_SOURCE_DIR}/Container.cpp"
  "${PROJECT_SOURCE_DIR}/dllmain.cpp"
  "${PROJECT_SOURCE_DIR}/Hashing.cpp"
  "${PROJECT_SOURCE_DIR}/InternalHelpers.cpp"

  # IO
  "${PROJECT_SOURCE_DIR}/IO/StreamReader.cpp"
  "${PROJECT_SOURCE_DIR}/IO/MemoryMappedReader.cpp"
  "${PROJECT_SOURCE_DIR}/IO/FileWriter.cpp"

  # DirectX
  "${PROJECT_SOURCE_DIR}/DirectX/DXHelpers.cpp"
  "${PROJECT_SOURCE_DIR}/DirectX/DXTexCrossPlat.cpp"
  "${PROJECT_SOURCE_DIR}/DirectX/DXTexCrossPlatConverters.cpp"
  "${PROJECT_SOURCE_DIR}/DirectX/detex/decompress-bc.c"
  "${PROJECT_SOURCE_DIR}/DirectX/detex/division-tables.c"

  # Sound
  "${PROJECT_SOURCE_DIR}/Audio/IMAADPCMDecoder.cpp"  
  "${PROJECT_SOURCE_DIR}/Audio/PCM16Decoder.cpp"  

  # Logging
  "${PROJECT_SOURCE_DIR}/Logging/Logger.cpp"
  "${PROJECT_SOURCE_DIR}/Logging/LoggerEntry.cpp"

  # Types
  "${PROJECT_SOURCE_DIR}/Types/Animation.cpp"
  "${PROJECT_SOURCE_DIR}/Types/BoneFrames.cpp"
  "${PROJECT_SOURCE_DIR}/Types/BoneWeight.cpp"
  "${PROJECT_SOURCE_DIR}/Types/Color4.cpp"
  "${PROJECT_SOURCE_DIR}/Types/Color4u8.cpp"
  "${PROJECT_SOURCE_DIR}/Types/Enums.cpp"
  "${PROJECT_SOURCE_DIR}/Types/LocalizeEntry.cpp"
  "${PROJECT_SOURCE_DIR}/Types/Matrix3x3.cpp"
  "${PROJECT_SOURCE_DIR}/Types/RotationFrame.cpp"
  "${PROJECT_SOURCE_DIR}/Types/SoundBankHeader.cpp"
  "${PROJECT_SOURCE_DIR}/Types/SoundClip.cpp"
  "${PROJECT_SOURCE_DIR}/Types/TerrainBufferEntry.cpp"
  "${PROJECT_SOURCE_DIR}/Types/TranslationFrame.cpp"
  "${PROJECT_SOURCE_DIR}/Types/Vector2.cpp"
  "${PROJECT_SOURCE_DIR}/Types/Vector3.cpp"
  "${PROJECT_SOURCE_DIR}/Types/Vector3u8.cpp"
  "${PROJECT_SOURCE_DIR}/Types/Vector4.cpp"
  "${PROJECT_SOURCE_DIR}/Types/WorldAnimationKey.cpp"
  "${PROJECT_SOURCE_DIR}/Types/Planning.cpp"

  # Wrappers
  "${PROJECT_SOURCE_DIR}/Wrappers/AnimationBank.cpp"
  "${PROJECT_SOURCE_DIR}/Wrappers/AnimationSkeleton.cpp"
  "${PROJECT_SOURCE_DIR}/Wrappers/CollisionMesh.cpp"
  "${PROJECT_SOURCE_DIR}/Wrappers/CollisionPrimitive.cpp"
  "${PROJECT_SOURCE_DIR}/Wrappers/Config.cpp"
  "${PROJECT_SOURCE_DIR}/Wrappers/EntityClass.cpp"
  "${PROJECT_SOURCE_DIR}/Wrappers/Instance.cpp"
  "${PROJECT_SOURCE_DIR}/Wrappers/Level.cpp"
  "${PROJECT_SOURCE_DIR}/Wrappers/Localization.cpp"
  "${PROJECT_SOURCE_DIR}/Wrappers/Material.cpp"
  "${PROJECT_SOURCE_DIR}/Wrappers/Model.cpp"
  "${PROJECT_SOURCE_DIR}/Wrappers/Script.cpp"
  "${PROJECT_SOURCE_DIR}/Wrappers/Segment.cpp"
  "${PROJECT_SOURCE_DIR}/Wrappers/Sound.cpp"
  "${PROJECT_SOURCE_DIR}/Wrappers/SoundBank.cpp"
  "${PROJECT_SOURCE_DIR}/Wrappers/Terrain.cpp"
  "${PROJECT_SOURCE_DIR}/Wrappers/Texture.cpp"
  "${PROJECT_SOURCE_DIR}/Wrappers/World.cpp"
  "${PROJECT_SOURCE_DIR}/Wrappers/SoundStream.cpp"
  "${PROJECT_SOURCE_DIR}/Wrappers/PlanSet.cpp"


  # Chunks
  "${PROJECT_SOURCE_DIR}/Chunks/BaseChunk.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/GenericChunk.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/HeaderNames.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/RawData.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/STR.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/STRMULT.cpp"

  # LVL
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/LVL.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/lvl_.cpp"

  "${PROJECT_SOURCE_DIR}/Chunks/LVL/coll/coll.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/coll/coll.INFO.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/coll/POSI.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/coll/TREE.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/coll/TREE.LEAF.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/coll/TREE.NODE.cpp"

  "${PROJECT_SOURCE_DIR}/Chunks/LVL/common/GenericClass.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/common/PROP.cpp"

  "${PROJECT_SOURCE_DIR}/Chunks/LVL/config/config.NAME.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/config/ConfigChunk.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/config/DATA.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/config/float.DATA.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/config/SCOP.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/config/string.DATA.cpp"

  "${PROJECT_SOURCE_DIR}/Chunks/LVL/gmod/gmod.cpp"

  "${PROJECT_SOURCE_DIR}/Chunks/LVL/Locl/Locl.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/Locl/Locl.BODY.cpp"

  "${PROJECT_SOURCE_DIR}/Chunks/LVL/modl/BMAP.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/modl/IBUF.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/modl/INFO.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/modl/LVL.modl.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/modl/modl.segm.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/modl/modl.VBUF.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/modl/MTRL.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/modl/segm.INFO.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/modl/SKIN.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/modl/TNAM.cpp"

  "${PROJECT_SOURCE_DIR}/Chunks/LVL/prim/MASK.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/prim/prim.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/prim/prim.DATA.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/prim/prim.INFO.cpp"

  "${PROJECT_SOURCE_DIR}/Chunks/LVL/scr_/scr_.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/scr_/scr_.INFO.cpp"

  "${PROJECT_SOURCE_DIR}/Chunks/LVL/skel/skel.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/skel/skel.INFO.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/skel/skel.XFRM.cpp"

  "${PROJECT_SOURCE_DIR}/Chunks/LVL/sound/StreamList.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/sound/Stream.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/sound/StreamInfo.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/sound/SoundBankList.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/sound/SampleBankData.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/sound/SampleBank.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/sound/SampleBankInfo.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/sound/_pad.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/sound/StreamData.cpp"

  "${PROJECT_SOURCE_DIR}/Chunks/LVL/tern/DTEX.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/tern/LTEX.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/tern/PCHS.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/tern/PTCH.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/tern/PTCH.IBUF.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/tern/PTCH.INFO.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/tern/PTCH.VBUF.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/tern/tern.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/tern/tern.INFO.cpp"

  "${PROJECT_SOURCE_DIR}/Chunks/LVL/tex_/BODY.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/tex_/FACE.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/tex_/FMT_.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/tex_/FMT_.INFO.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/tex_/LVL_.INFO.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/tex_/tex_.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/tex_/tex_.INFO.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/tex_/tex_.LVL_.cpp"

  "${PROJECT_SOURCE_DIR}/Chunks/LVL/wrld/anim.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/wrld/anmg.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/wrld/anmh.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/wrld/inst.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/wrld/regn.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/wrld/SIZE.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/wrld/wrld.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/wrld/wrld.INFO.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/wrld/XFRM.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/wrld/WorldAnimKeyChunk.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/wrld/anim.INFO.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/wrld/anmg.INFO.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/wrld/anmh.INFO.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/wrld/BARR.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/wrld/Hint.cpp"

  "${PROJECT_SOURCE_DIR}/Chunks/LVL/zaa_/BIN_.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/zaa_/MINA.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/zaa_/TADA.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/zaa_/TNJA.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/zaa_/zaa_.cpp"

  "${PROJECT_SOURCE_DIR}/Chunks/LVL/zaf_/BIN_.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/zaf_/LEKS.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/zaf_/TNOJ.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/zaf_/zaf_.cpp"

  # MSH
  "${PROJECT_SOURCE_DIR}/Chunks/MSH/ANM2.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/MSH/ATRB.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/MSH/BBOX.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/MSH/CYCL.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/MSH/ENVL.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/MSH/FLGS.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/MSH/FRAM.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/MSH/GEOM.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/MSH/KFR3.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/MSH/MATD.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/MSH/MATI.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/MSH/MATL.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/MSH/MNDX.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/MSH/MODL.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/MSH/MSH.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/MSH/MSH.DATA.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/MSH/MSH2.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/MSH/MTYP.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/MSH/NRML.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/MSH/POSL.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/MSH/SEGM.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/MSH/SHVO.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/MSH/SINF.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/MSH/STRP.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/MSH/SWCI.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/MSH/TRAN.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/MSH/UV0L.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/MSH/WGHT.cpp"

  # TERR
  "${PROJECT_SOURCE_DIR}/Chunks/TERR/TERR.cpp"

  # PLAN
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/plan/plan.cpp"
  "${PROJECT_SOURCE_DIR}/Chunks/LVL/plan/plnp.cpp"
)

option(LIBSWBF2_BUILD_TESTS "Build tests" ON)
mark_as_advanced(FORCE LIBSWBF2_BUILD_TESTS)
if(LIBSWBF2_BUILD_TESTS)
	add_subdirectory(Tests)
endif()
