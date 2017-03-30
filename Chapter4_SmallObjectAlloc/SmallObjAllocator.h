#ifndef SMALL_OBJ_ALLOCATOR_H
#define SMALL_OBJ_ALLOCATOR_H

#include <vector>

//Bottom of the heirarchy, handles the raw data
struct Chunk
{
	void Init(size_t blockSize, unsigned char blocks);
	void* Allocate(size_t blockSize);
	void Deallocate(void *p, size_t blockSize);
	void Release();

	bool HasBlock(void *p, size_t chunkLength) const;
	bool HasAvailable(unsigned char numBlocks) const;
	bool IsFilled() const;

	unsigned char* m_pData;
	unsigned char m_firstAvailableBlock;
	unsigned char m_blocksAvailable;
};

//Handles all Chunks of the size given to blockSize
class FixedAllocator
{
public:
	~FixedAllocator();
	void Initalize(size_t blockSize, size_t chunkSize);
	void* Allocate();
	bool Deallocate(void* p, Chunk* hint);
	const Chunk * HasBlock(void *p) const;
	Chunk* HasBlock(void *p)
	{
		return const_cast< Chunk * >(
			const_cast< const FixedAllocator * >(this)->HasBlock(p));
	}
	bool TrimEmptyChunk();
	bool TrimChunkList();

	size_t GetBlockSize() const;

private:
	size_t CountEmptyChunks() const;
	Chunk* VicinityFind(void* p) const;
	void DoDeallocate(void* p);

	size_t m_blockSize = 0;
	unsigned char m_numBlocks = 0 ;
	typedef std::vector<Chunk> Chunks;
	Chunks m_chunks;
	Chunk* m_allocChunk = 0;
	Chunk* m_deallocChunk = 0;
	Chunk* m_emptyChunk = 0;


	unsigned char m_minObjectsPerChunk = 8;
	unsigned char m_maxObjectsPerChunk = UCHAR_MAX;
};

class SmallObjAllocator
{

public:
	SmallObjAllocator(size_t pageSize, size_t maxObjectSize, size_t objectAlignSize);
	~SmallObjAllocator();

	void* Allocate(size_t numBytes);
	void Deallocate(void* p, size_t size);
	void Deallocate(void* p);
	size_t GetMaxObjectSize() const;
	size_t GetAlignment() const;

private:
	bool TrimExcessMemory();

	std::vector<FixedAllocator> m_pool;
	const size_t m_maxSmallObjectSize;
	const size_t m_objectAlignSize;
};

static const int DEFAULT_CHUNK_SIZE = 4096;
static const int MAX_SMALL_OBJECT_SIZE = 64;
static const int DEFAULT_OBJECT_ALIGNMENT = 4;
template
<
	size_t chunkSize = DEFAULT_CHUNK_SIZE,
	size_t maxSmallObjectSize = MAX_SMALL_OBJECT_SIZE,
	size_t objectAlignSize = DEFAULT_OBJECT_ALIGNMENT
>
class SmallObjectBase
{

public:
	static void* operator new(size_t size)
	{
		if (size > maxSmallObjectSize)
		{
			printf("SmallObjectBase: size is greater than maximum, using new \n");
			return ::operator new(size);
		}
		else
		{
			printf("SmallObjectBase: using custom allocator \n");
			return m_objAllocator.Allocate(size);
		}

	}; 

	static void operator delete (void *p, size_t size)
	{
		m_objAllocator.Deallocate(p);
	};

	static void* operator new[](size_t size)
	{
		m_objAllocator.Allocate(size);
	};

	static void operator delete [](void *p, size_t size)
	{
		m_objAllocator.Deallocate(p, size);
	}

protected:
	inline SmallObjectBase() {};
	inline SmallObjectBase(const SmallObjectBase &) {}
	inline SmallObjectBase & operator = (const SmallObjectBase &)
	{
		return *this;
	}
	inline ~SmallObjectBase() {}
	static SmallObjAllocator m_objAllocator;
};

#endif
