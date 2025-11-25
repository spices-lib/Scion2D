#pragma once
#include <string>

namespace SCION_CORE {

	struct SaveProject
	{
		std::string sProjectName{ "" };
		std::string sProjectPath{ "" };
		std::string sProjectFilePath{ "" };
		std::string sMainLuaScript{ "" };
	};

}