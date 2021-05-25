#include "pch.h"
#include "JsonIntegerParseHelper.h"

namespace UnitTests
{
	RTTI_DEFINITIONS(JsonIntegerParseHelper)
	RTTI_DEFINITIONS(JsonIntegerParseHelper::SharedData)

	using namespace FieaGameEngine;

	void JsonIntegerParseHelper::SharedData::Initialize()
	{
		JsonParseCoordinator::SharedData::Initialize();
		data.Clear();
	}

	gsl::owner<JsonIntegerParseHelper::SharedData*> JsonIntegerParseHelper::SharedData::Create() const
	{
		return new JsonIntegerParseHelper::SharedData();
	}

	void JsonIntegerParseHelper::Initialize()
	{
		IJsonParseHelper::Initialize();
		is_parsing = false;
	}

	bool JsonIntegerParseHelper::StartHandler(JsonParseCoordinator::SharedData& shared, const std::string& key, const Json::Value& value, bool, size_t)
	{
		JsonIntegerParseHelper::SharedData* shared_data = shared.As<JsonIntegerParseHelper::SharedData>();

		if ((!shared_data) || (key != integer_key))
		{
			return false;
		}

		if (is_parsing)
		{
			throw std::runtime_error("Already parsing integer.");
		}

		is_parsing = true;
		shared_data->data.PushBack(value.asInt());
		return true;
	}

	bool JsonIntegerParseHelper::EndHandler(JsonParseCoordinator::SharedData& shared, const std::string& key)
	{
		JsonIntegerParseHelper::SharedData* shared_data = shared.As<JsonIntegerParseHelper::SharedData>();

		if ((!shared_data)|| (key != integer_key) || (!is_parsing))
		{
			return false;
		}

		is_parsing = false;
		return true;
	}

	gsl::owner<IJsonParseHelper*> JsonIntegerParseHelper::Create() const
	{
		return new JsonIntegerParseHelper();
	}
}