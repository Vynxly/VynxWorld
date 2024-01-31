#pragma once
#include <Windows.h>
#include "libs/utils/memory.h"
#include <map>
#include "SDK.hpp"
#include "database.h"
#include <iostream>
#include <fstream>

typedef bool(*Tick)(SDK::APalPlayerCharacter* m_this, float DeltaSecond);
typedef void(*CatchRate)(SDK::APalCaptureJudgeObject* m_this);

static bool IsGameActive()
{
	HWND foregroundWindow = GetForegroundWindow();
	std::wstring windowName = L"Pal";

	if (foregroundWindow) {
		int length = GetWindowTextLength(foregroundWindow);
		std::wstring windowTitle(length, L'\0');
		GetWindowText(foregroundWindow, &windowTitle[0], length + 1);

		windowTitle.erase(std::find_if(windowTitle.rbegin(), windowTitle.rend(), [](wchar_t ch) {
			return !std::isspace(ch);
			}).base(), windowTitle.end());

		if (windowName == windowTitle) {
			return true;
		}
	}
	return false;
}


class config
{
public:
	DWORD64 ClientBase = 0;
	DWORD64 offset_Tick = 0x2AB1DC0;//APalPlayerCharacter::Tick
	DWORD64 offset_CatchRate = 0x26AE100; //APalCaptureJudgeObject::ChallengeCapture

	static void SaveLocation(const std::string& bossName, float x, float y, float z) {
		std::ofstream outFile("Location.txt", std::ios::app);

		if (!outFile) {
			std::cerr << "Failed to open Location.txt for writing." << std::endl;
			return;
		}

		outFile << "Boss: " << bossName << " | X: " << x << " - Y: " << y << " - Z: " << z << std::endl;

		outFile.close();
	}

	//�˵��ж�
	bool IsESP = false;
	bool IsAimbot = false;
	bool IsSpeedHack = false;
	bool IsAttackModiler = false;
	bool IsDefuseModiler = false;
	bool IsInfStamina = false;
	bool IsSafe = true;
	bool IsInfinAmmo = false;
	bool IsMuteki = false;
	bool IsAdmin = true;
	bool IsRevive = false;
	bool IsFullbright = false;
	bool IsToggledFly = false;
	bool isCatchRate = false;
	bool isDebugESP = false;
	bool bisOpenManager = false;
	bool bisRandomName = false;
	bool filterCharacters = true;
	bool filterPlayer = false;
	bool filterGuilds = false;
	bool filterMapParts = false;

	std::vector<std::string> ft_node_names = { "6E03F8464BAD9E458B843AA30BE1CC8F", "DDBBFFAF43D9219AE68DF98744DF0831", "603ED0CD4CFB9AFDC9E11F805594CCE5", "6282FE1E4029EDCDB14135AA4C171E4C", "9FBB93D84811BE424A37C391DBFBB476", "979BF2044C8E8FE559B598A95A83EDE3", "923B781345D2AB7ECED6BABD6E97ECE8", "16C7164C43E2B96BEDCC6696E2E592F7", "D27AFCAD472164F000D33A8D8B61FE8B", "41727100495D21DC905D309C53989914", "6DB6B7B2431CA2EFFFC88EB45805BA6A", "74270C2F45B8DCA66B6A1FAAA911D024", "DF9FB9CB41B43052A9C74FA79A826A50", "8CA5E9774FF1BBC1156ABCA09E616480", "15314BE94E2FB8D018D5749BE9A318F0", "79C561B747335A7A0A8FBF9FAE280E62", "23B9E99C4A454B99220AF7B4A58FD8DE", "A1BC65AA445619EF338E0388BC010648", "BF8B123244ADB794A06EA8A10503FBDD", "F8DF603B4C750B37D943C9AF6A911946", "596996B948716D3FD2283C8B5C6E829C", "28D514E74B51FD9EB078A891DB0787C2", "ACAE5FB04D48DE4197443E9C0993086B", "4D2F204549AB656CA1EA4B8E39C484F3", "1BDEABA240B1699541C17F83E59E61DF", "2BC5E46049E69D3549CFB58948BE3288", "91DAC6F34D2A9FD7F01471B5166C6C02", "41E36D9A4B2BA79A3AD1B7B83B16F77D", "76B000914943BADDC56BCFBAE2BF051E", "DC0ECF9241B4410C59EE619F56D1106A", "71C4B2B2407F2BBBD77572A20C7FF0F5", "EC94023A4CA571FF0FD19E90944F4231", "2A2B744B41AC79964DAE6B89CAC51FC3", "E0819EFB41581AEAC3A029B0EE2FE195", "22095BFA48A46172F8D154B2EBEB7493", "7C5E91514F6E84B0C1DEFFB52C7C4DBA", "AECFED0D41AFEE11F30B4F9687BC3243", "2EC07ACC4505CB726DE38A84246CA999", "F8E5CB8143F4FA2F6213E6B454569F87", "5F426B49469368B0C131D3A6DB8F7831", "A277AE6341EF40D84D711EA52303353F", "6231802D40C81C00445379AE238D9F90", "F6C005A14B38FE0B57F1C7869FD899CB", "7170881D44249E90902F728E240493AF", "3E8E504B4A3975FD3862E1BC85A5D4F6", "B001852C491FF5E70C4747BFF9972924", "2DE1702048A1D4A82126168C49BE51A9", "E88379634CB5B6117DA2E7B8810BFE0A", "3697999C458BF8A3C7973382969FBDF9", "65B10BB14ABDA9C2109167B21901D195", "4669582D4081BF550AFB66A05D043A3D", "FE90632845114C7FBFA4669D071E285F", "5970E8E648D2A83AFDFF7C9151D9BEF5", "B639B7ED4EE18A7AA09BA189EA703032", "099440764403D1508D9BADADF4848697", "B44AA24445864494E7569597ACCAEFC6", "3A0F123947BE045BC415C6B061A5285A", "F382ADAE4259150BF994FF873ECF242B" };
	char BossName[255];
	float mDebugESPDistance = 5.0f;

	//����
	float SpeedModiflers = 1.0f;
	float CatchRate = 1;
	int DamageUp = 0;
	int DefuseUp = 0;
	int EXP = 0;
	int Item = 1;
	int PalRank = 0;
	int MaxWeight = 9999999;
	int Tech = 0;
	float Pos[3] = { 0,0,0 };
	char ItemName[255];
	char PalName[255];
	char CharName[255];
	static char inputBuffer[100];
	char inputTextBuffer[255] = "";
	SDK::UWorld* gWorld = nullptr;
	int PalLvL = 1;
	SDK::APalPlayerCharacter* localPlayer = NULL;
	SDK::TArray<SDK::APalPlayerCharacter*> AllPlayers = {};
	SDK::UPalCharacterImportanceManager* UCIM = NULL;
	SDK::UObject* WorldContextObject = NULL;
	std::vector<std::string> db_filteredItems;
	
	//����
	static SDK::UWorld* GetUWorld();
	static SDK::UPalCharacterImportanceManager* GetCharacterImpManager();
	static SDK::APalPlayerCharacter* GetPalPlayerCharacter();
	static SDK::APalPlayerState* GetPalPlayerState();
	static SDK::TArray<SDK::APalPlayerCharacter*> GetTAllPlayers();
	static SDK::UPalPlayerInventoryData* GetInventoryComponent();
	static SDK::APalWeaponBase* GetPlayerEquippedWeapon();
	static SDK::ULocalPlayer* GetLocalPlayer();
	static bool	GetTAllPlayers(SDK::TArray<class SDK::APalCharacter*>* outResult);
	static bool	GetTAllImpNPC(SDK::TArray<class SDK::APalCharacter*>* outResult);
	static bool	GetTAllNPC(SDK::TArray<class SDK::APalCharacter*>* outResult);
	static bool	GetTAllPals(SDK::TArray<class SDK::APalCharacter*>* outResult);
	static bool GetAllActorsofType(SDK::UClass* mType, std::vector<SDK::AActor*>* outArray, bool bLoopAllLevels = false, bool bSkipLocalPlayer = false);
	static void Init();;
	static void Update(const char* filterText);
	static const std::vector<std::string>& GetFilteredItems();

	// Teleport Spots
	float AnubisLocation[3] = { -170269, 94163.7, -1527.83 };
	float AzurobeLocation[3] = { -300596, 136090, -2033.13 };
	float BeakonLocation[3] = { -237981, 2559.41, 8413.65 };
	float BroncherryAquaLocation[3] = { -330059.98, 81386.99, -1929.8 };
	float BroncherryLocation[3] = { -430777.18, 56266.50, -1774.57 };
	float BushiLocation[3] = { -349079.05, 104564.46, -121.11 };
	float ChilletLocation[3] = { -315195, 236714, 2452.12 };
	float DinossomLuxLocation[3] = { 125311, 318722, 266.384 };
	float ElizabeeLocation[3] = { -197627, 167101, -1964.75 };
	float FelbatLocation[3] = { -149024, -30562.7, -1291.73 };
	float FenglopeLocation[3] = { -332470, 40256.4, 1333.93 };
	float FrostallionLocation[3] = { 111978, -6394.85, 21940.2 };
	float GrintaleLocation[3] = { -234807, 321166, -1826.19 };
	float GumossLocation[3] = { -409885.46, 108120.6, -1949.58 };
	float JetragonLocation[3] = { -270762, -205837, 3834.92 };
	float Jormuntide2Location[3] = { -163184, 314483, -2100.13 };
	float JormuntideLocation[3] = { -244207, 74851.5, -1157.08 };
	float KatressLocation[3] = { -276223, 268992, -1209.67 };
	float KingpacaLocation[3] = { -336105, 181639, 901.022 };
	float LunarisLocation[3] = { -426589.48, 90677.8, 215.90 };
	float MammorestLocation[3] = { -341702, 243628, -533.76 };
	float MenastingLocation[3] = { -77943, 393182, 665.51 };
	float MossandaLuxLocation[3] = { -208233, 364318, 1167.46 };
	float NitewingLocation[3] = { -157475, 30628.2, -1894.64 };
	float PaladiusLocation[3] = { 187235, 349924, 2563.07 };
	float PenkingLocation[3] = { -285343.94, 210129.58, 854.23 };
	float PetalliaLocation[3] = { -245440, 149098, 913.216 };
	float QuivernLocation[3] = { -183795, 40661.4, 8547.3 };
	float RelaxasaurusLuxLocation[3] = { -282860, 64923.7, 5357.72 };
	float SiblexLocation[3] = { -92023.6, 273762, 18967.3 };
	float SuzakuLocation[3] = { -4320.07, 340927, 205.005 };
	float UnivoltLocation[3] = { -372419.57, 102508.29, -392 };
	float VaeletLocation[3] = { -147042, 217357, 2857.45 };
	float VerdashLocation[3] = { -119349, 288954, 3186.98 };
	float WarsectLocation[3] = { -226359, 231970, -1807.45 };
	float WumpoBotanLocation[3] = { -148826, 364859, -1831.33 };

};
extern config Config;

