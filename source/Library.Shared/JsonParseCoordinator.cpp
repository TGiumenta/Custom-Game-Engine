#include "pch.h"
#include "JsonParseCoordinator.h"
#include "IJsonParseHelper.h"
#include "json/value.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(JsonParseCoordinator)
	RTTI_DEFINITIONS(JsonParseCoordinator::SharedData)

#pragma region SharedData

	void JsonParseCoordinator::SharedData::Initialize()
	{
		coordinator = nullptr;
		nesting_depth = 0;
	}

	void JsonParseCoordinator::SharedData::Cleanup()
	{
	}

	void JsonParseCoordinator::SharedData::IncrementDepth()
	{
		assert(nesting_depth < std::numeric_limits<size_t>::max());
		++nesting_depth;
	}

	void JsonParseCoordinator::SharedData::DecrementDepth()
	{
		assert(nesting_depth > 0_z);
		--nesting_depth;
	}

#pragma endregion SharedData

	JsonParseCoordinator::JsonParseCoordinator(SharedData& new_shared_data) :
		data(&new_shared_data)
	{
		data->SetJsonParseCoordinator(*this);
	}

	JsonParseCoordinator::JsonParseCoordinator(JsonParseCoordinator&& other) noexcept :
		helpers(std::move(other.helpers)),
		data(other.data),
		parsed_file_name(std::move(other.parsed_file_name)),
		is_clone(other.is_clone)
	{
		other.data = nullptr;
	}

	JsonParseCoordinator& JsonParseCoordinator::operator=(JsonParseCoordinator&& other) noexcept
	{
		if (this != &other)
		{
			helpers = std::move(other.helpers);
			data = other.data;
			parsed_file_name = std::move(other.parsed_file_name);
			is_clone = other.is_clone;

			other.helpers.Clear();
			other.data = nullptr;
		}

		return *this;
	}

	JsonParseCoordinator::~JsonParseCoordinator()
	{
		if (is_clone)
		{
			for (const auto& helper : helpers)
			{
				delete helper;
			}

			delete data;
		}
	}

	void JsonParseCoordinator::Initialize()
	{
		data->Initialize();

		for (IJsonParseHelper* helper : helpers)
		{
			helper->Initialize();
		}

		SetFileName("");
	}

	void JsonParseCoordinator::Cleanup()
	{
		for (IJsonParseHelper* helper : helpers)
		{
			helper->Cleanup();
		}

		data->Cleanup();
	}

	JsonParseCoordinator* JsonParseCoordinator::Clone() const
	{
		SharedData* new_data = data->Create();
		JsonParseCoordinator* new_parse_coordinator = new JsonParseCoordinator(*new_data);

		for (const auto& helper : helpers)
		{
			new_parse_coordinator->AddHelper(*helper->Create());
		}

		new_parse_coordinator->is_clone = true;

		return new_parse_coordinator;
	}

	void JsonParseCoordinator::AddHelper(IJsonParseHelper& helper)
	{
		if (is_clone)
		{
			throw std::runtime_error("Cannot add helpers to a cloned ParseCoordinator.");
		}

		auto it = std::find_if(helpers.begin(), helpers.end(), [&helper](const IJsonParseHelper* h)
		{
			return helper.TypeIdInstance() == h->TypeIdInstance();
		});

		if (it != helpers.end())
		{
			throw std::runtime_error("This helpers or one of its tiype has already been added to this coordinator");
		}

		helpers.PushBack(&helper);
	}

	void JsonParseCoordinator::RemoveHelper(IJsonParseHelper& helper)
	{
		if (is_clone)
		{
			throw std::runtime_error("Cannot remove helpers from a cloned ParseCoordinator.");
		}

		helpers.Remove(&helper);
	}

	void JsonParseCoordinator::SetSharedData(SharedData* new_data)
	{
		if (is_clone)
		{
			throw std::runtime_error("Cannot change the SharedData on a cloned ParseCoordinator.");
		}

		data = new_data;
		data->SetJsonParseCoordinator(*this);
	}

	void JsonParseCoordinator::Parse(const std::string& json_string)
	{
		std::istringstream input(json_string);
		Parse(input);
	}

	void JsonParseCoordinator::ParseFromFile(const std::string& file_name)
	{
		std::ifstream stream(file_name);
		if (!stream.good())
		{
			throw std::runtime_error("Could not open file");
		}

		Parse(stream);
		SetFileName(file_name);
	}

	void JsonParseCoordinator::Parse(std::istream& json_data)
	{
		Initialize();

		Json::Value root;
		json_data >> root;

		ParseValue(root, false);
		Cleanup();
	}

	void JsonParseCoordinator::ParseValue(const Json::Value& value, bool is_array_element, size_t index) const
	{
		if (value.size() > 0)
		{
			data->IncrementDepth();

			const Json::Value::Members keys = value.getMemberNames();
			for (const std::string& key : keys)
			{
				ParseKeyValuePair(key, value[key], is_array_element, index);
			}

			data->DecrementDepth();
		}
	}

	void JsonParseCoordinator::ParseKeyValuePair(const std::string& key, const Json::Value& value, bool is_array_element, size_t index) const
	{
		if (value.isObject())
		{
			data->IncrementDepth();
			for (auto& helper : helpers)
			{
				if (helper->StartHandler(*data, key, value, false, index))
				{
					ParseValue(value, false);
					helper->EndHandler(*data, key);
					break;
				}
			}
			data->DecrementDepth();
		}
		else if (value.isArray())
		{
			size_t index_counter = 0;
			for (const auto& element : value)
			{
				if (element.isObject())
				{
					data->IncrementDepth();
					ParseValue(element, true, index_counter);
					data->DecrementDepth();
				}
				else
				{
					ParseKeyValuePair(key, element, true, index_counter);
				}
				++index_counter;
			}
		}
		else
		{
			WalkViableHandlers(key, value, is_array_element, index);
		}
	}

	void JsonParseCoordinator::WalkViableHandlers(const std::string& key, const Json::Value& value, bool is_array_element, const size_t index) const
	{
		for (auto& helper : helpers)
		{
			if (helper->StartHandler(*data, key, value, is_array_element, index))
			{
				helper->EndHandler(*data, key);
				break;
			}
		}
	}

	void JsonParseCoordinator::SetFileName(const std::string& file_name)
	{
		parsed_file_name = file_name;
	}
}
