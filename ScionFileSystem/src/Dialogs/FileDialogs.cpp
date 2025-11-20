#include "FileDialogs.h"
#include <tinyfiledialogs.h>

namespace SCION_FILESYSTEM {

	std::string FileDialog::OpenFileDialog(const std::string& sTitle, const std::string& sDefaultPath, const std::vector<const char*> filter, const std::string& sFilterDecs)
	{
		const char* file = tinyfd_openFileDialog(sTitle.c_str(), sDefaultPath.c_str(), filter.size(), filter.data(), sFilterDecs.c_str(), 1);
		if (!file)
		{
			return std::string{};
		}

		return std::string{ file };
	}

	std::string FileDialog::SaveFileDialog(const std::string& sTitle, const std::string& sDefaultPath, const std::vector<const char*> filter, const std::string& sFilterDecs)
	{
		const char* file = tinyfd_saveFileDialog(sTitle.c_str(), sDefaultPath.c_str(), filter.size(), filter.data(), sFilterDecs.c_str(), 1);
		if (!file)
		{
			return std::string{};
		}

		return std::string{ file };
	}

	std::string FileDialog::SelectFOLDERDialog(const std::string& sTitle, const std::string& sDefaultPath)
	{
		const char* folder = tinyfd_selectFolderDialog(sTitle.c_str(), sDefaultPath.c_str());
		if (!folder)
		{
			return std::string{};
		}

		return std::string{ folder };
	}

}