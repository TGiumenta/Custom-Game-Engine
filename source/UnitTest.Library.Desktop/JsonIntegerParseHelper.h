#pragma once

#include "IJsonParseHelper.h"
#include "JsonParseCoordinator.h"
#include <gsl/gsl>
#include "Vector.h"

namespace UnitTests
{
	class JsonIntegerParseHelper final : public FieaGameEngine::IJsonParseHelper
	{
		RTTI_DECLARATIONS(JsonIntegerParseHelper, FieaGameEngine::IJsonParseHelper)

	public:
		class SharedData final : public FieaGameEngine::JsonParseCoordinator::SharedData
		{
			RTTI_DECLARATIONS(SharedData, FieaGameEngine::JsonParseCoordinator::SharedData)

		public:
			void Initialize() override;
			gsl::owner<SharedData*> Create() const override;

			FieaGameEngine::Vector<std::int32_t> data;
		};

		void Initialize() override;
		bool StartHandler(
			FieaGameEngine::JsonParseCoordinator::SharedData& shared,
			const std::string& key,
			const Json::Value& value,
			bool is_array_element,
			size_t index) override;
		virtual bool EndHandler(FieaGameEngine::JsonParseCoordinator::SharedData& shared, const std::string& key) override;
		virtual gsl::owner<FieaGameEngine::IJsonParseHelper*> Create() const override;

	private:
		inline static const std::string integer_key = "Integer";
		bool is_parsing = false;
	};
}

