﻿#include "Reflection/Property/DelegateProperty/FMulticastDelegatePropertyDescriptor.h"
#include "Environment/FCSharpEnvironment.h"
#include "Reflection/Delegate/FMulticastDelegateHelper.h"
#include "Bridge/FTypeBridge.h"
#include "Template/TGetArrayLength.inl"

FMulticastDelegatePropertyDescriptor::FMulticastDelegatePropertyDescriptor(FProperty* InProperty):
	FPropertyDescriptor(InProperty),
	Class(nullptr),
	Type(nullptr)
{
	Class = FTypeBridge::GetMonoClass(MulticastDelegateProperty);

	const auto FoundMonoType = FCSharpEnvironment::GetEnvironment().GetDomain()->Class_Get_Type(Class);

	Type = FCSharpEnvironment::GetEnvironment().GetDomain()->Type_Get_Object(
		FoundMonoType);
}

void FMulticastDelegatePropertyDescriptor::Get(void* Src, void** Dest) const
{
	if (MulticastDelegateProperty != nullptr)
	{
		*Dest = NewWeakRef(Src);
	}
}

void FMulticastDelegatePropertyDescriptor::Get(void* Src, void* Dest) const
{
	if (MulticastDelegateProperty != nullptr)
	{
		*static_cast<void**>(Dest) = NewRef(Src);
	}
}

void FMulticastDelegatePropertyDescriptor::Set(void* Src, void* Dest) const
{
	if (MulticastDelegateProperty != nullptr)
	{
		const auto SrcMonoObject = static_cast<MonoObject*>(Src);

		const auto SrcMulticastDelegateHelper = FCSharpEnvironment::GetEnvironment().GetDelegate<
			FMulticastDelegateHelper>(SrcMonoObject);

		MulticastDelegateProperty->InitializeValue(Dest);

		const auto MulticastScriptDelegate = const_cast<FMulticastScriptDelegate*>(GetMulticastDelegate(Dest));

		FScriptDelegate ScriptDelegate;

		ScriptDelegate.BindUFunction(SrcMulticastDelegateHelper->GetUObject(),
		                             SrcMulticastDelegateHelper->GetFunctionName());

		MulticastScriptDelegate->Add(ScriptDelegate);
	}
}

const FMulticastScriptDelegate* FMulticastDelegatePropertyDescriptor::GetMulticastDelegate(void* InAddress) const
{
	return MulticastDelegateProperty->GetMulticastDelegate(InAddress);
}

MonoObject* FMulticastDelegatePropertyDescriptor::NewRef(void* InAddress) const
{
	auto Object = FCSharpEnvironment::GetEnvironment().GetDelegateObject<FMulticastDelegateHelper>(InAddress);

	if (Object == nullptr)
	{
		const auto MulticastDelegateHelper = new FMulticastDelegateHelper(
			const_cast<FMulticastScriptDelegate*>(GetMulticastDelegate(InAddress)),
			MulticastDelegateProperty->SignatureFunction);

		auto InParams = static_cast<void*>(Type);

		Object = FCSharpEnvironment::GetEnvironment().GetDomain()->Object_New(
			Class, TGetArrayLength(InParams), &InParams);

		const auto OwnerGarbageCollectionHandle = FCSharpEnvironment::GetEnvironment().GetGarbageCollectionHandle(
			InAddress, DelegateProperty);

		FCSharpEnvironment::GetEnvironment().AddDelegateReference(OwnerGarbageCollectionHandle, InAddress,
		                                                          MulticastDelegateHelper, Object);
	}

	return Object;
}

MonoObject* FMulticastDelegatePropertyDescriptor::NewWeakRef(void* InAddress) const
{
	const auto MulticastDelegateHelper = new FMulticastDelegateHelper(
		const_cast<FMulticastScriptDelegate*>(GetMulticastDelegate(InAddress)),
		MulticastDelegateProperty->SignatureFunction);

	auto InParams = static_cast<void*>(Type);

	const auto Object = FCSharpEnvironment::GetEnvironment().GetDomain()->Object_New(
		Class, TGetArrayLength(InParams), &InParams);

	FCSharpEnvironment::GetEnvironment().AddDelegateReference(InAddress, MulticastDelegateHelper, Object);

	return Object;
}
