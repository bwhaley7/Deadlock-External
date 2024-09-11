#include <cstdint>
#include "../Util/QAngle.h"
#include "../Util/Vector.h"

namespace GameOffsets {
    constexpr uintptr_t dwEntityList = 0x252D8B0;
    constexpr uintptr_t dwGameEntitySystem_highestEntityIndex = 0x5408;
    constexpr uintptr_t dwLocalPlayer = 0x252B4E0;

}

namespace enums {
    enum class EPlayerPlayState : std::uint32_t
    {
        EPlayerStateInvalid = 0xffffffff,
        EPlayerActive = 0x0,
        EPlayerAwaitingHeroAssignment = 0x1,
        EPlayerChoosingHero = 0x2,
        EPlayerPlayStateCount = 0x3,
    };
    enum class PlayerConnectedState : std::uint32_t
    {
        PlayerNeverConnected = 0xffffffff,
        PlayerConnected = 0x0,
        PlayerConnecting = 0x1,
        PlayerReconnecting = 0x2,
        PlayerDisconnecting = 0x3,
        PlayerDisconnected = 0x4,
        PlayerReserved = 0x5,
    };
}

namespace GameSDK {

    class CUtlSymbolLarge {
    public:
        static const uintptr_t m_Id = 0x0;
        static const uintptr_t m_pAsString = 0x4;
    };

    class CEntityComponent {
    public:
        static const uintptr_t __pad0000 = 0x0;
    };

    class CScriptComponent : public CEntityComponent {
    public:
        static const uintptr_t m_scriptClassName = 0x30;
    };

    class CEntityInstance {
    public:
        static const uintptr_t m_iszPrivateVScripts = 0x8;
        static const uintptr_t m_pEntity = 0x10;
        static const uintptr_t m_CScriptComponent = 0x28;
        static const uintptr_t m_bVisibleinPVS = 0x30;
    };

    class CEntityIdentity {
    public:
        static const uintptr_t m_nameStringableIndex = 0x14;
        static const uintptr_t m_name = 0x18;
        static const uintptr_t m_designerName = 0x20;
        static const uintptr_t m_flags = 0x30;
        static const uintptr_t m_pPrev = 0x58;
        static const uintptr_t m_pNext = 0x60;
        static const uintptr_t m_pPrevByClass = 0x68;
        static const uintptr_t m_pNextByClass = 0x70;
    };

    class CGameSceneNode {
    public:
        static const uintptr_t m_pOwner = 0x30;
        static const uintptr_t m_pParent = 0x38;
        static const uintptr_t m_pChild = 0x40;
        static const uintptr_t m_pNextSibling = 0x48;
        static const uintptr_t m_angRotation = 0xc0;
        static const uintptr_t m_flScale = 0xcc;
        static const uintptr_t m_vecAbsOrigin = 0xd0;
        static const uintptr_t m_angAbsRotation = 0xdc;
        static const uintptr_t m_flAbsScale = 0xe8;
    };

    class C_BaseEntity : public CEntityInstance {
    public:
        static const uintptr_t m_pGameSceneNode = 0x328;
        static const uintptr_t m_iMaxHealth = 0x348;
        static const uintptr_t m_iHealth = 0x34c;
        static const uintptr_t m_lifeState = 0x350;
        static const uintptr_t m_bTakesDamage = 0x351;
        static const uintptr_t m_nNoInterpolationTick = 0x368;
        static const uintptr_t m_flProxyRandomValue = 0x370;
        static const uintptr_t m_iEFlags = 0x374;
        static const uintptr_t m_nWaterType = 0x378;
        static const uintptr_t m_bInterpolateEvenWithNoModel = 0x379;
        static const uintptr_t m_bPredictionEligible = 0x37a;
        static const uintptr_t m_fFlags = 0x3f8;
        static const uintptr_t m_vecAbsVelocity = 0x3fc;
        static const uintptr_t m_flSpeed = 0x3e8;
    };

    struct PlayerDataGlobal_t {
        static const uintptr_t m_iLevel = 0x8;
        static const uintptr_t m_iMaxAmmo = 0xc;
        static const uintptr_t m_iHealthMax = 0x10;
        static const uintptr_t m_flHealthRegen = 0x14;
        static const uintptr_t m_iGoldNetWorth = 0x20;
        static const uintptr_t m_iAPNetWorth = 0x24;
        static const uintptr_t m_iCreepGold = 0x28;
        static const uintptr_t m_iHealth = 0x48;
        static const uintptr_t m_iPlayerKills = 0x4c;
        static const uintptr_t m_iPlayerAssists = 0x50;
        static const uintptr_t m_iDeaths = 0x54;
        static const uintptr_t m_iTeamNum = 0x3ef; //uint8_t
    };

    class CBasePlayerController : public C_BaseEntity {
    public:
        static const uintptr_t m_iConnected = 0x63c; // PlayerConnectedState	
    };

    class CCitadelPlayerController : public CBasePlayerController {
    public:
        static const uintptr_t m_ePlayState = 0x6d0;
        static const uintptr_t m_iGuidedBotMatchLastHits = 0x6d4;
        static const uintptr_t m_iGuidedBotMatchOrbsSecured = 0x6d8;
        static const uintptr_t m_PlayerDataGlobal = 0x728;
    };

}