#include <cassert>

namespace pinguu {
	namespace config {
		void write_config(std::string group, std::string item, std::string value) {

			char buffer[128];

			auto app_data_enc = Encrypt("APPDATA");
			auto user_name_enc = Encrypt("USERNAME");
			auto buffer_enc = Encrypt("%s\\%s.ini");

			sprintf(buffer, buffer_enc.decrypt(), getenv(app_data_enc.decrypt()), getenv(user_name_enc.decrypt()));
			WritePrivateProfileString(group.c_str(), item.c_str(), value.c_str(), buffer);

			app_data_enc.encrypt();
			user_name_enc.encrypt();
			buffer_enc.encrypt();
		}


		char* read_config(const char* group, const char* item) {
			const unsigned long buffer_size = 255;
			char buffer[buffer_size];

			auto app_data_enc = Encrypt("APPDATA");
			auto user_name_enc = Encrypt("USERNAME");
			auto buffer_enc = Encrypt("%s\\%s.ini");

			char buffer1[128];
			sprintf(buffer1, buffer_enc.decrypt(), getenv(app_data_enc.decrypt()), getenv(user_name_enc.decrypt()));

			GetPrivateProfileString(group, item, nullptr, buffer, buffer_size, buffer1);

			app_data_enc.encrypt();
			user_name_enc.encrypt();
			buffer_enc.encrypt();
			return buffer;
		}

		#define write_reg(name, text, value) { auto enc = Encrypt(text); auto enc2 = Encrypt(name); write_config(enc2.decrypt(), enc.decrypt(), value), enc.encrypt(), enc2.encrypt(); }
		#define read_reg(name, text) [&](){ auto enc = Encrypt(text); auto enc2 = Encrypt(name); char* ret = read_config(enc2.decrypt(), enc.decrypt()); enc.encrypt(); enc2.encrypt(); return ret; }()
		
		void save_config() {
			
			// aimbot
			write_reg("AIMBOT", "bEnabled", std::to_string(global::aimbot::config.bEnabled));
			write_reg("AIMBOT", "iHitbox", std::to_string(global::aimbot::config.iHitbox));
			write_reg("AIMBOT", "iKey", std::to_string(global::aimbot::config.iKey));
			write_reg("AIMBOT", "fSpeed", std::to_string(global::aimbot::config.fSpeed));
			write_reg("AIMBOT", "fRecoilStrength", std::to_string(global::aimbot::config.fRecoilStrength));
			write_reg("AIMBOT", "fMaxFov", std::to_string(global::aimbot::config.fMaxFov));
			write_reg("AIMBOT", "bNearestBone", std::to_string(global::aimbot::config.bNearestBone));
			write_reg("AIMBOT", "bNearestVisibleLocation", std::to_string(global::aimbot::config.bNearestVisibleLocation));
			write_reg("AIMBOT", "bConstant", std::to_string(global::aimbot::config.bConstant));
			write_reg("AIMBOT", "bAlternatePistoleKey", std::to_string(global::aimbot::config.bAlternatePistoleKey));
			write_reg("AIMBOT", "bAutoShot", std::to_string(global::aimbot::config.bAutoShot));

			// triggerbot
			write_reg("TRIGGERBOT", "bEnabled", std::to_string(global::triggerbot::config.bEnabled));
			write_reg("TRIGGERBOT", "bShotOnce", std::to_string(global::triggerbot::config.bShotOnce));
			write_reg("TRIGGERBOT", "iKey", std::to_string(global::triggerbot::config.iKey));
			write_reg("TRIGGERBOT", "iDelay", std::to_string(global::triggerbot::config.iDelay));

			// misc
			write_reg("MISC", "bStandaloneRecoi", std::to_string(global::misc::config.bStandaloneRecoil));
			write_reg("MISC", "bBunnyhop", std::to_string(global::misc::config.bBunnyhop));
			write_reg("MISC", "bDebug", std::to_string(global::misc::config.bDebug));
			write_reg("MISC", "bRandomAgent", std::to_string(global::misc::config.bRandomAgent));
			write_reg("MISC", "bInstaLock", std::to_string(global::misc::config.bInstaLock));
			write_reg("MISC", "bDrawBoneIds", std::to_string(global::misc::config.bDrawBoneIds));
			write_reg("MISC", "bPingEsp", std::to_string(global::misc::config.bPingEsp));
			write_reg("MISC", "bPingEspWarn", std::to_string(global::misc::config.bPingEspWarn));
			write_reg("MISC", "bThirdPerson", std::to_string(global::misc::config.bThirdPerson));
			write_reg("MISC", "bNoRisk", std::to_string(global::misc::config.bNoRisk));
			write_reg("MISC", "bRadarHack", std::to_string(global::misc::config.bRadarHack));
			write_reg("MISC", "bSpikeEsp", std::to_string(global::misc::config.bSpikeEsp));
			write_reg("MISC", "bAntiAfk", std::to_string(global::misc::config.bAntiAfk));
			write_reg("MISC", "iInstaLockAgent", std::to_string(global::misc::config.iInstaLockAgent));
			write_reg("MISC", "iBoneId", std::to_string(global::misc::config.iBoneId));
			write_reg("MISC", "iPingEspDelay", std::to_string(global::misc::config.iPingEspDelay));
			write_reg("MISC", "iThirdPersonUpZ", std::to_string(global::misc::config.iThirdPersonUpZ));
			write_reg("MISC", "iFresne", std::to_string(global::misc::config.iFresnel));

			// esp
			write_reg("ESP", "bEnabled", std::to_string(global::esp::config.bEnabled));
			write_reg("ESP", "bSkeleton", std::to_string(global::esp::config.bSkeleton));
			write_reg("ESP", "bDrawSkeletonHead", std::to_string(global::esp::config.bDrawSkeletonHead));
			write_reg("ESP", "bHotkeyVisuals", std::to_string(global::esp::config.bHotkeyVisuals));
			write_reg("ESP", "bDrawNames", std::to_string(global::esp::config.bDrawNames));
			write_reg("ESP", "bDrawDormant", std::to_string(global::esp::config.bDrawDormant));
			write_reg("ESP", "bDrawHealth", std::to_string(global::esp::config.bDrawHealth));
			write_reg("ESP", "iEspType", std::to_string(global::esp::config.iEspType));
			write_reg("ESP", "iKey", std::to_string(global::esp::config.iKey));
			write_reg("ESP", "fSkeletonThickness", std::to_string(global::esp::config.fSkeletonThickness));
			write_reg("ESP", "bDrawRecoil", std::to_string(global::esp::config.bDrawRecoil));
			write_reg("ESP", "bWireFrameGun", std::to_string(global::esp::config.bWireFrameGun));
			write_reg("ESP", "bWireFrameHands", std::to_string(global::esp::config.bWireFrameHands));
			write_reg("ESP", "bCharacterWeaponIcon", std::to_string(global::esp::config.bCharacterWeaponIcon));
			write_reg("ESP", "bCharacterIcon", std::to_string(global::esp::config.bCharacterIcon));
			write_reg("ESP", "colSkelHeadR", std::to_string(global::esp::config.colSkelHeadR));
			write_reg("ESP", "colSkelHeadG", std::to_string(global::esp::config.colSkelHeadG));
			write_reg("ESP", "colSkelHeadB", std::to_string(global::esp::config.colSkelHeadB));

			// chams
			write_reg("CHAMS", "bEnabled", std::to_string(global::chams::config.bEnabled));
			write_reg("CHAMS", "bAlways", std::to_string(global::chams::config.bAlways));
			write_reg("CHAMS", "bHideTeamOutlines", std::to_string(global::chams::config.bHideTeamOutlines));
			write_reg("CHAMS", "bHotkeyChams", std::to_string(global::chams::config.bHotkeyChams));
			write_reg("CHAMS", "bRainbowChams", std::to_string(global::chams::config.bRainbowChams));
			write_reg("CHAMS", "bOwnColorMode", std::to_string(global::chams::config.bOwnColorMode));
			write_reg("CHAMS", "iKey", std::to_string(global::chams::config.iKey));
			write_reg("CHAMS", "iOutlineMode", std::to_string(global::chams::config.iOutlineMode));
			write_reg("CHAMS", "fR", std::to_string(global::chams::config.fR));
			write_reg("CHAMS", "fG", std::to_string(global::chams::config.fG));
			write_reg("CHAMS", "fB", std::to_string(global::chams::config.fB));
			write_reg("CHAMS", "fA", std::to_string(global::chams::config.fA));

			// fresnel
			write_reg("FRESNE", "bEnabled", std::to_string(global::fresnel::config.bEnabled));
			write_reg("FRESNE", "fIntensity", std::to_string(global::fresnel::config.fIntensity));

			// skin changer
			write_reg("SKINCHANGER", "bEnabled", std::to_string(global::skin_changer::config.bEnabled));
			write_reg("SKINCHANGER", "iGunBuddy", std::to_string(global::skin_changer::config.iGunBuddy));
		}

		void load_config() {
			// aimbot
			global::aimbot::config.bEnabled = atoi(read_reg("AIMBOT", "bEnabled"));
			global::aimbot::config.iHitbox = atoi(read_reg("AIMBOT", "iHitbox"));
			global::aimbot::config.iKey = atoi(read_reg("AIMBOT", "iKey"));
			global::aimbot::config.fSpeed = atof(read_reg("AIMBOT", "fSpeed"));
			global::aimbot::config.fRecoilStrength = atof(read_reg("AIMBOT", "fRecoilStrength"));
			global::aimbot::config.fMaxFov = atof(read_reg("AIMBOT", "fMaxFov"));
			global::aimbot::config.bNearestBone = atoi(read_reg("AIMBOT", "bNearestBone"));
			global::aimbot::config.bNearestVisibleLocation = atoi(read_reg("AIMBOT", "bNearestVisibleLocation"));
			global::aimbot::config.bConstant = atoi(read_reg("AIMBOT", "bConstant"));
			global::aimbot::config.bAlternatePistoleKey = atoi(read_reg("AIMBOT", "bAlternatePistoleKey"));
			global::aimbot::config.bAutoShot = atoi(read_reg("AIMBOT", "bAutoShot"));

			// triggerbot
			global::triggerbot::config.bEnabled = atoi(read_reg("TRIGGERBOT", "bEnabled"));
			global::triggerbot::config.bShotOnce = atoi(read_reg("TRIGGERBOT", "bShotOnce"));
			global::triggerbot::config.iKey = atoi(read_reg("TRIGGERBOT", "iKey"));
			global::triggerbot::config.iDelay = atoi(read_reg("TRIGGERBOT", "iDelay"));

			// misc
			global::misc::config.bStandaloneRecoil = atoi(read_reg("MISC", "bStandaloneRecoi"));
			global::misc::config.bBunnyhop = atoi(read_reg("MISC", "bBunnyhop"));
			global::misc::config.bDebug = atoi(read_reg("MISC", "bDebug"));
			global::misc::config.bRandomAgent = atoi(read_reg("MISC", "bRandomAgent"));
			global::misc::config.bInstaLock = atoi(read_reg("MISC", "bInstaLock"));
			global::misc::config.bDrawBoneIds = atoi(read_reg("MISC", "bDrawBoneIds"));
			global::misc::config.bPingEsp = atoi(read_reg("MISC", "bPingEsp"));
			global::misc::config.bPingEspWarn = atoi(read_reg("MISC", "bPingEspWarn"));
			global::misc::config.bThirdPerson = atoi(read_reg("MISC", "bThirdPerson"));
			global::misc::config.bNoRisk = atoi(read_reg("MISC", "bNoRisk"));
			global::misc::config.bRadarHack = atoi(read_reg("MISC", "bRadarHack"));
			global::misc::config.bSpikeEsp = atoi(read_reg("MISC", "bSpikeEsp"));
			global::misc::config.bAntiAfk = atoi(read_reg("MISC", "bAntiAfk"));
			global::misc::config.iInstaLockAgent = atoi(read_reg("MISC", "iInstaLockAgent"));
			global::misc::config.iBoneId = atoi(read_reg("MISC", "iBoneId"));
			global::misc::config.iPingEspDelay = atoi(read_reg("MISC", "iPingEspDelay"));
			global::misc::config.iThirdPersonUpZ = atoi(read_reg("MISC", "iThirdPersonUpZ"));
			global::misc::config.iFresnel = atoi(read_reg("MISC", "iFresne"));

			// esp
			global::esp::config.bEnabled = atoi(read_reg("ESP", "bEnabled"));
			global::esp::config.bSkeleton = atoi(read_reg("ESP", "bSkeleton"));
			global::esp::config.bDrawSkeletonHead = atoi(read_reg("ESP", "bDrawSkeletonHead"));
			global::esp::config.bHotkeyVisuals = atoi(read_reg("ESP", "bHotkeyVisuals"));
			global::esp::config.bDrawNames = atoi(read_reg("ESP", "bDrawNames"));
			global::esp::config.bDrawDormant = atoi(read_reg("ESP", "bDrawDormant"));
			global::esp::config.bDrawHealth = atoi(read_reg("ESP", "bDrawHealth"));
			global::esp::config.iEspType = atoi(read_reg("ESP", "iEspType"));
			global::esp::config.iKey = atoi(read_reg("ESP", "iKey"));
			global::esp::config.fSkeletonThickness = atof(read_reg("ESP", "fSkeletonThickness"));
			global::esp::config.bDrawRecoil = atof(read_reg("ESP", "bDrawRecoil"));
			global::esp::config.bWireFrameGun = atof(read_reg("ESP", "bWireFrameGun"));
			global::esp::config.bWireFrameHands = atof(read_reg("ESP", "bWireFrameHands"));
			global::esp::config.bCharacterIcon = atof(read_reg("ESP", "bCharacterIcon"));
			global::esp::config.bCharacterWeaponIcon = atof(read_reg("ESP", "bCharacterWeaponIcon"));

			global::esp::config.colSkelHeadR = atof(read_reg("ESP", "colSkelHeadR"));
			global::esp::config.colSkelHeadG = atof(read_reg("ESP", "colSkelHeadG"));
			global::esp::config.colSkelHeadB = atof(read_reg("ESP", "colSkelHeadB"));

			// chams
			global::chams::config.bEnabled = atoi(read_reg("CHAMS", "bEnabled"));
			global::chams::config.bAlways = atoi(read_reg("CHAMS", "bAlways"));
			global::chams::config.bHideTeamOutlines = atoi(read_reg("CHAMS", "bHideTeamOutlines"));
			global::chams::config.bHotkeyChams = atoi(read_reg("CHAMS", "bHotkeyChams"));
			global::chams::config.bRainbowChams = atoi(read_reg("CHAMS", "bRainbowChams"));
			global::chams::config.bOwnColorMode = atoi(read_reg("CHAMS", "bOwnColorMode"));
			global::chams::config.iKey = atoi(read_reg("CHAMS", "iKey"));
			global::chams::config.iOutlineMode = atoi(read_reg("CHAMS", "iOutlineMode"));
			global::chams::config.fR = atof(read_reg("CHAMS", "fR"));
			global::chams::config.fG = atof(read_reg("CHAMS", "fG"));
			global::chams::config.fB = atof(read_reg("CHAMS", "fB"));
			global::chams::config.fA = atof(read_reg("CHAMS", "fA"));

			// fresnel
			global::fresnel::config.bEnabled = atoi(read_reg("FRESNE", "bEnabled"));
			global::fresnel::config.fIntensity = atof(read_reg("FRESNE", "fIntensity"));

			// skin changer
			global::skin_changer::config.bEnabled = atoi(read_reg("SKINCHANGER", "bEnabled"));
			global::skin_changer::config.iGunBuddy = atoi(read_reg("SKINCHANGER", "iGunBuddy"));

		}
	}
}
