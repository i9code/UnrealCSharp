﻿using System;
using Script.Library;
using Script.Common;

namespace Script.CoreUObject
{
    public partial class FAssetBundleData
    {
        public FAssetBundleEntry FindEntry(FName SearchName)
        {
            AssetBundleDataImplementation.AssetBundleData_FindEntryImplementation(GetHandle(), SearchName,
                out var OutValue);

            return OutValue;
        }

        public void AddBundleAsset(FName BundleName, FSoftObjectPath AssetPath) =>
            AssetBundleDataImplementation.AssetBundleData_AddBundleAssetImplementation(GetHandle(), BundleName,
                AssetPath);

        // @TODO
        // AddBundleAsset
        // AddBundleAssets
        // SetBundleAssets

        public void Reset() =>
            AssetBundleDataImplementation.AssetBundleData_ResetImplementation(GetHandle());

        // @TODO
        // ExportTextItem
        // ImportTextItem

        public FString ToDebugString()
        {
            AssetBundleDataImplementation.AssetBundleData_ToDebugStringImplementation(GetHandle(), out var OutValue);

            return OutValue;
        }
    }
}