#pragma once

#include <exception>
#include <chrono>

namespace FieaGameEngine
{
	class GameTime;

	class GameClock final
	{
	public:
		GameClock();
		GameClock(const GameClock&) = default;
		GameClock& operator=(const GameClock&) = default;
		GameClock(GameClock&&) = default;
		GameClock& operator=(GameClock&&) = default;
		~GameClock() = default;

		const std::chrono::high_resolution_clock::time_point& StartTime() const { return start_time; }
		const std::chrono::high_resolution_clock::time_point& CurrentTime() const { return current_time; }
		const std::chrono::high_resolution_clock::time_point& LastTime() const { return last_time; }

		void Reset();
		void UpdateGameTime(GameTime& gameTime);

	private:
		std::chrono::high_resolution_clock::time_point start_time;
		std::chrono::high_resolution_clock::time_point current_time;
		std::chrono::high_resolution_clock::time_point last_time;
	};
}