using System;

namespace Script.Mixin
{
    [AttributeUsage(AttributeTargets.Method | AttributeTargets.Property)]
    public class BlueprintGetterAttribute : Attribute
    {
    }
}