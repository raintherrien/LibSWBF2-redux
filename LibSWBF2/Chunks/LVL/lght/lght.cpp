#include "stdafx.h"
#include "lght.h"
#include "InternalHelpers.h"
#include "FileReader.h"
#include "Types/Enums.h"
#include "Logging/Logger.h"


namespace LibSWBF2::Chunks::LVL::lght
{
	void lght::RefreshSize()
	{
		THROW("Not implemented!");
	}

	void lght::WriteToStream(FileWriter& stream)
	{
		THROW("Not implemented!");
	}


	void lght::ReadFromStream(FileReader& stream)
	{       
        BaseChunk::ReadFromStream(stream);
        Check(stream);
        
        /*
         It seems in every ucfb, the first lght chunk contains
         all local lights, and the other per-world lght's merely index
         into the first one.  So for now, before we do per-world stuff,
         we just parse this first chunk.
         */

        if (!lght::skip)
        {
            LOG_WARN("START LGHT");
            STR<"NAME"_m> *p_Marker;
            READ_CHILD(stream, p_Marker); //Will determine meaning when investigating other lght chunks

            LOG_WARN("READ MARKER");

            while (ThereIsAnother(stream))
            {
                DATA_TAG *tempTag;
                SCOP_LGHT *tempBody;
                
                LOG_WARN("STARTING HEADER");

                READ_CHILD(stream, tempTag);

                LOG_WARN("READ HEADER");

                if (tempTag -> m_LocalLight){ //Check local flag before proceding
                    READ_CHILD(stream, tempBody);

                    LOG_WARN("READ BODY");
                    p_LightTags.Add(tempTag);
                    p_LightBodies.Add(tempBody);
                    m_NumLights++;
                }
                else 
                {
                    break;
                }
            }

            lght::skip = true;
        }

		BaseChunk::EnsureEnd(stream);
	}
}
