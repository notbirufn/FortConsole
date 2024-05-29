#pragma once

UObject* StaticFindObject(UObject* Class, UObject* InOuter, const TCHAR* Name, bool ExactClass)
{
    static Memcury::Scanner Scanner = Memcury::Scanner::FindStringRef(L"MaterialShaderQualitySettingsContainer").ScanFor({ Memcury::ASM::CALL }).RelativeOffset(1);

    if (Scanner.IsValid())
    {
        return reinterpret_cast<UObject* (*)(UObject*, UObject*, const TCHAR*, bool)>(Scanner.Get())(Class, InOuter, Name, ExactClass);
    }

    return nullptr;
}

namespace Console
{
    void Enable()
    {
        static UObject* FortEngine = StaticFindObject(nullptr, nullptr, L"/Engine/Transient.FortEngine_2147482646", false);

        UObject* ConsoleClass = *reinterpret_cast<UObject**>(uintptr_t(FortEngine) + 0x00F0);
        UObject* GameViewport = *reinterpret_cast<UObject**>(uintptr_t(FortEngine) + 0x0910);

        if (ConsoleClass && GameViewport)
        {
            static UObject* GameplayStatics = StaticFindObject(nullptr, nullptr, L"/Script/Engine.Default__GameplayStatics", false);
            static UObject* Function = StaticFindObject(nullptr, nullptr, L"/Script/Engine.GameplayStatics.SpawnObject", false);

            struct GameplayStatics_SpawnObject
            {
                UObject* ObjectClass;
                UObject* Param_Outer;
                UObject* ReturnValue;
            };

            GameplayStatics_SpawnObject Params{ ConsoleClass, GameViewport };

            reinterpret_cast<void (*)(UObject*, UObject*, void*)>(GameplayStatics->VTableObject[0x4D])(GameplayStatics, Function, &Params);

            if (Params.ReturnValue)
            {
                *reinterpret_cast<UObject**>(uintptr_t(GameViewport) + 0x0040) = Params.ReturnValue;
            }
        }
    }
}