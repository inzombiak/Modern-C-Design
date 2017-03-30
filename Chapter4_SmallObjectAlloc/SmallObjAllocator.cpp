#include "SmallObjAllocator.h"
#include <assert.h>;

void Chunk::Init(size_t blockSize, unsigned char blocks)
{
	m_pData = new unsigned char[blockSize * blocks];
	m_firstAvailableBlock = 0;
	m_blocksAvailable = blocks;
	unsigned char i = 0;
	unsigned char* p = m_pData;
	for (; i != blocks; p += blockSize)
		*p = ++i;
}

void* Chunk::Allocate(size_t blockSize)
{
	if (m_blocksAvailable == 0)
		return 0;
	unsigned char* pResult = m_pData + (m_firstAvailableBlock * blockSize);
	m_firstAvailableBlock = *pResult;
	--m_blocksAvailable;
	return pResult;
}

void Chunk::Deallocate(void *p, size_t blockSize)
{
	assert(p >= m_pData);
	unsigned char* toRelease = static_cast<unsigned char*>(p);
	//Make sure toRelease is aligned with blockSize
	assert((toRelease - m_pData) % blockSize == 0);
	*toRelease = m_firstAvailableBlock;
	m_firstAvailableBlock = static_cast<unsigned char>((toRelease - m_pData) / blockSize);
	assert(m_firstAvailableBlock == (toRelease - m_pData) / blockSize);
	++m_blocksAvailable;
}

void Chunk::Release()
{
	assert(NULL != m_pData);
	std::free(static_cast<void*>(m_pData));
}

bool Chunk::HasBlock(void *p, size_t chunkLength) const
{
	unsigned char* pChar = static_cast<unsigned char*>(p);
	return (m_pData <= pChar) && (pChar < m_pData + chunkLength);
}

bool Chunk::HasAvailable(unsigned char numBlocks) const
{
	return m_blocksAvailable == numBlocks;
}

bool Chunk::IsFilled() const
{
	return m_blocksAvailable == 0;
}

void FixedAllocator::Initalize(size_t blockSize, size_t chunkSize)
{
	assert(blockSize > 0);
	assert(chunkSize > blockSize);
	m_blockSize = blockSize;

	size_t numBlocks = chunkSize / blockSize;
	if (numBlocks > m_maxObjectsPerChunk)
		numBlocks = m_maxObjectsPerChunk;
	else if (numBlocks < m_minObjectsPerChunk)
		numBlocks = m_minObjectsPerChunk;
	
	m_numBlocks = static_cast<unsigned char>(numBlocks);
	assert(m_numBlocks == numBlocks);
}

FixedAllocator::~FixedAllocator()
{
	for (Chunks::iterator it = m_chunks.begin(); it != m_chunks.end(); ++it)
		it->Release();
}

void* FixedAllocator::Allocate()
{
	if (m_allocChunk == 0 || m_allocChunk->m_blocksAvailable == 0)
	{
		Chunks::iterator it = m_chunks.begin();
		for (;; ++it)
		{
			if (it == m_chunks.end())
			{
				m_chunks.reserve(m_chunks.size() + 1);
				Chunk newChunk;
				newChunk.Init(m_blockSize, m_numBlocks);
				m_chunks.push_back(newChunk);
				m_allocChunk = &m_chunks.back();
				m_deallocChunk = &m_chunks.back();
				break;
			}
			if (it->m_blocksAvailable > 0)
			{
				m_allocChunk = &*it;
				break;
			}
		}
	}

	assert(m_allocChunk != 0);
	assert(m_allocChunk->m_blocksAvailable > 0);
	return m_allocChunk->Allocate(m_blockSize);
}

bool FixedAllocator::Deallocate(void* p, Chunk* hint)
{
	assert(!m_chunks.empty());
	assert(&m_chunks.front() <= m_deallocChunk);
	assert(&m_chunks.back() >= m_deallocChunk);
	assert(&m_chunks.front() <= m_allocChunk);
	assert(&m_chunks.back() >= m_allocChunk);
	assert(CountEmptyChunks() < 2);

	Chunk *foundChunk = (hint == NULL) ? VicinityFind(p) : hint;
	if (foundChunk == NULL)
		return false;

	assert(foundChunk->HasBlock(p, m_numBlocks * m_blockSize));
	//if (foundChunk->IsB)
	m_deallocChunk = foundChunk;
	DoDeallocate(p);
	assert(CountEmptyChunks() < 2);

	return true;
}

const Chunk * FixedAllocator::HasBlock(void * p) const
{
	const std::size_t chunkLength = m_numBlocks* m_blockSize;
	for (Chunks::const_iterator it = m_chunks.begin(); it != m_chunks.end(); ++it)
	{
		const Chunk & chunk = *it;
		if (chunk.HasBlock(p, chunkLength))
			return &chunk;
	}
	return NULL;
}

bool FixedAllocator::TrimEmptyChunk()
{
	assert((m_emptyChunk == NULL) || (m_emptyChunk->HasAvailable(m_numBlocks)));
	if (m_emptyChunk == NULL) return false;

	// If emptyChunk_ points to valid Chunk, then chunk list is not empty.
	assert(!m_chunks.empty());
	// And there should be exactly 1 empty Chunk.
	assert(1 == CountEmptyChunks());

	Chunk * lastChunk = &m_chunks.back();
	if (lastChunk != m_emptyChunk)
		std::swap(*m_emptyChunk, *lastChunk);
	assert(lastChunk->HasAvailable(m_numBlocks));
	lastChunk->Release();
	m_chunks.pop_back();

	if (m_chunks.empty())
	{
		m_allocChunk = NULL;
		m_deallocChunk = NULL;
	}
	else
	{
		if (m_deallocChunk == m_emptyChunk)
		{
			m_deallocChunk = &m_chunks.front();
			assert(m_deallocChunk->m_blocksAvailable < m_numBlocks);
		}
		if (m_allocChunk == m_emptyChunk)
		{
			m_allocChunk = &m_chunks.back();
			assert(m_allocChunk->m_blocksAvailable < m_numBlocks);
		}
	}

	m_emptyChunk = NULL;
	assert(0 == CountEmptyChunks());

	return true;
}

bool FixedAllocator::TrimChunkList(void)
{
	if (m_chunks.empty())
	{
		assert(m_allocChunk == NULL);
		assert(m_deallocChunk == NULL);
	}

	if (m_chunks.size() == m_chunks.capacity())
		return false;

	//Trim excess capacity
	Chunks(m_chunks).swap(m_chunks);

	return true;
}

size_t FixedAllocator::GetBlockSize() const
{
	return m_blockSize;
}

size_t FixedAllocator::CountEmptyChunks() const
{
	return (m_emptyChunk == NULL) ? 0 : 1;

}

Chunk* FixedAllocator::VicinityFind(void* p) const
{
	if (m_chunks.empty())
		return NULL;

	assert(m_deallocChunk);
	const size_t chunkLength = m_numBlocks * m_blockSize;
	
	Chunk* lo = m_deallocChunk;
	Chunk* hi = m_deallocChunk + 1;
	const Chunk* loBound = &m_chunks.front();
	const Chunk* hiBound = &m_chunks.back() + 1;

	if (hi == hiBound)
		hi = NULL;

	while (true)
	{
		if (lo)
		{
			if (lo->HasBlock(p, chunkLength))
				return lo;

			if (lo == loBound)
			{
				lo = NULL;
				if (hi == NULL)
					break;
			}
			else
				--lo;
		}

		if (hi)
		{
			if (hi->HasBlock(p, chunkLength))
				return hi;

			if (++hi == hiBound)
			{
				hi = NULL;
				if (lo == NULL)
					break;
			}
		}
	}

	return NULL;
}

void FixedAllocator::DoDeallocate(void* p)
{
	assert(m_deallocChunk->HasBlock(p, m_numBlocks * m_blockSize));
	assert(m_deallocChunk != m_emptyChunk);
	assert(!m_deallocChunk->HasAvailable(m_numBlocks));
	assert((m_emptyChunk == NULL) || (m_emptyChunk->HasAvailable(m_numBlocks)));
	m_deallocChunk->Deallocate(p, m_blockSize);

	if (m_deallocChunk->HasAvailable(m_numBlocks))
	{
		//assert(m_emptyChunk != m_deallocChunk);

		if (m_emptyChunk != NULL)
		{
			Chunk* lastChunk = &m_chunks.back();

			if (lastChunk == m_deallocChunk)
				m_deallocChunk = m_emptyChunk;
			else if (lastChunk != m_emptyChunk)
				std::swap(*m_emptyChunk, *lastChunk);

			assert(lastChunk->HasAvailable(m_numBlocks));
			lastChunk->Release();
			m_chunks.pop_back();

			if ((m_allocChunk == lastChunk) || m_allocChunk->IsFilled())
				m_allocChunk = m_deallocChunk;
		}

		m_emptyChunk = m_deallocChunk;

	}

	assert((m_emptyChunk == NULL) || m_emptyChunk->HasAvailable(m_numBlocks));
}


size_t GetOffset(size_t numBytes, size_t alignment)
{
	const size_t alignExtra = alignment - 1;
	return (numBytes + alignExtra) / alignment;
}

SmallObjAllocator::SmallObjAllocator(size_t pageSize, size_t maxObjectSize, size_t objectAlignSize) : m_maxSmallObjectSize(maxObjectSize), m_objectAlignSize(objectAlignSize)
{
	assert(objectAlignSize != 0);
	size_t allocCount = GetOffset(maxObjectSize, objectAlignSize); 
	
	m_pool.resize(allocCount); 
	for (unsigned int i = 0; i < allocCount; ++i)
	{
		m_pool[i].Initalize((i + 1) * objectAlignSize, pageSize);
	}
}

SmallObjAllocator::~SmallObjAllocator()
{
	m_pool.clear();
}

void* SmallObjAllocator::Allocate(std::size_t numBytes)
{
	//if (numBytes > GetMaxObjectSize())
	//return DefaultAllocator(numBytes, doThrow);

	assert(m_pool.size() != 0);
	if (numBytes == 0)
		numBytes = 1;

	const std::size_t index = GetOffset(numBytes, m_objectAlignSize) - 1;
	const std::size_t allocCount = GetOffset(m_maxSmallObjectSize, m_objectAlignSize);

	assert(index < allocCount);

	FixedAllocator & allocator = m_pool[index];
	assert(allocator.GetBlockSize() >= numBytes);
	assert(allocator.GetBlockSize() < numBytes + GetAlignment());
	void * place = allocator.Allocate();

	if (( place == NULL) && TrimExcessMemory())
		place = allocator.Allocate();

	return place;
}

void SmallObjAllocator::Deallocate(void* p, size_t numBytes)
{
	if (p == NULL)
		return;

	if (numBytes > m_maxSmallObjectSize)
	{
		//DEFAUT
		return;
	}

	assert(m_pool.size() != 0);
	if (numBytes == 0)
		numBytes = 1;

	const std::size_t index = GetOffset(numBytes, GetAlignment()) - 1;
	const std::size_t allocCount = m_pool.size();
	
	assert(index < allocCount);
	
	FixedAllocator & allocator = m_pool[index];
	assert(allocator.GetBlockSize() >= numBytes);
	assert(allocator.GetBlockSize() < numBytes + GetAlignment());
	
	const bool found = allocator.Deallocate(p, NULL);
	assert(found);

}

void SmallObjAllocator::Deallocate(void* p)
{
	if (p == NULL)
		return;

	assert(m_pool.size() != 0);
	FixedAllocator* pAllocator = NULL;
	const size_t allocCount = m_pool.size();
	Chunk* chunk = NULL;
	
	for (size_t i = 0; i < allocCount; ++i)
	{
		chunk = m_pool[i].HasBlock(p);
		if (chunk != NULL)
		{
			pAllocator = &m_pool[i];
			break;
		}
	}

	if (pAllocator == NULL)
	{
		//DEFAULT
		return;
	}

	assert(chunk != NULL);
	const bool found = pAllocator->Deallocate(p, chunk);
	assert(found);

}

size_t SmallObjAllocator::GetMaxObjectSize() const
{
	return m_maxSmallObjectSize;
}

size_t SmallObjAllocator::GetAlignment() const
{
	return m_objectAlignSize;
}

bool SmallObjAllocator::TrimExcessMemory()
{
	bool found = false;
	const size_t allocCount = m_pool.size();
	unsigned int i = 0;

	for (; i < allocCount; ++i)
	{
		if (m_pool[i].TrimEmptyChunk())
			found = true;
	}

	for (i = 0; i < allocCount; ++i)
	{
		if (m_pool[i].TrimChunkList())
			found = true;
	}

	return found;
}

template<size_t cs, size_t os, size_t as>
SmallObjAllocator SmallObjectBase<cs, os, as>::m_objAllocator(cs, os, as);

template<>
SmallObjAllocator SmallObjectBase<>::m_objAllocator = SmallObjAllocator(DEFAULT_CHUNK_SIZE, MAX_SMALL_OBJECT_SIZE, DEFAULT_OBJECT_ALIGNMENT);