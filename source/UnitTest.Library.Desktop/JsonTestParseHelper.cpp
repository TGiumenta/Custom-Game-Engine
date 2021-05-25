#include "pch.h"
#include "JsonTestParseHelper.h"

namespace UnitTests
{
	RTTI_DEFINITIONS(JsonTestParseHelper)
	RTTI_DEFINITIONS(JsonTestParseHelper::SharedData)

	using namespace FieaGameEngine;

	void JsonTestParseHelper::SharedData::Initialize()
	{
		JsonParseCoordinator::SharedData::Initialize();
		max_depth = 0;
	}

	gsl::owner<JsonTestParseHelper::SharedData*> JsonTestParseHelper::SharedData::Create() const
	{
		return new JsonTestParseHelper::SharedData();
	}

	void JsonTestParseHelper::Initialize()
	{
		IJsonParseHelper::Initialize();
		start_handler_count = 0;
		end_handler_count = 0;
	}

	bool JsonTestParseHelper::StartHandler(JsonParseCoordinator::SharedData& shared, const std::string&, const Json::Value&, bool, size_t)
	{
		JsonTestParseHelper::SharedData* shared_data = shared.As<JsonTestParseHelper::SharedData>();

		if ((!shared_data)) { return false; }

		++start_handler_count;

		if (shared_data->Depth() > shared_data->max_depth)
		{
			shared_data->max_depth = shared_data->Depth();
		}

		return true;
	}

	bool JsonTestParseHelper::EndHandler(JsonParseCoordinator::SharedData& shared, const std::string&)
	{
		if (!shared.Is(JsonTestParseHelper::SharedData::TypeIdClass())) { return false; }

		++end_handler_count;

		return true;
	}

	gsl::owner<IJsonParseHelper*> JsonTestParseHelper::Create() const
	{
		return new JsonTestParseHelper();
	}
}