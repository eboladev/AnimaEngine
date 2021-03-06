//
//  AnimaFace.cpp
//  Anima
//
//  Created by Marco Zille on 26/11/14.
//
//

#include "AnimaFace.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaFace::AnimaFace(AnimaEngine* engine)
{
	ANIMA_ASSERT(engine != nullptr);
	_engine = engine;
	
	_indexes = nullptr;
	_indexesNumber = 0;
}

AnimaFace::AnimaFace(const AnimaFace& src)
{
	_engine = src._engine;
	_indexes = nullptr;
	_indexesNumber = 0;
	
	SetIndexes(src._indexes, src._indexesNumber);
}

AnimaFace::AnimaFace(AnimaFace&& src)
: _indexes(src._indexes)
, _indexesNumber(src._indexesNumber)
, _engine(src._engine)
{
	src._indexes = nullptr;
	src._indexesNumber = 0;
}

AnimaFace::~AnimaFace()
{
	ClearIndexes();
}

AnimaFace& AnimaFace::operator=(const AnimaFace& src)
{
	if (this != &src)
	{
		_engine = src._engine;
		SetIndexes(src._indexes, src._indexesNumber);
	}
	
	return *this;
}

AnimaFace& AnimaFace::operator=(AnimaFace&& src)
{
	if (this != &src)
	{
		_engine = src._engine;
		
		_indexes = src._indexes;
		_indexesNumber = src._indexesNumber;
		
		src._indexes = nullptr;
		src._indexesNumber = 0;
	}
	
	return *this;
}

void AnimaFace::SetIndexes(AUint* indexes, ASizeT n)
{
	ANIMA_ASSERT(_engine != nullptr);
	ClearIndexes();
	
	if(indexes != nullptr && n > 0)
	{
		_indexesNumber = n;
		_indexes = AnimaAllocatorNamespace::AllocateArray<AUint>(*(_engine->GetModelDataAllocator()), _indexesNumber);
		
		for (int i = 0; i < _indexesNumber; i++)
			_indexes[i] = indexes[i];
	}
}

void AnimaFace::AddIndex(const AUint& index)
{
	ANIMA_ASSERT(_engine != nullptr);
	AUint* tmpOldIndexes = AnimaAllocatorNamespace::AllocateArray<AUint>(*(_engine->GetModelDataAllocator()), _indexesNumber);
	
	for (int i = 0; i < _indexesNumber; i++)
		tmpOldIndexes[i] = _indexes[i];
	
	AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetModelDataAllocator()), _indexes);
	
	_indexesNumber++;
	_indexes = AnimaAllocatorNamespace::AllocateArray<AUint>(*(_engine->GetModelDataAllocator()), _indexesNumber);
	
	for (int i = 0; i < _indexesNumber - 1; i++)
		_indexes[i] = tmpOldIndexes[i];
	
	_indexes[_indexesNumber - 1] = index;
	
	AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetModelDataAllocator()), tmpOldIndexes);
}

void AnimaFace::ClearIndexes()
{
	if(_indexes != nullptr && _indexesNumber > 0)
	{
		AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetModelDataAllocator()), _indexes);
		_indexes = nullptr;
		_indexesNumber = 0;
	}
}

ASizeT AnimaFace::GetIndexesCount()
{
	return _indexesNumber;
}

AUint* AnimaFace::GetIndexes()
{
	return _indexes;
}

const AUint* AnimaFace::GetConstIndexes()
{
	return const_cast<AUint*>(_indexes);
}

bool AnimaFace::GetIndexes(AUint* outIndexes, ASizeT& outIndexesSize)
{
	if (outIndexesSize < _indexesNumber)
		return false;
	outIndexes = _indexes;
	return true;
}

bool AnimaFace::GetConstIndexes(AUint* outIndexes, ASizeT& outIndexesSize)
{
	if (outIndexesSize < _indexesNumber)
		return false;

	memcpy(outIndexes, _indexes, sizeof(AUint) * _indexesNumber);
	return true;
}

END_ANIMA_ENGINE_NAMESPACE
