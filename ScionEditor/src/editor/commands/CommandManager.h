#pragma once
#include "cmds/CreateTileToolCmds.h"
#include "AbstractCommandManager.h"

namespace SCION_EDITOR {

	using UndoableCommands = VarCommands<CreateTileToolAddCmd>;

	class CommandManager : public AbstractCommandManager<CreateTileToolAddCmd>
	{
		
	};
}