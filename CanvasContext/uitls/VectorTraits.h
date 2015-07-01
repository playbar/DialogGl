
#ifndef WTF_VectorTraits_h
#define WTF_VectorTraits_h

#include "RefPtr.h"
#include "OwnPtr.h"
#include <utility>

using std::pair;

template<typename T>
struct VectorTraitsBase
{
	static const bool needsDestruction = !IsPod<T>::value;
	static const bool needsInitialization = !IsPod<T>::value;
	static const bool canInitializeWithMemset = IsPod<T>::value;
	static const bool canMoveWithMemcpy = IsPod<T>::value;
	static const bool canCopyWithMemcpy = IsPod<T>::value;
	static const bool canFillWithMemset = IsPod<T>::value && (sizeof(T) == sizeof(char));
	static const bool canCompareWithMemcmp = IsPod<T>::value;
	template<typename U = void>
	struct NeedsTracingLazily {
		static const bool value = NeedsTracing<T>::value;
	};
	static const bool isWeak = IsWeak<T>::value;
};

template<typename T>
struct VectorTraits : VectorTraitsBase < T > { };

template<typename T>
struct SimpleClassVectorTraits : VectorTraitsBase < T >
{
	static const bool canInitializeWithMemset = true;
	static const bool canMoveWithMemcpy = true;
	static const bool canCompareWithMemcmp = true;
};

// We know OwnPtr and RefPtr are simple enough that initializing to 0 and
// moving with memcpy (and then not destructing the original) will totally
// work.
template<typename P>
struct VectorTraits<RefPtr<P> > : SimpleClassVectorTraits < RefPtr<P> > {};

template<typename P>
struct VectorTraits<OwnPtr<P> > : SimpleClassVectorTraits < OwnPtr<P> > {};

template<typename First, typename Second>
struct VectorTraits < pair<First, Second> >
{
	typedef VectorTraits<First> FirstTraits;
	typedef VectorTraits<Second> SecondTraits;

	static const bool needsDestruction = FirstTraits::needsDestruction || SecondTraits::needsDestruction;
	static const bool needsInitialization = FirstTraits::needsInitialization || SecondTraits::needsInitialization;
	static const bool canInitializeWithMemset = FirstTraits::canInitializeWithMemset && SecondTraits::canInitializeWithMemset;
	static const bool canMoveWithMemcpy = FirstTraits::canMoveWithMemcpy && SecondTraits::canMoveWithMemcpy;
	static const bool canCopyWithMemcpy = FirstTraits::canCopyWithMemcpy && SecondTraits::canCopyWithMemcpy;
	static const bool canFillWithMemset = false;
	static const bool canCompareWithMemcmp = FirstTraits::canCompareWithMemcmp && SecondTraits::canCompareWithMemcmp;
	template <typename U = void>
	struct NeedsTracingLazily {
		static const bool value = ShouldBeTraced<FirstTraits>::value || ShouldBeTraced<SecondTraits>::value;
	};
	static const bool isWeak = FirstTraits::isWeak || SecondTraits::isWeak;
};



#endif // WTF_VectorTraits_h
