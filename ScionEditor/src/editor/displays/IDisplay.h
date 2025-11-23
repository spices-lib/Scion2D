#pragma once
#include <vector>
#include <memory>

namespace SCION_EDITOR {

	struct IDisplay
	{
	public:

		virtual ~IDisplay() = default;
		virtual void Draw() = 0;
		virtual void Update() {}

	protected:
		virtual void DrawToolbar() {}
	};

	struct DisplayHolder
	{
		std::vector<std::shared_ptr<IDisplay>> displays;
	};
}