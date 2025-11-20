#include "JSONSerializer.h"
#include <Logger.h>

constexpr int MAX_DECIMA_PLACES = 3;

namespace SCION_FILESYSTEM {

	JSONSerializer::JSONSerializer(const std::string& sFilename)
		: m_Filestream {}
		, m_StringBuffer{}
		, m_pWriter{ std::make_unique<rapidjson::PrettyWriter<rapidjson::StringBuffer>>(m_StringBuffer) }
		, m_NumArraysStarted{0}
		, m_NumObjectsStarted{0}
	{
		m_Filestream.open(sFilename, std::ios::out | std::ios::trunc);

		if (!m_Filestream.is_open())
		{
			throw std::runtime_error("failed open file");
		}

		m_pWriter->SetMaxDecimalPlaces(MAX_DECIMA_PLACES);
	}

	JSONSerializer::~JSONSerializer()
	{
		if (m_Filestream.is_open())
		{
			m_Filestream.close();
		}
	}

	bool JSONSerializer::StartDocument()
	{
		if (m_NumObjectsStarted != 0)
		{
			SCION_ERROR("Document has alrdady been started. Please Reset the serializer.");
			return false;
		}
		++m_NumObjectsStarted;

		return m_pWriter->StartObject();
	}

	bool JSONSerializer::EndDocument()
	{
		if (m_NumObjectsStarted != 1)
		{
			SCION_ERROR("Failed to end document correctly. There shoule only be one object open. Did you forget to call EndObject()?");
			return false;
		}
		if (m_NumArraysStarted != 0)
		{
			SCION_ERROR("Failed to end document correctly. Too many arrays started. Did you forget to call EndArray()?");
			return false;
		}
		m_pWriter->EndObject();
		--m_NumObjectsStarted;

		m_Filestream << m_StringBuffer.GetString();
		m_Filestream.flush();

		return true;
	}

	bool JSONSerializer::Reset(const std::string& sFilename)
	{
		if (m_NumObjectsStarted != 0)
		{
			SCION_ERROR("Cannot Reset, Document has not been finished.");
			return false;
		}
		if (m_NumArraysStarted != 0)
		{
			SCION_ERROR("Cannot Reser, Too many arrays open.");
			return false;
		}

		if (m_Filestream.is_open())
		{
			m_Filestream.close();
		}

		m_Filestream.open(sFilename, std::ios::out | std::ios::trunc);

		if (!m_Filestream.is_open())
		{
			throw std::runtime_error("failed open file");
		}

		m_pWriter->SetMaxDecimalPlaces(MAX_DECIMA_PLACES);

		return true;
	}

	JSONSerializer& JSONSerializer::StartNewObject(const std::string& key)
	{
		++m_NumObjectsStarted;
		if (!key.empty())
		{
			m_pWriter->Key(key.c_str());
		}

		m_pWriter->StartObject();
		return *this;
	}

	JSONSerializer& JSONSerializer::EndObject()
	{
		--m_NumObjectsStarted;
		m_pWriter->EndObject();
		return *this;
	}

	JSONSerializer& JSONSerializer::StartNewArray(const std::string& key)
	{
		++m_NumArraysStarted;
		m_pWriter->Key(key.c_str());


		m_pWriter->StartArray();
		return *this;
	}

	JSONSerializer& JSONSerializer::EndArray()
	{
		--m_NumArraysStarted;
		m_pWriter->EndArray();
		return *this;
	}

}