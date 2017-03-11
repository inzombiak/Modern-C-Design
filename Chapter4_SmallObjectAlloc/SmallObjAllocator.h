#ifndef SMALL_OBJ_ALLOCATOR_H
#define SMALL_OBJ_ALLOCATOR_H

#include <vector>

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

class FixedAllocator
{
public:
	~FixedAllocator();
	void Initalize(size_t blockSize, size_t pageSize);
	void* Allocate();
	bool Deallocate(void* p, Chunk* hint);
	const Chunk * HasBlock(void * p) const;
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

#endif
