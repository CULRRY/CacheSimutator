#pragma once
#include <list>
#include <unordered_map>

using uint64 = unsigned __int64;
class Cache
{
	static constexpr int TOTAL_CACHE_BYTE			= 384 * 1024;
	static constexpr int NUM_OF_CORES				= 6;
	static constexpr int NUM_OF_WAYS				= 8;
	static constexpr int BYTE_SIZE_OF_CACHE_LINE	= 64;
	static constexpr int NUM_OF_INDEX_PER_CACHE		= TOTAL_CACHE_BYTE / NUM_OF_CORES / NUM_OF_WAYS / BYTE_SIZE_OF_CACHE_LINE / 2;

	enum : uint64
	{
		OFFSET_MASK		= BYTE_SIZE_OF_CACHE_LINE - 1,
		INDEX_MASK		= (NUM_OF_INDEX_PER_CACHE - 1) << 6,
		TAG_MASK		= ~(OFFSET_MASK + INDEX_MASK),
	};


	struct CacheData
	{
		std::unordered_map<uint64, std::list<uint64>::const_iterator> LRUScheduler;
		std::list<uint64> tags;
	};


public:
	Cache()
	{
		_data = new CacheData[NUM_OF_INDEX_PER_CACHE];
	}

	~Cache()
	{
		delete _data;
	}

	void Flush()
	{
		for (int i = 0; i < NUM_OF_INDEX_PER_CACHE; i++)
		{
			_data->tags.clear();
		}
	}



	template<typename T>
	bool Access(const T* data)
	{

		const uint64 dataAddress = reinterpret_cast<uint64>(data);
		uint64 index = (dataAddress & INDEX_MASK) >> 6;
		uint64 offset = dataAddress & OFFSET_MASK;
		uint64 tag = (dataAddress & TAG_MASK) >> 12;

		CacheData& chunk = _data[index];

		//const auto item = chunk.LRUScheduler.find(tag);

		//if (item != chunk.LRUScheduler.end())
		//{
		//	const auto it = item->second;
		//	chunk.tags.push_back(*it);
		//	chunk.tags.erase(it);
		//	//chunk.LRUScheduler[tag] = std::prev(chunk.tags.end());

		//	for (auto it = chunk.tags.begin(); it != chunk.tags.end(); ++it)
		//	{
		//		printf("%lld\n", *it);
		//	}
		//	printf("\n");

		//	return true;
		//}

		for (auto it = chunk.tags.begin(); it != chunk.tags.end(); ++it)
		{
			if (tag == *it)
			{
				chunk.tags.push_back(*it);
				chunk.tags.erase(it);

				for (auto it = chunk.tags.begin(); it != chunk.tags.end(); ++it)
				{
					printf("%lld\n", *it);
				}
				printf("\n");

				return true;
			}
		}


		chunk.tags.push_back(tag);
		//chunk.LRUScheduler[tag] = std::prev(chunk.tags.end());
		if (chunk.tags.size() > NUM_OF_WAYS)
		{
			//chunk.LRUScheduler.erase(chunk.tags.front());
			chunk.tags.pop_front();
		}

		for (auto it = chunk.tags.begin(); it != chunk.tags.end(); ++it)
		{
			printf("%lld\n", *it);
		}
		printf("\n");

		return false;
		
	}


private:
	CacheData* _data;
};

