#pragma once

#include "IJsonParseHelper.h"
#include "JsonParseCoordinator.h"
#include <gsl/gsl>
#include "Vector.h"

namespace UnitTests
{
	class JsonTestParseHelper final : public FieaGameEngine::IJsonParseHelper
	{
		RTTI_DECLARATIONS(JsonTestParseHelper, FieaGameEngine::IJsonParseHelper)

	public:
		class SharedData final : public FieaGameEngine::JsonParseCoordinator::SharedData
		{
			RTTI_DECLARATIONS(SharedData, FieaGameEngine::JsonParseCoordinator::SharedData)

		public:
			void Initialize() override;
			gsl::owner<SharedData*> Create() const override;

			size_t max_depth = 0;
		};

		void Initialize() override;
		bool StartHandler(
			FieaGameEngine::JsonParseCoordinator::SharedData& shared,
			const std::string& key,
			const Json::Value& value,
			bool is_array_element,
			size_t index) override;
		virtual bool EndHandler(FieaGameEngine::JsonParseCoordinator::SharedData& shared, const std::string& key) override;
		virtual gsl::owner<IJsonParseHelper*> Create() const override;

		std::uint32_t start_handler_count = 0;
		std::uint32_t end_handler_count = 0;
	};
}

