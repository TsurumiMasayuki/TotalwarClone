#pragma once

namespace Game
{
	enum class GameState
	{
		PreparePhase,
		CombatPhaseBegin,
		CombatPhase,
		Result
	};

	extern GameState g_GameState;
}