#pragma once
#include "RTTI.h"
#include "SList.h"
#include <gsl/gsl>

namespace FieaGameEngine
{
	class IJsonParseHelper;

	/// <summary>
	/// The infrastructure for translating JSON data into Datums and Scopes. Will contain a list of helpers that 
	/// will help iterate through key/value pairs within the JSON for translation. These helpers are added by 
	/// the user prior to parsing. Derives from RTTI.
	/// </summary>
	class JsonParseCoordinator : public RTTI
	{
		RTTI_DECLARATIONS(JsonParseCoordinator, RTTI)

	public:

		/// <summary>
		/// Represents all the data shared between each of the helpers in the list and is meant to be extended 
		/// in each of the helpers. Only one shared data exists on a coordinator at a time, but each of the 
		/// helpers get its own shared data which will contain a vector of its type to store the JSON values.
		/// </summary>
		class SharedData : public RTTI
		{
			friend JsonParseCoordinator;

			RTTI_DECLARATIONS(SharedData, RTTI)

		public:

			/// <summary>
			/// Default constructor for a shareddata
			/// <remarks> Since this class is abstract, this constructor can't be called directly </remarks>
			/// </summary>
			SharedData() = default;
			
			/// <summary>
			/// Default copy constructor for a shareddata
			/// </summary>
			/// <param name="data"> The data to copy </param>
			/// <remarks> Since this class is abstract, this constructor can't be called directly </remarks>
			SharedData(const SharedData& data) = default;
			
			/// <summary>
			/// Default move constructor for a shareddata
			/// </summary>
			/// <param name="data"> The data to move </param>
			/// <remarks> Since this class is abstract, this constructor can't be called directly </remarks>
			SharedData(SharedData&& data) noexcept = default;
			
			/// <summary>
			/// Default copy assignment for a shareddata
			/// </summary>
			/// <param name="data"> The data to copy </param>
			/// <remarks> Since this class is abstract, this assignment can't be called directly </remarks>
			/// <returns> The copied SharedData </returns>
			SharedData& operator=(const SharedData& data) = default;
			
			/// <summary>
			/// Default move assignment for a shareddata
			/// </summary>
			/// <param name="data"> The data to move </param>
			/// <remarks> Since this class is abstract, this assignment can't be called directly </remarks>
			/// <returns> The moved SharedData </returns>
			SharedData& operator=(SharedData&& data) = default;
			
			/// <summary>
			/// Pure virtual destructor meant to be overriden in each helper's class
			/// </summary>
			virtual ~SharedData() = 0 {};

			/// <summary>
			/// Essentially "default constructs" this shareddata. Resets its members to the default
			/// </summary>
			virtual void Initialize();

			/// <summary>
			/// Essentially destructs this shareddata without destroying the object
			/// </summary>
			virtual void Cleanup();

			/// <summary>
			/// Pure virtual method that's meant to to create a shareddata during the Coordinator's clone 
			/// </summary>
			/// <returns> A pointer to the new SharedData </returns>
			virtual gsl::owner<SharedData*> Create() const = 0;

			/// <summary>
			/// Queries the data member coordinator
			/// </summary>
			/// <returns> The address of the JsonParseCoordinator associated with this shareddata </returns>
			JsonParseCoordinator* GetJsonParseCoordinator() { return coordinator; }

			/// <summary>
			/// Queries the data member coordinator
			/// </summary>
			/// <remarks> Const version of the other GetJsonParseCoordinator </remarks>
			/// <returns> The address of the JsonParseCoordinator associated with this shareddata </returns>
			const JsonParseCoordinator* GetJsonParseCoordinator() const { return coordinator; }

			/// <summary>
			/// Indicates that the parser has gone down a layer into the JSON
			/// </summary>
			void IncrementDepth();

			/// <summary>
			/// Indicates that the parser has finished with a layer of the JSON
			/// </summary>
			void DecrementDepth();

			/// <summary>
			/// Queries the data member depth
			/// </summary>
			/// <returns> An indicator of how deep the parser currently is into the JSON object </returns>
			size_t Depth() const { return nesting_depth; }

		private:
			void SetJsonParseCoordinator(JsonParseCoordinator& json_parse_coordinator) { coordinator = &json_parse_coordinator; }

			JsonParseCoordinator* coordinator{ nullptr };
			size_t nesting_depth{ 0 };
		};

		/// <summary>
		/// Deleted default constructor to ensure a ParseCoordinator is created with a SharedData
		/// </summary>
		JsonParseCoordinator() = delete;

		/// <summary>
		/// Default constructor that ensures a ParseCoordinator is created with a SharedData
		/// <param name="new_shared_data"> The SharedData to assign to this coordinator </param>
		/// </summary>
		explicit JsonParseCoordinator(SharedData& new_shared_data);

		/// <summary>
		/// Deleted copy constructor to ensure the user utilized the Clone method instead
		/// <param name="coordinator"> The coordinator to copy </param>
		/// </summary>
		JsonParseCoordinator(const JsonParseCoordinator& other) = delete;

		/// <summary>
		/// Move constructor which creates a new coordinator based on moving the existing other coordinator
		/// </summary>
		/// <param name="other"> The original list to copy</param>
		/// <remarks> Move constructor uses r-values instead of l-values </remarks>
		JsonParseCoordinator(JsonParseCoordinator&& other) noexcept;

		/// <summary>
		/// Deleted copy assignment to ensure the user utilized the Clone method instead
		/// </summary>
		/// <param name = "coordinator"> The coordinator to equate this coordinator to </param>
		/// <returns> The lhs coordinator (this) after equalizing them </returns>
		JsonParseCoordinator& operator=(const JsonParseCoordinator& other) = delete;

		/// <summary>
		/// Move assignment operator for coordinators
		/// </summary>
		/// <param name = "other"> The coordinator to move to the "this" coordinator to </param>
		/// <remarks> Assignment operator uses r-values instead of l-values </remarks>
		/// <returns> The lhs coordinator (this) after moving the "other" coordinator </returns>
		JsonParseCoordinator& operator=(JsonParseCoordinator&& other) noexcept;

		/// <summary>
		/// Destroys only cloned coordinators and allows for default construction of non-cloned coordinators
		/// </summary>
		~JsonParseCoordinator();

		/// <summary>
		/// Essentially "default constructs" this parsecoordinator. Resets its members to default
		/// </summary>
		void Initialize();

		/// <summary>
		/// Essentially destructs this shareddata without destroying the object
		/// </summary>
		void Cleanup();
		/// <summary>
		/// Creates a new heap allocated coordinator based on the target of invocation
		/// </summary>
		/// <remarks> Creates new helpers polymorphically based on the current list of helpers </remarks>
		/// <remarks> Marks the new coordinator as a clone </remarks>
		/// <returns> A pointer to the new list which is heap-allocated </returns>
		JsonParseCoordinator* Clone() const;

		/// <summary>
		/// Adds a helper to the list, as the name suggests
		/// <exception cref="runtime_error"> If the coordinator is cloned, cannot alter the helper list </exception>
		/// </summary>
		void AddHelper(IJsonParseHelper& helper);
		
		/// <summary>
		/// Removes a helper from the list, as the name suggests
		/// <exception cref="runtime_error"> If the coordinator is cloned, cannot alter the helper list </exception>
		/// </summary>
		void RemoveHelper(IJsonParseHelper& helper);

		/// <summary>
		/// Converts a string of JSON data to a Json::Value foir use with the private overload of Parse
		/// </summary>
		void Parse(const std::string& json_string);
		
		/// <summary>
		/// Converts a file (string represented) of JSON data to a Json::Value for use with the private overload of Parse
		/// </summary>
		void ParseFromFile(const std::string& file_name);
		
		/// <summary>
		/// Converts an istream of JSON data to a Json::Value for use with the private overload of Parse
		/// </summary>
		void Parse(std::istream& json_data);

		/// <summary>
		/// Queries the file name 
		/// </summary>
		/// <returns> The final name associated with this coordinator </returns>
		const std::string& GetFileName() const { return parsed_file_name; }

		/// <summary>
		/// Queries the SharedData address
		/// </summary>
		/// <returns> The address of the SharedData associated with this coordinator </returns>
		SharedData* GetSharedData() { return data; }

		/// <summary>
		/// Queries the SharedData address
		/// </summary>
		/// <remarks> Const version of other GetSharedData </remarks>
		/// <returns> The address of the SharedData associated with this coordinator </returns>
		const SharedData* GetSharedData() const { return data; }
		
		/// <summary>
		/// Discards the current SharedData reference and replaces it
		/// </summary>
		/// <param name="new_data"> The new SharedData to set </param>
		/// <exception cref="runtime_error"> If the coordinator is cloned, cannot change the SharedData </exception>
		void SetSharedData(SharedData* new_data);

		/// <summary>
		/// Retrieves the list of helpers on this coordinator
		/// </summary>
		/// <returns> The list of helpers </returns>
		const SList<IJsonParseHelper*>& Helpers() const { return helpers; }

		/// <summary>
		/// Queries the clone indicator on the coordinator
		/// </summary>
		/// <returns> An indicator if this current coordinator is a cloned one or not </returns>
		bool IsClone() const { return is_clone; }

	private:
		void ParseValue(const Json::Value& value, bool is_array_element = false, size_t index = 0) const;
		void ParseKeyValuePair(const std::string& key, const Json::Value& value, const bool is_array_element, size_t index) const;
		void WalkViableHandlers(const std::string& key, const Json::Value& value, const bool is_array_element, const size_t index) const;
		void SetFileName(const std::string& file_name);

		SList<IJsonParseHelper*> helpers;
		SharedData* data{ nullptr };
		std::string parsed_file_name;
		bool is_clone = false;
	};
}