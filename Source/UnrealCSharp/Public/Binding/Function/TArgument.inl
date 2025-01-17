#pragma once

#include "Binding/Core/TPropertyValue.inl"
#include "UEVersion.h"

template <typename T, typename Enable = void>
struct TArgument
{
};

template <typename T>
struct TBaseArgument
{
	using Type = typename TDecay<T>::Type;

	TBaseArgument() = default;

	explicit TBaseArgument(MonoObject* InMonoObject):
		Value{TPropertyValue<Type, Type>::Set(InMonoObject)}
	{
	}

	Type& Get()
	{
		return Value;
	}

	MonoObject* Set()
	{
		return TPropertyValue<Type, Type>::Get(&Value);
	}

	constexpr bool IsOut() const
	{
		return TAnd<TNot<TIsConst<T>>, TIsReferenceType<T>>::Value;
	}

protected:
	Type Value;
};

template <typename T>
struct TSingleArgument :
	TBaseArgument<T>
{
	using TBaseArgument<T>::TBaseArgument;
};

template <typename T>
struct TReferenceArgument :
	TBaseArgument<T>
{
	using Super = TBaseArgument<T>;

	using Super::TBaseArgument;

	using Type = typename Super::Type;

	MonoObject* Set()
	{
		return TPropertyValue<Type, Type>::Get(new Type(Super::Value));
	}
};

template <typename T>
struct TContainerArgument :
	TReferenceArgument<T>
{
	using TReferenceArgument<T>::TReferenceArgument;
};

template <typename T>
struct TMultiArgument :
	TReferenceArgument<T>
{
	using TReferenceArgument<T>::TReferenceArgument;
};

template <typename T>
struct TBindingArgument :
	TReferenceArgument<T>
{
	using TReferenceArgument<T>::TReferenceArgument;
};

template <typename T>
struct TScriptStructArgument :
	TReferenceArgument<T>
{
	using TReferenceArgument<T>::TReferenceArgument;
};

template <typename T>
struct TBindingEnumArgument :
	TSingleArgument<T>
{
	using TSingleArgument<T>::TSingleArgument;
};

template <typename T>
#if UE_T_IS_SAME
struct TArgument<T, typename TEnableIf<TIsSame<typename TDecay<T>::Type, uint8>::Value>::Type> :
#else
struct TArgument<T, typename TEnableIf<std::is_same_v<typename TDecay<T>::Type, uint8>>::Type> :
#endif
	TSingleArgument<T>
{
	using TSingleArgument<T>::TSingleArgument;
};

template <typename T>
#if UE_T_IS_SAME
struct TArgument<T, typename TEnableIf<TIsSame<typename TDecay<T>::Type, uint16>::Value>::Type> :
#else
struct TArgument<T, typename TEnableIf<std::is_same_v<typename TDecay<T>::Type, uint16>>::Type> :
#endif
	TSingleArgument<T>
{
	using TSingleArgument<T>::TSingleArgument;
};

template <typename T>
#if UE_T_IS_SAME
struct TArgument<T, typename TEnableIf<TIsSame<typename TDecay<T>::Type, uint32>::Value>::Type> :
#else
struct TArgument<T, typename TEnableIf<std::is_same_v<typename TDecay<T>::Type, uint32>>::Type> :
#endif
	TSingleArgument<T>
{
	using TSingleArgument<T>::TSingleArgument;
};

template <typename T>
#if UE_T_IS_SAME
struct TArgument<T, typename TEnableIf<TIsSame<typename TDecay<T>::Type, uint64>::Value>::Type> :
#else
struct TArgument<T, typename TEnableIf<std::is_same_v<typename TDecay<T>::Type, uint64>>::Type> :
#endif
	TSingleArgument<T>
{
	using TSingleArgument<T>::TSingleArgument;
};

template <typename T>
#if UE_T_IS_SAME
struct TArgument<T, typename TEnableIf<TIsSame<typename TDecay<T>::Type, int8>::Value>::Type> :
#else
struct TArgument<T, typename TEnableIf<std::is_same_v<typename TDecay<T>::Type, int8>>::Type> :
#endif
	TSingleArgument<T>
{
	using TSingleArgument<T>::TSingleArgument;
};

template <typename T>
#if UE_T_IS_SAME
struct TArgument<T, typename TEnableIf<TIsSame<typename TDecay<T>::Type, int16>::Value>::Type> :
#else
struct TArgument<T, typename TEnableIf<std::is_same_v<typename TDecay<T>::Type, int16>>::Type> :
#endif
	TSingleArgument<T>
{
	using TSingleArgument<T>::TSingleArgument;
};

template <typename T>
#if UE_T_IS_SAME
struct TArgument<T, typename TEnableIf<TIsSame<typename TDecay<T>::Type, int32>::Value>::Type> :
#else
struct TArgument<T, typename TEnableIf<std::is_same_v<typename TDecay<T>::Type, int32>>::Type> :
#endif
	TSingleArgument<T>
{
	using TSingleArgument<T>::TSingleArgument;
};

template <typename T>
#if UE_T_IS_SAME
struct TArgument<T, typename TEnableIf<TIsSame<typename TDecay<T>::Type, int64>::Value>::Type> :
#else
struct TArgument<T, typename TEnableIf<std::is_same_v<typename TDecay<T>::Type, int64>>::Type> :
#endif
	TSingleArgument<T>
{
	using TSingleArgument<T>::TSingleArgument;
};

template <typename T>
#if UE_T_IS_SAME
struct TArgument<T, typename TEnableIf<TIsSame<typename TDecay<T>::Type, bool>::Value>::Type> :
#else
struct TArgument<T, typename TEnableIf<std::is_same_v<typename TDecay<T>::Type, bool>>::Type> :
#endif
	TSingleArgument<T>
{
	using TSingleArgument<T>::TSingleArgument;
};

template <typename T>
#if UE_T_IS_SAME
struct TArgument<T, typename TEnableIf<TIsSame<typename TDecay<T>::Type, float>::Value>::Type> :
#else
struct TArgument<T, typename TEnableIf<std::is_same_v<typename TDecay<T>::Type, float>>::Type> :
#endif
	TSingleArgument<T>
{
	using TSingleArgument<T>::TSingleArgument;
};

template <typename T>
#if UE_T_IS_SAME
struct TArgument<T,
                 typename TEnableIf<TAnd<
	                 TIsDerivedFrom<typename TRemovePointer<typename TDecay<T>::Type>::Type, UObject>,
	                 TNot<TIsSame<typename TRemovePointer<typename TDecay<T>::Type>::Type, UClass>>>::Value>
                 ::Type> :
	TSingleArgument<T>
#else
struct TArgument<T,
                 typename TEnableIf<
	                 TIsDerivedFrom<typename TRemovePointer<typename TDecay<T>::Type>::Type, UObject>::Value &&
	                 !std::is_same_v<typename TRemovePointer<typename TDecay<T>::Type>::Type, UClass>>
                 ::Type> :
	TSingleArgument<T>
#endif
{
	using TSingleArgument<T>::TSingleArgument;
};

#if UE_OBJECT_PTR
template <typename T>
struct TArgument<T, typename TEnableIf<TIsTObjectPtr<typename TDecay<T>::Type>::Value>::Type> :
	TSingleArgument<T>
{
	using TSingleArgument<T>::TSingleArgument;
};
#endif

template <typename T>
#if UE_T_IS_SAME
struct TArgument<T, typename TEnableIf<TIsSame<typename TDecay<T>::Type, FName>::Value>::Type> :
#else
struct TArgument<T, typename TEnableIf<std::is_same_v<typename TDecay<T>::Type, FName>>::Type> :
#endif
	TSingleArgument<T>
{
	using TSingleArgument<T>::TSingleArgument;
};

template <typename T>
struct TArgument<T, typename TEnableIf<TIsTScriptInterface<typename TDecay<T>::Type>::Value>::Type> :
	TMultiArgument<T>
{
	using TMultiArgument<T>::TMultiArgument;
};

template <typename T>
struct TArgument<T, typename TEnableIf<TIsUStruct<typename TDecay<T>::Type>::Value>::Type> :
	TReferenceArgument<T>
{
	using TReferenceArgument<T>::TReferenceArgument;
};

template <typename T>
#if UE_T_IS_SAME
struct TArgument<T, typename TEnableIf<TIsSame<typename TDecay<T>::Type, FString>::Value>::Type> :
#else
struct TArgument<T, typename TEnableIf<std::is_same_v<typename TDecay<T>::Type, FString>>::Type> :
#endif
	TSingleArgument<T>
{
	using TSingleArgument<T>::TSingleArgument;
};

template <typename T>
#if UE_T_IS_SAME
struct TArgument<T, typename TEnableIf<TIsSame<typename TDecay<T>::Type, FText>::Value>::Type> :
#else
struct TArgument<T, typename TEnableIf<std::is_same_v<typename TDecay<T>::Type, FText>>::Type> :
#endif
	TSingleArgument<T>
{
	using TSingleArgument<T>::TSingleArgument;
};

template <typename T>
struct TArgument<T, typename TEnableIf<TIsTWeakObjectPtr<typename TDecay<T>::Type>::Value>::Type> :
	TMultiArgument<T>
{
	using TMultiArgument<T>::TMultiArgument;
};

template <typename T>
struct TArgument<T, typename TEnableIf<TIsTLazyObjectPtr<typename TDecay<T>::Type>::Value>::Type> :
	TMultiArgument<T>
{
	using TMultiArgument<T>::TMultiArgument;
};

template <typename T>
struct TArgument<T, typename TEnableIf<TIsTSoftObjectPtr<typename TDecay<T>::Type>::Value>::Type> :
	TMultiArgument<T>
{
	using TMultiArgument<T>::TMultiArgument;
};

template <typename T>
#if UE_T_IS_SAME
struct TArgument<T, typename TEnableIf<TIsSame<typename TDecay<T>::Type, double>::Value>::Type> :
#else
struct TArgument<T, typename TEnableIf<std::is_same_v<typename TDecay<T>::Type, double>>::Type> :
#endif
	TSingleArgument<T>
{
	using TSingleArgument<T>::TSingleArgument;
};

template <typename T>
struct TArgument<T, typename TEnableIf<TIsTMap<typename TDecay<T>::Type>::Value>::Type> :
	TContainerArgument<T>
{
	using TContainerArgument<T>::TContainerArgument;
};

template <typename T>
struct TArgument<T, typename TEnableIf<TIsTSet<typename TDecay<T>::Type>::Value>::Type> :
	TContainerArgument<T>
{
	using TContainerArgument<T>::TContainerArgument;
};

template <typename T>
struct TArgument<T, typename TEnableIf<TIsTSubclassOf<typename TDecay<T>::Type>::Value>::Type> :
	TMultiArgument<T>
{
	using TMultiArgument<T>::TMultiArgument;
};

template <typename T>
#if UE_T_IS_SAME
struct TArgument<T, typename TEnableIf<TIsSame<typename TRemovePointer<typename TDecay<T>::Type>::Type,
                                               UClass>::Value>::Type> :
#else
struct TArgument<T, typename TEnableIf<std::is_same_v<typename TRemovePointer<typename TDecay<T>::Type>::Type,
                                                      UClass>>::Type> :
#endif
	TMultiArgument<T>
{
	using TMultiArgument<T>::TMultiArgument;
};

template <typename T>
struct TArgument<T, typename TEnableIf<TIsTArray<typename TDecay<T>::Type>::Value>::Type> :
	TContainerArgument<T>
{
	using TContainerArgument<T>::TContainerArgument;
};

template <typename T>
struct TArgument<T, typename TEnableIf<TAnd<TIsEnum<typename TDecay<T>::Type>,
                                            TNot<TIsNotUEnum<typename TDecay<T>::Type>>>
	                 ::Value>::Type> :
	TSingleArgument<T>
{
	using TSingleArgument<T>::TSingleArgument;
};

template <typename T>
struct TArgument<T, typename TEnableIf<TIsTEnumAsByte<typename TDecay<T>::Type>::Value>::Type> :
	TSingleArgument<T>
{
	using TSingleArgument<T>::TSingleArgument;
};

template <typename T>
struct TArgument<T, typename TEnableIf<TIsTSoftClassPtr<typename TDecay<T>::Type>::Value>::Type> :
	TMultiArgument<T>
{
	using TMultiArgument<T>::TMultiArgument;
};
