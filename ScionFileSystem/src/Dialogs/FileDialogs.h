#pragma once
#include <string>
#include <vector>

namespace SCION_FILESYSTEM {

	class FileDialog
	{
	public:

		std::string OpenFileDialog(const std::string& sTitle = "Open", const std::string& sDefaultPath = "",
			const std::vector<const char*> filter = {}, const std::string& sFilterDecs = "");

		std::string SaveFileDialog(const std::string& sTitle = "Save", const std::string& sDefaultPath = "",
			const std::vector<const char*> filter = {}, const std::string& sFilterDecs = "");

		std::string SelectFOLDERDialog(const std::string& sTitle = "Save", const std::string& sDefaultPath = "");

	private:
	};
}