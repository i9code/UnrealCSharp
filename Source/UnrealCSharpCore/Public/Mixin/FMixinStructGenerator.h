#pragma once

#include "CSharpScriptStruct.h"
#include "mono/metadata/object.h"

class FMixinStructGenerator
{
public:
	static void Generator();

#if WITH_EDITOR
	static void CodeAnalysisGenerator();
#endif

	static void Generator(MonoClass* InMonoClass, bool bReInstance = false);

	static bool IsMixinStruct(MonoClass* InMonoClass);

private:
	static void BeginGenerator(UScriptStruct* InScriptStruct);

	static void EndGenerator(UScriptStruct* InScriptStruct);

	static UCSharpScriptStruct* GeneratorCSharpScriptStruct(UPackage* InOuter, const FString& InName);

#if WITH_EDITOR
	static void ReInstance(UScriptStruct* InScriptStruct, const TArray<UBlueprint*>& InBlueprints);
#endif

	static void GeneratorProperty(MonoClass* InMonoClass, UScriptStruct* InScriptStruct);
};
