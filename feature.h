#pragma once
#include "include/Menu.hpp"
#include "config.h"
#include <Windows.h>

void ESP();

void SetFullbright(bool bIsSet);

void ForceJoinGuild(SDK::APalCharacter* targetPlayer);

void RespawnLocalPlayer(bool bIsSafe);

void AddWaypointLocation(std::string wpName);

void AddItemToInventoryByName(SDK::UPalPlayerInventoryData* data, char* itemName, int count);

void GiveTechExploit(__int32 Tech);

void RenderWaypointsToScreen();

void ESP_DEBUG(float mDist, ImVec4 color, SDK::UClass* mEntType = SDK::AActor::StaticClass());

void DrawUActorComponent(SDK::TArray<SDK::UActorComponent*> Comps, ImColor color);

inline SDK::FVector2D pokemonCOORD;

