#ifndef WTF_Alignment_h
#define WTF_Alignment_h

#include <algorithm>
#include <stdint.h>

#define WTF_ALIGN_OF(type) __alignof(type)
#define WTF_ALIGNED(variable_type, variable, n) __declspec(align(n)) variable_type variable

typedef char AlignedBufferChar;
template<size_t size, size_t alignment> struct AlignedBuffer;
template<size_t size> struct AlignedBuffer < size, 1 > { AlignedBufferChar buffer[size]; };
template<size_t size> struct AlignedBuffer < size, 2 > { WTF_ALIGNED(AlignedBufferChar, buffer[size], 2); };
template<size_t size> struct AlignedBuffer < size, 4 > { WTF_ALIGNED(AlignedBufferChar, buffer[size], 4); };
template<size_t size> struct AlignedBuffer < size, 8 > { WTF_ALIGNED(AlignedBufferChar, buffer[size], 8); };
template<size_t size> struct AlignedBuffer < size, 16 > { WTF_ALIGNED(AlignedBufferChar, buffer[size], 16); };
template<size_t size> struct AlignedBuffer < size, 32 > { WTF_ALIGNED(AlignedBufferChar, buffer[size], 32); };
template<size_t size> struct AlignedBuffer < size, 64 > { WTF_ALIGNED(AlignedBufferChar, buffer[size], 64); };

template <size_t size, size_t alignment>
void swap(AlignedBuffer<size, alignment>& a, AlignedBuffer<size, alignment>& b)
{
	for (size_t i = 0; i < size; ++i)
		std::swap(a.buffer[i], b.buffer[i]);
}

template <uintptr_t mask>
inline bool isAlignedTo(const void* pointer)
{
	return !(reinterpret_cast<uintptr_t>(pointer)& mask);
}

#endif // WTF_Alignment_h
