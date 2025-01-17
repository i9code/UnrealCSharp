﻿#include "Registry/FClassRegistry.h"
#include "Domain/FDomain.h"
#include "Common/FUnrealCSharpFunctionLibrary.h"

FClassRegistry::FClassRegistry()
{
	Initialize();
}

FClassRegistry::~FClassRegistry()
{
	Deinitialize();
}

void FClassRegistry::Initialize()
{
}

void FClassRegistry::Deinitialize()
{
	for (auto& ClassDescriptorPair : ClassDescriptorMap)
	{
		delete ClassDescriptorPair.Value;

		ClassDescriptorPair.Value = nullptr;
	}

	ClassDescriptorMap.Empty();

	for (auto& PropertyDescriptorPair : PropertyDescriptorMap)
	{
		delete PropertyDescriptorPair.Value;

		PropertyDescriptorPair.Value = nullptr;
	}

	PropertyDescriptorMap.Empty();

	FunctionHashMap.Empty();

	for (auto& FunctionDescriptorPair : FunctionDescriptorMap)
	{
		delete FunctionDescriptorPair.Value;

		FunctionDescriptorPair.Value = nullptr;
	}

	FunctionDescriptorMap.Empty();
}

FClassDescriptor* FClassRegistry::GetClassDescriptor(const UStruct* InStruct) const
{
	const auto FoundClassDescriptor = ClassDescriptorMap.Find(InStruct);

	return FoundClassDescriptor != nullptr ? *FoundClassDescriptor : nullptr;
}

FClassDescriptor* FClassRegistry::GetClassDescriptor(const FName& InClassName) const
{
	const auto InClass = LoadObject<UStruct>(nullptr, *InClassName.ToString());

	return InClass != nullptr ? GetClassDescriptor(InClass) : nullptr;
}

FClassDescriptor* FClassRegistry::AddClassDescriptor(const FDomain* InDomain, UStruct* InStruct)
{
	const auto FoundClassDescriptor = ClassDescriptorMap.Find(InStruct);

	if (FoundClassDescriptor != nullptr)
	{
		return *FoundClassDescriptor;
	}

	const auto FoundMonoClass = InDomain->Class_From_Name(
		FUnrealCSharpFunctionLibrary::GetClassNameSpace(InStruct),
		FUnrealCSharpFunctionLibrary::GetFullClass(InStruct));

	if (FoundMonoClass == nullptr)
	{
		return nullptr;
	}

	const auto ClassDescriptor = new FClassDescriptor(InStruct, FoundMonoClass);

	ClassDescriptorMap.Add(InStruct, ClassDescriptor);

	return ClassDescriptor;
}

void FClassRegistry::RemoveClassDescriptor(const UStruct* InStruct)
{
	if (const auto FoundClassDescriptor = ClassDescriptorMap.Find(InStruct))
	{
		delete FoundClassDescriptor;

		ClassDescriptorMap.Remove(InStruct);
	}
}

FFunctionDescriptor* FClassRegistry::GetFunctionDescriptor(const uint32 InFunctionHash)
{
	if (const auto FoundFunctionDescriptor = FunctionDescriptorMap.Find(InFunctionHash))
	{
		return *FoundFunctionDescriptor;
	}

	if (const auto FoundFunctionHash = FunctionHashMap.Find(InFunctionHash))
	{
		if (const auto FoundFunctionDescriptor = FoundFunctionHash->Key->
		                                                            GetFunctionDescriptor(FoundFunctionHash->Value))
		{
			FunctionDescriptorMap.Add(InFunctionHash, FoundFunctionDescriptor);

			return FoundFunctionDescriptor;
		}
	}

	return nullptr;
}

FPropertyDescriptor* FClassRegistry::GetPropertyDescriptor(const uint32 InPropertyHash)
{
	const auto FoundPropertyDescriptor = PropertyDescriptorMap.Find(InPropertyHash);

	return FoundPropertyDescriptor != nullptr ? *FoundPropertyDescriptor : nullptr;
}

void FClassRegistry::AddFunctionDescriptor(const uint32 InFunctionHash, FFunctionDescriptor* InFunctionDescriptor)
{
	FunctionDescriptorMap.Add(InFunctionHash, InFunctionDescriptor);
}

void FClassRegistry::AddFunctionHash(const uint32 InFunctionHash, FClassDescriptor* InClassDescriptor,
                                     const FName& InFunctionName)
{
	FunctionHashMap.Add(InFunctionHash, TPair<FClassDescriptor*, FName>(InClassDescriptor, InFunctionName));
}

void FClassRegistry::RemoveFunctionDescriptor(const uint32 InFunctionHash)
{
	if (const auto FoundFunctionDescriptor = FunctionDescriptorMap.Find(InFunctionHash))
	{
		delete *FoundFunctionDescriptor;

		FunctionDescriptorMap.Remove(InFunctionHash);

		FunctionHashMap.Remove(InFunctionHash);
	}
}

void FClassRegistry::AddPropertyDescriptor(const uint32 InPropertyHash, FPropertyDescriptor* InPropertyDescriptor)
{
	PropertyDescriptorMap.Add(InPropertyHash, InPropertyDescriptor);
}

void FClassRegistry::RemovePropertyDescriptor(const uint32 InPropertyHash)
{
	if (const auto FoundPropertyDescriptor = PropertyDescriptorMap.Find(InPropertyHash))
	{
		delete *FoundPropertyDescriptor;

		PropertyDescriptorMap.Remove(InPropertyHash);
	}
}
