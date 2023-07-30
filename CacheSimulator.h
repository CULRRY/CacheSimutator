#pragma once

#include <iostream>
#include "Cache.h"
#include <unordered_map>




class CacheSimulator
{


public:

	void Flush()
	{
		_cache.Flush();
		_totalAccessCount = 0;
		_totalHitCount = 0;
		_accessCountPerAddress.clear();
		_hitCountPerAddress.clear();
	}

	template <typename T>
	bool Access(const T& data)
	{

		_totalAccessCount++;
		++_accessCountPerAddress[reinterpret_cast<uint64>(&data)];
		if (_cache.Access(&data))
		{
			_totalHitCount++;
			++_hitCountPerAddress[reinterpret_cast<uint64>(&data)];
			return true;
		}

		return false;

	}

	void PrintTotalHitRate()
	{
		printf("%d / %d, HitRate: %lf\n", _totalHitCount, _totalAccessCount, (double)_totalHitCount / _totalAccessCount * 100);
	}

	template <typename T>
	void PrintHitRate(const T& data)
	{
		const auto accessCountIter = _accessCountPerAddress.find(reinterpret_cast<uint64>(&data));
		auto hitCountIter = _hitCountPerAddress.find(reinterpret_cast<uint64>(&data));

		if (hitCountIter == _hitCountPerAddress.end())
		{
			_hitCountPerAddress[reinterpret_cast<uint64>(&data)] = 0;
			hitCountIter = _hitCountPerAddress.find(reinterpret_cast<uint64>(&data));
		}

		if (accessCountIter == _accessCountPerAddress.end())
		{
			printf("0x%p, 0 / 0, HitRate: 0%%\n", &data);
		}
		else
		{
			printf("0x%p, %lld / %lld, HitRate: %lf%%\n", &data, hitCountIter->second, accessCountIter->second, static_cast<double>(hitCountIter->second) / accessCountIter->second * 100);
		}
	}

private:
	std::unordered_map<uint64, uint64> _accessCountPerAddress;
	std::unordered_map<uint64, uint64> _hitCountPerAddress;

	int _totalAccessCount	= 0;
	int _totalHitCount		= 0;
	Cache _cache;



};

