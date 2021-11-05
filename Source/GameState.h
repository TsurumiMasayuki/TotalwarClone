#pragma once

namespace Game
{
	enum class GameState
	{
		PreparePhase,
		CombatPhaseBegin,
		CombatPhase
	};

	extern GameState g_GameState;
}