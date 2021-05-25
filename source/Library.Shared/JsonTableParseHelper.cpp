#include "pch.h"
#include "JsonTableParseHelper.h"
#include "Scope.h"
#include "Datum.h"
#include "Factory.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(JsonTableParseHelper)
	RTTI_DEFINITIONS(JsonTableParseHelper::SharedData)

	JsonTableParseHelper::SharedData::SharedData(Scope& scope) :
		root(&scope)
	{
	}

	void JsonTableParseHelper::SharedData::Initialize()
	{
		JsonParseCoordinator::SharedData::Initialize();
	}

	void JsonTableParseHelper::Initialize()
	{
		IJsonParseHelper::Initialize();
	}

	void JsonTableParseHelper::Cleanup()
	{
		IJsonParseHelper::Cleanup();
	}

	bool JsonTableParseHelper::StartHandler(JsonParseCoordinator::SharedData& shared, const std::string& key, const Json::Value& value, bool /*is_array_element*/, size_t index)
	{
		JsonTableParseHelper::SharedData* shared_data = shared.As<JsonTableParseHelper::SharedData>();
		if ((!shared_data)) { return false; }

		if (key == class_key)
		{
			assert(stack_frames.IsEmpty() == false);
			stack_frames.Top().class_name = value.asString();
		}
		else if (key == type_key)
		{
			assert(stack_frames.IsEmpty() == false);
			stack_frames.Top().type = (Datum::name_to_type_map.at(value.asString()));
		}
		else if (key == value_key)
		{
			assert(stack_frames.IsEmpty() == false);
			StackFrame& current_frame = stack_frames.Top();

			if (current_frame.type != Datum::DatumTypes::Table)
			{
				Datum& datum = current_frame.scope.Append(current_frame.key);
				datum.SetType(current_frame.type);
				SetDatumValue(datum, value, index);
			}
			else
			{
				if (current_frame.class_name.empty())
				{
					Scope& new_frame = current_frame.scope.AppendScope(current_frame.key);
					StackFrame frame{ key, "Scope", Datum::DatumTypes::Table, new_frame };
					stack_frames.Push(frame);
				}
				else
				{
					Scope* context = Factory<Scope>::Create(current_frame.class_name);
					assert(context != nullptr);
					current_frame.scope.Adopt(*context, current_frame.key);

					StackFrame frame{ key, current_frame.class_name, Datum::DatumTypes::Table, *context };
					stack_frames.Push(frame);
				}
			}
		}
		else
		{
			Scope& scope = stack_frames.IsEmpty() ? *shared_data->root : stack_frames.Top().scope;
			stack_frames.Push({ key, "", Datum::DatumTypes::Unknown, scope });
		}

		return true;
	}

	bool JsonTableParseHelper::EndHandler(JsonParseCoordinator::SharedData& shared, const std::string& key)
	{
		assert(shared.Is(JsonTableParseHelper::SharedData::TypeIdClass()));

#ifdef NDEBUG
		UNREFERENCED_LOCAL(shared);
#endif

		// Same-As Testing
		if (&key == &stack_frames.Top().key)
		{
			stack_frames.Pop();
		}

		return true;
	}

	void JsonTableParseHelper::SetDatumValue(Datum& datum, const Json::Value& value, size_t index)
	{
		if (datum.IsExternal())
		{
			datum.SetFromString(value.asString(), index);
		}
		else
		{
			datum.PushBackFromString(value.asString());
		}
	}
}