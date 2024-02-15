#include "memory.h"

MemorySegmentHeader* FirstFreeMemorySegment;

void memset(void* start, unsigned char value, unsigned long long num)
{
	for (unsigned char* ptr = (unsigned char*)start; ptr < (unsigned char*)((unsigned long long)start + num); ptr++)
	{
		*ptr = value;
	}
}

void memcpy(void* destination, void* source, unsigned long long num)
{
	if (num <= 8) 
	{
		unsigned char* valPtr = (unsigned char*)source;
		for (unsigned char* ptr = (unsigned char*)destination; ptr < (unsigned char*)((unsigned long long)destination + num); ptr++)
		{
			*ptr = *valPtr;
			valPtr++;
		}

		return;
	}

	unsigned long long proceedingBytes = num % 8;
	unsigned long long newnum = num - proceedingBytes;
	unsigned long long* srcptr = (unsigned long long*)source;

	for (unsigned long long* destptr = (unsigned long long*)destination; destptr < (unsigned long long*)((unsigned long long)destination + newnum); destptr++) {
		*destptr = *srcptr;
		srcptr++;
	}

	unsigned char* srcptr8 = (unsigned char*)((unsigned long long)source + newnum);
	for (unsigned char* destptr8 = (unsigned char*)((unsigned long long)destination + newnum); destptr8 < (unsigned char*)((unsigned long long)destination + num); destptr8++)
	{
		*destptr8 = *srcptr8;
		srcptr8++;
	}

}

void InitHeap(unsigned long long heapAddress, unsigned long long heapLength)
{
	FirstFreeMemorySegment = (MemorySegmentHeader*)heapAddress;
	FirstFreeMemorySegment->MemoryLength = heapLength - sizeof(MemorySegmentHeader);
	FirstFreeMemorySegment->NextSegment = 0;
	FirstFreeMemorySegment->PreviousSegment = 0;
	FirstFreeMemorySegment->NextFreeSegment = 0;
	FirstFreeMemorySegment->PreviousFreeSegment = 0;
	FirstFreeMemorySegment->Free = true;
}

void* calloc(unsigned long long size)
{
	void* mallocVal = malloc(size);
	memset(mallocVal, 0, size);
	return mallocVal;
}

void* calloc(unsigned long long num, unsigned long long size)
{
	return calloc(num * size);
}

void* malloc(unsigned long long size) 
{
	unsigned long long remainder = size % 8;
	size -= remainder;
	if (remainder != 0) size += 8;

	MemorySegmentHeader* currentMemorySegment = FirstFreeMemorySegment;

	while (true) {
		if (currentMemorySegment->MemoryLength >= size) {

			if (currentMemorySegment->MemoryLength > size + sizeof(MemorySegmentHeader))
			{
				MemorySegmentHeader* newSegmentHeader = (MemorySegmentHeader*)((unsigned long long)currentMemorySegment + sizeof(MemorySegmentHeader) + size);
				
				newSegmentHeader->Free = true;
				newSegmentHeader->MemoryLength = ((unsigned long long)currentMemorySegment->MemoryLength) - (sizeof(MemorySegmentHeader) + size);
				newSegmentHeader->NextFreeSegment = currentMemorySegment->NextFreeSegment;
				newSegmentHeader->NextSegment = currentMemorySegment->NextSegment;
				newSegmentHeader->PreviousSegment = currentMemorySegment;
				newSegmentHeader->PreviousFreeSegment = currentMemorySegment->PreviousFreeSegment;

				currentMemorySegment->NextFreeSegment = newSegmentHeader;
				currentMemorySegment->NextSegment = newSegmentHeader;
				currentMemorySegment->MemoryLength = size; 
			}
			if (currentMemorySegment == FirstFreeMemorySegment)
			{
				FirstFreeMemorySegment = currentMemorySegment->NextFreeSegment;
			}
			currentMemorySegment->Free = false;
			
			if (currentMemorySegment->PreviousFreeSegment != 0) currentMemorySegment->PreviousFreeSegment->NextFreeSegment = currentMemorySegment->NextFreeSegment;
			if (currentMemorySegment->NextFreeSegment != 0) currentMemorySegment->NextFreeSegment->PreviousFreeSegment = currentMemorySegment->PreviousFreeSegment;
			if (currentMemorySegment->PreviousSegment != 0) currentMemorySegment->PreviousSegment->NextFreeSegment = currentMemorySegment->NextFreeSegment;
			if (currentMemorySegment->NextSegment != 0) currentMemorySegment->NextSegment->PreviousFreeSegment = currentMemorySegment->PreviousFreeSegment;

			return currentMemorySegment + 1;
		}
		if (currentMemorySegment->NextFreeSegment == 0)
		{
			return 0; 
		}
		currentMemorySegment = currentMemorySegment->NextFreeSegment;
	}
	return 0; 
}

void* realloc(void* address, unsigned long long newSize)
{
	MemorySegmentHeader* oldSegmentHeader;

	AlignedMemorySegmentHeader* AMSH = (AlignedMemorySegmentHeader*)address - 1;
	if (AMSH->IsAligned)
	{
		oldSegmentHeader = (MemorySegmentHeader*)(unsigned long long)AMSH->MemorySegmentHeaderAddress;
	}
	else
	{
		oldSegmentHeader = ((MemorySegmentHeader*)address) - 1;
	}

	unsigned long long smallerSize = newSize;
	if (oldSegmentHeader->MemoryLength < newSize) smallerSize = oldSegmentHeader->MemoryLength;
	void* newMem = malloc(newSize);
	memcpy(newMem, address, smallerSize);
	free(address);
	return(newMem);
}

void CombineFreeSegments(MemorySegmentHeader* a, MemorySegmentHeader* b)
{
	if (a == 0) return;
	if (b == 0) return;
	if (a < b) {
		a->MemoryLength += b->MemoryLength + sizeof(MemorySegmentHeader);
		a->NextSegment = b->NextSegment;
		a->NextFreeSegment = b->NextFreeSegment;
		b->NextSegment->PreviousSegment = a;
		b->NextSegment->PreviousFreeSegment = a;
		b->NextFreeSegment->PreviousFreeSegment = a;
	}
	else {
		b->MemoryLength += a->MemoryLength + sizeof(MemorySegmentHeader);
		b->NextSegment = a->NextSegment;
		b->NextFreeSegment = a->NextFreeSegment;
		a->NextSegment->PreviousSegment = b;
		a->NextSegment->PreviousFreeSegment = b;
		a->NextFreeSegment->PreviousFreeSegment = b;
	}
}

void free(void* address)
{
	MemorySegmentHeader* currentMemorySegment;
		
	AlignedMemorySegmentHeader* AMSH = (AlignedMemorySegmentHeader*)address - 1;
	if (AMSH->IsAligned)
	{
		currentMemorySegment = (MemorySegmentHeader*)(unsigned long long)AMSH->MemorySegmentHeaderAddress;
	}
	else
	{
		currentMemorySegment = ((MemorySegmentHeader*)address) - 1;
	}
	currentMemorySegment->Free = true;

	if (currentMemorySegment < FirstFreeMemorySegment) FirstFreeMemorySegment = currentMemorySegment;

	if (currentMemorySegment->NextFreeSegment != 0)
	{
		if (currentMemorySegment->NextFreeSegment->PreviousFreeSegment < currentMemorySegment)
			currentMemorySegment->NextFreeSegment->PreviousFreeSegment = currentMemorySegment;
	}
	if (currentMemorySegment->PreviousFreeSegment != 0)
	{
		if (currentMemorySegment->PreviousFreeSegment->NextFreeSegment > currentMemorySegment)
			currentMemorySegment->PreviousFreeSegment->NextFreeSegment = currentMemorySegment;
	}
	if (currentMemorySegment->NextSegment != 0)
	{
		currentMemorySegment->NextSegment->PreviousSegment = currentMemorySegment;
		if (currentMemorySegment->NextSegment->Free) CombineFreeSegments(currentMemorySegment, currentMemorySegment->NextSegment);
	}
	if (currentMemorySegment->PreviousSegment != 0)
	{
		currentMemorySegment->PreviousSegment->NextSegment = currentMemorySegment;
		if (currentMemorySegment->PreviousSegment->Free) CombineFreeSegments(currentMemorySegment, currentMemorySegment->PreviousSegment);
	}
}

void* aligned_alloc(unsigned long long alignment, unsigned long long size)
{
	unsigned long long alignmentRemainder = alignment % 8;
	alignment -= alignmentRemainder;
	if (alignmentRemainder != 0) alignment += 8;

	unsigned long long sizeRemainder = size % 8;
	size -= sizeRemainder;
	if (sizeRemainder != 0) size += 8;

	unsigned long long fullSize = size + alignment;

	void* mallocVal = malloc(fullSize);
	unsigned long long address = (unsigned long long)mallocVal;

	unsigned long long remainder = address % alignment;
	address -= remainder;
	if (remainder != 0)
	{
		address += alignment;

		AlignedMemorySegmentHeader* AMSH = (AlignedMemorySegmentHeader*)address - 1;
		AMSH->IsAligned = true;
		AMSH->MemorySegmentHeaderAddress = (unsigned long long)mallocVal - sizeof(MemorySegmentHeader);
	}

	return (void*)address;
}