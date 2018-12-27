#pragma once

#include "Priv/TRBTreeImpl.h"
#include "UT/TKeyValueIterator.h"
#include "../Cont/TVector.h"
#include "../Templ/TComparer.h"
#include <boost/serialization/split_member.hpp>

/**
* TODO First:
* 1. Remove (by key).
* 1.1. Write more RB-Tree complex tests
*
* TODO Remove extra RARE cases debug:
* 1. Case 1: Fill container with TEN(10) elements, remove FOURTH (4) our SIXTH(6).
* 2. Case 2: Fill container with TEN(10) elements, remove FIFTH (5).
* 3. Case 3: Fill container with THIRTY(30) elements, remove TWENTY-FIVEth (25).
* 4. Case 4: Fill container with FIFTY(50) elements, remove TWENTY-FIVEth (25).
*
* TODO Constructors:
* 1. From other tree
*
* TODO Getters
* 1. operator[] 
* 2. Get with moving KeyValue pair (actually removing)
* 3. Get with moving value only
* 4. Get pointer to KeyValue for the given key.
*
* TODO Setters
* 1. Setter
*
* TODO Output
* 1. Debug print
*
* CopyConstruction/Asignment
* 1. Move copy-construction/assignment
*
* CopyTo/CopyUnorderedTo functions:
* 1. CopyTo(TArray) function.
* 2. We need Move versions of this functions (i.e. moving entire tree to C-array).
*
* TODO Remove by moving
* 1. We must destruct and mark removed the node that the key was moved to in any case
* (to avoid in to be enumerated again when using the CopyUnorderedTo etc.)
*
* TODO Adding interface:
* 1. Hint iterator position
* 2. Emplace
*
* TODO Key/Value iterator:
* 1. Insertion while iterating
* 2. Backward iteration support:
* 2.1. Function AtLast
* 2.2. Getters (ReverseIterator)
*
* TODO Key only iterator:
* 1. 
*
* TODO Traverse:
* 1. Create the Stack-overflow unit-test for traverse;
*
* TODO:
* 1. Move key only
* 2. Move value only
*
* TODO:
* 1. Checked find functions.
* 2. FindRef functions.
* 3. FindOrDefault.
*
* TODO Extra Remove:
* 1. FindAndRemoveChecked
* 2. RemoveAndCopyValue
*
* TODO Set operations:
* 1. Intersect:
* 1.1. CopyIntersect (DONE)
* 2. Union
* 3. Difference
*
* TODO Not Yet Impl:
* 1) FindIteratorForInRange Impl
* 2) AppendUnionTo
* 3) AppendDifferenceTo
* 3.1.) AppendRangeEnd (+CODED)
* 3.2.) AddEnd (+CODED)
* 3.2.1) AddEndImpl (+CODED)
* 3.2.1.1) AddAfterImpl (+CODED)
* 4) AddHintImpl
*/

/**
* Red-black tree.
*
* WARNING!!! This implementation does NOT automatically free the node memory.
*/
template<class KVTypeArg, class ComparerArg = TComparer<typename KVTypeArg::KeyType>>
class TRBTree
{
public:
	static_assert(IsKVType<KVTypeArg>::Value, "TRBTree: KVTypeArg must be KVType");

	friend boost::serialization::access;

	/**
	* Any TRBTree instance is to be treated as a friend to make it possible to optimize some operations.
	*/
	friend class TRBTree;

	/**
	* KeyValue
	*/
	using KeyValueType = typename TKeyValue<KVTypeArg>;

	/**
	* Element is always const.
	*/
	using ElementType = const KeyValueType;

	/**
	* Key type.
	*/
	using KeyType = typename KeyValueType::KeyType;

	/**
	* Value type.
	*/
	using ValueType = typename KeyValueType::ValueType;

private:
	static auto GetThisTypeHelper()->std::remove_reference_t<decltype(*this)>;

public:
	/**
	* Type of this tree.
	*/
	using ThisType = decltype(GetThisTypeHelper());

	/**
	* Iterator type.
	*/
	template<class TreeTypeArg>
	class TGeneralIterator;

	using IteratorType = TGeneralIterator<ThisType>;
	using ConstIteratorType = TGeneralIterator<const ThisType>;

	using KeyIteratorType = TForwardKeyIterator<ConstIteratorType, /*bExposeIterator*/true>;

	/**
	* Capacity to be used for the buffer by default.
	*/
	constexpr static int DEFAULT_CAPACITY = 1024;

	/**
	* Constructs with the default capacity.
	*/
	TRBTree() : TRBTree{ DEFAULT_CAPACITY } {}

	/**
	* Constructs with the given desired capacity.
	*/
	explicit TRBTree(int InDesiredCapacity) :
		Buffer(InDesiredCapacity, EForceInitCapacity::Value )
	,	RootIdx{ INDEX_NONE }
	,	Count{ 0 } {}

	/**
	* Constructs as a copy of another container.
	*/
	TRBTree(const ThisType& InOther) :
		Buffer( InOther.Count, EForceInitCapacity::Value )
	,	RootIdx{ INDEX_NONE }
	,	Count{ 0 }
	{
		Append(InOther);
	}

	/**
	* Constructs as a copy of another container.
	*/
	template<class OtherComparerArg>
	TRBTree(const TRBTree<KVTypeArg, OtherComparerArg>& InOther) :
		Buffer( InOther.Count, EForceInitCapacity::Value )
	,	RootIdx{ INDEX_NONE }
	,	Count{ 0 }
	{
		Append(InOther);
	}

	/**
	* Copy-assigns another container.
	*/
	TRBTree& operator=(const ThisType& InOther)
	{
		Clear();
		Append(InOther);
		return *this;
	}

	/**
	* Copy-assigns another container.
	*/
	template<class OtherComparerArg>
	TRBTree& operator=(const TRBTree<KVTypeArg, OtherComparerArg>& InOther)
	{
		Clear();
		Append(InOther);
		return *this;
	}

	/**
	* Move-constructs.
	*/
	TRBTree(TRBTree&& InOther) :
		Count(InOther.Count)
	,	RootIdx(InOther.RootIdx)
	,	Buffer(std::move(InOther.Buffer))
	{
		// @TODO: Refactor: Code duplication
		InOther.Count = 0;
		InOther.RootIdx = INDEX_NONE;
	}

	/**
	* Move-assigns another container.
	*/
	TRBTree& operator=(ThisType&& InOther)
	{
		Count = InOther.Count;
		RootIdx = InOther.RootIdx;
		Buffer = std::move(InOther.Buffer);

		// @TODO: Refactor: Code duplication
		InOther.Count = 0;
		InOther.RootIdx = INDEX_NONE;

		return *this;
	}

	/**
	* Move-assigns another container.
	*/
	template<class OtherComparerArg>
	TRBTree& operator=(TRBTree<KVTypeArg, OtherComparerArg>&& InOther)
	{
		Clear();
		Append(std::move(InOther));
		return *this;
	}

	/**
	* Move-constructs.
	*/
	template<class OtherComparerArg>
	TRBTree(TRBTree<KVTypeArg, OtherComparerArg>&& InOther) :
		RootIdx{ INDEX_NONE }
	,	Count{ 0 }
	{
		Append(std::move(InOther));
	}

	/**
	* Count of nodes.
	*/
	__forceinline int Num() const { return Count; }

	/**
	* Returns true if buffer is empty.
	*/
	__forceinline bool Empty() const { return 0 == Count; }


	/**
	* Returns iterator to the first Key/Value pair.
	*/
	ConstIteratorType Iterator() const
	{
		return GetFirstIteratorImpl<ConstIteratorType>(this);
	}

	/**
	* Returns iterator to the first Key/Value pair.
	*/
	IteratorType Iterator()
	{
		return GetFirstIteratorImpl<IteratorType>(this);
	}

	/**
	* Returns end iterator.
	*/
	IteratorType EndIterator()
	{
		return IteratorType::EndIterator(this);
	}

	/**
	* Returns const iterator to the first Key/Value pair.
	*/
	ConstIteratorType ConstIterator() const
	{
		return GetFirstIteratorImpl<ConstIteratorType>(this);
	}

	/**
	* Returns const end iterator.
	*/
	ConstIteratorType ConstEndIterator() const
	{
		return ConstIteratorType::EndIterator(this);
	}

	/**
	* Returns const end iterator.
	*/
	ConstIteratorType EndIterator() const
	{
		return ConstEndIterator();
	}

	/**
	* C++ range iteration support.
	*/
	IteratorType begin() { return Iterator(); }

	/**
	* C++ range iteration support.
	*/
	ConstIteratorType begin() const { return ConstIterator(); }

	/**
	* C++ range iteration support.
	*/
	ConstIteratorType cbegin() const { return begin(); }

	/**
	* C++ range iteration support.
	*/
	IteratorType end() { return EndIterator(); }

	/**
	* C++ range iteration support.
	*/
	ConstIteratorType end() const { return ConstEndIterator(); }

	/**
	* C++ range iteration support.
	*/
	ConstIteratorType cend() const { return end(); }

	/**
	* Returns key iterator to the first element.
	*/
	KeyIteratorType KeyIterator() const { return KeyIteratorType{ Iterator() }; }

	/**
	* Returns key iterator to the last element.
	*/
	KeyIteratorType EndKeyIterator() const { return KeyIteratorType{ EndIterator() }; }

	/**
	* Returns KeyValue with a minimal key.
	* Container must be NON-empty.
	*/
	const KeyValueType& Min() const
	{
		BOOST_ASSERT_MSG( ! Empty(), "TRBTree::Min: Container must be NON-empty");
		return GetNode(GetDeepestNodeRef(TRBTreeImpl::ChildNodeRef::RootNode(), TRBTreeImpl::LEFT_CHILD_IDX))->GetKV();
	}

	/**
	* Returns KeyValue with a maximal key.
	* Container must be NON-empty.
	*/
	const KeyValueType& Max() const
	{
		BOOST_ASSERT_MSG( ! Empty(), "TRBTree::Max: Container must be NON-empty");
		return GetNode(GetDeepestNodeRef(TRBTreeImpl::ChildNodeRef::RootNode(), TRBTreeImpl::RIGHT_CHILD_IDX))->GetKV();
	}

	/**
	* Returns minimal key.
	* Container must be NON-empty.
	*/
	const KeyType& MinKey() const
	{
		return Min().Key;
	}

	/**
	* Returns maximal key.
	* Container must be NON-empty.
	*/
	const KeyType& MaxKey() const
	{
		return Max().Key;
	}

	/**
	* boost::serialization support.
	*/
	template<class Archive>
	void save(Archive& Ar, const unsigned int Version) const
	{
		Ar << Count;
		for (const KeyValueType& KV : *this)
		{
			Ar << KV;
		}
	}

	template<class Archive>
	void load(Archive& Ar, const unsigned int Version)
	{
		Clear();
		// @TODO: Optimize
		int32_t CountToLoad = 0;
		Ar >> CountToLoad;
		for (int i = 0; i < CountToLoad; i++)
		{
			KeyValueType KV;
			Ar >> KV;
			bool bAdded = Add(KV.Key, KV.Value);
			BOOST_ASSERT_MSG(bAdded, "TRBTree: load: adding deserialized value must always succeed");
		}
	}

	BOOST_SERIALIZATION_SPLIT_MEMBER()


	/**
	* Count bytes needed to store this class with overhead of capacity.
	* Include both the class layout and dynamic memory.
	*/
	int32_t CountTotalBytes() const
	{
		return sizeof(*this) + ElementSize() * Buffer.MaxLen();
	}

	/**
	* Count bytes needed to store only the minimum size required to store the given buffer.
	*/
	int32_t CountMinimumSizeBytes() const
	{
		return sizeof(*this) + ElementSize() * Count;
	}

	/**
	* Counts the overhead of capacity.
	*/
	int32_t CountCapacityOverhead() const
	{
		return CountTotalBytes() - CountMinimumSizeBytes();
	}

	/**
	* Count only bytes that are to be serialized.
	*/
	int32_t CountSerializeBytes() const
	{
		return sizeof(Count) + ElementSize() * Count;
	}

	/**
	* Size of a single element instance.
	* NOTE: For pointers accounts only pointer size and not the size of pointed-to content.
	*/
	static constexpr int32_t ElementSize()
	{
		return sizeof(NodeType);
	}

	/**
	* Clears the container.
	*/
	void Clear()
	{
		// NOTE: We must clear the buffer, because node may contain non-POD types.
		Buffer.Clear();
		RootIdx = 0;
		Count = 0;
	}

	/**
	* Makes the buffer capable to contain at least the given amount of elements.
	*/
	void ReserveGrow(int InNewCapacity)	
	{
		Buffer.ReserveGrow(InNewCapacity);
	}

	/**
	* Frees extra space. 
	* Optionally rearranges the elements in the buffer, so they are linear in memory.
	*/
	void ShrinkToFit()
	{
		// @TODO
		Buffer.ShrinkToFit();
	}

	/**
	* Counts all elements for those predicate returns true.
	*
	* @UNIT-TESTED
	*/
	template<class Pred>
	int32_t CountPredicate(const Pred& P) const
	{
		return CountPredicate_InRange(P, Iterator(), EndIterator());
	}

	/**
	* Counts all keys for those predicate returns true.
	*
	* @UNIT-TESTED
	*/
	template<class Pred>
	int32_t CountKeyPredicate(const Pred& P) const
	{
		return CountKeyPredicate_InRange(P, Iterator(), EndIterator());
	}

	/**
	* Counts all values for those predicate returns true.
	*
	* @UNIT-TESTED
	*/
	template<class Pred>
	int32_t CountValuePredicate(const Pred& P) const
	{
		return CountValuePredicate_InRange(P, Iterator(), EndIterator());
	}

	/**
	* Counts all values equal to the given one.
	*
	* @UNIT-TESTED
	*/
	int32_t CountValues(const ValueType& InValue) const
	{
		return CountValues_InRange(InValue, Iterator(), EndIterator());
	}

	/**
	* Counts all elements for those predicate does NOT return true.
	*
	* @see: CountPredicate
	*
	* @UNIT-TESTED
	*/
	template<class Pred>
	int32_t CountNotPredicate(const Pred& P) const
	{
		return CountNotPredicate_InRange(P, Iterator(), EndIterator());
	}

	/**
	* Counts all keys for those predicate does NOT return true.
	*
	* @see: CountPredicate
	*
	* @UNIT-TESTED
	*/
	template<class Pred>
	int32_t CountNotKeyPredicate(const Pred& P) const
	{
		return CountNotKeyPredicate_InRange(P, Iterator(), EndIterator());
	}

	/**
	* Counts all values for those predicate does NOT return true.
	*
	* @see: CountPredicate
	*
	* @UNIT-TESTED
	*/
	template<class Pred>
	int32_t CountNotValuePredicate(const Pred& P) const
	{
		return CountNotValuePredicate_InRange(P, Iterator(), EndIterator());
	}

	/**
	* Counts all values that not equal to the given one.
	*
	* @see: CountPredicate
	*
	* @UNIT-TESTED
	*/
	int32_t CountOtherValues(const ValueType& InValue) const
	{
		return CountOtherValues_InRange(InValue, Iterator(), EndIterator());
	}

	/**
	* Counts all elements for those predicate returns true in the given range.
	*
	* @UNIT-TESTED
	*/
	template<class Pred>
	int32_t CountPredicate_InRange(const Pred& P, ConstIteratorType ItStart, ConstIteratorType ItEnd) const
	{
		int32_t ResultCount = 0;
		for(ConstIteratorType It = ItStart; It != ItEnd; ++It)
		{
			if( P(*It) )
			{
				++ResultCount;
			}
		}
		return ResultCount;
	}

	/**
	* Counts all keys for those predicate returns true in the given range.
	*
	* @UNIT-TESTED
	*/
	template<class Pred>
	int32_t CountKeyPredicate_InRange(const Pred& P, ConstIteratorType ItStart, ConstIteratorType ItEnd) const
	{
		return CountPredicate_InRange
		(
			[&P](const KeyValueType& KV)->bool { return P (KV.Key); },
			ItStart, ItEnd
		);
	}

	/**
	* Counts all values for those predicate returns true in the given range.
	*/
	template<class Pred>
	int32_t CountValuePredicate_InRange(const Pred& P, ConstIteratorType ItStart, ConstIteratorType ItEnd) const
	{
		return CountPredicate_InRange
		(
			[&P](const KeyValueType& KV)->bool { return P (KV.Value); },
			ItStart, ItEnd
		);
	}

	/**
	* Counts all values equal to the given one in the given range.
	*
	* @UNIT-TESTED
	*/
	int32_t CountValues_InRange(const ValueType& InValue, ConstIteratorType ItStart, ConstIteratorType ItEnd) const
	{
		int32_t ResultCount = 0;
		for(ConstIteratorType It = ItStart; It != ItEnd; ++It)
		{
			if( It.GetValue() == InValue )
			{
				++ResultCount;
			}
		}
		return ResultCount;
	}

	/**
	* Counts all elements for those predicate does NOT return true in the given range.
	*
	* @see: CountPredicate
	*
	* @UNIT-TESTED
	*/
	template<class Pred>
	int32_t CountNotPredicate_InRange(const Pred& P, ConstIteratorType ItStart, ConstIteratorType ItEnd) const
	{
		int32_t ResultCount = 0;
		for(ConstIteratorType It = ItStart; It != ItEnd; ++It)
		{
			if( ! P(*It) )
			{
				++ResultCount;
			}
		}
		return ResultCount;
	}

	/**
	* Counts all keys for those predicate does NOT return true in the given range.
	*
	* @UNIT-TESTED
	*/
	template<class Pred>
	int32_t CountNotKeyPredicate_InRange(const Pred& P, ConstIteratorType ItStart, ConstIteratorType ItEnd) const
	{
		return CountNotPredicate_InRange
		(
			[&P](const KeyValueType& KV)->bool { return P (KV.Key); },
			ItStart, ItEnd
		);
	}

	/**
	* Counts all values for those predicate does NOT return true in the given range.
	*
	* @UNIT-TESTED
	*/
	template<class Pred>
	int32_t CountNotValuePredicate_InRange(const Pred& P, ConstIteratorType ItStart, ConstIteratorType ItEnd) const
	{
		return CountNotPredicate_InRange
		(
			[&P](const KeyValueType& KV)->bool { return P (KV.Value); },
			ItStart, ItEnd
		);
	}

	/**
	* Counts all values not equal to the given one in the given range.
	*
	* @UNIT-TESTED
	*/
	int32_t CountOtherValues_InRange(const ValueType& InValue, ConstIteratorType ItStart, ConstIteratorType ItEnd) const
	{
		int32_t ResultCount = 0;
		for(ConstIteratorType It = ItStart; It != ItEnd; ++It)
		{
			if( It.GetValue() != InValue )
			{
				++ResultCount;
			}
		}
		return ResultCount;
	}

	/**
	* Returns reference to value for the given key.
	* WARNING! Key/Value with the given Key must be registered.
	*/
	const ValueType& operator[](const KeyType& InKey) const
	{
		const ValueType* pValue = GetValueOrNull(InKey);
		BOOST_ASSERT_MSG(pValue, "TRBTree: using indexing ([]) assumes that the given key/value pair is registered");
		return *pValue;
	}

	/**
	* Returns reference to value for the given key.
	* WARNING! Key/Value with the given Key must be registered.
	*/
	ValueType& operator[](const KeyType& InKey)
	{
		ValueType* pValue = GetValueOrNull(InKey);
		BOOST_ASSERT_MSG(pValue, "TRBTree: using indexing ([]) assumes that the given key/value pair is registered");
		return *pValue;
	}

	/**
	* Returns pointer to value for the given key.
	* Or returns nullptr, if not found.
	*/
	const ValueType* GetValueOrNull(const KeyType& InKey) const
	{
		return GetValueOrNull(InKey, ComparerArg());
	}

	/**
	* Returns pointer to value for the given key.
	* Or returns nullptr, if not found.
	*/
	ValueType* GetValueOrNull(const KeyType& InKey)
	{
		return GetValueOrNull(InKey, ComparerArg());
	}

	/**
	* Returns pointer to value for the given key.
	* Or returns nullptr, if not foundd.
	*/
	template<class ComparerTypeArg>
	const ValueType* GetValueOrNull(const KeyType& InKey, ComparerTypeArg InComparer) const
	{
		TRBTreeImpl::ChildNodeRef NodeRef = TRBTreeImpl::ChildNodeRef::Invalid();
		bool bFound = FindNode(InKey, /*OutNodeRef*/ NodeRef, InComparer);
		if (!bFound)
		{
			return nullptr;
		}
		return &GetNode(NodeRef)->GetValue();
	}

	/**
	* Returns pointer to value for the given key.
	* Or returns nullptr, if not found.
	*/
	template<class  ComparerTypeArg>
	ValueType* GetValueOrNull(const KeyType& InKey, ComparerTypeArg InComparer)
	{
		TRBTreeImpl::ChildNodeRef NodeRef = TRBTreeImpl::ChildNodeRef::Invalid();
		bool bFound = FindNode(InKey, /*OutNodeRef*/ NodeRef, InComparer);
		if (!bFound)
		{
			return nullptr;
		}
		return &GetNode(NodeRef)->GetValue();
	}
	

	/**
	* Returns true, if contains the given key.
	*
	* @UNIT-TESTED
	*/
	bool Contains(const KeyType& InKey) const
	{
		return nullptr != Find(InKey);
	}

	/**
	* Returns true, if contains value satisfying the given predicate.
	*
	* @UNIT-TESTED
	*/
	bool ContainsValue(const ValueType& InValue) const
	{
		return FindIteratorByValue(InValue);
	}

	/**
	* Returns true, if contains value other than the given.
	*
	* @UNIT-TESTED
	*/
	bool ContainsOtherValue(const ValueType& InValue) const
	{
		return FindIteratorByNotValue(InValue);
	}

	/**
	* Returns true, if contains value satisfying the given predicate.
	*
	* @UNIT-TESTED
	*/
	template<class Pred>
	bool ContainsPredicate(const Pred& P) const
	{
		return FindIteratorByPredicate(P);
	}

	/**
	* Returns true, if contains value satisfying the given predicate that takes const reference to key (const Key&).
	*
	* @UNIT-TESTED
	*/
	template<class Pred>
	bool ContainsKeyPredicate(const Pred& P) const
	{
		return FindIteratorByKeyPredicate(P);
	}


	/**
	* Returns true, if contains value for whicle given predicate that takes const reference to key (const Key&) returns false.
	*
	* @UNIT-TESTED
	*/
	template<class Pred>
	bool ContainsNotKeyPredicate(const Pred& P) const
	{
		return FindIteratorByNotKeyPredicate(P);
	}

	/**
	* Returns true, if contains value satisfying the given predicate in the given range.
	*
	* @UNIT-TESTED
	*/
	template<class Pred>
	bool ContainsPredicate_InRange(const Pred& P, ConstIteratorType FirstIt, ConstIteratorType LastIt) const
	{
		return FindIteratorByPredicate_InRange(P, FirstIt, LastIt);
	}

	/**
	* Returns true, if contains value satisfying the given predicate in the given range.
	*
	* @UNIT-TESTED
	*/
	template<class Pred>
	bool ContainsKeyPredicate_InRange(const Pred& P, ConstIteratorType FirstIt, ConstIteratorType LastIt) const
	{
		return FindIteratorByKeyPredicate_InRange(P, FirstIt, LastIt);
	}

	/**
	* Returns true, if does NOT contain value satisfying the given predicate.
	*
	* @UNIT-TESTED
	*/
	template<class Pred>
	bool ContainsNotPredicate(const Pred& P) const
	{
		return FindIteratorByNotPredicate(P);
	}

	/**
	* Returns true, if contains the given value in the given range.
	*
	* @UNIT-TESTED
	*/
	bool ContainsValue_InRange(const ValueType& InValue, ConstIteratorType FirstIt, ConstIteratorType LastIt) const
	{
		return FindIteratorByValue_InRange(InValue, FirstIt, LastIt);
	}
	
	/**
	* Returns true, if contains value other than the given in the given range.
	*
	* @UNIT-TESTED
	*/
	bool ContainsOtherValue_InRange(const ValueType& InValue, ConstIteratorType FirstIt, ConstIteratorType LastIt) const
	{
		return FindIteratorByNotValue_InRange(InValue, FirstIt, LastIt);
	}

	/**
	* Returns true, if does NOT contain value satisfying the given predicate in the given range.
	*
	* @UNIT-TESTED
	*/
	template<class Pred>
	bool ContainsNotPredicate_InRange(const Pred& P, ConstIteratorType FirstIt, ConstIteratorType LastIt) const
	{
		return FindIteratorByNotPredicate_InRange(P, FirstIt, LastIt);
	}

	/**
	* Returns true, if does NOT contain value satisfying the given predicate in the given range.
	*
	* @UNIT-TESTED
	*/
	template<class Pred>
	bool ContainsNotKeyPredicate_InRange(const Pred& P, ConstIteratorType FirstIt, ConstIteratorType LastIt) const
	{
		return FindIteratorByNotKeyPredicate_InRange(P, FirstIt, LastIt);
	}

	/**
	* Checks that all key/value pairs satisfy the given predicate.
	*/
	template<class Pred>
	bool All(const Pred& P) const
	{
		return false == ContainsNotPredicate(P);
	}

	/**
	* Checks that all keys satisfy the given predicate.
	*/
	template<class Pred>
	bool AllKeys(const Pred& P) const
	{
		return false == ContainsNotKeyPredicate(P);
	}

	/**
	* Checks that all values satisfy the given predicate.
	*/
	//template<class Pred>
	//bool AllValues(const Pred& P) const
	//{
	//	return false == ContainsOtherValue(P);
	//}

	/**
	* Searches iterator by the given Key.
	*
	* @Returns: Iterator to the element (or end iterator, if NOT found)
	*/
	ConstIteratorType FindIteratorFor(const KeyType& InKey) const
	{
		return FindIteratorFor(InKey, ComparerArg());
	}

	/**
	* Searches iterator by the given Key according to the given criteria.
	*
	* @Returns: Iterator to the element (or end iterator, if NOT found)
	*/
	template<class ComparerType>
	ConstIteratorType FindIteratorFor(const KeyType& InKey, ComparerType InComparer) const
	{
		return FindIteratorAtImpl<ConstIteratorType>(InKey, this, InComparer);
	}

	/**
	* Searches iterator by the given Key in the given range.
	*
	* @Returns: Iterator to the element (or end iterator, if NOT found)
	*/
	ConstIteratorType FindIteratorFor_InRange(const KeyType& InKey, ConstIteratorType ItFirst, ConstIteratorType ItLast) const
	{
		return FindIteratorAtInRangeImpl<ConstIteratorType>(InKey, this, ComparerArg(), ItFirst, ItLast);
	}

	/**
	* Searches iterator by the given Key according to the given criteria in the given range.
	*
	* @Returns: Iterator to the element (or end iterator, if NOT found)
	*/
	template<class ComparerType>
	ConstIteratorType FindIteratorFor_InRange(const KeyType& InKey, ComparerType InComparer, ConstIteratorType ItFirst, ConstIteratorType ItLast) const
	{
		return FindIteratorAtInRangeImpl<ConstIteratorType>(InKey, this, InComparer, ItFirst, ItLast);
	}

	/**
	* Searches iterator by the given Key.
	*
	* @Returns: Iterator to the element (or end iterator, if NOT found)
	*/
	IteratorType FindIteratorFor(const KeyType& InKey)
	{
		return FindIteratorFor(InKey, ComparerArg());
	}

	/**
	* Searches iterator by the given value.
	*
	* @returns: Iterator to the element (or end iterator, if NOT found)
	* @see: FindIteratorFor
	*
	* @UNIT-TESTED
	*/
	ConstIteratorType FindIteratorByValue(const ValueType& InValue) const
	{
		return FindIteratorByValue_Impl<ConstIteratorType>(InValue, this);
	}

	/**
	* Searches iterator by the given value.
	*
	* @returns: Iterator to the element (or end iterator, if NOT found)
	* @see: FindIteratorFor
	*
	* @UNIT-TESTED
	*/
	ConstIteratorType FindIteratorByValue_InRange(const ValueType& InValue, ConstIteratorType FirstIt, ConstIteratorType LastIt) const
	{
		return FindIteratorByValue_InRange_Impl<ConstIteratorType>(FirstIt, LastIt, InValue, this);
	}

	/**
	* Searches iterator by the given value.
	*
	* @returns: Iterator to the element (or end iterator, if NOT found)
	* @see: FindIteratorFor
	*
	* @UNIT-TESTED
	*/
	IteratorType FindIteratorByValue(const ValueType& InValue)
	{
		return FindIteratorByValue_Impl<IteratorType>(InValue, this);
	}

	/**
	* Searches iterator by the given value.
	*
	* @returns: Iterator to the element (or end iterator, if NOT found)
	* @see: FindIteratorFor
	*
	* @UNIT-TESTED
	*/
	IteratorType FindIteratorByValue_InRange(const ValueType& InValue, IteratorType FirstIt, IteratorType LastIt)
	{
		return FindIteratorByValue_InRange_Impl<IteratorType>(FirstIt, LastIt, InValue, this);
	}

	/**
	* Searches iterator of first value other than the given.
	*
	* @returns: Iterator to the element (or end iterator, if NOT found)
	* @see: FindIteratorFor
	*
	* @UNIT-TESTED
	*/
	ConstIteratorType FindIteratorByNotValue(const ValueType& InValue) const
	{
		return FindIteratorByNotValue_Impl<ConstIteratorType>(InValue, this);
	}

	/**
	* Searches iterator of first value other than the given.
	*
	* @returns: Iterator to the element (or end iterator, if NOT found)
	* @see: FindIteratorFor
	*
	* @UNIT-TESTED
	*/
	ConstIteratorType FindIteratorByNotValue_InRange(const ValueType& InValue, ConstIteratorType FirstIt, ConstIteratorType LastIt) const
	{
		return FindIteratorByNotValue_InRange_Impl<ConstIteratorType>(FirstIt, LastIt, InValue, this);
	}

	/**
	* Searches iterator of first value other than the given.
	*
	* @returns: Iterator to the element (or end iterator, if NOT found)
	* @see: FindIteratorFor
	*
	* @UNIT-TESTED
	*/
	IteratorType FindIteratorByNotValue(const ValueType& InValue)
	{
		return FindIteratorByNotValue_Impl<IteratorType>(InValue, this);
	}

	/**
	* Searches iterator of first value other than the given.
	*
	* @returns: Iterator to the element (or end iterator, if NOT found)
	* @see: FindIteratorFor
	*
	* @UNIT-TESTED
	*/
	IteratorType FindIteratorByNotValue_InRange(const ValueType& InValue, IteratorType FirstIt, IteratorType LastIt)
	{
		return FindIteratorByNotValue_InRange_Impl<IteratorType>(FirstIt, LastIt, InValue, this);
	}

	/**
	* Searches iterator by the given predicate.
	*
	* @returns: Iterator to the element (or end iterator, if NOT found)
	* @see: FindIteratorFor
	*
	* @UNIT-TESTED
	*/
	template<class Pred>
	ConstIteratorType FindIteratorByPredicate(const Pred& P) const
	{
		return FindIteratorByPredicateImpl<ConstIteratorType>(P, this);
	}

	/**
	* Searches iterator by the given  predicate that takes reference to key (const Key&).
	*
	* @returns: Iterator to the element (or end iterator, if NOT found)
	* @see: FindIteratorFor
	*
	* @UNIT-TESTED
	*/
	template<class Pred>
	ConstIteratorType FindIteratorByKeyPredicate(const Pred& P) const
	{
		return FindIteratorByKeyPredicateImpl<ConstIteratorType>(P, this);
	}

	/**
	* Searches iterator to key for which the given predicate that takes const reference to key (const Key&) returns false.
	*
	* @returns: Iterator to the element (or end iterator, if NOT found)
	* @see: FindIteratorFor
	*
	* @UNIT-TESTED
	*/
	template<class Pred>
	ConstIteratorType FindIteratorByNotKeyPredicate(const Pred& P) const
	{
		return FindIteratorByNotKeyPredicateImpl<ConstIteratorType>(P, this);
	}

	/**
	* Searches iterator by the given predicate.
	*
	* @returns: Iterator to the element (or end iterator, if NOT found)
	* @see: FindIteratorFor
	*
	* @UNIT-TESTED
	*/
	template<class Pred>
	IteratorType FindIteratorByPredicate(const Pred& P)
	{
		return FindIteratorByPredicateImpl<IteratorType>(P, this);
	}

	/**
	* Searches iterator by the given  predicate that takes reference to key (const Key&).
	*
	* @returns: Iterator to the element (or end iterator, if NOT found)
	* @see: FindIteratorFor
	*
	* @UNIT-TESTED
	*/
	template<class Pred>
	IteratorType FindIteratorByKeyPredicate(const Pred& P)
	{
		return FindIteratorByKeyPredicateImpl<IteratorType>(P, this);
	}

	/**
	* Searches iterator to key for which the given predicate that takes const reference to key (const Key&) returns false.
	*
	* @returns: Iterator to the element (or end iterator, if NOT found)
	* @see: FindIteratorFor
	*
	* @UNIT-TESTED
	*/
	template<class Pred>
	IteratorType FindIteratorByNotKeyPredicate(const Pred& P)
	{
		return FindIteratorByNotKeyPredicateImpl<IteratorType>(P, this);
	}

	/**
	* Searches iterator of element that does NOT satisfy the given predicate.
	*
	* @returns: Iterator to the element (or end iterator, if NOT found)
	* @see: FindIteratorFor
	*
	* @UNIT-TESTED
	*/
	template<class Pred>
	IteratorType FindIteratorByNotPredicate(const Pred& P)
	{
		return FindIteratorByPredicateImpl<IteratorType>
		(
			[&P](const KeyValueType& KV)->bool
			{
				return ! P (KV);
			},
			this
		);
	}

	/**
	* Searches iterator of element that does NOT satisfy the given predicate.
	*
	* @returns: Iterator to the element (or end iterator, if NOT found)
	* @see: FindIteratorFor
	*
	* @UNIT-TESTED
	*/
	template<class Pred>
	ConstIteratorType FindIteratorByNotPredicate(const Pred& P) const
	{
		return FindIteratorByPredicateImpl<ConstIteratorType>
		(
				[&P](const KeyValueType& KV)->bool
				{
					return ! P(KV);
				},
				this
		);
	}

	/**
	* Searches iterator by the given predicate in the given range.
	*
	* @returns: Iterator to the element. Returns end (!) iterator (NOT last of range !), if NOT found.
	* @see: FindIteratorFor
	*
	* @UNIT-TESTED
	*/
	template<class Pred>
	IteratorType FindIteratorByPredicate_InRange(const Pred& P, IteratorType FirstIt, IteratorType LastIt)
	{
		return FindIteratorByPredicate_InRange_Impl<IteratorType>(FirstIt, LastIt, P, this);
	}

	/**
	* Searches iterator by the given key predicate in the given range.
	*
	* @returns: Iterator to the element. Returns end (!) iterator (NOT last of range !), if NOT found.
	* @see: FindIteratorFor
	*
	* @UNIT-TESTED
	*/
	template<class Pred>
	IteratorType FindIteratorByKeyPredicate_InRange(const Pred& P, IteratorType FirstIt, IteratorType LastIt)
	{
		return FindIteratorByKeyPredicate_InRange_Impl<IteratorType>(FirstIt, LastIt, P, this);
	}

	/**
	* Searches iterator by the given predicate in the given range.
	*
	* @returns: Iterator to the element. Returns end (!) iterator (NOT last of range !), if NOT found.
	* @see: FindIteratorFor
	*
	* @UNIT-TESTED
	*/
	template<class Pred>
	ConstIteratorType FindIteratorByPredicate_InRange(const Pred& P, ConstIteratorType FirstIt, ConstIteratorType LastIt) const
	{
		return FindIteratorByPredicate_InRange_Impl<ConstIteratorType>(FirstIt, LastIt, P, this);
	}

	/**
	* Searches iterator by the given predicate in the given range.
	*
	* @returns: Iterator to the element. Returns end (!) iterator (NOT last of range !), if NOT found.
	* @see: FindIteratorFor
	*
	* @UNIT-TESTED
	*/
	template<class Pred>
	ConstIteratorType FindIteratorByKeyPredicate_InRange(const Pred& P, ConstIteratorType FirstIt, ConstIteratorType LastIt) const
	{
		return FindIteratorByKeyPredicate_InRange_Impl<ConstIteratorType>(FirstIt, LastIt, P, this);
	}

	/**
	* Searches iterator that does NOT sasify the given predicate in the given range.
	*
	* @returns: Iterator to the element. Returns end (!) iterator (NOT last of range !), if NOT found.
	* @see: FindIteratorFor
	*
	* @UNIT-TESTED
	*/
	template<class Pred>
	IteratorType FindIteratorByNotPredicate_InRange(const Pred& P, IteratorType FirstIt, IteratorType LastIt)
	{
		return FindIteratorByNotPredicate_InRange_Impl<IteratorType>(FirstIt, LastIt, P, this);
	}

	/**
	* Searches iterator that does NOT sasify the given predicate in the given range.
	*
	* @returns: Iterator to the element. Returns end (!) iterator (NOT last of range !), if NOT found.
	* @see: FindIteratorFor
	*
	* @UNIT-TESTED
	*/
	template<class Pred>
	IteratorType FindIteratorByNotKeyPredicate_InRange(const Pred& P, IteratorType FirstIt, IteratorType LastIt)
	{
		return FindIteratorByNotKeyPredicate_InRange_Impl<IteratorType>(FirstIt, LastIt, P, this);
	}

	/**
	* Searches iterator that does NOT sasify the given predicate in the given range.
	*
	* @returns: Iterator to the element. Returns end (!) iterator (NOT last of range !), if NOT found.
	* @see: FindIteratorFor
	*
	* @UNIT-TESTED
	*/
	template<class Pred>
	ConstIteratorType FindIteratorByNotPredicate_InRange(const Pred& P, ConstIteratorType FirstIt, ConstIteratorType LastIt) const
	{
		return FindIteratorByNotPredicate_InRange_Impl<ConstIteratorType>(FirstIt, LastIt, P, this);
	}

	/**
	* Searches iterator that does NOT sasify the given predicate in the given range.
	*
	* @returns: Iterator to the element. Returns end (!) iterator (NOT last of range !), if NOT found.
	* @see: FindIteratorFor
	*
	* @UNIT-TESTED
	*/
	template<class Pred>
	ConstIteratorType FindIteratorByNotKeyPredicate_InRange(const Pred& P, ConstIteratorType FirstIt, ConstIteratorType LastIt) const
	{
		return FindIteratorByNotKeyPredicate_InRange_Impl<ConstIteratorType>(FirstIt, LastIt, P, this);
	}

	/**
	* Searches iterator by the given Key according to the given criteria.
	*
	* @Returns: Iterator to the element (or end iterator, if NOT found)
	*/
	template<class ComparerType>
	IteratorType FindIteratorFor(const KeyType& InKey, ComparerType InComparer)
	{
		return FindIteratorAtImpl<IteratorType>(InKey, this, InComparer);
	}

	/**
	* Searches Key-Value pair by the given key.
	*
	* @Returns: Pointer to the Key-Value pair (or nullptr, if NOT found).
	*/
	const KeyValueType* Find(const KeyType& InKey) const
	{
		return Find(InKey, ComparerArg());
	}

	/**
	* Searches Key-Value pair by the given key using the given comparer.
	*
	* @Returns: Pointer to the Key-Value pair (or nullptr, if NOT found).
	*/
	template<class SearchKeyType, class ComparerType>
	const KeyValueType* Find(const SearchKeyType& InKey, ComparerType InComparer) const
	{
		if (Empty())
		{
			return nullptr;
		}

		TRBTreeImpl::ChildNodeRef NodeRef = TRBTreeImpl::ChildNodeRef::Invalid();
		bool bFound = FindNode(InKey, /*OutNodeRef*/ NodeRef, InComparer);
		if ( ! bFound )
		{
			return nullptr;
		}
		return &GetNode(NodeRef)->GetKV();
	}

	/**
	* Searches Value by the given key.
	*
	* @Returns: Pointer to the Value (or nullptr, if NOT found).
	*/
	const ValueType* FindValue(const KeyType& InKey) const { return FindValueImpl(InKey); }

	/**
	* Searches Value by the given key (mutable version).
	*
	* @returns: Pointer to the Value (or nullptr, if NOT found).
	* @see: Non-const version
	*/
	ValueType* FindValue(const KeyType& InKey) { return const_cast<ValueType*>(FindValueImpl(InKey)); }

	/**
	* Removes key/value pair that the given iterator points to.
	*
	* The iterator must be valid NON-end iterator.
	*/
	void RemoveAt(ConstIteratorType InIt)
	{
		BOOST_ASSERT_MSG(InIt, "TRBTree: RemoveAt: The iterator must be valid NON-end iterator");
		RemoveAndDestroy(InIt.NodeRef);
	}

	/**
	* Moves the key/value as return value.
	*
	* The iterator must be valid NON-end iterator.
	*/
	KeyValueType GetMovedByIterator(ConstIteratorType InIt)
	{
		BOOST_ASSERT_MSG(InIt, "TRBTree: RemoveAt: The iterator must be valid NON-end iterator");
		return std::move(RemoveAndMove(InIt.NodeRef));
	}

	/*
	* Removes value with the given key from the tree.
	*
	* @returns: true, if was found and removed (otherwise false).
	*/
	bool Remove(const KeyType& InKey)
	{
		return Remove(InKey, ComparerArg());
	}

	/*
	* Removes value with the given key from the tree using the given Comparer.
	*
	* @returns: true, if was found and removed (otherwise false). 
	*/
	template<class SearchKeyType, class Comparer>
	bool Remove(const SearchKeyType& InKey, Comparer InComparer)
	{
		if (Empty())
		{
			return false;
		}

		TRBTreeImpl::ChildNodeRef NodeRef = TRBTreeImpl::ChildNodeRef::Invalid();
		bool const bFound = FindNode(InKey, NodeRef, InComparer);
		if (bFound)
		{
			RemoveAndDestroy(NodeRef);
		}

		return bFound;
	}

	/**
	* Removes the first key/value pair that satisfy the given predicate.
	* The predicate must take the reference to const KeyValueType (const KeyValueType&).
	*
	* @returns: true if element with the given predicate was in the container (otherwise, false)
	*/
	template<class Pred>
	bool RemoveFirstPredicate(const Pred& P)
	{
		return RemoveFirstPredicate_InRange(P, Iterator(), EndIterator());
	}

	/**
	* Removes the first key/value pair that satisfy the given predicate.
	* The predicate must take the reference to const KeyType (const KeyType&).
	*
	* @returns: true if element with the given predicate was in the container (otherwise, false)
	*/
	template<class Pred>
	bool RemoveFirstKeyPredicate(const Pred& P)
	{
		return RemoveFirstKeyPredicate_InRange(P, Iterator(), EndIterator());
	}

	/**
	* Removes the first value.
	* The predicate must take the reference to const KeyValueType (const KeyValueType&).
	*
	* @returns: true if element with the given predicate was in the container (otherwise, false)
	*/
	bool RemoveFirstValue(const ValueType& InValue)
	{
		return RemoveFirstValue_InRange
		(
			InValue, Iterator(), EndIterator()
		);
	}

	/**
	* Removes the first key/value pair that satisfy the given predicate in the given range.
	* The predicate must take the reference to const KeyValueType (const KeyValueType&).
	*
	* @returns: true if element with the given predicate was in the container (otherwise, false);
	*/
	template<class Pred>
	bool RemoveFirstPredicate_InRange(const Pred& P, IteratorType FirstIt, IteratorType LastIt)
	{
		IteratorType NextAfterRemovedIt { Iterator() };
		return RemoveFirstPredicate_InRange(P, FirstIt, LastIt, /*Out*/NextAfterRemovedIt);
	}

	/**
	* Removes the first key/value pair that satisfy the given predicate in the given range.
	* The predicate must take the reference to const KeyType (const KeyType&).
	*
	* @returns: true if element with the given predicate was in the container (otherwise, false);
	*/
	template<class Pred>
	bool RemoveFirstKeyPredicate_InRange(const Pred& P, IteratorType FirstIt, IteratorType LastIt)
	{
		IteratorType NextAfterRemovedIt{ Iterator() };
		return RemoveFirstKeyPredicate_InRange(P, FirstIt, LastIt, /*Out*/NextAfterRemovedIt);
	}

	/**
	* Removes the first key/value pair that satisfy the given predicate in the given range.
	* The predicate must take the reference to const KeyType (const KeyType&).
	*
	* @returns: true if element with the given predicate was in the container (otherwise, false);
	* @out: OutNextAfterRemovedIt:
	*		iterator to the next element, that is immediately followed the removed element
	*		(or End, if the last element was removed).
	*		Valid only if element was really found and removed.
	*/
	template<class Pred>
	bool RemoveFirstKeyPredicate_InRange(const Pred& P, IteratorType FirstIt, IteratorType LastIt, IteratorType& OutNextAfterRemovedIt)
	{
		return RemoveFirstPredicate_InRange
		(
			[&P](const KeyValueType& KV)
			{
				return P(KV.Key);
			},
			FirstIt, LastIt,
			/*Out*/OutNextAfterRemovedIt
		);
	}

	/**
	* Removes the first key/value pair that satisfy the given predicate in the given range.
	* The predicate must take the reference to const KeyValueType (const KeyValueType&).
	*
	* @returns: true if element with the given predicate was in the container (otherwise, false);
	* @out: OutNextAfterRemovedIt: 
	*		iterator to the next element, that is immediately followed the removed element 
	*		(or End, if the last element was removed). 
	*		Valid only if element was really found and removed.
	*/
	template<class Pred>
	bool RemoveFirstPredicate_InRange(const Pred& P, IteratorType FirstIt, IteratorType LastIt, IteratorType& OutNextAfterRemovedIt)
	{
		IteratorType It = FindIteratorByPredicate_InRange(P, FirstIt, LastIt);
		if ( It == EndIterator() )
		{
			return false;
		}
		OutNextAfterRemovedIt = GetNextIt(It);
		RemoveAt(It);
		return true;
	}

	/**
	* Removes firts value equal to the given one in the given range.
	*
	* @returns: true if element with the given predicate was in the container (otherwise, false);
	* @see: RemoveFirstPredicate
	*/
	bool RemoveFirstValue_InRange(const ValueType& InValue, IteratorType ItFirst, IteratorType ItLast)
	{
		return RemoveFirstPredicate_InRange
		(
			[&InValue](const KeyValueType& KV)->bool { return KV.Value == InValue; },
			ItFirst, ItLast
		);
	}

	/**
	* Removes all key/value pairs that satisfy the given predicate.
	* The predicate must take the reference to const KeyValueType (const KeyValueType&).
	*
	* @returns: number of removed elements.
	* @see: RemoveFirstPredicate
	*/
	template<class Pred>
	int32_t RemoveAllPredicate(const Pred& P)
	{
		return RemoveAllPredicate_InRange(P, Iterator(), EndIterator());
	}

	/**
	* Removes all key/value pairs that satisfy the given predicate.
	* The predicate must take the reference to const KeyValueType (const KeyValueType&).
	*
	* @returns: number of removed elements.
	* @see: RemoveFirstPredicate
	*/
	template<class Pred>
	int32_t RemoveAllKeyPredicate(const Pred& P)
	{
		return RemoveAllKeyPredicate_InRange(P, Iterator(), EndIterator());
	}

	/**
	* Removes all values equal to the given one in the given range.
	*
	* @returns: number of removed elements.
	* @see: RemoveFirstPredicate
	*/
	int32_t RemoveAllValues(const ValueType& InValue)
	{
		return RemoveAllValues_InRange(InValue, Iterator(), EndIterator());
	}


	/**
	* Removes all values equal to the given one in the given range.
	*
	* @returns: number of removed elements.
	* @see: RemoveFirstPredicate
	*/
	int32_t RemoveAllValues_InRange(const ValueType& InValue, IteratorType ItFirst, IteratorType ItLast)
	{
		return RemoveAllPredicate_InRange
		(
			[&InValue](const KeyValueType& KV)->bool { return KV.Value == InValue; },
			ItFirst, ItLast
		);
	}

	/**
	* Removes all key/value pairs that satisfy the given predicate.
	* The predicate must take the reference to const KeyValueType (const KeyValueType&).
	*
	* @returns: number of removed elements.
	* @see: RemoveFirstPredicate
	*/
	template<class Pred>
	int32_t RemoveAllPredicate_InRange(const Pred& P, IteratorType ItFirst, IteratorType ItLast)
	{
		// @TODO: Optimize
		int32_t Count = 0;
		IteratorType It = ItFirst;
		while (true)
		{
			if ( It == ItLast )
			{
				return Count;
			}

			IteratorType NextIt { ItFirst };

			if ( ! RemoveFirstPredicate_InRange(P, It, ItLast, /*OutNext*/NextIt) )
			{
				return Count;
			}

			Count++;
			It = NextIt;
		}
		return Count;
	}

	/**
	* Removes all key/value pairs that satisfy the given predicate.
	* The predicate must take the reference to const KeyType (const KeyType&).
	*
	* @returns: number of removed elements.
	* @see: RemoveFirstPredicate
	*/
	template<class Pred>
	int32_t RemoveAllKeyPredicate_InRange(const Pred& P, IteratorType ItFirst, IteratorType ItLast)
	{
		return RemoveAllPredicate_InRange
		(
			[&P](const KeyValueType& KV)->bool
			{
				return P(KV.Key);
			},
			ItFirst, ItLast
		);
	}

	/*
	* Moves value with the given key from the tree.
	*
	* @returns: true, if was found and moved (otherwise false).
	*/
	bool MoveByKey(const KeyType& InKey, KeyValueType& OutMovedKV)
	{
		if (Empty())
		{
			return false;
		}

		TRBTreeImpl::ChildNodeRef NodeRef = TRBTreeImpl::ChildNodeRef::Invalid();
		bool const bFound = FindNode(InKey, NodeRef, ComparerArg());
		if (bFound)
		{
			OutMovedKV = std::move(RemoveAndMove(NodeRef));
		}

		return bFound;
	}

	/**
	* Moves the given key from the tree and removes the element.
	*
	* @returns: true, if was found and moved (otherwise false).
	*/
	bool RemoveAndMoveKey(const KeyType& InKey, KeyType& OutMovedKey)
	{
		KeyValueType KV;
		bool bMoved = MoveByKey(InKey, /*OutMovedKV*/KV);
		if (bMoved)
		{
			OutMovedKey = std::move(KV.Key);
		}
		return bMoved;
	}

	/**
	* Moves the given key from the tree and removes the element.
	*
	* @returns: true, if was found and moved (otherwise false).
	*/
	bool RemoveAndMoveValue(const KeyType& InKey, ValueType& OutMovedValue)
	{
		KeyValueType KV;
		bool bMoved = MoveByKey(InKey, /*OutMovedKV*/KV);
		if (bMoved)
		{
			OutMovedValue = std::move(KV.Value);
		}
		return bMoved;
	}

	void AppendRangeEnd(ConstIteratorType FirstIt, ConstIteratorType LastIt)
	{
		for(ConstIteratorType It = FirstIt; It != LastIt; ++It)
		{
			AddEnd(*it);
		}
	}

	/**
	* Adds a new node from the end of the container.
	* WARNING!!! the new value DO must have a key that is greater than the maximum key value (if any). To be checked with assert.
	*
	* @returns: Iterator to the inserted value (must be always non-end).
	*/
	IteratorType AddEnd(const KeyValueType& InKV)
	{
		return AddEndImpl(InKV);
	}

	/**
	* Adds a new node from the end of the container by moving.
	* WARNING!!! the new value DO must have a key that is greater than the maximum key value (if any). To be checked with assert.
	* @see: AddEnd (non-move version)
	*
	* @returns: Iterator to the inserted value (must be always non-end).
	*/
	IteratorType AddEnd(KeyValueType&& InKV)
	{
		return AddEndImpl(std::move(InKV));
	}

	/**
	* Adds a new node before the given position of the container.
	* WARNING!!! The new key must be less than ItPos key and greater than element, the ItPos is position after.
	*
	* @returns: Iterator to the inserted value (must be always non-end).
	*/
	IteratorType AddAfter(ConstIteratorType ItPos, const KeyValueType& InKV)
	{
		KeyValueType KV { InKV };
		return AddAfterImpl(ItPos, std::move(KV));
	}

	/**
	* Adds a new node before the given position of the container by moving.
	* WARNING!!! The new key must be less than ItPos key and greater than element, the ItPos is position after.
	*
	* @returns: Iterator to the inserted value (must be always non-end).
	*/
	IteratorType AddAfter(ConstIteratorType ItPos, KeyValueType&& InKV)
	{
		return AddAfterImpl(ItPos, std::move(InKV));
	}
	
	/**
	* Adds a new node from the given hint position.
	* Not guaranteed that this position will be accounted, but may optimize.
	*/
	bool AddHint(ConstIteratorType ItPos, KeyValueType&& InKV)
	{
		return Add(InKV);
	}

	/**
	* Adds a new node from the given hint position.
	* Not guaranteed that this position will be accounted, but may optimize.
	*/
	bool AddHint(ConstIteratorType ItPos, const KeyValueType& InKV)
	{
		return Add(InKV);
	}

	/**
	* Registers the given key/value.
	* If key/value with the given key is already registered, removes it.
	*
	* @Returns: new iterator to the element.
	*/
	IteratorType Replace(KeyValueType&& InKV)
	{
		return ReplaceImpl(std::move(InKV));
	}

	/**
	* Replaces the keyvalue pair corresponding the given key with other key-value.
	* Or (if not registered) registers a new pair.
	*
	* @returns: new iterator to the element.
	*/
	IteratorType ReplaceByKey(const KeyType& InKey, KeyValueType&& InKV)
	{
		return ReplaceByKeyImpl(InKey, std::move(InKV));
	}

	/**
	* Replaces the key/value pair corresponding the given key with other key-value.
	* Or (if not registered) registers a new pair.
	*
	* @returns: new iterator to the element.
	*/
	IteratorType ReplaceByKey(const KeyType& InKey, const KeyValueType& InKV)
	{
		return ReplaceByKeyImpl(InKey, InKV);
	}

	/**
	* Replaces the key/value pair that the given iterator points to with other key-value.
	* The iterator be valid NON-end iterator.
	*
	* @returns: new iterator to the element.
	*/
	IteratorType ReplaceByIterator(ConstIteratorType It, KeyValueType&& InKV)
	{
		return ReplaceByIteratorImpl(It, std::move(InKV));
	}

	/**
	* Replaces the key/value pair that the given iterator points to with other key-value.
	* The iterator be valid NON-end iterator.
	*
	* @returns: new iterator to the element.
	*/
	IteratorType ReplaceByIterator(ConstIteratorType It, const KeyValueType& InKV)
	{
		return ReplaceByIteratorImpl(It, InKV);
	}

	/**
	* Registers the given key/value.
	* If key/value with the given key is already registered, removes it.
	*
	* @Returns: iterator to the element.
	*/
	IteratorType Replace(const KeyValueType& InKV)
	{
		return ReplaceImpl(InKV);
	}

	/**
	* Adds a new node to the tree if it was not there already.
	* Otherwise just returns the stored key/value WITHOUT updating old value of the key.
	*
	* @Returns: iterator to the added or existed element.
	*/
	IteratorType GetOrAdd(const KeyValueType& InKV)
	{
		static_assert(std::is_copy_constructible_v<KeyValueType>, "TRBTree: Add: KeyValueType must be copy-constructible");
		KeyValueType KV = InKV;
		return GetOrAdd(std::move(KV));
	}

	/**
	* Adds a new node to the tree if it was not there already.
	* Otherwise just returns the stored key/value WITHOUT updating old value of the key.
	*
	* @Returns: iterator to the added or existed element.
	*/
	IteratorType GetOrAdd(KeyValueType&& InKV)
	{
		TRBTreeImpl::ChildNodeRef NodeRef = TRBTreeImpl::ChildNodeRef::Invalid();
		AddImpl(std::move(InKV), /*Out*/NodeRef);
		return IteratorType(this, NodeRef);
	}

	/**
	* Adds a new node to the tree if it was not there already.
	* Otherwise just returns the stored key/value WITHOUT updating old value of the key.
	*
	* @Returns: iterator to the added or existed element.
	*/
	IteratorType GetOrAdd(const KeyType& InKey, const ValueType& InValue)
	{
		return GetOrAdd(KeyValueType{ InKey, InValue });
	}

	/**
	* Adds a new node to the tree if it was not there already.
	* Otherwise just returns the stored key/value WITHOUT updating old value of the key.
	*
	* @Returns: iterator to the added or existed element.
	*/
	IteratorType GetOrAdd(KeyType&& InKey, ValueType&& InValue)
	{
		return GetOrAdd(KeyValueType{ std::move(InKey), std::move(InValue) });
	}

	/**
	* Adds a new node to the tree if it was not there already.
	* Otherwise just returns the stored key/value WITHOUT updating old value of the key.
	*
	* @Returns: iterator to the added or existed element.
	*/
	IteratorType GetOrAdd(KeyType&& InKey, const ValueType& InValue)
	{
		return GetOrAdd(KeyValueType{ std::move(InKey), InValue });
	}

	/**
	* Adds a new node to the tree by moving.
	*
	* @Returns: true if was added (or false if already was in the tree).
	*/
	IteratorType GetOrAdd(const KeyType& InKey, ValueType&& InValue)
	{
		return GetOrAdd(KeyValueType{ InKey, std::move(InValue) });
	}

	/**
	* Emplaces a new key value to the tree.
	*/
	template<class ... Args>
	bool AddEmplaceValue(const KeyType& InKey, Args&&... InValueArgs)
	{
		return AddEmplaceHintValue(ConstEndIterator(), InKey, std::forward<Args>(InValueArgs)...);
	}

	/**
	* Emplaces a new key value to the tree at the given iterator position.
	*/
	template<class ... Args>
	bool AddEmplaceHintValue(ConstIteratorType ItPos, const KeyType& InKey, Args&&... InValueArgs)
	{
		// @TODO: Optimize: Implement hint:
		TRBTreeImpl::ChildNodeRef NodeRef = TRBTreeImpl::ChildNodeRef::Invalid();
		AddImpl(KeyValueType{ InKey, ValueType{std::forward<Args>(InValueArgs)...} }, /*Out*/NodeRef);
		return IteratorType(this, NodeRef);
	}

	/**
	* Emplaces a new key value to the tree.
	*/
	template<class ... Args>
	bool AddEmplaceKey(const ValueType& InValue, Args&&... InValueArgs)
	{
		return AddEmplaceHintKey(ConstEndIterator(), InValue, std::forward<Args>(InValueArgs)...);
	}

	/**
	* Emplaces a new key value to the tree at the given iterator position.
	*/
	template<class ... Args>
	bool AddEmplaceHintKey(ConstIteratorType ItPos, const ValueType& InValue, Args&&... InValueArgs)
	{
		// @TODO: Optimize: Implement hint:
		TRBTreeImpl::ChildNodeRef NodeRef = TRBTreeImpl::ChildNodeRef::Invalid();
		AddImpl(KeyValueType{ KeyType{ std::forward<Args>(InValueArgs)... }, InValue}, /*Out*/NodeRef);
		return IteratorType(this, NodeRef);
	}

	/**
	* Adds a new node to the tree.
	*
	* @Returns: true if was added (or false if already was in the tree).
	*/
	bool Add(const KeyValueType& InKV)
	{
		static_assert(std::is_copy_constructible_v<KeyValueType>, "TRBTree: Add: KeyValueType must be copy-constructible");
		KeyValueType KV = InKV;
		TRBTreeImpl::ChildNodeRef NodeRef = TRBTreeImpl::ChildNodeRef::Invalid();
		return AddImpl(std::move(KV), /*Out*/NodeRef);
	}

	/**
	* Adds key/value pairs from C-array buffer. 
	* Key/Value pairs are copied.
	*/
	void Append(const KeyValueType* pInSource, int32_t InCopiedCount)
	{
		BOOST_ASSERT(pInSource);
		Buffer.ReserveGrow(Count + InCopiedCount);
		for (int i = 0; i < InCopiedCount; i++) 
		{
			Add(pInSource[i]);
		}
	}

	/**
	* Adds the given range of elements, assuming that the range is sorted.
	*/
	void AppendSorted(const KeyValueType* pInSource, int32_t InCopiedCount)
	{
		BOOST_ASSERT(pInSource);
		// @TODO: optimize
		if (InCopiedCount == 0) { return; }
		Buffer.ReserveGrow(Count + InCopiedCount);
		const KeyValueType* pPrev = pInSource;
		Add(*pPrev);
		for (int i = 1; i < InCopiedCount; i++)
		{
			BOOST_ASSERT_MSG(CompareLessOrEqual(ComparerArg().Compare(pPrev->Key, pInSource[i].Key)), "TRBTree: AddSorted: C-array values are not sorted by ascending key");
			Add(pInSource[i]);
			pPrev = pInSource + i;
		}
	}

	/**
	* Adds key/value pairs from C-array buffer.
	* Key/Value pairs are moved.
	*/
	void AppendMoved(KeyValueType* pInSource, int32_t InCopiedCount)
	{
		BOOST_ASSERT(pInSource);
		Buffer.ReserveGrow(Count + InCopiedCount);
		for (int i = 0; i < InCopiedCount; i++)
		{
			Add(std::move(pInSource[i]));
		}
	}

	/**
	* Adds key/value pairs from C-array buffer, assuming that the range is sorted.
	*/
	void AppendMovedSorted(KeyValueType* pInSource, int32_t InCopiedCount)
	{
		AppendMoved(pInSource, InCopiedCount);
	}

	/**
	* Adds key/value pairs from range specified by a pair of iterators.
	*
	* Iterator must satisfy the Iterator concept requirements (see IteratorUtils.h).
	*/
	template<class SourceIteratorType>
	void AppendRange(SourceIteratorType FirstIt, SourceIteratorType LastIt)
	{
		static_assert(IsIterator<SourceIteratorType>::Value, "RBTree: AppendRange: Provided type is NOT iterator");
		// @TODO: Check that iterator is NOT iterator of this particular RBTree.
		static_assert(std::is_same<std::remove_cv_t<std::remove_reference_t<decltype(*FirstIt)>>, KeyValueType>::value, "TRBTree: Append iterator range: first iterator must return type convertible to KeyValue pair");
		static_assert(std::is_same<std::remove_cv_t<std::remove_reference_t<decltype(*LastIt)>>, KeyValueType>::value, "TRBTree: Append iterator range: last iterator must return type convertible to KeyValue pair");
		for (SourceIteratorType It = FirstIt; It != LastIt; ++It)
		{
			Add(*It);
		}
	}

	/**
	* Adds key/value pairs from range specified by a pair of iterators,
	* Assuming that the range is sorted.
	*
	* Iterator must satisfy the Iterator concept requirements (see IteratorUtils.h).
	*/
	template<class SourceIteratorType>
	void AppendRangeSorted(SourceIteratorType FirstIt, SourceIteratorType LastIt)
	{
		// @TODO: Optimize
		AppendRange(FirstIt, LastIt);
	}


	/**
	* Adds key/value pairs from range specified by the given iterator
	* (Iterated to the end).
	*
	* Iterator must satisfy the Iterator concept requirements (see IteratorUtils.h).
	*/
	template<class SourceIteratorType>
	void AppendRange(SourceIteratorType It)
	{
		static_assert(IsIterator<SourceIteratorType>::Value, "TVector: AppendRange: It must be iterator");
		// @TODO: Check that iterator is NOT iterator of this particular RBTree.
		for (SourceIteratorType CurrIt = It; CurrIt; ++CurrIt)
		{
			Add(*CurrIt);
		}
	}

	/**
	* Adds key/value pairs from range specified by the given iterator
	* (Iterated to the end).
	* Assuming that the range is sorted.
	*
	* Iterator must satisfy the Iterator concept requirements (see IteratorUtils.h).
	*/
	template<class SourceIteratorType>
	void AppendRangeSorted(SourceIteratorType It)
	{
		AppendRange(It);
	}

	/**
	* Adds the given initializer-list of key/value pairs.
	*
	* Warning: All keys in the initializer list must be unique.
	*/
	void Append(std::initializer_list<KeyValueType> Source)
	{
		for(const KeyValueType& KV : Source)
		{
			Add(KV);
		}
	}

	/**
	* Adds the TRBTree pairs.
	*/
	template<class OtherComparerArg>
	void Append(const TRBTree<KVTypeArg, OtherComparerArg>& InSource)
	{
		for (const KeyValueType& KV : InSource)
		{
			Add(KV);
		}
	}

	/**
	* Adds the TRBTree Key/Value pairs by moving.
	*/
	template<class OtherComparerArg>
	void Append(TRBTree<KVTypeArg, OtherComparerArg>&& InSource)
	{
		// @TODO: Perform real moving
		AppendMoved(InSource);
	}

	/**
	* Adds the TRBTree Key/Value pairs by moving.
	*/
	template<class OtherComparerArg>
	void AppendMoved(TRBTree<KVTypeArg, OtherComparerArg>& InSource)
	{
		// @NOTE: Temporary workaround while iteration does NOT work correctly when removing in the middle of iteration
		{
			for (int i = 0; i < InSource.Count; i++)
			{
				NodeType* pSrcNode = &InSource.Buffer[i];
				if (pSrcNode->Exists())
				{
					Add(std::move(pSrcNode->GetKV()));
				}
			}

			InSource.Clear();
		}

		/*
		TRBTree<KVTypeArg, ComparerArg>::IteratorType It = InSource.Iterator();
		while (true)
		{
			if ( ! It )
			{
				break;
			}
			TRBTree<KVTypeArg, ComparerArg>::IteratorType NextIt = It;
			++NextIt;
			Add(std::move(InSource.GetMovedByIterator(It)));
			It = NextIt;
		}
		*/
	}

	/**
	* Adds the TVector Key/Value pairs.
	*/
	template<template<class> class OtherResizePolicy>
	void AppendSorted(const TVector<KeyValueType, OtherResizePolicy>& InSource)
	{
		// @TODO: Optimize
		Append(InSource);
	}

	/**
	* Adds the TVector Key/Value pairs.
	*/
	template<template<class> class OtherResizePolicy>
	void Append(const TVector<KeyValueType, OtherResizePolicy>& InSource)
	{
		for (const KeyValueType& KV : InSource)
		{
			Add(KV);
		}
	}

	/**
	* Move the TVector Key/Value pairs.
	*/
	template<template<class> class OtherResizePolicy>
	void Append(TVector<KeyValueType, OtherResizePolicy>&& Source)
	{
		AppendMoved(Source);
	}

	/**
	* Move the TVector Key/Value pairs.
	*/
	template<template<class> class OtherResizePolicy>
	void AppendMoved(TVector<KeyValueType, OtherResizePolicy>& InSource)
	{
		for (KeyValueType& KV : InSource)
		{
			Add(std::move(KV));
		}
	}



	/**
	* Adds a new node to the tree by moving.
	*
	* @Returns: true if was added (or false if already was in the tree).
	*/
	bool Add(KeyValueType&& InKV)
	{
		static_assert(std::is_move_constructible_v<KeyValueType>, "TRBTree: Add (&&): KeyValueType must be move-constructible");
		TRBTreeImpl::ChildNodeRef NodeRef = TRBTreeImpl::ChildNodeRef::Invalid();
		return AddImpl(std::move(InKV), /*Out*/NodeRef);
	}

	/**
	* Adds a new node to the tree.
	*
	* @Returns: true if was added (or false if already was in the tree).
	*/
	bool Add(const KeyType& InKey, const ValueType& InValue)
	{
		return Add(KeyValueType{ InKey, InValue });
	}

	/**
	* Adds a new node to the tree by moving.
	*
	* @Returns: true if was added (or false if already was in the tree).
	*/
	bool Add(KeyType&& InKey, ValueType&& InValue)
	{
		return Add(KeyValueType{ std::move(InKey), std::move(InValue) });
	}

	/**
	* Adds a new node to the tree by moving.
	*
	* @Returns: true if was added (or false if already was in the tree).
	*/
	bool Add(KeyType&& InKey, const ValueType& InValue)
	{
		return Add(KeyValueType{ std::move(InKey), InValue });
	}

	/**
	* Adds a new node to the tree by moving.
	*
	* @Returns: true if was added (or false if already was in the tree).
	*/
	bool Add(const KeyType& InKey, ValueType&& InValue)
	{
		return Add(KeyValueType{ InKey, std::move(InValue) });
	}

	/**
	* Removes a node from the tree.
	*
	* @Returns: true if was deleted (or false if value was not found).
	*/

	/**
	* Copies all key-value pairs to the given Buffer.
	* Preserves order.
	*/
	void CopyTo(KeyValueType* pInBuffer)
	{
		KeyValueType* pCurr = pInBuffer;
		ForEach
		(
			[&pCurr](const KeyValueType& KV) { *pCurr = KV; pCurr++; }
		);
	}

	/**
	* Copies all keys to the given Buffer.
	* Preserves order.
	*/
	void CopyKeysTo(KeyType* pInBuffer)
	{
		KeyType* pCurr = pInBuffer;
		ForEachKeys
		(
			[&pCurr](const KeyType& K) { *pCurr = K; pCurr++; }
		);
	}

	/**
	* Copies all value to the given Buffer.
	* Preserves order.
	*/
	void CopyValuesTo(ValueType* pInBuffer)
	{
		ValueType* pCurr = pInBuffer;
		ForEachValues
		(
			[&pCurr](const ValueType& V) { *pCurr = V; pCurr++; }
		);
	}

	/*
	* Copies all key-value pairs to the given Buffer without preserving order.
	*
	* Buffer must be capable to store all the values.
	*/
	void CopyUnorderedTo(KeyValueType* pInBuffer)
	{
		// TODO Optimization: 
		// Iteration over all the nodes may be not the best way.
		BOOST_ASSERT(pInBuffer);
		for (int i = 0; i < Count; i++)
		{
			const NodeType* pSrcNode = GetNode(i);
			if (pSrcNode->Exists())
			{
				pInBuffer[i] = pSrcNode->GetKV();
			}
		}
	}

	/*
	* Copies all keys to the given Buffer without preserving order.
	* Buffer must be capable to store all the values.
	*/
	void CopyUnorderedKeysTo(KeyType* pInBuffer)
	{
		// TODO Optimization: 
		// Iteration over all the nodes may be not the best way.
		BOOST_ASSERT(pInBuffer);
		for (int i = 0; i < Count; i++)
		{
			const NodeType* pSrcNode = GetNode(i);
			if (pSrcNode->Exists())
			{
				pInBuffer[i] = pSrcNode->GetKey();
			}
		}
	}

	/*
	* Copies all values to the given Buffer without preserving order.
	* Buffer must be capable to store all the values.
	*/
	void CopyUnorderedValuesTo(ValueType* pInBuffer)
	{
		// TODO Optimization: 
		// Iteration over all the nodes may be not the best way.
		BOOST_ASSERT(pInBuffer);
		for (int i = 0; i < Count; i++)
		{
			const NodeType* pSrcNode = GetNode(i);
			if (pSrcNode->Exists())
			{
				pInBuffer[i] = pSrcNode->GetValue();
			}
		}
	}

	/**
	* Traverses in the order of the keys.
	*
	* @param TraverseFunc: function that takes reference to key-value pair.
	*/
	template<class TraverseFunc>
	void ForEach(const TraverseFunc& Func)
	{
		return TraverseImpl(Func, this);
	}

	/**
	* Traverses in the order of the keys.
	*
	* @param TraverseFunc: function that takes reference to key-value pair.
	*/
	template<class TraverseFunc>
	void ForEach(const TraverseFunc& Func) const
	{
		return TraverseImpl(Func, this);
	}

	/**
	* Traverses in the order of the keys.
	*
	* @param TraverseFunc: function that takes reference to key pair.
	*/
	template<class TraverseFunc>
	void ForEachKeys(const TraverseFunc& Func)
	{
		return ForEach
		(
			[&Func](const KeyValueType& KV)
			{
				return Func(KV.Key);
			}
		);
	}

	/**
	* Traverses in the order of the keys.
	*
	* @param TraverseFunc: function that takes reference to key pair.
	*/
	template<class TraverseFunc>
	void ForEachKeys(const TraverseFunc& Func) const
	{
		return ForEach
		(
			[&Func](const KeyValueType& KV)
			{
				return Func(KV.Key);
			}
		);
	}

	/**
	* Traverses in the order of the values.
	*
	* @param TraverseFunc: function that takes reference to key pair.
	*/
	template<class TraverseFunc>
	void ForEachValue(const TraverseFunc& Func)
	{
		return ForEach
		(
			[&Func](const KeyValueType& KV)
			{
				return Func(KV.Value);
			}
		);
	}

	/**
	* Traverses in the order of the values.
	*
	* @param TraverseFunc: function that takes reference to key pair.
	*/
	template<class TraverseFunc>
	void ForEachValue(const TraverseFunc& Func) const
	{
		return ForEach
		(
			[&Func](const KeyValueType& KV)
			{
				return Func(KV.Value);
			}
		);
	}

	/**
	* Checks that tree is in the correct state.
	* Slow! To be called on the unit-testing stage only.
	*
	* Checks that: 
	* - Tree is correct;
	* - Red-black invariants hold;
	*/
	bool DebugCheckValid()
	{
		if (Num() == 0) { return true; }

		if ( GetNode(RootIdx)->IsRed() ) { return false; }

		struct SubtreeContext
		{
			TRBTreeImpl::ChildNodeRef NodeRef;
			const SubtreeContext* pParent = nullptr;
			const SubtreeContext* pMaxAncestor = nullptr;
			const SubtreeContext* pMinAncestor = nullptr;

			/**
			* Count of black nodes on the path from the root to this node. 
			* Nil nodes are excluded.
			*/
			int BlackPathLength = 0;

			bool IsRoot() const { return NodeRef.IsRoot(); }

			const KeyType* GetMinKey(const TRBTree* pTree) const
			{
				return pMinAncestor ? &pMinAncestor->GetKey(pTree) : nullptr;
			}

			const KeyType* GetMaximKey(const TRBTree* pTree) const
			{
				return pMaxAncestor ? &pMaxAncestor->GetKey(pTree) : nullptr;
			}

			const KeyType& GetKey(const TRBTree* pTree) const
			{
				return GetNode(pTree)->GetKey();
			}

			const KeyType& GetParentKey(const TRBTree* pTree) const
			{
				BOOST_ASSERT(pParent);
				return pParent->GetKey(pTree);
			}
			const NodeType* GetNode(const TRBTree* pTree) const { return pTree->GetNode(NodeRef); }
			const SubtreeContext* GetGrandpa() const
			{
				if (IsRoot() || pParent->IsRoot())
				{
					return nullptr; 
				}
			 	return pParent->pParent;
			}

			SubtreeContext
			(
				TRBTreeImpl::ChildNodeRef InNodeRef, const SubtreeContext* pInParent,
				const SubtreeContext* pInMinAncestor,
				const SubtreeContext* pInMaxAncestor,
				int InBlackPathLength
			) :
				NodeRef(InNodeRef)
			,	pParent(pInParent) 
			,	pMaxAncestor(pInMaxAncestor)
			,	pMinAncestor(pInMinAncestor)
			,	BlackPathLength(InBlackPathLength)
			{
				BOOST_ASSERT(pParent || InNodeRef.IsRoot());
				BOOST_ASSERT(pMaxAncestor || pMinAncestor || InNodeRef.IsRoot());
			}
		};

		/**
		* Points to the node that is a leaf of the reference black path node.
		*/
		const SubtreeContext* pReferenceBlackPath = nullptr;
		/**
		* Contains a context for each iterated element.
		*/
		TVector<SubtreeContext> Way;
		Way.ReserveGrow(Num());
		Way.Push(SubtreeContext{ TRBTreeImpl::ChildNodeRef::RootNode(), nullptr, nullptr, nullptr, 0 });
		for (int CurrNodeIdx = 0; CurrNodeIdx < Num(); CurrNodeIdx++)
		{
			const SubtreeContext* pNode = &Way[CurrNodeIdx];

			bool bLeaf = true;
			for (TRBTreeImpl::NodeChildIndex childIdx = TRBTreeImpl::LEFT_CHILD_IDX; childIdx <= TRBTreeImpl::RIGHT_CHILD_IDX; childIdx++)			
			{
				TRBTreeImpl::ChildNodeRef ChildRef = GetChildNodeRef(pNode->NodeRef, childIdx);
				if (NodeExists(ChildRef))
				{
					bLeaf = false;

					const SubtreeContext* pChildMinAncestor = nullptr;
					const SubtreeContext* pChildMaxAncestor = nullptr;
					if (childIdx == TRBTreeImpl::LEFT_CHILD_IDX)
					{
						pChildMaxAncestor = pNode;
						pChildMinAncestor = pNode->pMinAncestor;
					}
					else
					{
						pChildMinAncestor = pNode;
						pChildMaxAncestor = pNode->pMaxAncestor;
					}

					int BlackPathLength = pNode->BlackPathLength;
					if (GetNode(ChildRef)->IsBlack())
					{
						BlackPathLength++;
					}

					SubtreeContext* pChildContext = Way.Push
					(
						SubtreeContext
						{ 
							ChildRef,
							pNode, pChildMinAncestor, pChildMaxAncestor,
							BlackPathLength
						}
					);

					// Check that child context is valid
					const KeyType* pChildKey = &pChildContext->GetKey(this);
					const KeyType* pMinKey = pChildContext->GetMinKey(this);
					const KeyType* pMaximKey = pChildContext->GetMaximKey(this);
					if ( pMinKey && ( *pChildKey  < *pMinKey) )
					{
						return false;
					}

					if ( pMaximKey && (*pChildKey > *pMaximKey) )
					{
						return false;
					}

					// Check that parent reference is valid
					if (ChildRef.ParentIdx != GetNodeIndex(pNode->NodeRef))
					{
						return false;
					}
				}
			}

			if (bLeaf)
			{
				if (pReferenceBlackPath)
				{
					bool bBlackPathValid = (pNode->BlackPathLength == pReferenceBlackPath->BlackPathLength);
					if ( ! bBlackPathValid)
					{
						return false;
					}
				}
				else
				{
					pReferenceBlackPath = pNode;
				}
			}
		}
		
		return true;
	}

private:
	friend class NodeIterator;

	using NodeType = TRBTreeImpl::Node<KVTypeArg>;

public:
	/**
	* Iterates KeyValue pairs in the order of their keys.
	*/
	// @TODO: Change class to bidirectional, when Backward iteration implemented
	template<class TreeTypeArg>
	class TGeneralIterator : public TIteratorBase
	<
		TGeneralIterator<TreeTypeArg>,
		std::is_const<TreeTypeArg>, EIteratorClass::Forward
	>
	{
	public:
		template<class OtherTreeType>
		friend class TGeneralIterator;
		
		// To make NodeRef accessible within the tree
		friend typename TreeTypeArg;

		using ThisType = TGeneralIterator<TreeTypeArg>;

		/**
		* Key/Value type of this iterator.
		*/
		using KeyValueType = typename TreeTypeArg::KeyValueType;

		using ElementType = const KeyValueType;

		/**
		* Key that this iterator iterates.
		*/
		using KeyType = const typename KeyValueType::KeyType;

		/**
		* Value that this iterator iterates.
		*/
		using ValueType = typename KeyValueType::ValueType;

		/**
		* Constructs iterator from reference to the given node of the tree.
		*
		* If node reference is invalid, End iterator is created.
		*/
		TGeneralIterator(TreeTypeArg *pInTree, TRBTreeImpl::ChildNodeRef InNodeRef) :
			pTree{ pInTree }
		,	NodeRef{ InNodeRef }
		{
			BOOST_ASSERT(pTree);
		}

		/**
		* Copy constructs.
		*/
		template<class OtherTreeType>
		TGeneralIterator(const TGeneralIterator<OtherTreeType>& InOther) :
			NodeRef{TRBTreeImpl::ChildNodeRef::Invalid()}
		{
			static_assert(IteratorAssignableByConst<ThisType, TGeneralIterator<OtherTreeType>>::Value, "Iterators const mismatch");
			*this = InOther;
		}

		/**
		* Copies.
		*/
		template<class OtherTreeType>
		TGeneralIterator& operator=(const TGeneralIterator<OtherTreeType>& InOther)
		{
			static_assert(IteratorAssignableByConst<ThisType, TGeneralIterator<OtherTreeType>>::Value, "Iterators const mismatch");
			pTree = InOther.pTree;
			NodeRef = InOther.NodeRef;
			return *this;
		}

		/**
		* Returns End iterator.
		*/
		static TGeneralIterator EndIterator(TreeTypeArg *pInTree)
		{
			return TGeneralIterator(pInTree, TRBTreeImpl::ChildNodeRef::Invalid());
		}

		/**
		* Returns current Key/Value pair.
		* Iterator must be a non-end iterator.
		*/
		__forceinline const KeyValueType& GetKeyValue() const
		{
			BOOST_ASSERT(!IsEnd());
			return pTree->GetNode(NodeRef)->GetKV();
		}

		/**
		* Returns current Key/Value pair.
		* Iterator must be a non-end iterator.
		*/
		__forceinline const KeyValueType& operator*() const
		{
			return GetKeyValue();
		}

		/**
		* Pointer to key-value.
		*/
		__forceinline const KeyValueType* GetPtr() const
		{
			return &GetKeyValue();
		}

		/**
		* Member access by pointer.
		*/
		__forceinline const KeyValueType* operator->() const
		{
			return GetPtr();
		}

		/**
		* Returns key.
		*/
		__forceinline const KeyType& GetKey() const { return GetKeyValue().Key; }

		/**
		* Returns value.
		*/
		__forceinline const ValueType& GetValue() const { return GetKeyValue().Value; }

		/**
		* NOT Is end iterator.
		*/
		__forceinline operator bool() const
		{
			return ! IsEnd();
		}

		/**
		* Is end iterator.
		*/
		__forceinline bool operator!() const
		{
			return IsEnd();
		}

		/**
		* Is end iterator.
		*/
		__forceinline bool IsEnd() const
		{
			return IsFloatingEnd();
		}

		/**
		* Is this is end iterator, that always points to the end of the container 
		* (ever after container is changed).
		*/
		__forceinline bool IsFloatingEnd() const
		{
			return NodeRef.IsNull();
		}

		/**
		* Sets current value
		*/
		void SetValue(const ValueType& InValue)
		{
			GetNode()->GetKV().Value = InValue;
		}

		/**
		* Advances iterator to the next KeyValue pair.
		*/
		TGeneralIterator& operator++()
		{
			AdvanceNext();
			return *this;
		}

		/**
		* Advances iterator to the next KeyValue pair.
		*/
		TGeneralIterator operator++(int)
		{
			TGeneralIterator OldIt = *this;
			TGeneralIterator::operator++();
			return OldIt;
		}

		/**
		* Advances an iterator to previous KeyValue pair.
		*/
		TGeneralIterator& operator--();

		/**
		* Advances iterator to the previus KeyValue pair.
		*/
		TGeneralIterator operator--(int);

		template<class OtherTreeType>
		bool operator==(TGeneralIterator<OtherTreeType> B) const
		{
			if (IsEnd() && B.IsEnd())
			{
				return true;
			}
			else if (IsEnd() || B.IsEnd())
			{
				return false;
			}
			return GetKeyValue().Key == B.GetKeyValue().Key;
		}

		template<class OtherTreeType>
		bool operator!=(TGeneralIterator<OtherTreeType> B) const
		{
			return !(operator==(B));
		}

	protected:
		decltype(auto) GetNode() const { return pTree->GetNode(NodeRef); }

		void AdvanceNext()
		{
			if (GetNode()->HasChild(TRBTreeImpl::RIGHT_CHILD_IDX))
			{
				// Find the minimal value in the right subtree;
				NodeRef = pTree->GetDeepestNodeRef(pTree->GetChildNodeRef(NodeRef, TRBTreeImpl::RIGHT_CHILD_IDX), TRBTreeImpl::LEFT_CHILD_IDX);
			}
			else
			{
				// we have no right child, so getting up
				if (NodeRef.IsRoot())
				{
					// We reached the root and have no right child, so stop the iteration
					NodeRef = TRBTreeImpl::ChildNodeRef::Invalid();
				}
				else
				{
					// we have NOT reached the root yet
					if (NodeRef.ChildIdx == TRBTreeImpl::LEFT_CHILD_IDX)
					{
						// We are on the node that is the left child of its parent,
						// and it has no right child itself (left subtree is already iterated). 
						// That's why our parent is the next in order for iteration.
						NodeRef = pTree->GetParentNodeRef(NodeRef);
					}
					else
					{
						// We are on the node that is the right child of its parent,
						// and it has no right child itself (left subtree is already iterated).
						// So, we have no other way than backtrack.
						bool bEndIteration = pTree->BacktrackRightSubtreeIteration(/*Out*/NodeRef);

						// We may end up iteration by climbing from right parent to the root node. 
						// That would mean that the iteration is over.
						if (bEndIteration)
						{
							NodeRef = TRBTreeImpl::ChildNodeRef::Invalid();
						}
					}
				}
			}
		}

	private:
		TreeTypeArg *pTree;
		TRBTreeImpl::ChildNodeRef NodeRef;
	};

private:
	/**
	* Iterates nodes.
	*/
	class NodeIterator
	{
	public:
		/**
		* Constructor
		*/
		NodeIterator(const TRBTree *pInTree, TRBTreeImpl::ChildNodeRef InNodeRef) :
			pTree(pInTree)
		,	NodeRef(InNodeRef) 
		{
			BOOST_ASSERT(pTree);
		}

		/**
		* Returns true if node does NOT exist.
		*/
		__forceinline bool DoesNotExist() const
		{
			return ( ! pTree->NodeExists(NodeRef) );
		}

		/**
		* Reference to node relative to parent 
		* (null in the case of the root node).
		*/
		__forceinline TRBTreeImpl::ChildNodeRef GetNodeRef() const { return NodeRef; }

		/**
		* Parent node.
		*/
		__forceinline const NodeType* GetParentNode() const { return pTree->GetNode(ChildRef.ParentIdx); }

		/**
		* Node.
		*/
		__forceinline const NodeType* GetNode() const { return pTree->GetNode(NodeRef); }

		/**
		* Returns child node reference.
		*/
		__forceinline TRBTreeImpl::ChildNodeRef GetChildRef(TRBTreeImpl::NodeChildIndex ChildIdx) const	
		{
			return pTree->GetChildNodeRef(NodeRef, ChildIdx);
		}

		/**
		* Returns iterator to left.
		*/
		NodeIterator GetLeft() const { return NodeIterator{ pTree, GetChildRef(0) }; }
	
		/**
		* Returns iterator to right.
		*/
		NodeIterator GetRight() const { return NodeIterator{pTree, GetChildRef(1) };
	}

	private:
		/**
		* Pointer to tree.
		*/
		const TRBTree *pTree;
		TRBTreeImpl::ChildNodeRef NodeRef;
	};

	template<class InKVTypeArg>
	IteratorType ReplaceByKeyImpl(const KeyType& InKey, InKVTypeArg&& InKV)
	{
		// @TODO: Optimize? GetOrAdd performs extra check
		IteratorType It = FindIteratorFor(InKey);
		if (It.IsEnd())
		{
			IteratorType It = GetOrAdd(std::forward<InKVTypeArg>(InKV));
		}
		else
		{
			IteratorType NewIt = ReplaceByIterator(It, std::forward<InKVTypeArg>(InKV));
			return NewIt;
		}
	}

	template<class OtherKVTypeArg>
	IteratorType ReplaceByIteratorImpl(ConstIteratorType It, OtherKVTypeArg&& InKV)
	{
		BOOST_ASSERT(It);
		RemoveAt(It);
		// @TODO: Optimize: Doing extra check here
		It = GetOrAdd(std::forward<KeyValueTypeArg>(InKV));
		return It;
	}

	template<class KeyValueTypeArg>
	IteratorType ReplaceImpl(KeyValueTypeArg&& InKV)
	{
		IteratorType It = FindIteratorFor(InKV.Key);
		if (It)
		{
			bool bReplaced = Remove(InKV.Key);
			BOOST_ASSERT(bReplaced);
		}
		// @TODO: Optimize: Doing extra check here
		It = GetOrAdd(std::forward<KeyValueTypeArg>(InKV));
		return It;
	}


	/**
	* Searches Value by the given key.
	*
	* @Returns: Pointer to the Value (or nullptr, if NOT found).
	*/
	const ValueType* FindValueImpl(const KeyType& InKey) const
	{
		const KeyValueType* KeyValue = Find(InKey);
		if (KeyValue == nullptr)
		{
			return nullptr;
		}
		return &KeyValue->Value;
	}

	/**
	* Traverses non-empty subtree in the order of the keys.
	*
	* @param TraverseFunc: function that takes reference to key-value pair.
	*/
	template<class TraverseFunc>
	void TraverseSubtree(TRBTreeImpl::ChildNodeRef InRootRef, TraverseFunc Func)
	{
		BOOST_ASSERT_MSG( NodeExists(InRootRef), "TRBTree::TraverseSubtree: subtree must be non-empty" );
		
		// TODO: May this implementation overflow the stack?

		TRBTreeImpl::ChildNodeRef const LeftRef = GetChildNodeRef(InRootRef, TRBTreeImpl::LEFT_CHILD_IDX);
		if ( NodeExists(LeftRef) )
		{
			TraverseSubtree(LeftRef, Func);
		}

		Func(GetNode(InRootRef)->GetKV());

		TRBTreeImpl::ChildNodeRef const RightRef = GetChildNodeRef(InRootRef, TRBTreeImpl::RIGHT_CHILD_IDX);
		if ( NodeExists(RightRef))
		{
			TraverseSubtree(RightRef, Func);
		}
	}

	/**
	* Searches node with the given key in NON-EMPTY (!) container in the given range.
	*
	* Returns true, if the node is found.
	*/
	template<class SearchKeyType, class Comparer>
	bool FindNode_InRange
	(
		const SearchKeyType& InKey, TRBTreeImpl::ChildNodeRef& OutNodeRef, Comparer InComparer,
		ConstIteratorType ItStart, ConstIteratorType ItEnd
	) const 
	{
		BOOST_ASSERT(false && "FindNode_InRange: NOT yet implemented");  return false;
	}

	/**
	* Searches node with the given key in NON-EMPTY (!) container.
	*
	* Returns true, if the node is found.
	*/
	template<class SearchKeyType, class Comparer>
	bool FindNode
	(
		const SearchKeyType& InKey, TRBTreeImpl::ChildNodeRef& OutNodeRef, Comparer InComparer
	) const
	{
		BOOST_ASSERT_MSG( ! Empty(), "TRBTree: FindNode: Container must be non-empty" );

		OutNodeRef = TRBTreeImpl::ChildNodeRef::RootNode();
		NodeIterator It { this, OutNodeRef };

		while (true)
		{
			const KeyType* pCurrKey = &(It.GetNode()->GetKey());

			int CompareResult = InComparer.Compare(*pCurrKey, InKey);
			
			if (CompareEqual(CompareResult))
			{
				return true;
			}

			if(CompareGreater(CompareResult))
			{
				It = It.GetLeft();
			}
			else if(CompareLess(CompareResult))
			{
				It = It.GetRight();
			}

			OutNodeRef = It.GetNodeRef();

			if (It.DoesNotExist())
			{
				return false;
			}
		}

		BOOST_ASSERT_MSG(false, "Should NOT get here");
		return false;
	}

	/**
	* Removes node by the given reference.
	* WARNING!!! The node content is still alive, however 
	* (key/value is NOT destroyed).
	*
	* Returns pointer to the removed node.
	*/
	NodeType* DoRemoveNodeByRef(TRBTreeImpl::ChildNodeRef NodeRef, TRBTreeImpl::ChildNodeRef& OutSubstitutorRef, bool& bOutNeedsFixup)
	{
		// Save node to mark it deleted later
		NodeType* const pNode = GetNode(NodeRef);

		TRBTreeImpl::NodeChildIndex ChildToRelinkToIdx = INDEX_NONE;
		if (!pNode->HasChild(TRBTreeImpl::LEFT_CHILD_IDX))
		{
			// Warning!!! We relink to other (possibly existing) node!!!
			ChildToRelinkToIdx = TRBTreeImpl::RIGHT_CHILD_IDX;
		}
		else if (!pNode->HasChild(TRBTreeImpl::RIGHT_CHILD_IDX))
		{
			// Warning!!! We relink to other (possibly existing) node!!!
			ChildToRelinkToIdx = TRBTreeImpl::LEFT_CHILD_IDX;
		}

		if (ChildToRelinkToIdx == INDEX_NONE)
		{
			//RemoveNode_MakeRightSubtreeChildOfPredecessor(NodeRef, OutSubstitutorRef, bOutNeedsFixup);
			RemoveNode_NodeHasBothChildren(NodeRef, OutSubstitutorRef, bOutNeedsFixup);
		}
		else
		{
			/**
			* If node has only one child we can remove it by relinking its parent to the child.
			*/
			RemoveNode_LinkToChild(NodeRef, ChildToRelinkToIdx, OutSubstitutorRef, bOutNeedsFixup);
		}

		// We decrement count here, however, the node is still alive.
		Count--;
		return pNode;
	}

	void RemoveNode_NodeHasBothChildren(TRBTreeImpl::ChildNodeRef InNodeRef, TRBTreeImpl::ChildNodeRef& OutSubstitutorRef, bool& bOutNeedsFixup)
	{
		TRBTreeImpl::NodeIndex const OldNodeIdx = GetNodeIndex(InNodeRef);
		const NodeType* const pOldNode = GetNode(OldNodeIdx);

		TRBTreeImpl::ChildNodeRef const RightChildRef = GetChildNodeRef(InNodeRef, TRBTreeImpl::RIGHT_CHILD_IDX);
		TRBTreeImpl::NodeIndex const OldRightChildIdx = GetNodeIndex(RightChildRef);

		TRBTreeImpl::ChildNodeRef const LeftChildRef = GetChildNodeRef(InNodeRef, TRBTreeImpl::LEFT_CHILD_IDX);
		TRBTreeImpl::NodeChildIndex OldLeftChildIdx = GetNodeIndex(LeftChildRef);

		TRBTreeImpl::ChildNodeRef const PredecessorRef = GetDeepestNodeRef(LeftChildRef, /*ChildIdx=*/ TRBTreeImpl::RIGHT_CHILD_IDX);
		TRBTreeImpl::NodeIndex const PredecessorIdx = GetNodeIndex(PredecessorRef);

		/**
		* Determine, whether Red-black tree properties corrupted and needs a fixup.
		* WARNING! To be called before color is copied from the predecessor.
		*/
		bOutNeedsFixup = GetNode(PredecessorIdx)->IsBlack();

		GetNode(PredecessorIdx)->CopyColorFrom(pOldNode);

		TRBTreeImpl::ChildNodeRef const RightChildOfPredecessorRef = GetChildNodeRef(PredecessorRef, TRBTreeImpl::RIGHT_CHILD_IDX);
		BOOST_ASSERT_MSG(!NodeExists(RightChildOfPredecessorRef), "TRBTree::RemoveNode_MakeRightSubtreeChildOfPredecessor: Right child of the predecessor must NOT exist");
		OutSubstitutorRef = GetChildNodeRef(PredecessorRef, TRBTreeImpl::LEFT_CHILD_IDX);
		TRBTreeImpl::NodeIndex SubstitutorIdx = GetNodeIndex(OutSubstitutorRef);

		// Relink Y on place of Z
		LinkToNewParentByNewReference(PredecessorIdx, InNodeRef);

		// Relink right subtree of the Z on place of the right subtree of Y
		LinkToNewParentByNewReference(OldRightChildIdx, RightChildOfPredecessorRef);

		// Manage left subtree of Z
		if (OldLeftChildIdx != PredecessorIdx)
		{
			LinkToNewParentByNewReference(OldLeftChildIdx, OutSubstitutorRef); // OldLeftChildIdx == left(z)		
			LinkToNewParentByNewReference(SubstitutorIdx, PredecessorRef); // substitutorIdx == left(y)
		}			
	}

	// @TODO: Bugged function, Use RemoveNode_NodeHasBothChildren instead
	void RemoveNode_MakeRightSubtreeChildOfPredecessor(TRBTreeImpl::ChildNodeRef InNodeRef, TRBTreeImpl::ChildNodeRef& OutSubstitutorRef, bool& bOutNeedsFixup)
	{
		TRBTreeImpl::NodeIndex const OldNodeIdx = GetNodeIndex(InNodeRef);
		const NodeType* const pOldNode = GetNode(OldNodeIdx);

		TRBTreeImpl::ChildNodeRef const RightChildRef = GetChildNodeRef(InNodeRef, TRBTreeImpl::RIGHT_CHILD_IDX);
		TRBTreeImpl::NodeIndex const OldRightChildIdx = GetNodeIndex(RightChildRef);

		TRBTreeImpl::ChildNodeRef const LeftChildRef = GetChildNodeRef(InNodeRef, TRBTreeImpl::LEFT_CHILD_IDX);
		TRBTreeImpl::NodeChildIndex OldLeftChildIdx = GetNodeIndex(LeftChildRef);
		
		TRBTreeImpl::ChildNodeRef const PredecessorRef = GetDeepestNodeRef(LeftChildRef, /*ChildIdx=*/ TRBTreeImpl::RIGHT_CHILD_IDX);
		TRBTreeImpl::NodeIndex const PredecessorIdx = GetNodeIndex(PredecessorRef);


		/**
		* Determine, whether Red-black tree properties corrupted and needs a fixup.
		* WARNING! To be called before color is copied from the predecessor.
		*/
		bOutNeedsFixup = GetNode(PredecessorIdx)->IsBlack();

		GetNode(PredecessorIdx)->CopyColorFrom(pOldNode);
		
		TRBTreeImpl::ChildNodeRef const RightChildOfPredecessorRef = GetChildNodeRef(PredecessorRef, TRBTreeImpl::RIGHT_CHILD_IDX);
		BOOST_ASSERT_MSG( ! NodeExists(RightChildOfPredecessorRef), "TRBTree::RemoveNode_MakeRightSubtreeChildOfPredecessor: Right child of the predecessor must NOT exist");
		OutSubstitutorRef = GetChildNodeRef(PredecessorRef, TRBTreeImpl::LEFT_CHILD_IDX);
		TRBTreeImpl::NodeChildIndex SubstitutorIdx = GetNodeIndex(OutSubstitutorRef);

		// TODO: Old code: remove after testing:
		//LinkToNewParentByNewReference(OldLeftChildIdx, InNodeRef);
		//LinkToNewParentByNewReference(OldRightChildIdx, RightChildOfPredecessorRef);
		//GetNode(OldNodeIdx)->SetChild(TRBTreeImpl::LEFT_CHILD_IDX, INDEX_NONE);

		LinkToNewParentByNewReference(PredecessorIdx, InNodeRef);
		if (OldLeftChildIdx != PredecessorIdx)
		{
			LinkToNewParentByNewReference(OldLeftChildIdx, OutSubstitutorRef);
			LinkToNewParentByNewReference(SubstitutorIdx, PredecessorRef);			
		}
		else
		{
			LinkToNewParentByNewReference(SubstitutorIdx, PredecessorRef);
			// In this case substitutor's position relative to the predecessor is kept the same.
		}
		LinkToNewParentByNewReference(OldRightChildIdx, RightChildOfPredecessorRef);
	}

	/**
	* Removes node by linking to the given child.
	*
	* The given child may be absent.
	*/
	void RemoveNode_LinkToChild(TRBTreeImpl::ChildNodeRef InNodeRef, TRBTreeImpl::NodeChildIndex ChildIdx, TRBTreeImpl::ChildNodeRef& OutSubstitutorRef, bool& bOutNeedsFixup)
	{
		TRBTreeImpl::NodeIndex NodeIdx = GetNodeIndex(InNodeRef);
		TRBTreeImpl::ChildNodeRef const NextNodeRef = GetChildNodeRef(InNodeRef, ChildIdx);
		bool const bChildExists = NodeExists(NextNodeRef);
		TRBTreeImpl::NodeIndex const NextNodeIdx = bChildExists ? GetNodeIndex(NextNodeRef) : INDEX_NONE;

		/**
		* Determine whether red-black properties are corrupted and needs a fixup.
		* WARNING!! To be checked only AFTER color is updated.
		*/
		bOutNeedsFixup = GetNode(InNodeRef)->IsBlack();

		//if (bChildExists)
		//{
		//	GetNode(NextNodeIdx)->CopyColorFrom(GetNode(InNodeRef));
		//}

		
		if (InNodeRef.IsRoot())
		{
			if (bChildExists)
			{
				GetNode(NextNodeIdx)->ParentIdx = RootIdx;	
			}
			RootIdx = NextNodeIdx;
		}
		else
		{
			if (bChildExists)
			{
				GetNode(NextNodeIdx)->ParentIdx = InNodeRef.ParentIdx;
			}
			GetNode(InNodeRef.ParentIdx)->SetChild(InNodeRef.ChildIdx, NextNodeIdx);
		}

		/**
		* WARNING!!! Because we relinked the substitutor (child node) to parent of the NodeRef,
		* we must return the reference relative to that parent.
		*/
		OutSubstitutorRef = InNodeRef;
	}

	/**
	* Implementation of the Add function that inserts exactly before or after element with the given reference
	*/
	bool AddHintImpl(KeyValueType&& InKV, TRBTreeImpl::ChildNodeRef& InOutNodeRef)
	{
		BOOST_ASSERT_MSG(false, "TRBTree: AddImpl: NOT yet impl"); return false;
	}

	/**
	* Implementation of the Add function.
	*/
	bool AddImpl(KeyValueType&& InKV, TRBTreeImpl::ChildNodeRef& OutNodeRef)
	{
		bool bWasAlreadyInContainer = false;

		if ( ! TryHandleAddEmptyCase(std::move(InKV), /*Out*/OutNodeRef) )
		{
			if ( FindNode(InKV.Key, /*Out*/OutNodeRef, ComparerArg()))
			{
				bWasAlreadyInContainer = true;
			}
			else
			{
				AddNewNodeAtRef(std::move(InKV), OutNodeRef);
			}
		}
		bool const bAddedNow = ( ! bWasAlreadyInContainer );

		return PostAddImpl(bAddedNow, OutNodeRef);
	}

	/**
	* Common finishing code for all AddImpl functions.
	*/
	bool PostAddImpl(bool bInAddedNow, TRBTreeImpl::ChildNodeRef NodeRef)
	{
		if (bInAddedNow && Num() >= 3)
		{
			FixupRedBlackAfterAdd(NodeRef);
		}
		// Uncomment for testing purposes only (will greately slow):
		BOOST_ASSERT_MSG(DebugCheckValid(), "TRBTree::Add: tree state must be valid");
		return bInAddedNow;
	}

	/**
	* Handles the case when the container is empty.
	*
	* @returns: true if was handled as empty.
	* @argument: InKV: will only be moved if contained is empty.
	*/
	bool TryHandleAddEmptyCase(KeyValueType&& InKV, TRBTreeImpl::ChildNodeRef& OutNodeRef)
	{
		if (Empty())
		{
			OutNodeRef = TRBTreeImpl::ChildNodeRef::RootNode();
			RootIdx = CreateNewNode(std::move(InKV), INDEX_NONE);
			GetNode(RootIdx)->MakeBlack();
			return true;
		}
		return false;
	}

	/**
	* Creates a new node.
	* Automatically increments the count of nodes.
	*/
	TRBTreeImpl::NodeIndex CreateNewNode(KeyValueType&& InKV, TRBTreeImpl::NodeIndex InParentIdx)
	{
		TRBTreeImpl::NodeIndex const NewNodeIndex = TRBTreeImpl::NodeIndex { Buffer.Len() };
		NodeType NewNode { std::move(InKV), InParentIdx };
		Buffer.Add(std::move(NewNode));
		Count++;
		return NewNodeIndex;
	}

	/**
	* Creates and appends a node by the specific reference.
	*/
	void AddNewNodeAtRef(KeyValueType&& InKV, TRBTreeImpl::ChildNodeRef Where)
	{
		BOOST_ASSERT_MSG( ! Empty(), "TRBTree::AddNewNodeAtRef: the container must contain at least one node");
		BOOST_ASSERT_MSG( ! Where.IsRoot(), "TRBTree::AddNewNodeAtRef: we cannot append at the root node position" );
		BOOST_ASSERT_MSG( ! NodeExists(Where), "TRBTree::AddNewNodeAtRef: the position must be not occupied" );
		TRBTreeImpl::NodeIndex const NewNodeIdx = CreateNewNode(std::move(InKV), Where.ParentIdx);
		NodeType* const pParentNode = GetParentNode(Where);
		pParentNode->SetChild(Where.ChildIdx, NewNodeIdx);
	}

	/**
	* Removes element by the given ref and destroys it.
	*/
	void RemoveAndDestroy(TRBTreeImpl::ChildNodeRef NodeRef)
	{
		NodeType* const pNode = RemoveImpl(NodeRef);
		pNode->Destroy();
	}

	/**
	* Removes element by the given ref and moves the corresponding Key/Value pair as the result.
	*/
	KeyValueType RemoveAndMove(TRBTreeImpl::ChildNodeRef NodeRef)
	{
		NodeType* const pNode = RemoveImpl(NodeRef);
		KeyValueType MoveKV = std::move(pNode->GetKV());
		// NOTE: We must mark destroyed the node anyway
		pNode->Destroy();
		return std::move(MoveKV);
	}

	/**
	* Removes element by the given ref.
	* Node by the given reference must exist.
	* Common code path for all remove operations.
	*
	* @returns: Removed node.
	*/
	NodeType* RemoveImpl(TRBTreeImpl::ChildNodeRef NodeRef)
	{
		BOOST_ASSERT_MSG(NodeExists(NodeRef), "TRBTree: RemoveImpl: Referenced node must exist");

		bool bNeedsFixup = false;
		TRBTreeImpl::ChildNodeRef NodeSubstitutorRef = TRBTreeImpl::ChildNodeRef::Invalid();
		NodeType* const pNode = DoRemoveNodeByRef(NodeRef, /*Out*/NodeSubstitutorRef, /*Out*/bNeedsFixup);
		// Deal with the deleted node (we must done it AFTER the links are updated to avoid some assertion to fire)
		if (bNeedsFixup && (!Empty()))
		{
			FixupRedBlackAfterRemove(NodeSubstitutorRef);
		}

		// Uncomment for testing purposes only (will greately slow):
		BOOST_ASSERT_MSG(DebugCheckValid(), "TRBTree::Remove: tree state must be valid");
		return pNode;
	}

	/**
	* Fixups the properties of the Red-black tree after node is removed.
	*/
	void FixupRedBlackAfterRemove(TRBTreeImpl::ChildNodeRef SubstitutorNodeRef)
	{
		BOOST_ASSERT_MSG( ! Empty() , "TRBTree::FixupRedBlackAfterRemove: The tree must be non-empty");

		TRBTreeImpl::ChildNodeRef CurrNodeRef = SubstitutorNodeRef;
		NodeType* pCurrNode = nullptr;
		while (true)
		{
			if (CurrNodeRef.IsRoot())
			{
				pCurrNode = GetNode(CurrNodeRef);
				break;
			}

			TRBTreeImpl::ChildNodeRef BrotherRef{ CurrNodeRef.ParentIdx, TRBTreeImpl::InvertChildIndex(CurrNodeRef.ChildIdx) };
			TRBTreeImpl::ChildNodeRef ParentRef = GetParentNodeRef(CurrNodeRef);
			if ( ! NodeExists(CurrNodeRef) )
			{
				BOOST_ASSERT_MSG( NodeExists(BrotherRef), "TRBTree:FixupRedBlackAfterRemove: at this point the brother must exist");
				GetNode(BrotherRef)->MakeRed();
				CurrNodeRef = ParentRef;
				continue;
			}

			pCurrNode = GetNode(CurrNodeRef);

			TRBTreeImpl::NodeIndex CurrNodeIdx = GetNodeIndex(CurrNodeRef);
			if (pCurrNode->IsRed())
			{
				break;
			}
				
			if ( ! NodeExists(BrotherRef) )
			{
				//BOOST_ASSERT_MSG(false, "TRBTree: FixupRedBlackAfterRemove: The case is NOT yet coded");
				CurrNodeRef = ParentRef;
				break;
			}
			else
			{
				NodeType* pBrother = GetNode(BrotherRef);

				TRBTreeImpl::ChildNodeRef BrotherChildRef = GetChildNodeRef(BrotherRef, CurrNodeRef.ChildIdx);
				TRBTreeImpl::ChildNodeRef BrotherOtherChildRef = GetChildNodeRef(BrotherRef, BrotherRef.ChildIdx);

				if (pBrother->IsRed())
				{
					GetNode(BrotherRef)->MakeBlack();
					GetNode(ParentRef)->MakeRed();

					RotateBrotherUpAroundParent_AfterDeleted(BrotherRef, ParentRef);

					// TODO: Remove
					//RotateAround(ParentRef, BrotherRef.ChildIdx);

					// Really, brother reference should NOT change during rotation:
					//BrotherRef = TRBTreeImpl::ChildNodeRef(OldParentIdx, BrotherRef.ChildIdx);

					// However, the referenced brother is changed:
					pBrother = GetNode(BrotherRef);

					/**
					* Parent now has another parent, so we must recalculate the reference.
					*
					* TODO: OPTIMIZATION: Should we recalculate it always, maybe we will not use it?
					*/
					ParentRef = GetParentNodeRef(CurrNodeRef);
				}

				BOOST_ASSERT_MSG(pBrother->IsBlack(), "TRBTree::FixupRedBlackAfterRemove: Case 2: At this point the brother must be black");

				BrotherChildRef = GetChildNodeRef(BrotherRef, CurrNodeRef.ChildIdx);
				BrotherOtherChildRef = GetChildNodeRef(BrotherRef, BrotherRef.ChildIdx);
				
				bool bBrotherChild_BlackOrNull = true;
				bool bBrotherOtherChild_BlackOrNull = true;

				NodeType* pBrotherChild = nullptr;
				NodeType* pBrotherOtherChild = nullptr;

				if (NodeExists(BrotherChildRef))
				{
					pBrotherChild = GetNode(BrotherChildRef);
					bBrotherChild_BlackOrNull = pBrotherChild->IsBlack();
				}
				else
				{
					pBrotherChild = nullptr;
					bBrotherChild_BlackOrNull = true;
				}

				if (NodeExists(BrotherOtherChildRef))
				{
					pBrotherOtherChild = GetNode(BrotherOtherChildRef);
					bBrotherOtherChild_BlackOrNull = pBrotherOtherChild->IsBlack();
				}
				else
				{
					pBrotherOtherChild = nullptr;
					bBrotherOtherChild_BlackOrNull = true;
				}

				// Second case (see Cormen)
				if (bBrotherChild_BlackOrNull && bBrotherOtherChild_BlackOrNull)
				{
					pBrother->MakeRed();
					CurrNodeRef = ParentRef;
					// TODO: Should we make the parent black ? (it was NOT in the Corman)
					continue;
				}

				// Third case
				if (bBrotherOtherChild_BlackOrNull)
				{
					BOOST_ASSERT_MSG(pBrotherChild->IsRed(), "TRBTree::FixupRedBlackAfterRemove: case 3: At this point brother child must be red");
					pBrotherChild->MakeBlack();
					pBrother->MakeRed();

					// TODO: We need to disable recoloring here
					RotateAround(BrotherRef, CurrNodeRef.ChildIdx);

					/**
					* NOTE: Brother is always relative to the current node,
					* and the current node is NOT changed.
					* So, because the parent of the current node is not changed,
					* and reference to the brother is relative to it, the reference to the brother is NOT changed.
					*/
					pBrother = GetNode(BrotherRef);

					/**
					*  We must update the links to the children.
					*/
					if (NodeExists(BrotherChildRef))
					{
						pBrotherChild = GetNode(BrotherChildRef);
						bBrotherChild_BlackOrNull = pBrotherChild->IsBlack();
					}
					else
					{
						pBrotherChild = nullptr;
						bBrotherChild_BlackOrNull = true;
					}

					if (NodeExists(BrotherOtherChildRef))
					{
						pBrotherOtherChild = GetNode(BrotherOtherChildRef);
						bBrotherOtherChild_BlackOrNull = pBrotherOtherChild->IsBlack();
					}
					else
					{
						pBrotherOtherChild = nullptr;
						bBrotherOtherChild_BlackOrNull = true;
					}
				}
				BOOST_ASSERT_MSG(pBrother->IsBlack(), "TRBTree::FixupRedBlackAfterRemove: Case  4: At this point the brother must be black");
				BOOST_ASSERT_MSG(pBrotherOtherChild->IsRed(), "TRBTree::FixupRedBlackAfterRemove: Case 4: At this point other child of brother must be red");

				// Fourth case
				NodeType* pParent = GetNode(ParentRef);
				pBrother->CopyColorFrom(pParent);
				pParent->MakeBlack();
				if (pBrotherOtherChild)
				{
					// Should the brother child indice are to be updated?
					pBrotherOtherChild->MakeBlack();
				}

				pBrother->CopyColorFrom(pParent);
				pParent->MakeBlack();
				pBrotherOtherChild->MakeBlack();
				RotateBrotherUpAroundParent_AfterDeleted(BrotherRef, ParentRef);
				
				//RotateAround(ParentRef, BrotherRef.ChildIdx);

				// We must set current node to root, because of the MakeBlack call after the loop
				pCurrNode = GetNode(RootIdx);
				break;
			}
		}

		pCurrNode->MakeBlack();
	}

	/**
	* Rotates brother up around the parent node.
	*/
	void RotateBrotherUpAroundParent_AfterDeleted(TRBTreeImpl::ChildNodeRef BrotherRef, TRBTreeImpl::ChildNodeRef ParentRef)
	{
		BOOST_ASSERT(NodeExists(BrotherRef));
		BOOST_ASSERT(NodeExists(ParentRef));

		TRBTreeImpl::ChildNodeRef const BrotherChildRef = GetChildNodeRef(BrotherRef, TRBTreeImpl::InvertChildIndex(BrotherRef.ChildIdx));
		TRBTreeImpl::ChildNodeRef const BrotherOtherChildRef = GetChildNodeRef(BrotherRef, BrotherRef.ChildIdx);

		if (NodeExists(BrotherOtherChildRef))
		{
			// Same child idx of brother and child of brother
			Rotate_SameChildIdx(GetNodeIndex(BrotherOtherChildRef), ParentRef, BrotherOtherChildRef.ChildIdx);
		}
		else
		{
			// Different child idx of brother and child of brother
			Rotate_DifferentChildIdx(GetNodeIndex(BrotherChildRef), ParentRef, BrotherChildRef.ChildIdx);
		}
	}

	/**
	* Rotates around the given node.
	* Correctly choose which mode to rotate.
	*
	* @argument: ChildIdx: index of the child of the node to rotate around
	* (if child is left, then we rotate to the right and vice versa).
	*/
	void RotateAround(TRBTreeImpl::ChildNodeRef AroundNode, TRBTreeImpl::NodeChildIndex ChildIdx)
	{
		TRBTreeImpl::ChildNodeRef ParentRef = GetChildNodeRef(AroundNode, ChildIdx);
		TRBTreeImpl::ChildNodeRef NodeRef = GetChildNodeRef(ParentRef, TRBTreeImpl::LEFT_CHILD_IDX);
		if ( ! NodeExists(NodeRef) )
		{
			NodeRef = GetChildNodeRef(ParentRef, TRBTreeImpl::RIGHT_CHILD_IDX);
		}
		// TODO: Substitute with concrete particular case rotations
		RotateAndColorSubtree_ForNewRedNode(NodeRef, ParentRef, AroundNode);
	}

	/**
	* Fixups the properties of the Red-black tree after the new node addition.
	*/
	void FixupRedBlackAfterAdd(TRBTreeImpl::ChildNodeRef NodeRef)
	{
		BOOST_ASSERT_MSG(Num()>= 3, "TRBTree::FixupRedBlackAfterAdd: The tree must already contain at least 3 nodes (including the new added one) before the fixup operation");
		while (true)
		{
			TRBTreeImpl::ChildNodeRef ParentRef = GetParentNodeRef(NodeRef);

			if ( ! NodeExists(ParentRef) )
			{
				return;
			}

			// NOTE: WE skip the check that parent is root, because if root, then by definition black.
			if (/*ParentRef.IsRoot()|| */ GetNode(ParentRef)->IsBlack())
			{
				return;
			}

			// NOTE: Grandpa always exists because parent is not a root node.
			TRBTreeImpl::ChildNodeRef GrandpaRef = GetParentNodeRef(ParentRef);
			TRBTreeImpl::ChildNodeRef UncleRef = GetChildNodeRef(GrandpaRef, TRBTreeImpl::InvertChildIndex(ParentRef.ChildIdx));
			if ( ( ! NodeExists(UncleRef) ) || GetNode(UncleRef)->IsBlack())
			{
				// At this point both parent and the new node are red,
				// and we cannot perform recoloring because uncle is black.
				NodeRef = RotateAndColorSubtree_ForNewRedNode(NodeRef, ParentRef, GrandpaRef);
				return;
			}
			else
			{
				// At this point both parent and the new node are red,
				// and the is uncle is red, so we can perform recoloring.
				GetNode(ParentRef)->MakeBlack();
				GetNode(UncleRef)->MakeBlack();
				if ( ! GrandpaRef.IsRoot() )
				{
					GetNode(GrandpaRef)->MakeRed();
					NodeRef = GrandpaRef;
				}
				else
				{
					return;
				}
			}
		}
	}

	/**
	* Handles all rotation cases.
	* Returns reference to the node to be visited next.
	*/
	TRBTreeImpl::ChildNodeRef RotateAndColorSubtree_ForNewRedNode(TRBTreeImpl::ChildNodeRef NodeRef, TRBTreeImpl::ChildNodeRef ParentRef, TRBTreeImpl::ChildNodeRef GrandpaRef)
	{
		int const NodeIndex = GetNodeIndex(NodeRef);
		GetNode(GrandpaRef)->MakeRed();
		if (NodeRef.ChildIdx == ParentRef.ChildIdx)
		{
			GetNode(ParentRef)->MakeBlack();
			return Rotate_SameChildIdx(NodeIndex, GrandpaRef, NodeRef.ChildIdx);
		}
		else
		{
			GetNode(NodeIndex)->MakeBlack();
			return Rotate_DifferentChildIdx(NodeIndex, GrandpaRef, NodeRef.ChildIdx);
		}
	}

	/**
	* Handles rotation case where new node and its parent
	* are left and right childs or vice versa.
	*/
	TRBTreeImpl::ChildNodeRef Rotate_DifferentChildIdx(TRBTreeImpl::NodeIndex NodeIndex, TRBTreeImpl::ChildNodeRef GrandpaRef, TRBTreeImpl::NodeChildIndex ChildIdx)
	{
		TRBTreeImpl::NodeChildIndex const InvertedChildIdx = TRBTreeImpl::InvertChildIndex(ChildIdx);

		TRBTreeImpl::ChildNodeRef const OldParentRef = GetChildNodeRef(GrandpaRef, InvertedChildIdx);
		TRBTreeImpl::ChildNodeRef const OldNewNodeRef = GetChildNodeRef(OldParentRef, ChildIdx);
		// Child of the new node with childIdx equal to the child idx of the new node itself.
		TRBTreeImpl::ChildNodeRef const ChildOfNewRef = GetChildNodeRef(OldNewNodeRef, ChildIdx);
		TRBTreeImpl::ChildNodeRef const OtherChildOfNewRef = GetChildNodeRef(OldNewNodeRef, InvertedChildIdx);

		TRBTreeImpl::NodeIndex const OldParentIdx = GetNodeIndex(OldParentRef);
		TRBTreeImpl::NodeIndex const OldGrandpaIdx = GetNodeIndex(GrandpaRef);
		TRBTreeImpl::NodeIndex const OldChildOfNewIdx = GetNodeIndex(ChildOfNewRef);
		TRBTreeImpl::NodeIndex const OldOtherChildOfNewIdx = GetNodeIndex(OtherChildOfNewRef);

		LinkToNewParentByNewReference(NodeIndex, GrandpaRef);
		LinkToNewParentByNewReference(OldParentIdx, OtherChildOfNewRef);
		LinkToNewParentByNewReference(OldGrandpaIdx, ChildOfNewRef);
		LinkToNewParentByNewReference(OldOtherChildOfNewIdx, OldNewNodeRef);
		LinkToNewParentByNewReference(OldChildOfNewIdx, OldParentRef);

		return GrandpaRef;
	}

	/**
	* Handles rotation case where both the new node and its parent
	* are both left or right childs.
	*/
	TRBTreeImpl::ChildNodeRef Rotate_SameChildIdx(TRBTreeImpl::NodeIndex NodeIndex, TRBTreeImpl::ChildNodeRef GrandpaRef, TRBTreeImpl::NodeChildIndex ChildIdx)
	{
		TRBTreeImpl::ChildNodeRef const OldParentRef = GetChildNodeRef(GrandpaRef, ChildIdx);
		TRBTreeImpl::ChildNodeRef const BrotherRef = GetChildNodeRef(OldParentRef, TRBTreeImpl::InvertChildIndex(ChildIdx));
		TRBTreeImpl::NodeIndex const OldParentIdx = GetNodeIndex(OldParentRef);
		TRBTreeImpl::NodeIndex const OldGrandpaIdx = GetNodeIndex(GrandpaRef);
		TRBTreeImpl::NodeIndex const OldBrotherIdx = GetNodeIndex(BrotherRef);
		
		LinkToNewParentByNewReference(OldParentIdx, GrandpaRef);
		LinkToNewParentByNewReference(OldBrotherIdx, OldParentRef);
		LinkToNewParentByNewReference(OldGrandpaIdx, BrotherRef);

		return GrandpaRef;
	}

	/**
	* Relocates node, so that it can be accessed by a new reference.
	*
	* Correctly setups link from node to the new parent, 
	* and from the new parent's child to the node.
	* However, link from old parent to old child is NOT invalidated.
	*
	* Function works correctly if the NodeIdx is INDEX_NONE (meaning that no node exist).
	*/
	void LinkToNewParentByNewReference(TRBTreeImpl::NodeIndex NodeIdx, TRBTreeImpl::ChildNodeRef NewRef)
	{
		if (NodeIdx != INDEX_NONE)
		{
			NodeType* const pNode = GetNode(NodeIdx);
			pNode->ParentIdx = NewRef.ParentIdx;
		}
		if (NewRef.IsRoot())
		{
			RootIdx = NodeIdx;
		}
		else
		{
			GetNode(NewRef.ParentIdx)->SetChild(NewRef.ChildIdx, NodeIdx);
		}
	}

	/**
	* Gets node by index.
	*/
	__forceinline const NodeType* GetNode(int InIdx) const { return &Buffer[InIdx]; };

	/**
	* Gets node by index.
	*/
	__forceinline NodeType* GetNode(int InIdx) { return &Buffer[InIdx]; };

	/**
	* Returns index of the node by the given reference.
	*
	* @returns: index of the node, or INDEX_NONE, if no node by the given ref.
	*/
	TRBTreeImpl::NodeIndex GetNodeIndex(const TRBTreeImpl::ChildNodeRef InNodeRef) const
	{
		if (InNodeRef.IsRoot())
		{
			return RootIdx;
		}
		
		return GetParentNode(InNodeRef)->GetChild(InNodeRef.ChildIdx);
	}

	/**
	* Gets parent node reference.
	*/
	TRBTreeImpl::ChildNodeRef GetParentNodeRef(const TRBTreeImpl::ChildNodeRef InNodeRef) const
	{
		const TRBTreeImpl::NodeIndex ParentIdx = InNodeRef.ParentIdx;
		const NodeType* const pParent = GetParentNode(InNodeRef);
		BOOST_ASSERT_MSG(pParent, "TRBTree::GetParentNodeRef: parent must exist");
		if (pParent->ParentIdx == INDEX_NONE) 
		{
			// If parent of parent is root, we return the root 
			return TRBTreeImpl::ChildNodeRef::RootNode();
		}
		else
		{
			// if parent of parent is an ordinary node
			const TRBTreeImpl::NodeIndex ParentParentIdx = pParent->ParentIdx;
			const NodeType* const pParentParent = GetNode(ParentParentIdx);
			TRBTreeImpl::NodeChildIndex ChildOfParentParentIndex = INDEX_NONE;
			if (ParentIdx == pParentParent->GetChild(TRBTreeImpl::LEFT_CHILD_IDX))
			{
				ChildOfParentParentIndex = TRBTreeImpl::LEFT_CHILD_IDX;
			}
			else
			{
				BOOST_ASSERT(ParentIdx == pParentParent->GetChild(TRBTreeImpl::RIGHT_CHILD_IDX));
				ChildOfParentParentIndex = TRBTreeImpl::RIGHT_CHILD_IDX;
			}
			return TRBTreeImpl::ChildNodeRef{ ParentParentIdx, ChildOfParentParentIndex };
		}
	}

	/**
	* Gets parent node by reference.
	*/
	__forceinline const NodeType* GetParentNode(TRBTreeImpl::ChildNodeRef InRef) const
	{
		BOOST_ASSERT_MSG(InRef.IsValid(), "TRBTree::GetParentNode(ChildNodeRef): reference must be valid");
		if(INDEX_NONE == InRef.ParentIdx)
		{
			return nullptr;
		}
		return GetNode(InRef.ParentIdx);
	}

	/**
	* Gets parent node by reference.
	*/
	__forceinline NodeType* GetParentNode(TRBTreeImpl::ChildNodeRef InRef)
	{
		BOOST_ASSERT_MSG(InRef.IsValid(), "TRBTree::GetParentNode(ChildNodeRef): reference must be valid");
		if(INDEX_NONE == InRef.ParentIdx)
		{
			return nullptr;
		}
		return GetNode(InRef.ParentIdx);
	}

	/**
	* Gets node by reference impl.
	*/
	__forceinline const NodeType* GetNodeImpl(TRBTreeImpl::ChildNodeRef InRef) const
	{
		BOOST_ASSERT_MSG(InRef.IsValid(), "TRBTree::GetNodeImpl(ChildNodeRef): reference must be valid");
		BOOST_ASSERT_MSG(NodeExists(InRef), "TRBTree::GetNodeImpl(ChildNodeRef): the node must exist");
		if(InRef.IsRoot())
		{
			return GetNode(RootIdx);
		}
		return GetNode(GetParentNode(InRef)->GetChild(InRef.ChildIdx));
	}

	/**
	* Gets node by reference.
	*/
	__forceinline const NodeType* GetNode(TRBTreeImpl::ChildNodeRef InRef) const
	{
		return GetNodeImpl(InRef);
	}

	/**
	* Gets node by reference.
	*/
	__forceinline NodeType* GetNode(TRBTreeImpl::ChildNodeRef InRef)
	{
		return const_cast<NodeType*>(GetNodeImpl(InRef));
	}

	/**
	* Gets child node by index.
	*/
	__forceinline NodeType* GetChildNode(NodeType* pInParent, TRBTreeImpl::NodeChildIndex InChildIdx)
	{
		BOOST_ASSERT(pInParent);
		return GetNode(pInParent->GetChild(InChildIdx));
	}

	/**
	* Gets child node by index.
	*/
	__forceinline const NodeType* GetChildNode(const NodeType* pInParent, TRBTreeImpl::NodeChildIndex InChildIdx) const
	{
		BOOST_ASSERT(pInParent);
		return GetNode(pInParent->GetChild(InChildIdx));
	}

	/**
	* Returns true if node that the given ref references exists.
	*/
	bool NodeExists(TRBTreeImpl::ChildNodeRef NodeRef) const
	{
		if(NodeRef.IsRoot())
		{
			return ! Empty();
		}
		return INDEX_NONE != (GetNode(NodeRef.ParentIdx)->GetChild(NodeRef.ChildIdx));			
	}

	/**
	* Gets child node reference.
	* The node itself and the corresponding child must exist.
	*/
	__forceinline TRBTreeImpl::ChildNodeRef GetChildNodeRef(TRBTreeImpl::ChildNodeRef NodeRef, TRBTreeImpl::NodeChildIndex InChildIdx) const
	{
		BOOST_ASSERT( NodeExists(NodeRef) );
		int NextChildParentIdx = INDEX_NONE;
		if (NodeRef.IsRoot())
		{
			NextChildParentIdx = RootIdx;
		}
		else
		{
			NextChildParentIdx = GetParentNode(NodeRef)->GetChild(NodeRef.ChildIdx);
		}
		BOOST_ASSERT_MSG(INDEX_NONE != NextChildParentIdx, "TRBTree::GetChildNodeRef: the given child must exist");
		
		return TRBTreeImpl::ChildNodeRef{ NextChildParentIdx, InChildIdx };
	}

	/**
	* Returns iterator to the first element.
	*
	* NOTE: We pass this pointer to make this function act either like const or mutable depending on context,
	* however formally be const.
	*/
	template<class IteratorTypeArg, class ThisTypeArg> IteratorTypeArg GetFirstIteratorImpl(ThisTypeArg InThisPtr) const
	{
		if (Empty())
		{
			return IteratorTypeArg(InThisPtr, TRBTreeImpl::ChildNodeRef::Invalid());
		}
		else
		{
			return IteratorTypeArg(InThisPtr, GetDeepestNodeRef(TRBTreeImpl::ChildNodeRef::RootNode(), TRBTreeImpl::LEFT_CHILD_IDX));
		}
	}


	template<class TraverseFunc, class ThisType>
	void TraverseImpl(const TraverseFunc& Func, ThisType pThis) const
	{
		if (pThis->Empty())
		{
			return;
		}

		return pThis->TraverseSubtree(TRBTreeImpl::ChildNodeRef::RootNode(), Func);
	}

	template<class IteratorTypeArg, class Pred, class ThisTypeArg>
	IteratorTypeArg FindIteratorByPredicateImpl(const Pred& P, ThisTypeArg* pInThis) const
	{
		return FindIteratorByPredicate_InRange_Impl(pInThis->Iterator(), pInThis->EndIterator(), P, pInThis);
	}

	template<class IteratorTypeArg, class Pred, class ThisTypeArg>
	IteratorTypeArg FindIteratorByKeyPredicateImpl(const Pred& P, ThisTypeArg* pInThis) const
	{
		return FindIteratorByPredicate_InRange_Impl
		(
			pInThis->Iterator(), pInThis->EndIterator(), 
			[&P](const KeyValueType& KV)->bool { return P(KV.Key); },
			pInThis
		);
	}

	template<class IteratorTypeArg, class Pred, class ThisTypeArg>
	IteratorTypeArg FindIteratorByNotKeyPredicateImpl(const Pred& P, ThisTypeArg* pInThis) const
	{
		return FindIteratorByNotPredicate_InRange_Impl
		(
			pInThis->Iterator(), pInThis->EndIterator(),
			[&P](const KeyValueType& KV)->bool { return P(KV.Key); },
			pInThis
		);
	}

	template<class IteratorTypeArg, class Pred, class ThisTypeArg>
	IteratorTypeArg FindIteratorByNotKeyPredicate_InRange_Impl(IteratorTypeArg FirstIt, const IteratorTypeArg LastIt, const Pred& P, ThisTypeArg* pInThis) const
	{
		return FindIteratorByPredicate_InRange_Impl
		(
			FirstIt, LastIt,
			[&P](const KeyValueType& KV) -> bool
			{
				return ! P(KV.Key);
			},
			pInThis
		);
	}

	template<class IteratorTypeArg, class Pred, class ThisTypeArg>
	IteratorTypeArg FindIteratorByKeyPredicate_InRange_Impl(IteratorTypeArg FirstIt, const IteratorTypeArg LastIt, const Pred& P, ThisTypeArg* pInThis) const
	{
		return FindIteratorByPredicate_InRange_Impl
		(
			FirstIt, LastIt,
			[&P](const KeyValueType& KV) -> bool
			{
				return P(KV.Key);
			},
			pInThis
		);
	}

	template<class IteratorTypeArg, class Pred, class ThisTypeArg>
	IteratorTypeArg FindIteratorByNotPredicate_InRange_Impl(IteratorTypeArg FirstIt, const IteratorTypeArg LastIt, const Pred& P, ThisTypeArg* pInThis) const
	{
		return FindIteratorByPredicate_InRange_Impl
		(
			FirstIt, LastIt, 
			[&P](const KeyValueType& KV) -> bool
			{
				return ! P(KV);
			},
			pInThis
		);
	}

	template<class IteratorTypeArg, class ThisTypeArg>
	IteratorTypeArg FindIteratorByValue_Impl(const ValueType& InValue, ThisTypeArg* pInThis) const
	{
		return FindIteratorByValue_InRange_Impl(pInThis->Iterator(), pInThis->EndIterator(), InValue, pInThis);
	}

	template<class IteratorTypeArg, class ThisTypeArg>
	IteratorTypeArg FindIteratorByValue_InRange_Impl(IteratorTypeArg FirstIt, const IteratorTypeArg LastIt, const ValueType& InValue, ThisTypeArg* pInThis) const
	{
		return FindIteratorByPredicate_InRange_Impl
		(
			FirstIt, LastIt,
			[&InValue](const KeyValueType& KV)->bool
			{
				return KV.Value == InValue;
			},
			pInThis
		);
	}

	template<class IteratorTypeArg, class ThisTypeArg>
	IteratorTypeArg FindIteratorByNotValue_Impl(const ValueType& InValue, ThisTypeArg* pInThis) const
	{
		return FindIteratorByNotValue_InRange_Impl(pInThis->Iterator(), pInThis->EndIterator(), InValue, pInThis);
	}

	template<class IteratorTypeArg, class ThisTypeArg>
	IteratorTypeArg FindIteratorByNotValue_InRange_Impl(IteratorTypeArg FirstIt, const IteratorTypeArg LastIt, const ValueType& InValue, ThisTypeArg* pInThis) const
	{
		return FindIteratorByPredicate_InRange_Impl
		(
			FirstIt, LastIt,
			[&InValue](const KeyValueType& KV)->bool
			{
				return KV.Value != InValue;
			},
			pInThis
		);
	}

	template<class IteratorTypeArg, class Pred, class ThisTypeArg>
	IteratorTypeArg FindIteratorByPredicate_InRange_Impl(IteratorTypeArg FirstIt, const IteratorTypeArg LastIt, const Pred& P, ThisTypeArg* pInThis) const
	{
		for (IteratorTypeArg It = FirstIt; It != LastIt; ++It)
		{
			if (P(*It))
			{
				return It;
			}
		}
		return pInThis->EndIterator();
	}

	template<class IteratorTypeArg, class ThisTypeArg, class ComparerType> IteratorTypeArg FindIteratorAtInRangeImpl
	(
		const KeyType& InKey, ThisTypeArg* pInThis, ComparerType InComparer,
		ConstIteratorType ItFirst, ConstIteratorType ItLast
	) const
	{
		bool bFound = (ItFirst != ItLast);
		TRBTreeImpl::ChildNodeRef NodeRef = TRBTreeImpl::ChildNodeRef::Invalid();
		if (bFound)
		{
			bFound = FindNode_InRange(InKey, /*OutNodeRef*/ NodeRef, InComparer, ItFirst, ItLast);
		}
		if (!bFound)
		{
			return IteratorTypeArg::EndIterator(pInThis);
		}
		return IteratorTypeArg(pInThis, NodeRef);
	}

	template<class IteratorTypeArg, class ThisTypeArg, class ComparerType> IteratorTypeArg FindIteratorAtImpl(const KeyType& InKey, ThisTypeArg* pInThis, ComparerType InComparer) const
	{
		// @TODO: Code duplication: implement via the FindIteratorAtInRangeImpl

		bool bFound = !Empty();
		TRBTreeImpl::ChildNodeRef NodeRef = TRBTreeImpl::ChildNodeRef::Invalid();
		if (bFound)
		{
			bFound = FindNode(InKey, /*OutNodeRef*/ NodeRef, InComparer);
		}
		if (!bFound)
		{
			return IteratorTypeArg::EndIterator(pInThis);
		}
		return IteratorTypeArg(pInThis, NodeRef);
	}

	/**
	* Get the deepest left or right child node.
	*/
	TRBTreeImpl::ChildNodeRef GetDeepestNodeRef(TRBTreeImpl::ChildNodeRef InSearchRoot, TRBTreeImpl::NodeChildIndex InChildIdx) const
	{
		TRBTreeImpl::ChildNodeRef NodeRef = InSearchRoot;
		while (true)
		{
			TRBTreeImpl::ChildNodeRef NextNodeRef = GetChildNodeRef(NodeRef, InChildIdx);
			if ( ! NodeExists(NextNodeRef) )
			{
				return NodeRef;
			}
			NodeRef = NextNodeRef;
		}
	}

	/**
	* Walks by parent links skipping all right childs and the first left child.
	*
	* Returns true, if we should end up iteration.
	*/
	bool BacktrackRightSubtreeIteration(TRBTreeImpl::ChildNodeRef& InOutNodeRef) const
	{
		BOOST_ASSERT_MSG( ! InOutNodeRef.IsRoot(), "TRBTree: BacktrackRightSubtreeIteration: must be called on NON-root node only");
		BOOST_ASSERT_MSG( InOutNodeRef.ChildIdx == TRBTreeImpl::RIGHT_CHILD_IDX, "TRBTree: BacktrackRightSubtreeIteration: must start on the node that is a right child of its parent");
		while (true)
		{
			// We must climb up by the first left child.
			bool bFromLeftChild = (InOutNodeRef.ChildIdx == TRBTreeImpl::LEFT_CHILD_IDX);
			InOutNodeRef = GetParentNodeRef(InOutNodeRef);
			
			if (InOutNodeRef.IsRoot())
			{
				// We may end up iteration by climbing from right child to the root parent node.
				// That would mean that we need to end up iteration.
				return ! bFromLeftChild;
			}
			else
			{
				if (bFromLeftChild)
				{
					// We may end up iteration by climbing from left child to the root parent node.
					// That would mean that we need to end up iteration.
					return InOutNodeRef.IsRoot();
				}
				// At this point we climbed the right child, so we must proceed iteration further.
				BOOST_ASSERT_MSG( ! bFromLeftChild, "TRBTree: BacktrackRightSubtreeIteration: At this point we must be climbed from the right child");
			}
		}
		BOOST_ASSERT_MSG(false, "TRBTree: BacktrackRightSubtreeIteration: Should NOT get here");
		return false;
	}

	template<class KeyValueTypeArg>
	IteratorType AddEndImpl(KeyValueTypeArg&& InKV)
	{
		return AddAfterImpl(EndIterator(), std::forward<KeyValueTypeArg>(InKV));	
	}

	IteratorType AddAfterImpl(ConstIteratorType ItPos, KeyValueType&& InKV)
	{
		// @TODO: OPTIMIZE
		BOOST_REQUIRE_MSG(ItPos.IsEnd() || InKV.Key < ItPos.GetKey(), "TRBTree: AddAfterImpl: Unable to insert key after this position: next key must be greater");
		TRBTreeImpl::ChildNodeRef NodeRef = TRBTreeImpl::ChildNodeRef::Invalid();
		AddImpl(std::move(InKV), /*Out*/NodeRef);
		return IteratorType{this, NodeRef};
	}
	
	/**
	* Buffer for storing nodes.
	* Each slot of the buffer may store a node or may not.
	*/
	TVector<NodeType> Buffer;

	/**
	* Index of the root node (or INDEX_NONE if empty).
	*/
	int RootIdx;

	/**
	* Count of nodes.
	*/
	int Count;
};
template<class KVTypeArg, class ComparerArg, class OtherComparerArg>
bool operator==(const TRBTree<KVTypeArg, ComparerArg>& A, const TRBTree<KVTypeArg, OtherComparerArg>& B)
{
	if (A.Num() != B.Num())
	{
		return false;
	}

	// @TODO: Optimize: for the case when Comparer arg is the same
	for (const auto& KV : A)
	{
		const auto pOtherKV = B.Find(KV.Key);
		if (nullptr == pOtherKV)
		{
			return false;
		}
		if (pOtherKV->Value != KV.Value)
		{
			return false;
		}
	}

	return true;
}

template<class KVTypeArg, class ComparerArg, class OtherComparerArg>
bool operator!=(const TRBTree<KVTypeArg, ComparerArg>& A, const TRBTree<KVTypeArg, OtherComparerArg>& B)
{
	return ! operator==(A, B);
}

template
<
	class Strm, class KVTypeArg, class ComparerArg, 
	typename = typename std::enable_if<typename IsOutputStream<Strm>::Value>::type
>
Strm& operator<<(Strm& S, const TRBTree<KVTypeArg, ComparerArg>& InCont)
{
	for(const auto& KV : InCont)
	{
		S << KV << std::endl;
	}
	return S;
}

/**
* Intersect key sets and returns a new set.
* The result set will contain only that values that are contained both in the current set and the other set.
* WARNING!!! The result set is not cleared and space is not reserved in the result set.
*
* @UNIT-TESTED
*/
template<class KVTypeArg, class ComparerArg>
void AppendIntersectTo(TRBTree<KVTypeArg, ComparerArg>& OutResult, const TRBTree<KVTypeArg, ComparerArg>& InSet, const TRBTree<KVTypeArg, ComparerArg>& InOtherSet)
{
	typename TRBTree<KVTypeArg, ComparerArg>::ConstIteratorType OtherIt = InOtherSet.ConstIterator();
	for (typename TRBTree<KVTypeArg, ComparerArg>::ConstIteratorType It = InSet.Iterator(); It; ++It)
	{
		OtherIt = InOtherSet.FindIteratorFor_InRange(It.GetKey(), OtherIt, InOtherSet.ConstEndIterator());
		if (OtherIt.IsEnd())
		{
			return;
		}
			
		OutResult.Add(It.GetKeyValue());
			
		++OtherIt;
		++It;
	}
}

/**
* @UNIT-TESTED
*/
template<class KVTypeArg, class ComparerArg>
void AppendDifferenceTo(TRBTree<KVTypeArg, ComparerArg>& OutResult, const TRBTree<KVTypeArg, ComparerArg>& InSet, const TRBTree<KVTypeArg, ComparerArg>& InOtherSet)
{ 
	using ItType = typename TRBTree<KVTypeArg, ComparerArg>::ConstIteratorType;
	ItType It = InSet.ConstIterator();
	ItType OtherIt = InOtherSet.ConstIterator();
	while(OtherIt)
	{
		if(It.IsEnd())
		{
			break;
		}
		bool bInclude = true;
		while(true)
		{
			if(OtherIt.IsEnd())
			{
				break;
			}
			auto CmpResult = ComparerArg().Compare(It.GetKey(), OtherIt.GetKey());
			if(CompareEqual(CmpResult))
			{
				bInclude = false;
				++OtherIt;
				break;
			}

			if(CompareGreater(CmpResult))
			{
				break;
			}
			
			++OtherIt;
		}	
		if(OtherIt.IsEnd())
		{
			break;
		}
		if(bInclude)
		{
			// WARNING!!! We may NOT use AddEnd here, because the result may already contain elements
			OutResult.Add(*It);
		}
		++It;
	}
	// WARNING!!! We may NOT use AddRangeEnd here, because the result may already contain elements
	OutResult.AppendRange(It, InSet.ConstEndIterator());
}

/**
* Union key sets and return a new set.
* The result will contain all values contained either in this or other container.
* WARNING!!! The result set is not cleared and space is not reserved in the result set.
*
* @UNIT-TESTED
*/
template<class KVTypeArg, class ComparerArg>
void AppendUnionTo(TRBTree<KVTypeArg, ComparerArg>& OutResult, const TRBTree<KVTypeArg, ComparerArg>& InSet, const TRBTree<KVTypeArg, ComparerArg>& InOtherSet)
{
	AppendUnionTo(OutResult, InSet, InOtherSet.ConstIterator(), InOtherSet.ConstEndIterator());
}

/**
* Unions current container and the given range of key/value pairs, returns a new set as an output argument.
* The result will contain all values contained either in this or other container.
* WARNING!!! The result set is not cleared and space is not reserved in the result set.
*/
template<class KVTypeArg, class ComparerArg, class OtherIteratorArg>
void AppendUnionTo(TRBTree<KVTypeArg, ComparerArg>& OutResult, const TRBTree<KVTypeArg, ComparerArg>& InSet, OtherIteratorArg ItFirst, OtherIteratorArg ItLast)
{
	AppendUnionTo(OutResult, InSet.ConstIterator(), InSet.ConstEndIterator(), ItFirst, ItLast);
}

/**
* Unions the given subrange and the given range of key/value pairs, returns a new set as an output argument.
* The result will contain all values contained either in this or other container.
* WARNING!!! The result set is not cleared and space is not reserved in the result set.
*/
template<class KVTypeArg, class ComparerArg, class IteratorArg, class OtherIteratorArg>
void AppendUnionTo(TRBTree<KVTypeArg, ComparerArg>& OutResult, IteratorArg ItFirst, IteratorArg ItLast, OtherIteratorArg ItOtherFirst, OtherIteratorArg ItOtherLast)
{
	using ElementType = typename TRBTree<KVTypeArg, ComparerArg>::ElementType;

	static_assert(IsIterator<OtherIteratorArg>::Value, "TRBTree: AppendUnionTo: iterator range must be represented by iterators");
	static_assert(TypeEqual<typename IteratorArg::ElementType, ElementType>::Value, "TRBTree: AppendUnionTo: iterator element type must correspond the result RBTree");
	
	static_assert(IsIterator<OtherIteratorArg>::Value, "TRBTree: AppendUnionTo: other iterator range must be represented by iterators");
	static_assert(TypeEqual<typename OtherIteratorArg::ElementType, ElementType>::Value, "TRBTree: AppendUnionTo: other iterator element type must correspond the result RBTree");

	for (IteratorArg It = ItFirst; It != ItLast; ++It)
	{
		OutResult.Add(*It);
	}

	for (IteratorArg ItOther = ItOtherFirst; ItOther != ItOtherLast; ++ItOther)
	{
		OutResult.Add(*ItOther);
	}
}


/**
* Returns intersection of two RBTrees
*/
template<class KVTypeArg, class ComparerArg>
TRBTree<KVTypeArg, ComparerArg> GetIntersect(const TRBTree<KVTypeArg, ComparerArg>& InSet, const TRBTree<KVTypeArg, ComparerArg>& InOtherSet)
{
	TRBTree<KVTypeArg, ComaprerArg> Result;
	AppendIntersectTo(Result, InSet, InOtherSet);
	return Result;
}

/**
* Returns union of two RBTrees.
*/
template<class KVTypeArg, class ComparerArg>
TRBTree<KVTypeArg, ComparerArg> GetUnion(const TRBTree<KVTypeArg, ComparerArg>& InSet, const TRBTree<KVTypeArg, ComparerArg>& InOtherSet)
{
	TRBTree<KVTypeArg, ComaprerArg> Result;
	AppendUnionTo(Result, InSet, InOtherSet);
	return Result;
}

/**
* Returns difference of two RBTrees.
*/
template<class KVTypeArg, class ComparerArg>
TRBTree<KVTypeArg, ComparerArg> GetDifference(const TRBTree<KVTypeArg, ComparerArg>& InSet, const TRBTree<KVTypeArg, ComparerArg>& InOtherSet)
{
	TRBTree<KVTypeArg, ComaprerArg> Result;
	AppendDifferenceTo(Result, InSet, InOtherSet);
	return Result;
}
