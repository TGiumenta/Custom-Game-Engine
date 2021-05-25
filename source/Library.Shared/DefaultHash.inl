namespace FieaGameEngine
{
	const size_t hash_prime = 33;

	inline size_t AdditiveHash(const std::uint8_t* data, const size_t size)
	{
		size_t hash = 0;
		for (size_t index = 0; index < size; ++index)
		{
			hash += hash_prime * data[index];
		}

		return hash;
	}

	template <typename T>
	inline size_t DefaultHash<T>::operator()(const T& key) const
	{
		const std::uint8_t* data = reinterpret_cast<const std::uint8_t*>(&key);		
		return AdditiveHash(data, sizeof(T));
	}

	template <>
	struct DefaultHash<char*>
	{
		inline size_t operator()(char* key) const
		{	
			const std::uint8_t* data = reinterpret_cast<std::uint8_t*>(key);
			return AdditiveHash(data, strlen(key));
		}
	};

	template <>
	struct DefaultHash<const char*>
	{
		inline size_t operator()(const char* key) const
		{
			const std::uint8_t* data = reinterpret_cast<const std::uint8_t*>(key);
			return AdditiveHash(data, strlen(key));
		}
	};

	template <>
	struct DefaultHash<char* const>
	{
		inline size_t operator()(char* const key) const
		{
			const std::uint8_t* data = reinterpret_cast<const std::uint8_t*>(key);
			return AdditiveHash(data, strlen(key));
		}
	};

	template <>
	struct DefaultHash<const char* const>
	{
		inline size_t operator()(const char* const key) const
		{
			const std::uint8_t* data = reinterpret_cast<const std::uint8_t*>(key);
			return AdditiveHash(data, strlen(key));
		}
	};

	template <>
	struct DefaultHash<std::string>
	{
		inline size_t operator()(const std::string& key) const
		{
			const std::uint8_t* data = reinterpret_cast<const std::uint8_t*>(key.c_str());
			return AdditiveHash(data, key.length());
		}
	};

	template <>
	struct DefaultHash<std::wstring>
	{
		inline size_t operator()(const std::wstring& key) const
		{
			const std::uint8_t* data = reinterpret_cast<const std::uint8_t*>(key.c_str());
			return AdditiveHash(data, key.length());
		}
	};

	template <>
	struct DefaultHash<int>
	{
		inline size_t operator()(const int key) const
		{
			return static_cast<size_t>(key);
		}
	};
}