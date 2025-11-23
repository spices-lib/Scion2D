#pragma once
#include "cmds/CreateTileToolCmds.h"
#include "cmds/RectToolCmds.h"
#include "AbstractCommandManager.h"

namespace SCION_EDITOR {

	using UndoableCommands = VarCommands<CreateTileToolAddCmd, CreateTileToolRemoveCmd, RectToolAddTilesCmd, RectToolRemoveTilesCmd>;

	class CommandManager : public AbstractCommandManager<CreateTileToolAddCmd, CreateTileToolRemoveCmd, RectToolAddTilesCmd, RectToolRemoveTilesCmd>
	{
		
	};
}