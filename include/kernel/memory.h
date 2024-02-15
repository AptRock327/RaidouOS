#pragma once

void memset(void* start, unsigned char value, unsigned long long num);

void memcpy(void* destination, void* source, unsigned long long num);

void InitHeap(unsigned long long heapAddress, unsigned long long heapLength);

void* calloc(unsigned long long size);

void* realloc(void* address, unsigned long long newSize);

void* malloc(unsigned long long size);

void* aligned_alloc(unsigned long long alignment, unsigned long long size);

void free(void* address);

struct MemorySegmentHeader
{
	unsigned long long MemoryLength;
	MemorySegmentHeader* NextSegment;
	MemorySegmentHeader* PreviousSegment;
	MemorySegmentHeader* NextFreeSegment;
	MemorySegmentHeader* PreviousFreeSegment;
	bool Free;
};

struct AlignedMemorySegmentHeader
{
	unsigned long long MemorySegmentHeaderAddress : 63;
	bool IsAligned : 1;
};

void InitHeap(unsigned long long heapAddress, unsigned long long heapLength);

void* calloc(unsigned long long size);

void* calloc(unsigned long long num, unsigned long long size);

void* malloc(unsigned long long size);

void* realloc(void* address, unsigned long long newSize);

void CombineFreeSegments(MemorySegmentHeader* a, MemorySegmentHeader* b);

void free(void* address);

void* aligned_alloc(unsigned long long alignment, unsigned long long size);