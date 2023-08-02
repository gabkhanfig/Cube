#pragma once

#include <concepts>
#include "Hash.h"
#include "../Array/DynamicArray.h"

template<typename T>
concept Hashable = requires(T a)
{
	{ gk::hash<T>(a) } -> std::convertible_to<uint64>;
};

namespace gk 
{
	template<typename K, typename V>
	struct Pair
	{
		K _key;
		V _value;
		Pair(const K& key, const V& value) : _key(key), _value(value) {}
	};



	/** Key is required to be hashable. To make it hashable, the gk::hash function must be specialized. Example:
	* template<>
	* constexpr size_t gk::hash<int>(int key) { return size_t(key); }
	* This allows the key type to be used in the hashmap.
	*/
	template<typename Key, typename Value>
	requires Hashable<Key>
	struct Hashmap
	{
	public:

		struct Bucket {
			gk::darray<Pair<Key, Value>*> data;
		};


		Hashmap() {
			for (uint64 i = 0; i < 8; i++) {
				buckets.Add(Bucket());
			}
		}

		void Insert(const Key& key, const Value& value) {
			const uint64 hash = gk::hash(key);
			const uint64 bucketToDo = hash % GetBucketCount();
			buckets[bucketToDo].data.Add(new Pair(key, value));
		}

		Value* Find(const Key& key) {
			const uint64 hash = gk::hash(key);
			const uint64 bucketToCheck = hash % GetBucketCount();
			gk::darray<Pair<Key, Value>*> & = buckets[bucketToCheck].data;
		
		}



		gk::darray<Bucket> buckets;

		inline uint64 GetBucketCount() const { return buckets.Size(); }








	};

}









