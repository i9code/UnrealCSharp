﻿#include "Domain/InternalCall/FAssetBundleDataImplementation.h"
#include "Binding/Class/TReflectionClassBuilder.inl"
#include "Binding/ScriptStruct/TScriptStruct.inl"
#include "Environment/FCSharpEnvironment.h"
#include "Macro/NamespaceMacro.h"

struct FRegisterAssetBundleData
{
	FRegisterAssetBundleData()
	{
		TReflectionClassBuilder<FAssetBundleData>(NAMESPACE_LIBRARY)
			.Function("FindEntry", FAssetBundleDataImplementation::AssetBundleData_FindEntryImplementation)
			.Function("AddBundleAsset", FAssetBundleDataImplementation::AssetBundleData_AddBundleAssetImplementation)
			.Function("Reset", FAssetBundleDataImplementation::AssetBundleData_ResetImplementation)
			.Function("ToDebugString", FAssetBundleDataImplementation::AssetBundleData_ToDebugStringImplementation)
			.Register();
	}
};

static FRegisterAssetBundleData RegisterAssetBundleData;

void FAssetBundleDataImplementation::AssetBundleData_FindEntryImplementation(
	const FGarbageCollectionHandle InGarbageCollectionHandle, MonoObject* SearchName, MonoObject** OutValue)
{
	const auto AssetBundleData = FCSharpEnvironment::GetEnvironment().GetAddress<
		UScriptStruct, FAssetBundleData>(InGarbageCollectionHandle);

	const auto FoundMonoClass = TPropertyClass<FAssetBundleEntry, FAssetBundleEntry>::Get();

	const auto NewMonoObject = FCSharpEnvironment::GetEnvironment().GetDomain()->Object_New(FoundMonoClass);

	*OutValue = NewMonoObject;

	const auto OutAssetBundleEntry = FCSharpEnvironment::GetEnvironment().GetAddress<
		UScriptStruct, FAssetBundleEntry>(NewMonoObject);

	if (AssetBundleData != nullptr && OutAssetBundleEntry != nullptr)
	{
		*OutAssetBundleEntry = *AssetBundleData->FindEntry(UTF8_TO_TCHAR(
			FCSharpEnvironment::GetEnvironment().GetDomain()->String_To_UTF8(FCSharpEnvironment::GetEnvironment().
				GetDomain()->Object_To_String(SearchName, nullptr))));
	}
}

void FAssetBundleDataImplementation::AssetBundleData_AddBundleAssetImplementation(
	const FGarbageCollectionHandle InGarbageCollectionHandle, MonoObject* BundleName, const MonoObject* AssetPath)
{
	const auto AssetBundleData = FCSharpEnvironment::GetEnvironment().GetAddress<
		UScriptStruct, FAssetBundleData>(InGarbageCollectionHandle);

	const auto SoftObjectPath = FCSharpEnvironment::GetEnvironment().GetAddress<
		UScriptStruct, FSoftObjectPath>(InGarbageCollectionHandle);

	if (AssetBundleData != nullptr && SoftObjectPath != nullptr)
	{
		AssetBundleData->AddBundleAsset(
			UTF8_TO_TCHAR(
				FCSharpEnvironment::GetEnvironment().GetDomain()->String_To_UTF8(FCSharpEnvironment::GetEnvironment().
					GetDomain()->Object_To_String(BundleName, nullptr))), *SoftObjectPath);
	}
}

void FAssetBundleDataImplementation::AssetBundleData_ResetImplementation(
	const FGarbageCollectionHandle InGarbageCollectionHandle)
{
	const auto AssetBundleData = FCSharpEnvironment::GetEnvironment().GetAddress<
		UScriptStruct, FAssetBundleData>(InGarbageCollectionHandle);

	if (AssetBundleData != nullptr)
	{
		AssetBundleData->Reset();
	}
}

void FAssetBundleDataImplementation::AssetBundleData_ToDebugStringImplementation(
	const FGarbageCollectionHandle InGarbageCollectionHandle, MonoObject** OutValue)
{
	const auto AssetBundleData = FCSharpEnvironment::GetEnvironment().GetAddress<
		UScriptStruct, FAssetBundleData>(InGarbageCollectionHandle);

	if (AssetBundleData != nullptr)
	{
		const auto ResultString = AssetBundleData->ToDebugString();

		const auto FoundMonoClass = TPropertyClass<FString, FString>::Get();

		auto NewMonoString = static_cast<void*>(FCSharpEnvironment::GetEnvironment().GetDomain()->String_New(
			TCHAR_TO_UTF8(*ResultString)));

		const auto NewMonoObject = FCSharpEnvironment::GetEnvironment().GetDomain()->Object_New(
			FoundMonoClass, 1, &NewMonoString);

		*OutValue = NewMonoObject;
	}
}
