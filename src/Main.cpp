#include "Logging.h"

using namespace SKSE;
using namespace SKSE::log;
using namespace SKSE::stl;

std::string asHexAddress(uint64_t decAddress) { return std::format("{:x}", decAddress); }

SKSEPluginLoad(const LoadInterface* skse) {
    InitializeLogging();
    const auto* plugin = PluginDeclaration::GetSingleton();
    auto version = plugin->GetVersion();
    log::info("{} {} is starting...", plugin->GetName(), version);
    Init(skse);

    bool gameIsAE = REL::Module::IsAE();
    bool gameIsVR = REL::Module::IsVR();

    std::string versionId = "";
    uint64_t baseAddress = 0;

    if (gameIsAE) { 
        versionId = "AE";
    } else if(gameIsVR) {
        versionId = "VR";
    } else {
        versionId = "SE";
    }
    
    if (gameIsVR) {
        baseAddress = (uint64_t)GetModuleHandleA("SkyrimVR.exe");
        log::info("SkyrimVR.exe ({}) - Base Address: {}", versionId, asHexAddress(baseAddress));
    } else {
        baseAddress = (uint64_t)GetModuleHandleA("SkyrimSE.exe");
        log::info("SkyrimSE.exe ({}) - Base Address: {}", versionId, asHexAddress(baseAddress));
    }

    // Prevent Auto Draw
    {
        // This is taken directly from the original mod (https://github.com/adamhynek/stuf_vr/) without testing it first.
        // There seems to be no equivalent to the function at this memory location in SE and AE, but maybe this is important for making this work correctly specifically in VR. (UNTESTED!)
        if (gameIsVR) {
           auto drawEitherHandLoc = baseAddress + 0x730A53; // Hardcoded offset for VR
           uint_fast8_t assemblyHex[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }; // 6 Bytes of Noops
           std::span<uint_fast8_t> instructions(assemblyHex);

           REL::safe_write(drawEitherHandLoc, instructions);

           log::info("[VR only location] Either hand draw location - Replacing instructions at address: {} [{}]",
               asHexAddress(drawEitherHandLoc), 
               asHexAddress(drawEitherHandLoc - baseAddress));
        }


        uint_fast8_t returnInstruction = 0xC3;  // x64 return instruction as hex

        // The hex values at the end are offsets into the instructions we want to overwrite, from the beginning of the subroutines.
        auto drawBothHandsLoc = REL::RelocationID(41345, 42419).address() + 0x90;  // SE(41345,140708a80) / AE(42419,744dd0) + (0x90 [144])
        auto drawRightHandLoc = REL::RelocationID(41364, 42438).address() + 0x4f;  // SE(41364,140709af0) / AE(42438,140745EC0) + (0x4f [79]) 
        auto drawLeftHandLoc = REL::RelocationID(41361, 42435).address() + 0x4f;  // SE(41361,1407096b0) / AE(42435,140745a80) + (0x4f [79])

        log::info("Both hands draw location - Replacing instructions at address: {} [{}]", 
            asHexAddress(drawBothHandsLoc), 
            asHexAddress(drawBothHandsLoc - baseAddress));
        REL::safe_write(drawBothHandsLoc, returnInstruction);

        log::info("Right hand draw location - Replacing instructions at address: {} [{}]",
            asHexAddress(drawRightHandLoc), 
            asHexAddress(drawRightHandLoc - baseAddress));
        REL::safe_write(drawRightHandLoc, returnInstruction);

        log::info("Left hand draw location - Replacing instructions at address: {} [{}]", 
            asHexAddress(drawLeftHandLoc), 
            asHexAddress(drawLeftHandLoc - baseAddress));
        REL::safe_write(drawLeftHandLoc, returnInstruction);
    }

    log::info("{} has finished.", plugin->GetName());
    return true;
}
