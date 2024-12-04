namespace global
{
	HMODULE hProcess;

	namespace aimbot {
		struct aimbot_config {
			bool bEnabled = false;

			int iHitbox = 0;
			int iKey = 0;

			float fSpeed = 2;
			float fRecoilStrength = 1;
			float fMaxFov = 1.9;

			bool bNearestBone = false;
			bool bNearestVisibleLocation = false;
			bool bConstant = true;
			bool bAlternatePistoleKey = false;
			bool bAutoShot = false;
		};

		aimbot_config config;
	}

	namespace triggerbot {
		struct triggerbot_config {
			bool bEnabled = false;
			bool bShotOnce = true;

			int iDelay = 50;
			int iKey = 0;
		};

		triggerbot_config config;
	}

	namespace misc
	{
		struct misc_config {
			bool bStandaloneRecoil = false;
			bool bBunnyhop = true;
			bool bDebug = false;
			bool bRandomAgent = false;
			bool bInstaLock = false;
			bool bDrawBoneIds = false;
			bool bPingEsp = false;
			bool bPingEspWarn = false;
			bool bThirdPerson = false;
			bool bNoRisk = false;
			bool bRadarHack = false;
			bool bSpikeEsp = false;
			bool bAntiAfk = false;

			int iInstaLockAgent = 0;
			int iBoneId = 0;
			int iPingEspDelay = 80;
			int iThirdPersonUpZ = 0;
			int iFresnel;
		};

		misc_config config;
	}

	namespace esp
	{
		struct esp_config {
			bool bEnabled = false;
			bool bSkeleton = false;
			bool bDrawSkeletonHead = false;
			bool bHotkeyVisuals = false;
			bool bDrawNames = false;
			bool bDrawHealth = false;
			bool bDrawDormant = true;
			bool bWireFrameGun = false;
			bool bWireFrameHands = false;	
			bool bDrawRecoil = false;
			bool bCharacterIcon = false;
			bool bCharacterWeaponIcon = false;

			int iEspType = 0;
			int iKey = 0;

			float fSkeletonThickness = 1.0f;

			float colSkelHeadR = 1;
			float colSkelHeadG = 0;
			float colSkelHeadB = 0;
		};

		esp_config config;
	}

	namespace chams
	{
		struct chams_config {
			bool bEnabled = false;
			bool bAlways = false;
			bool bHideTeamOutlines = true;
			bool bHotkeyChams = false;
			bool bRainbowChams = false;
			bool bOwnColorMode = false;

			int iKey = 0;
			int iOutlineMode = 0;

			flinearcolor flColor = { 2.093f, 0.019f, 20.0f, 5.9f };

			float fR = 2.093f;
			float fG = 0.019f;
			float fB = 20.0f;
			float fA = 5.9f;
		};

		chams_config config;
	}

	namespace fresnel {
		struct fresnel_config {
			bool bEnabled = false;

			float fIntensity = 1.f;

			flinearcolor flColor = { 2.093f, 0.019f, 20.0f, 5.9f };
			flinearcolor flAllyColor = { 2.093f, 0.019f, 20.0f, 5.9f };
		};

		fresnel_config config;
	}

	namespace skin_changer
	{
		struct skinchanger_config {
			bool bEnabled = false;

			int iGunBuddy = 0;
		};

		skinchanger_config config;
	}
}
