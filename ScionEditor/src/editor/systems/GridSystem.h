#pragma once
#include <memory>

namespace SCION_RENDERING {
	class RectBatchRenderer;
	class Camera2D;
}

namespace SCION_EDITOR {

	class GridSystem
	{
	private:

		std::unique_ptr<SCION_RENDERING::RectBatchRenderer> m_pBatchRenderer;

	public:

		GridSystem();
		~GridSystem() = default;

		void Update(SCION_RENDERING::Camera2D& camera);
	};
	
}
