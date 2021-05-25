#pragma once

#include "IJsonParseHelper.h"
#include "JsonParseCoordinator.h"
#include "Stack.h"
#include "Datum.h"

namespace FieaGameEngine
{
	class Scope;
	class Datum;

	/// <summary>
	/// Will specifically determine the logic for how data is parsed through the start and 
	/// end handlers. Reads in some json information, and based on its key, interacts with 
	/// the engine to create classes at runtime based on string name.
	/// stack_frames refers to the stack of stackframes that contains all information about the scopes being processed
	/// is_parsing refers to if the parser is actively parsing an element as to not overwrite a scope
	/// </summary>
	class JsonTableParseHelper final : public IJsonParseHelper
	{
		RTTI_DECLARATIONS(JsonTableParseHelper, IJsonParseHelper)

	public:
		/// <summary>
		/// Keeps track of the root scope and therefore all of its children
		/// root refers to the base of the scope tree
		/// </summary>
		class SharedData final : public JsonParseCoordinator::SharedData
		{
			RTTI_DECLARATIONS(SharedData, JsonParseCoordinator::SharedData)

		public:
			/// <summary>
			/// Default constructor for class initialization
			/// <remarks> Deleted to prevent the user from creating a shareddata without the root scope </remarks>
			/// </summary>
			SharedData() = delete;

			/// <summary>
			/// Default constructor for a shareddata for class initialization
			/// </summary>
			/// <param name="scope"> The root scope to set </param>
			explicit SharedData(Scope& scope);

			/// <summary>
			/// Copy constructor which creates a new shareddata based on the existing other shareddata
			/// </summary>
			/// <param name="other"> The original shareddata to copy </param>
			SharedData(const SharedData& other) = default;

			/// <summary>
			/// Copy constructor which creates a new shareddata based on the existing other shareddata
			/// </summary>
			/// <param name="other"> The original shareddata to copy </param>
			/// <remarks> Copy constructor uses rvalues instead of lvalues </remarks>
			SharedData(SharedData&& other) noexcept = default;

			/// <summary>
			/// Default equality operator for shareddatas
			/// </summary>
			/// <param name = "other"> The shareddata to equate this shareddata to </param>
			/// <returns> The lhs shareddata (this) after equalizing them </returns>
			SharedData& operator=(const SharedData& other) = default;

			/// <summary>
			/// Default equality operator for shareddatas
			/// </summary>
			/// <param name = "other"> The shareddata to equate this shareddata to </param>
			/// <remarks> Equality operator uses rvalues instead of lvalues </remarks>
			/// <returns> The lhs shareddata (this) after equalizing them </returns>
			SharedData& operator=(SharedData&& other) = default;

			/// <summary>
			/// Defaulted destructor
			/// </summary>
			~SharedData() = default;

			/// <summary>
			/// Essentially "default constructs" this shareddata, resets member variables
			/// </summary>
			void Initialize() override;
			
			/// <summary>
			/// Creates a new parsehelper on the heap
			/// </summary>
			/// <returns> A pointer to the new heap allocated shareddata, meant to be deleted by the caller </returns>
			gsl::owner<SharedData*> Create() const override { return new JsonTableParseHelper::SharedData(*root); }

			Scope* root = nullptr;
		};

		/// <summary>
		/// Essentially "default constructs" this parsehelper, resetting its members
		/// </summary>
		void Initialize() override;

		/// <summary>
		/// Cleans up the parsehelper based on how it was initialized
		/// </summary>
		void Cleanup() override;

		/// <summary>
		/// Handles the actual parsing of the value parameter and handles stack frame logic
		/// </summary>
		/// <param name="shared"> The shareddata for this object being parsed </param>
		/// <param name="key"> The key of the json being processed </param>
		/// <param name="value"> The value of the json being processed </param>
		/// <param name="is_array_element"> Determines if the element is part of an array or not </param>
		/// <param name="index"> The index of the potential array position </param>
		bool StartHandler(
			JsonParseCoordinator::SharedData& shared,
			const std::string& key,
			const Json::Value& value,
			bool is_array_element,
			size_t index) override;
		
		/// <summary>
		/// Pops the top stack frame off because the key/value is processed by this point
		/// </summary>
		/// <param name="shared"> The shareddata for this object being parsed </param>
		/// <param name="key"> The key of the json being processed </param>
		virtual bool EndHandler(JsonParseCoordinator::SharedData& shared, const std::string& key) override;
		
		/// <summary>
		/// Creates a new parsehelper on the heap
		/// </summary>
		/// <returns> A pointer to the new heap allocated parsehelper, meant to be deleted by the caller </returns>
		virtual gsl::owner<IJsonParseHelper*> Create() const override { return new JsonTableParseHelper(); }

	private:
		void SetDatumValue(Datum& datum, const Json::Value& value, size_t index);

		inline static const std::string class_key = "class";
		inline static const std::string type_key = "type";
		inline static const std::string value_key = "value";

		struct StackFrame
		{
			const std::string& key;
			std::string class_name;
			Datum::DatumTypes type;
			Scope& scope;
		};

		Stack<StackFrame> stack_frames;

		bool is_parsing = false;
	};
}