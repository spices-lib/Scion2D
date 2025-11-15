#pragma once
#include <vector>
#include <memory>

namespace SCION_EDITOR {

	struct IDisplay
	{
		virtual ~IDisplay() = default;
		virtual void Draw() = 0;
		virtual void Update() {}
	};

	struct DisplayHolder
	{
		std::vector<std::shared_ptr<IDisplay>> displays;
	};
}