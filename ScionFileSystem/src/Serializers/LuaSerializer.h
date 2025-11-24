#pragma once
#include <fstream>
#include <prettywriter.h>

namespace SCION_FILESYSTEM {

	class LuaSerializer
	{
	private:

	public:

		LuaSerializer(const std::string& sFilename) {}
		~LuaSerializer() = default;

		void AddComment() {}
	};

}