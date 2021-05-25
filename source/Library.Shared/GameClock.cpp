#include "pch.h"
#include "GameClock.h"
#include "GameTime.h"

using namespace std::chrono;

namespace FieaGameEngine
{
    GameClock::GameClock()
	{
        Reset();
    }

    void GameClock::Reset()
    {
		start_time = high_resolution_clock::now();
        current_time = start_time;
        last_time = current_time;
    }

    void GameClock::UpdateGameTime(GameTime& gameTime)
    {
        current_time = high_resolution_clock::now();

		gameTime.SetCurrentTime(current_time);
		gameTime.SetTotalGameTime(duration_cast<milliseconds>(current_time - start_time));
		gameTime.SetElapsedGameTime(duration_cast<milliseconds>(current_time - last_time));
        last_time = current_time;
    }
}