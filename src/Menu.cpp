#include "../pch.h"
#include "../include/Menu.hpp"
#include "SDK.hpp"
#include "config.h"
#include <algorithm>

std::string rand_str(const int len)
{
    std::string str;
    char c;
    int idx;
    for (idx = 0; idx < len; idx++)
    {
        c = 'a' + rand() % 26;
        str.push_back(c);
    }
    return str;
}
void Damage(SDK::APalCharacter* character, int32 damage)
{
    SDK::FPalDamageInfo  info = SDK::FPalDamageInfo();
    info.AttackElementType = SDK::EPalElementType::Normal;
    info.Attacker = Config.GetPalPlayerCharacter();
    info.AttackerGroupID = Config.GetPalPlayerState()->IndividualHandleId.PlayerUId;
    info.AttackerLevel = 50;
    info.AttackType = SDK::EPalAttackType::Weapon;
    info.bApplyNativeDamageValue = true;
    info.bAttackableToFriend = true;
    info.IgnoreShield = true;
    info.NativeDamageValue = damage;
    Config.GetPalPlayerState()->SendDamage_ToServer(character, info);
}

int InputTextCallback(ImGuiInputTextCallbackData* data) {
    char inputChar = data->EventChar;
    Config.Update(Config.inputTextBuffer);

    return 0;
}

SDK::FPalDebugOtomoPalInfo palinfo = SDK::FPalDebugOtomoPalInfo();
SDK::TArray<SDK::EPalWazaID> EA = { 0U };

CatchRate CRate;
CatchRate OldRate;

void DetourCatchRate(SDK::APalCaptureJudgeObject* p_this) {
    if (p_this) {
        //p_this->ChallengeCapture_ToServer(Config.localPlayer, Config.CatchRate);
        p_this->ChallengeCapture(Config.localPlayer, Config.CatchRate);
    }
}


void ToggleCatchRate(bool catchrate) {
    if (catchrate) {
        if (CRate == NULL) {
            CRate = (CatchRate)(Config.ClientBase + Config.offset_CatchRate);
            MH_CreateHook(CRate, DetourCatchRate, reinterpret_cast<void**>(OldRate));
            MH_EnableHook(CRate);
            return;
        }
        MH_EnableHook(CRate);
        return;
    }
    else
    {
        MH_DisableHook(CRate);

    }
}


void AddItem(SDK::UPalPlayerInventoryData* data, char* itemName, int count)
{
    SDK::UKismetStringLibrary* lib = SDK::UKismetStringLibrary::GetDefaultObj();

    //Convert FNAME
    wchar_t  ws[255];
    swprintf(ws, 255, L"%hs", itemName);
    SDK::FName Name = lib->Conv_StringToName(SDK::FString(ws));
    //Call
    data->RequestAddItem(Name, count, true);
}
void SpawnPal(char* PalName, int rank, int lvl = 1)
{
    SDK::UKismetStringLibrary* lib = SDK::UKismetStringLibrary::GetDefaultObj();

    //Convert FNAME
    wchar_t  ws[255];
    swprintf(ws, 255, L"%hs", PalName);
    SDK::FName Name = lib->Conv_StringToName(SDK::FString(ws));
    //Call
    if (Config.GetPalPlayerCharacter() != NULL)
    {
        if (Config.GetPalPlayerCharacter()->GetPalPlayerController() != NULL)
        {
            if (Config.GetPalPlayerCharacter()->GetPalPlayerController())
            {
                if (Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState())
                {
                    EA[0] = SDK::EPalWazaID::AirCanon;
                    palinfo.Level = lvl;
                    palinfo.Rank = rank;
                    palinfo.PalName.Key = Name;
                    palinfo.WazaList = EA;
                    palinfo.PassiveSkill = NULL;
                    Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState()->Debug_CaptureNewMonsterByDebugOtomoInfo_ToServer(palinfo);
                }
            }
        }
    }
}

void AnyWhereTP(SDK::FVector& vector, bool IsSafe)
{
    if (!IsSafe)
    {
        if (Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState() != NULL)
        {

            SDK::FGuid guid = Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPlayerUId();
            vector = { vector.X,vector.Y + 100,vector.Z };
            Config.GetPalPlayerCharacter()->GetPalPlayerController()->Transmitter->Player->RegisterRespawnLocation_ToServer(guid, vector);
            Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState()->RequestRespawn();
        }
    }
    else
    {
        if (Config.GetPalPlayerCharacter()->GetPalPlayerController())
        {
            vector = { vector.X,vector.Y + 100,vector.Z };
            Config.GetPalPlayerCharacter()->GetPalPlayerController()->Debug_Teleport2D(vector);
        }
    }
    return;
}

void ExploitFly(bool IsFly)
{
    SDK::APalPlayerCharacter* p_appc = Config.GetPalPlayerCharacter();
    if (p_appc != NULL)
    {
        if (IsFly)
        {
            if (Config.GetPalPlayerCharacter()->GetPalPlayerController() != NULL)
            {
                Config.GetPalPlayerCharacter()->GetPalPlayerController()->StartFlyToServer();
            }
        }
        else
        {
            if (Config.GetPalPlayerCharacter()->GetPalPlayerController() != NULL)
            {
                Config.GetPalPlayerCharacter()->GetPalPlayerController()->EndFlyToServer();
            }
        }

    }
}

namespace DX11_Base {

    namespace Styles {
        void InitStyle()
        {
            ImGuiStyle* style = &ImGui::GetStyle();
            ImVec4* colors = ImGui::GetStyle().Colors;

            //	STYLE PROPERTIES
            //Borders
            ImGui::GetStyle().WindowBorderSize = 0.0f;
            ImGui::GetStyle().FrameBorderSize = 0.0f;
            ImGui::GetStyle().PopupBorderSize = 0.0f;

            //Rounding
            ImGui::GetStyle().WindowRounding = 0.0f;
            ImGui::GetStyle().ChildRounding = 0.0f;
            ImGui::GetStyle().FrameRounding = 0.0f;
            ImGui::GetStyle().PopupRounding = 0.0f;
            ImGui::GetStyle().ScrollbarRounding = 0.0f;
            ImGui::GetStyle().GrabRounding = 0.0f;
            ImGui::GetStyle().LogSliderDeadzone = 0.0f;
            ImGui::GetStyle().TabRounding = 0.0f;

            //  Base ImGui Styling , Aplying a custyom style is left up to you.
            //ImGui::StyleColorsDark();

            /// EXAMPLE COLOR 
            //colors[ImGuiCol_FrameBg] = ImVec4(0, 0, 0, 0);

            //	COLORS
            if (g_Menu->dbg_RAINBOW_THEME) {
                //  RGB MODE STLYE PROPERTIES
                colors[ImGuiCol_Separator] = ImVec4(g_Menu->dbg_RAINBOW);
                colors[ImGuiCol_TitleBg] = ImVec4(0, 0, 0, 1.0f);
                colors[ImGuiCol_TitleBgActive] = ImVec4(0, 0, 0, 1.0f);
                colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0, 0, 0, 1.0f);
            }
            else {
                /// YOUR DEFAULT STYLE PROPERTIES HERE
                colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
                colors[ImGuiCol_TextDisabled] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
                colors[ImGuiCol_WindowBg] = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);
                colors[ImGuiCol_ChildBg] = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);
                colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);
                colors[ImGuiCol_Border] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
                colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
                colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
                colors[ImGuiCol_FrameBgHovered] = ImVec4(0.05f, 0.52f, 0.13f, 1.00f);
                colors[ImGuiCol_FrameBgActive] = ImVec4(0.05f, 0.52f, 0.13f, 1.00f);
                colors[ImGuiCol_TitleBg] = ImVec4(0.459f, 0.188f, 0.651f, 1.0f);
                colors[ImGuiCol_TitleBgActive] = ImVec4(0.459f, 0.188f, 0.651f, 1.0f);
                colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
                colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
                colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
                colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
                colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
                colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
                colors[ImGuiCol_CheckMark] = ImVec4(0.459f, 0.188f, 0.651f, 1.0f);
                colors[ImGuiCol_SliderGrab] = ImVec4(0.459f, 0.188f, 0.651f, 1.0f);
                colors[ImGuiCol_SliderGrabActive] = ImVec4(0.459f, 0.188f, 0.651f, 1.0f);
                colors[ImGuiCol_Button] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
                colors[ImGuiCol_ButtonHovered] = ImVec4(0.05f, 0.52f, 0.13f, 1.00f);
                colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
                colors[ImGuiCol_Header] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
                colors[ImGuiCol_HeaderHovered] = ImVec4(0.05f, 0.52f, 0.13f, 1.00f);
                colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
                colors[ImGuiCol_Separator] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
                colors[ImGuiCol_SeparatorHovered] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
                colors[ImGuiCol_SeparatorActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
                colors[ImGuiCol_ResizeGrip] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
                colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.05f, 0.52f, 0.13f, 1.00f);
                colors[ImGuiCol_ResizeGripActive] = ImVec4(0.459f, 0.188f, 0.651f, 1.0f);
                colors[ImGuiCol_Tab] = ImVec4(0.459f, 0.188f, 0.651f, 1.0f);
                colors[ImGuiCol_TabHovered] = ImVec4(0.05f, 0.52f, 0.13f, 1.00f);
                colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
                colors[ImGuiCol_TabUnfocused] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
                colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
                colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
                colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.05f, 0.52f, 0.13f, 1.00f);
                colors[ImGuiCol_PlotHistogram] = ImVec4(0.459f, 0.188f, 0.651f, 1.0f);
                colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.05f, 0.52f, 0.13f, 1.00f);
                colors[ImGuiCol_TableHeaderBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
                colors[ImGuiCol_TableBorderStrong] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
                colors[ImGuiCol_TableBorderLight] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
                colors[ImGuiCol_TableRowBg] = ImVec4(0.07f, 0.07f, 0.07f, 1.00f);
                colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
                colors[ImGuiCol_TextSelectedBg] = ImVec4(0.05f, 0.52f, 0.13f, 1.00f);
                colors[ImGuiCol_DragDropTarget] = ImVec4(0.459f, 0.188f, 0.651f, 1.0f);
                colors[ImGuiCol_NavHighlight] = ImVec4(0.05f, 0.52f, 0.13f, 1.00f);
                colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.05f, 0.52f, 0.13f, 1.00f);
                colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.05f, 0.52f, 0.13f, 1.00f);
                colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.05f, 0.52f, 0.13f, 1.00f);
            }
        }
    }

    namespace Tabs {
        void TABPlayer()
        {

            //�л�����һ��
            //ImGui::Checkbox("ESP", &Config.IsESP);

            //ImGui::Checkbox("Debug ESP", &Config.isDebugESP);

            ImGui::Checkbox("Godmode", &Config.IsMuteki);

            ImGui::Checkbox("Speed Hack", &Config.IsSpeedHack);

            ImGui::Checkbox("Attack Hack", &Config.IsAttackModiler);

            ImGui::Checkbox("Defense Hack", &Config.IsDefuseModiler);

            ImGui::Checkbox("Infinite Stamina", &Config.IsInfStamina);

            ImGui::Checkbox("Infinite Ammo", &Config.IsInfinAmmo);

            //ImGui::Checkbox("Revive", &Config.IsRevive);

            ImGui::InputText("Character Name", Config.CharName, sizeof(Config.CharName));
            if (ImGui::Button("Change Name", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
            {
                if (Config.GetPalPlayerCharacter() != NULL)
                {
                    if (Config.GetPalPlayerCharacter()->GetPalPlayerController() != NULL)
                    {
                        SDK::UKismetStringLibrary* lib = SDK::UKismetStringLibrary::GetDefaultObj();

                        wchar_t  ws[255];
                        swprintf(ws, 255, L"%hs", Config.CharName);
                        Config.GetPalPlayerCharacter()->GetPalPlayerController()->Transmitter->NetworkIndividualComponent->UpdateCharacterNickName_ToServer(Config.GetPalPlayerCharacter()->CharacterParameterComponent->IndividualHandle->ID, SDK::FString(ws));
                    }
                }
            }
            ImGui::Checkbox("Open Manager (Laggy)", &Config.bisOpenManager);

            //��������һ��
            ImGui::SliderFloat("Speed Multiplier", &Config.SpeedModiflers, 1, 10);
            ImGui::SliderInt("Attack Multiplier", &Config.DamageUp, 0, 200000);
            ImGui::SliderInt("Defense Multiplier", &Config.DefuseUp, 0, 200000);

            //��ť����һ��
            /*if (ImGui::Button("PrintPlayerAddr", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
            {
                SDK::APalPlayerCharacter* p_appc = Config.GetPalPlayerCharacter();
                if (p_appc != NULL)
                {
                    g_Console->printdbg("\n\n[+] APalPlayerCharacter: %x [+]\n\n", g_Console->color.green, p_appc);
                }
            }*/

            if (ImGui::Button("Print Coords", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
            {
                SDK::APalPlayerCharacter* p_appc = Config.GetPalPlayerCharacter();
                if (p_appc != NULL)
                {
                    if (Config.GetPalPlayerCharacter()->GetPalPlayerController() != NULL)
                    {
                        SDK::FVector PlayerLocation = p_appc->K2_GetActorLocation();
                        std::string MyLocation = std::format("\nX: {} | Y: {} | Z: {}\n", PlayerLocation.X, PlayerLocation.Y, PlayerLocation.Z);
                        g_Console->printdbg(MyLocation.c_str(), g_Console->color.green, p_appc);
                    }
                }
            }

            SDK::APalPlayerCharacter* p_appc = Config.GetPalPlayerCharacter();
            if (p_appc != NULL)
            {
                if (Config.GetPalPlayerCharacter()->GetPalPlayerController() != NULL)
                {
                    SDK::FVector PlayerLocation = p_appc->K2_GetActorLocation();
                    std::string MyLocation = std::format("X: {} | Y: {} | Z: {}", PlayerLocation.X, PlayerLocation.Y, PlayerLocation.Z);
                    ImGui::Text(MyLocation.c_str());
                }
            }
        }
        void TABExploit()
        {
            //�����õİ�
            //Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState()->RequestSpawnMonsterForPlayer(name, 5, 1);
            ImGui::Checkbox("Safe Teleports", &Config.IsSafe);
            ImGui::InputFloat3("Pos:", Config.Pos);

            if (ImGui::Button("Coord TP", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
            {
                if (Config.GetPalPlayerCharacter() != NULL)
                {
                    if (Config.GetPalPlayerCharacter()->GetPalPlayerController() != NULL)
                    {
                        if (Config.Pos != NULL)
                        {
                            SDK::FVector vector = { Config.Pos[0],Config.Pos[1],Config.Pos[2] };
                            AnyWhereTP(vector, Config.IsSafe);
                        }
                    }
                }
            }

            if (ImGui::Button("Base Teleport (F5)", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
            {
                SDK::APalPlayerCharacter* p_appc = Config.GetPalPlayerCharacter();
                if (p_appc != NULL)
                {
                    if (Config.GetPalPlayerCharacter()->GetPalPlayerController() != NULL)
                    {
                        if (Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState() != NULL)
                        {
                            if (Config.IsSafe)
                            {
                                Config.GetPalPlayerCharacter()->GetPalPlayerController()->TeleportToSafePoint_ToServer();
                            }
                            else
                            {
                                Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState()->RequestRespawn();
                            }

                        }
                    }
                }

            }

            ImGui::InputInt("EXP:", &Config.EXP);
            if (ImGui::Button("Give EXP", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
            {
                SDK::APalPlayerCharacter* p_appc = Config.GetPalPlayerCharacter();
                if (p_appc != NULL)
                {
                    if (Config.GetPalPlayerCharacter()->GetPalPlayerController() != NULL)
                    {
                        if (Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState() != NULL)
                        {
                            if (Config.EXP >= 0)
                            {
                                Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState()->GrantExpForParty(Config.EXP);
                            }
                        }
                    }
                }
            }

            ImGui::InputInt("Tech:", &Config.Tech);
            if (ImGui::Button("Give Tech", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
                GiveTechExploit(Config.Tech);

            /*if (ImGui::BeginCombo("Pal Name", combo_preview_Pal, flags))
            {
                for (int n = 0; n < IM_ARRAYSIZE(PalNames); n++)
                {
                    const bool is_selected = (palSelecteditem == n);
                    if (ImGui::Selectable(PalNames[n], is_selected))
                        palSelecteditem = n;

                    // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            //ImGui::InputText("PalName", Config.PalName, sizeof(Config.PalName));
            ImGui::InputInt("PalRank", &Config.PalRank);
            ImGui::InputInt("Pallvl", &Config.PalLvL);
            if (ImGui::Button("Spawn Pal", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
            {
                if (Config.GetPalPlayerCharacter() != NULL)
                {
                    if (Config.GetPalPlayerCharacter()->GetPalPlayerController() != NULL)
                    {
                        if (Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState() != NULL)
                        {
                            if (Config.PalName != NULL)
                            {
                                g_Console->printdbg("\n\n[+] PalName: %s [+]\n\n", g_Console->color.green, Config.ItemName);
                                SpawnPal((char*)combo_preview_Pal, Config.PalRank, Config.PalLvL);
                            }
                        }
                    }
                }
            }*/

            if (ImGui::Button("Toggle Float", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
            {
                Config.IsToggledFly = !Config.IsToggledFly;
                ExploitFly(Config.IsToggledFly);
            }

            if (ImGui::Button("God Health", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
            {
                SDK::APalPlayerCharacter* p_appc = Config.GetPalPlayerCharacter();
                if (p_appc != NULL)
                {
                    if (Config.GetPalPlayerCharacter()->GetPalPlayerController() != NULL)
                    {
                        if (Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState() != NULL)
                        {
                            SDK::FFixedPoint fixpoint = SDK::FFixedPoint();
                            fixpoint.Value = 99999999;
                            Config.GetPalPlayerCharacter()->ReviveCharacter_ToServer(fixpoint);
                        }
                    }
                }
            }

            if (ImGui::Button("Normal Health", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
            {
                SDK::APalPlayerCharacter* p_appc = Config.GetPalPlayerCharacter();
                if (p_appc != NULL)
                {
                    if (Config.GetPalPlayerCharacter()->GetPalPlayerController() != NULL)
                    {
                        if (Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState() != NULL)
                        {
                            SDK::FFixedPoint fixpoint = SDK::FFixedPoint();
                            fixpoint.Value = Config.GetPalPlayerCharacter()->CharacterParameterComponent->GetMaxHP().Value;
                            Config.GetPalPlayerCharacter()->ReviveCharacter_ToServer(fixpoint);
                        }
                    }
                }
            }

            //if (ImGui::Button("Max Weight", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
            //{
            //    SDK::APalPlayerCharacter* p_appc = Config.GetPalPlayerCharacter();
            //    if (p_appc != NULL)
            //    {
            //        if (Config.GetPalPlayerCharacter()->GetPalPlayerController() != NULL)
            //        {
            //            if (Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState() != NULL)
            //            {
            //                Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState()->InventoryData->MaxInventoryWeight = Config.MaxWeight;
            //            }
            //        }
            //    }
            //}

            //if (ImGui::Button("Catch Rate", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
            //{
            //    Config.isCatchRate = !Config.isCatchRate;
            //    ToggleCatchRate(Config.isCatchRate);
            //}
            //ImGui::InputFloat("Catch Rate Modifier", &Config.CatchRate);
        }

        void TABItemSpawner()
        {
            static int num_to_add = 1;
            static int category = 10;

            ImGui::InputInt("Amount:", &num_to_add);

            ImGui::Combo("Item Category", &category, "Accessories\0Ammo\0Armor\0Crafting Materials\0Eggs\0Food\0Hats\0\Medicine\0Money\0Other\0Pal Spheres\0Seeds\0Tools\0Weapons\0\All\0");

            std::initializer_list list = database::all;

            switch (category)
            {
            case 1:
                list = database::ammo;
                break;
            case 2:
                list = database::armor;
                break;
            case 3:
                list = database::craftingmaterials;
                break;
            case 4:
                list = database::eggs;
                break;
            case 5:
                list = database::food;
                break;
            case 6:
                list = database::hats;
                break;
            case 7:
                list = database::medicine;
                break;
            case 8:
                list = database::money;
                break;
            case 9:
                list = database::other;
                break;
            case 10:
                list = database::palspheres;
                break;
            case 11:
                list = database::seeds;
                break;
            case 12:
                list = database::toolss;
                break;
            case 13:
                list = database::weapons;
                break;
            case 14:
                list = database::all;
                break;
            default:
                list = database::all;
            }

            int cur_size = 0;

            static char item_search[100];;

            ImGui::InputText("Search", item_search, IM_ARRAYSIZE(item_search));
            for (const auto& item : list) {
                std::istringstream ss(item);
                std::string left_text, right_text;

                std::getline(ss, left_text, '|');
                std::getline(ss, right_text);

                auto right_to_lower = right_text;
                std::string item_search_to_lower = item_search;

                std::transform(right_to_lower.begin(), right_to_lower.end(), right_to_lower.begin(), ::tolower);
                std::transform(item_search_to_lower.begin(), item_search_to_lower.end(), item_search_to_lower.begin(), ::tolower);

                if (item_search[0] != '\0' && (right_to_lower.find(item_search_to_lower) == std::string::npos))
                    continue;

                if (cur_size != 0 && cur_size < 20)
                {
                    ImGui::SameLine();
                }
                else if (cur_size != 0)
                {
                    cur_size = 0;
                }

                cur_size += right_text.length();

                ImGui::PushID(item);
                if (ImGui::Button(right_text.c_str()))
                {
                    SDK::UPalPlayerInventoryData* InventoryData = Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState()->GetInventoryData();//rebas
                    AddItem(InventoryData, (char*)left_text.c_str(), num_to_add);
                }
                ImGui::PopID();
            }
        }
        void TABQuickTP() // Credits to NuLLxD for compressing/making it look nicer and saving like 1000 lines of junk.
        {
            class TeleportTabs {
            public:
                TeleportTabs() : currentPage(0), buttonsPerPage(9) {
                    // Add your teleport locations here
                    locationMap["Anubis"] = Config.AnubisLocation;
                    locationMap["Azurobe"] = Config.AzurobeLocation;
                    locationMap["Beakon"] = Config.BeakonLocation;
                    locationMap["Broncherry Aqua"] = Config.BroncherryAquaLocation;
                    locationMap["Broncherry"] = Config.BroncherryLocation;
                    locationMap["Bushi"] = Config.BushiLocation;
                    locationMap["Chillet"] = Config.ChilletLocation;
                    locationMap["Dinossom Lux"] = Config.DinossomLuxLocation;
                    locationMap["Elizabee"] = Config.ElizabeeLocation;
                    locationMap["Felbat"] = Config.FelbatLocation;
                    locationMap["Fenglope"] = Config.FenglopeLocation;
                    locationMap["Frostallion"] = Config.FrostallionLocation;
                    locationMap["Grintale"] = Config.GrintaleLocation;
                    locationMap["Gumoss"] = Config.GumossLocation;
                    locationMap["Jetragon"] = Config.JetragonLocation;
                    locationMap["Jormuntide2"] = Config.Jormuntide2Location;
                    locationMap["Jormuntide"] = Config.JormuntideLocation;
                    locationMap["Katress"] = Config.KatressLocation;
                    locationMap["Kingpaca"] = Config.KingpacaLocation;
                    locationMap["Lunaris"] = Config.LunarisLocation;
                    locationMap["Mammorest"] = Config.MammorestLocation;
                    locationMap["Menasting"] = Config.MenastingLocation;
                    locationMap["Mossanda Lux"] = Config.MossandaLuxLocation;
                    locationMap["Nitewing"] = Config.NitewingLocation;
                    locationMap["Paladius"] = Config.PaladiusLocation;
                    locationMap["Penking"] = Config.PenkingLocation;
                    locationMap["Petallia"] = Config.PetalliaLocation;
                    locationMap["Quivern"] = Config.QuivernLocation;
                    locationMap["Relaxasaurus"] = Config.RelaxasaurusLuxLocation;
                    locationMap["Siblex"] = Config.SiblexLocation;
                    locationMap["Suzaku"] = Config.SuzakuLocation;
                    locationMap["Univolt"] = Config.UnivoltLocation;
                    locationMap["Vaelet"] = Config.VaeletLocation;
                    locationMap["Verdash"] = Config.VerdashLocation;
                    locationMap["Warsect"] = Config.WarsectLocation;
                    locationMap["Wumpo Botan"] = Config.WumpoBotanLocation;
                    // Add more teleport locations as needed

                    // Extract button names from the map
                    for (const auto& pair : locationMap) {
                        teleportLocations.push_back(pair.first);
                    }

                    UpdateCurrentPageButtons();
                }

                void Draw() {
                    // Draw buttons only if there are locations
                    if (teleportLocations.empty()) {
                        ImGui::Text("No teleport locations available.");
                        return;
                    }

                    // Draw buttons
                    for (int i = 0; i < currentButtons.size(); ++i) {
                        if (ImGui::Button(currentButtons[i].c_str(), ImVec2(ImGui::GetContentRegionAvail().x - 3, 20))) {
                            // Button logic
                            TeleportToLocation(currentButtons[i]);
                        }
                    }

                    // Draw page navigation
                    ImGui::Separator();
                    ImGui::Text("Page: %d", currentPage + 1);
                    ImGui::SameLine();
                    if (ImGui::Button("Previous")) {
                        currentPage = (currentPage - 1 + totalPages) % totalPages;
                        UpdateCurrentPageButtons();
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Next")) {
                        currentPage = (currentPage + 1) % totalPages;
                        UpdateCurrentPageButtons();
                    }
                }

                void TeleportToLocation(const std::string& location) {
                    // Check if the location exists in the map
                    if (locationMap.find(location) != locationMap.end()) {
                        SDK::FVector vector = { locationMap[location][0], locationMap[location][1], locationMap[location][2] };
                        AnyWhereTP(vector, Config.IsSafe);
                    }
                }

            private:
                void UpdateCurrentPageButtons() {
                    int startIdx = currentPage * buttonsPerPage;
                    int endIdx = (currentPage + 1) * buttonsPerPage;

                    if (startIdx < 0) {
                        startIdx = 0;
                    }

                    if (endIdx > teleportLocations.size()) {
                        endIdx = teleportLocations.size();
                    }

                    currentButtons.clear();
                    for (int i = startIdx; i < endIdx; ++i) {
                        currentButtons.push_back(teleportLocations[i]);
                    }

                    totalPages = (teleportLocations.size() + buttonsPerPage - 1) / buttonsPerPage;
                }

            private:
                std::vector<std::string> teleportLocations;
                std::vector<std::string> currentButtons;
                int currentPage;
                int buttonsPerPage;
                int totalPages;
                std::map<std::string, float*> locationMap; // Map button names to their corresponding locations in Config
            };
            static TeleportTabs teleportTabs; // Static to retain state between calls
            // Render your user interface
            teleportTabs.Draw();
        }

        void TABDevious()
        {
            if (ImGui::Button("Crash Server", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))//
            {
                if (Config.GetPalPlayerCharacter() != NULL)
                {
                    if (Config.GetPalPlayerCharacter()->GetPalPlayerController() != NULL)
                    {
                        Config.GetPalPlayerCharacter()->GetPalPlayerController()->RequestLiftup_ToServer(NULL);
                    }
                }
            if (ImGui::Button("Bypass Password Locks", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
                {
                    SDK::UWorld* world = Config.GetUWorld();
                    if (!world)
                        return;

                    SDK::TUObjectArray* objects = world->GObjects;

                    for (int i = 0; i < objects->NumElements; ++i) {
                        SDK::UObject* object = objects->GetByIndex(i);

                        if (!object) {
                            continue;
                        }

                        if (!object->IsA(SDK::UPalMapObjectPasswordLockModule::StaticClass())) {
                            continue;
                        }

                        SDK::UPalMapObjectPasswordLockModule* locked = (SDK::UPalMapObjectPasswordLockModule*)object;
                        if (!locked) {
                            continue;
                        }
                        locked->LockState = SDK::EPalMapObjectPasswordLockState::Unlock;
                    }
                }
            }
        }
    
        void TABGameBreaking()
        {
            if (ImGui::Button("Max Level (LVL 50)", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
            {
                SDK::APalPlayerCharacter* p_appc = Config.GetPalPlayerCharacter();
                if (p_appc != NULL)
                {
                    if (Config.GetPalPlayerCharacter()->GetPalPlayerController() != NULL)
                    {
                        if (Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState() != NULL)
                        {
                            Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState()->GrantExpForParty(99999999);
                        }
                    }
                }
            }
            if (ImGui::Button("All Effigies", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20))) //credit to bennett
            {
                SDK::APalPlayerCharacter* pPalCharacter = Config.GetPalPlayerCharacter();
                if (!pPalCharacter)
                    return;

                SDK::UWorld* world = Config.GetUWorld();
                if (!world)
                    return;

                SDK::TUObjectArray* objects = world->GObjects;

                for (int i = 0; i < objects->NumElements; ++i) {
                    SDK::UObject* object = objects->GetByIndex(i);

                    if (!object) {
                        continue;
                    }

                    if (!object->IsA(SDK::APalLevelObjectRelic::StaticClass())) {
                        continue;
                    }

                    SDK::APalLevelObjectObtainable* relic = (SDK::APalLevelObjectObtainable*)object;
                    if (!relic) {
                        continue;
                    }

                    ((SDK::APalPlayerState*)pPalCharacter->PlayerState)->RequestObtainLevelObject_ToServer(relic);
                }
            }
            if (ImGui::Button("Unlock Fast Travels", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
            {
                std::vector<std::string> keyStrings = {
                    "6E03F8464BAD9E458B843AA30BE1CC8F","DDBBFFAF43D9219AE68DF98744DF0831","603ED0CD4CFB9AFDC9E11F805594CCE5","6282FE1E4029EDCDB14135AA4C171E4C","9FBB93D84811BE424A37C391DBFBB476","979BF2044C8E8FE559B598A95A83EDE3","923B781345D2AB7ECED6BABD6E97ECE8",
                    "16C7164C43E2B96BEDCC6696E2E592F7","D27AFCAD472164F000D33A8D8B61FE8B","41727100495D21DC905D309C53989914","6DB6B7B2431CA2EFFFC88EB45805BA6A","74270C2F45B8DCA66B6A1FAAA911D024","DF9FB9CB41B43052A9C74FA79A826A50","8CA5E9774FF1BBC1156ABCA09E616480",
                    "15314BE94E2FB8D018D5749BE9A318F0","79C561B747335A7A0A8FBF9FAE280E62","23B9E99C4A454B99220AF7B4A58FD8DE","A1BC65AA445619EF338E0388BC010648","BF8B123244ADB794A06EA8A10503FBDD","F8DF603B4C750B37D943C9AF6A911946","596996B948716D3FD2283C8B5C6E829C",
                    "28D514E74B51FD9EB078A891DB0787C2","ACAE5FB04D48DE4197443E9C0993086B","4D2F204549AB656CA1EA4B8E39C484F3","1BDEABA240B1699541C17F83E59E61DF","2BC5E46049E69D3549CFB58948BE3288","91DAC6F34D2A9FD7F01471B5166C6C02","41E36D9A4B2BA79A3AD1B7B83B16F77D",
                    "76B000914943BADDC56BCFBAE2BF051E","DC0ECF9241B4410C59EE619F56D1106A","71C4B2B2407F2BBBD77572A20C7FF0F5","EC94023A4CA571FF0FD19E90944F4231","2A2B744B41AC79964DAE6B89CAC51FC3","E0819EFB41581AEAC3A029B0EE2FE195","22095BFA48A46172F8D154B2EBEB7493",
                    "7C5E91514F6E84B0C1DEFFB52C7C4DBA","AECFED0D41AFEE11F30B4F9687BC3243","2EC07ACC4505CB726DE38A84246CA999","F8E5CB8143F4FA2F6213E6B454569F87","5F426B49469368B0C131D3A6DB8F7831","A277AE6341EF40D84D711EA52303353F","6231802D40C81C00445379AE238D9F90",
                    "F6C005A14B38FE0B57F1C7869FD899CB","7170881D44249E90902F728E240493AF","3E8E504B4A3975FD3862E1BC85A5D4F6","B001852C491FF5E70C4747BFF9972924","2DE1702048A1D4A82126168C49BE51A9","E88379634CB5B6117DA2E7B8810BFE0A","3697999C458BF8A3C7973382969FBDF9",
                    "65B10BB14ABDA9C2109167B21901D195","4669582D4081BF550AFB66A05D043A3D","FE90632845114C7FBFA4669D071E285F","5970E8E648D2A83AFDFF7C9151D9BEF5","B639B7ED4EE18A7AA09BA189EA703032","099440764403D1508D9BADADF4848697","B44AA24445864494E7569597ACCAEFC6",
                    "3A0F123947BE045BC415C6B061A5285A","F382ADAE4259150BF994FF873ECF242B","01ACCA6E4BDAA68220821FB05AB54E4D","866881DB443444AA7F4E7C8E5DCDAA29","75BD9923489E2A4EBCED5A81175D5928","513E166044565A0BD3360F94142577E8"
                };

                SDK::APalPlayerCharacter* pPalCharacter = Config.GetPalPlayerCharacter();

                for (const std::string& keyString : keyStrings)
                {
                    SDK::UKismetStringLibrary* lib = SDK::UKismetStringLibrary::GetDefaultObj();
                    //Convert FNAME
                    wchar_t  ws[255];
                    swprintf(ws, 255, L"%hs", keyString.c_str());
                    SDK::FName FNameKS = lib->Conv_StringToName(SDK::FString(ws));

                    ((SDK::APalPlayerState*)pPalCharacter->PlayerState)->RequestUnlockFastTravelPoint_ToServer(FNameKS);
                }

            }
            if (ImGui::Button("Kill Aura", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
            {
                if (Config.GetPalPlayerCharacter() != NULL)
                {
                    if (Config.GetPalPlayerCharacter()->GetPalPlayerController() != NULL)
                    {
                        if (Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState())
                        {
                            SDK::TArray<SDK::AActor*> T = Config.GetUWorld()->PersistentLevel->Actors;
                            for (int i = 0; i < T.Num(); i++)
                            {

                                if (T[i] != NULL)
                                {
                                    if (T[i]->IsA(SDK::APalCharacter::StaticClass()))
                                    {
                                        SDK::APalCharacter* monster = (SDK::APalCharacter*)T[i];
                                        if (monster->IsLocallyControlled() || monster->GetCharacterParameterComponent()->IsOtomo())
                                        {
                                            continue;
                                        }
                                        Config.GetPalPlayerState()->SendDeath_ToServer(monster);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if (ImGui::Button("Boss Battle Kill Aura", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20)))
            {
                if (Config.GetPalPlayerCharacter() != NULL)
                {
                    auto localplayer = Config.GetPalPlayerCharacter();
                    if (localplayer->GetPalPlayerController() != NULL)
                    {
                        auto localcontroller = localplayer->GetPalPlayerController();
                        //localcontroller->Transmitter->BossBattle->RequestBossBattleEntry_ToServer(SDK::EPalBossType::GrassBoss, Config.GetPalPlayerCharacter());
                        if (Config.GetUWorld() != NULL)
                        {
                            SDK::TArray<SDK::AActor*> T = Config.GetUWorld()->PersistentLevel->Actors;
                            if (T.IsValid())
                            {
                                for (int i = 0; i < T.Num(); i++)
                                {
                                    if (T[i] != NULL)
                                    {
                                        if (T[i]->IsA(SDK::APalPlayerCharacter::StaticClass()))
                                        {
                                            auto other = (SDK::APalPlayerCharacter*)T[i];
                                            if (other->GetPalPlayerController() != NULL)
                                            {
                                                if (other->GetPalPlayerController()->IsLocalPlayerController())
                                                {
                                                    continue;
                                                }
                                            }
                                            localcontroller->Transmitter->BossBattle->RequestBossBattleEntry_ToServer(SDK::EPalBossType::ElectricBoss, other);
                                        }
                                    }
                                }
                            }
                            localcontroller->Transmitter->BossBattle->RequestBossBattleStart_ToServer(SDK::EPalBossType::ElectricBoss, localplayer);
                        }
                    }
                }
            }
        }
        void TABConfig()
        {
            ImGui::Text("VynxWorld | Palworld Exploit Tool");
            ImGui::Text("Version: v1.1");
            ImGui::Text("https://vynx.dev");
            ImGui::Text("Credits to: bluesword007 & itzkaydo");

            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
            if (ImGui::Button("Unload DLL", ImVec2(ImGui::GetContentRegionAvail().x - 3, 20))) {
#if DEBUG
                g_Console->printdbg("\n\n[+] Unloading Initialized [+]\n\n", g_Console->color.red);

#endif
                g_KillSwitch = TRUE;
            }
        }
	}

	void Menu::Draw()
	{
        //if (Config.IsESP)
        //{
        //    ESP();
        //}
		if (g_GameVariables->m_ShowMenu)
			MainMenu();
        if (Config.bisOpenManager && g_GameVariables->m_ShowMenu)
        {
            ManagerMenu();
        }

		if (g_GameVariables->m_ShowHud)
			HUD(&g_GameVariables->m_ShowHud);

		if (g_GameVariables->m_ShowDemo)
			ImGui::ShowDemoWindow();

        if (Config.isDebugESP)
            ESP_DEBUG(Config.mDebugESPDistance, ImVec4(0.24f, 0.23f, 0.29f, 1.00f));
	}
    void Menu::ManagerMenu()
    {
        if (ImGui::Begin("Manager", &g_GameVariables->m_ShowMenu, 96))
        {
            if (Config.GetUWorld() != NULL)
            {
                ImGui::Checkbox("Filter Characters", &Config.filterCharacters);
                ImGui::SameLine();
                ImGui::Checkbox("Players", &Config.filterPlayer);
                ImGui::SameLine();
                ImGui::Checkbox("Guilds", &Config.filterGuilds);
                ImGui::SameLine();
                ImGui::Checkbox("Map Objects", &Config.filterMapParts);
                SDK::TArray<SDK::AActor*> T = Config.GetUWorld()->PersistentLevel->Actors;

                for (int i = 0; i < T.Num(); i++)
                {
                    SDK::AActor* Actor = T[i];

                    if (Actor == nullptr) continue;

                    SDK::APalCharacter* Character = (SDK::APalCharacter*)Actor;
                    std::string name;

                    bool Skip = Config.filterCharacters || Config.filterPlayer || Config.filterGuilds || Config.filterMapParts;

                    if (Config.filterCharacters && Actor->IsA(SDK::APalCharacter::StaticClass())) Skip = false;
                    else if (Config.filterPlayer && Actor->IsA(SDK::APalPlayerCharacter::StaticClass())) Skip = false;
                    if (Config.filterGuilds && Actor->IsA(SDK::APalGuildInfo::StaticClass())) Skip = false;
                    if (Config.filterMapParts && Actor->IsA(SDK::APalMapObject::StaticClass())) Skip = false;
                    if (Actor->IsA(SDK::APalCaptureJudgeObject::StaticClass())) Skip = false;

                    if (Skip) continue;

                    if (Actor->IsA(SDK::APalPlayerCharacter::StaticClass()))
                    {
                        if (!Character) { continue; }
                        SDK::FString fn;
                        Character->CharacterParameterComponent->GetNickname(&fn);
                        if (fn.IsValid())
                            name = fn.ToString();
                        else
                            name = "unknown";
                    }
                    else if (Actor->IsA(SDK::APalGuildInfo::StaticClass()))
                        name = Character->Name.GetRawString() + "[]" + ((SDK::APalGuildInfo*)Actor)->Guild->GuildName.ToString();
                    else
                        name = Character->Name.GetRawString();
                    ImGui::PushID(i);
                    if (ImGui::Button("TP"))
                    {
                        if (Config.GetPalPlayerCharacter() != NULL)
                        {
                            if (!Character) { continue; }
                            SDK::FVector vector = Character->K2_GetActorLocation();
                            AnyWhereTP(vector, Config.IsSafe);
                        }
                    }

                    if (Actor->IsA(SDK::APalCharacter::StaticClass())) {
                        ImGui::SameLine();
                        if (ImGui::Button(GetAsyncKeyState(VK_LSHIFT) ? "-99%" : "Kill"))
                            Damage(Character, GetAsyncKeyState(VK_LSHIFT) ? (Character->CharacterParameterComponent->GetHP().Value - 1) / 1000 : 99999999999);
                    }

                    if (Actor->IsA(SDK::APalPlayerCharacter::StaticClass())) {
                        ImGui::SameLine();
                        if (ImGui::Button("Join Guild")) {
                            SDK::FGuid guid1 = Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPlayerUId();
                            SDK::FGuid guid2;

                            guid2 = SDK::UPalUtility::GetDefaultObj()->GetPlayerUIDByActor(Actor);

                            g_Console->printdbg("\n\n[+] Player 1 GUID: %x\nPlayer 2 GUID: %x [+]\n\n", g_Console->color.green, guid1, guid2);

                            Config.GetPalPlayerCharacter()->GetPalPlayerController()->Transmitter->Group->RequestJoinGuildForPlayer_ToServer(guid1, guid2);
                        }
                    }
                    if (Actor->IsA(SDK::APalMapLevelObject::StaticClass())) {
                        ImGui::SameLine();
                        if (ImGui::Button("Destroy")) {
                            SDK::APalMapLevelObject* Object = (SDK::APalMapLevelObject*)Actor;

                            Config.GetPalPlayerCharacter()->GetPalPlayerController()->Transmitter->MapObject->RequestDismantleObject_ToServer(Object->LevelObjectInstanceId);
                        }
                    }
                    if (Character->IsA(SDK::APalPlayerCharacter::StaticClass()))
                    {
                        ImGui::SameLine();
                        if (ImGui::Button("Steal Name"))
                        {
                            if (Config.GetPalPlayerCharacter() != NULL)
                            {
                                auto controller = Config.GetPalPlayerCharacter()->GetPalPlayerController();
                                if (controller != NULL)
                                {
                                    auto player = (SDK::APalPlayerCharacter*)Character;
                                    SDK::FString fakename;
                                    player->CharacterParameterComponent->GetNickname(&fakename);
                                    Config.GetPalPlayerCharacter()->GetPalPlayerController()->Transmitter->NetworkIndividualComponent->UpdateCharacterNickName_ToServer(Config.GetPalPlayerCharacter()->CharacterParameterComponent->IndividualHandle->ID, fakename);
                                }
                            }
                        }
                    }
                    ImGui::SameLine();
                    ImGui::Text(name.c_str());

                    ImGui::PopID();
                }
            }
        }
        ImGui::End();
    }

	void Menu::MainMenu()
	{
        if (!g_GameVariables->m_ShowDemo)
            Styles::InitStyle();

        if (g_Menu->dbg_RAINBOW_THEME) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(g_Menu->dbg_RAINBOW));
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(g_Menu->dbg_RAINBOW));
            ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImVec4(g_Menu->dbg_RAINBOW));
            //ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(g_Menu->dbg_RAINBOW));
        }
        if (!ImGui::Begin("VynxWorld | Palworld Exploit Tool", &g_GameVariables->m_ShowMenu, 96))
        {
            ImGui::End();
            return;
        }
        if (g_Menu->dbg_RAINBOW_THEME) {
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
            ImGui::PopStyleColor();
        }
        
        //  Display Menu Content
        //Tabs::TABMain();

        ImGui::Text("https://vynx.dev");

        if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
        {
          if (ImGui::BeginTabItem("Player"))
           {
               Tabs::TABPlayer();
               ImGui::EndTabItem();
           }
          if (ImGui::BeginTabItem("Exploits"))
          {
              Tabs::TABExploit();
              ImGui::EndTabItem();
          }
          if (ImGui::BeginTabItem("Item Spawner"))
          {
              Tabs::TABItemSpawner();
              ImGui::EndTabItem();
          }
          if (ImGui::BeginTabItem("Quick TP"))
          {
              Tabs::TABQuickTP();
              ImGui::EndTabItem();
          }
          if (ImGui::BeginTabItem("GameBreaking"))
          {
              Tabs::TABGameBreaking();
              ImGui::EndTabItem();
          }
          if (ImGui::BeginTabItem("Devious"))
          {
              Tabs::TABDevious();
              ImGui::EndTabItem();
          }
          if (ImGui::BeginTabItem("Info"))
          {
              Tabs::TABConfig();
              ImGui::EndTabItem();
          }
         
           ImGui::EndTabBar();
        }
        ImGui::End();

        
	}


	void Menu::HUD(bool* p_open)
	{

	}

    void Menu::Loops()
    {
        if (Config.GetPalPlayerCharacter() != NULL)
        {
            if (Config.GetPalPlayerCharacter()->ShooterComponent != NULL && Config.GetPalPlayerCharacter()->ShooterComponent->CanShoot())
            {
                if (Config.GetPalPlayerCharacter()->ShooterComponent->GetHasWeapon() != NULL)
                {
                    Config.GetPalPlayerCharacter()->ShooterComponent->GetHasWeapon()->IsRequiredBullet = !Config.IsInfinAmmo;
                }
            }
        }

        //if ((GetAsyncKeyState(VK_F2) & 1) && IsGameActive())
        //{
        //    SDK::APalPlayerCharacter* p_appc = Config.GetPalPlayerCharacter();
        //    if (p_appc != NULL)
        //    {
        //        if (Config.GetPalPlayerCharacter()->GetPalPlayerController() != NULL)
        //        {
        //            if (Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState() != NULL)
        //            {
        //                SDK::FVector PlayerLocation = p_appc->K2_GetActorLocation();
        //                float PosX = PlayerLocation.X;
        //                float PosY = PlayerLocation.Y;
        //                float PosZ = PlayerLocation.Z;

        //                Config.SaveLocation(Config.BossName, PosX, PosY, PosZ);
        //            }
        //        }
        //    }
        //}

        if ((GetAsyncKeyState(VK_F5) & 1))
        {
            SDK::APalPlayerCharacter* p_appc = Config.GetPalPlayerCharacter();
            if (p_appc != NULL)
            {
                if (Config.GetPalPlayerCharacter()->GetPalPlayerController() != NULL)
                {
                    if (Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState() != NULL)
                    {
                        if (Config.IsSafe)
                        {
                            Config.GetPalPlayerCharacter()->GetPalPlayerController()->TeleportToSafePoint_ToServer();
                        }
                        else
                        {
                            Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState()->RequestRespawn();

                        }

                    }
                }
            }
        }
        if ((GetAsyncKeyState(VK_F6) & 1))
        {
            SDK::APalPlayerCharacter* p_appc = Config.GetPalPlayerCharacter();
            if (p_appc != NULL)
            {
                if (Config.GetPalPlayerCharacter()->GetPalPlayerController() != NULL)
                {
                    if (Config.GetPalPlayerCharacter()->GetPalPlayerController()->GetPalPlayerState() != NULL)
                    {
                        SDK::FFixedPoint fixpoint = SDK::FFixedPoint();
                        fixpoint.Value = 99999999;
                        Config.GetPalPlayerCharacter()->ReviveCharacter_ToServer(fixpoint);

                    }
                }
            }
        }
        if (Config.IsSpeedHack)
        {
            if (Config.GetUWorld()
                || Config.GetUWorld()->PersistentLevel
                || Config.GetUWorld()->PersistentLevel->WorldSettings)
            {
                Config.GetUWorld()->OwningGameInstance->LocalPlayers[0]->PlayerController->AcknowledgedPawn->CustomTimeDilation = Config.SpeedModiflers;
            }
        }
        if (Config.GetPalPlayerCharacter() != NULL)
        {
            if (Config.GetPalPlayerCharacter()->CharacterParameterComponent != NULL)
            {
                Config.GetPalPlayerCharacter()->CharacterParameterComponent->bIsEnableMuteki = Config.IsMuteki;
            }
        }
        if (Config.IsAttackModiler)
        {
            if (Config.GetPalPlayerCharacter() != NULL && Config.GetPalPlayerCharacter()->CharacterParameterComponent->AttackUp != Config.DamageUp)
            {
                if (Config.GetPalPlayerCharacter()->CharacterParameterComponent != NULL)
                {
                    Config.GetPalPlayerCharacter()->CharacterParameterComponent->AttackUp = Config.DamageUp;
                }
            }
        }
        if (Config.IsDefuseModiler)
        {
            if (Config.GetPalPlayerCharacter() != NULL && Config.GetPalPlayerCharacter()->CharacterParameterComponent->DefenseUp != Config.DefuseUp)
            {
                if (Config.GetPalPlayerCharacter()->CharacterParameterComponent != NULL)
                {
                    Config.GetPalPlayerCharacter()->CharacterParameterComponent->DefenseUp = Config.DefuseUp;
                }
            }
        }
        if (Config.IsInfStamina)
        {
            if (Config.GetPalPlayerCharacter() != NULL)
            {
                if (Config.GetPalPlayerCharacter()->CharacterParameterComponent != NULL)
                {
                    Config.GetPalPlayerCharacter()->CharacterParameterComponent->ResetSP();
                    SDK::TArray<SDK::AActor*> T = Config.GetUWorld()->PersistentLevel->Actors;
                    for (int i = 0; i < T.Num(); i++)
                    {
                        if (T[i] != NULL)
                        {
                            if (T[i]->IsA(SDK::APalCharacter::StaticClass()))
                            {
                                SDK::APalCharacter* Character = (SDK::APalCharacter*)T[i];
                                if (Character->IsPlayerControlled())
                                {
                                    Character->CharacterParameterComponent->ResetSP();
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
        //if (Config.IsRevive)
        //{
        //    if (Config.GetPalPlayerCharacter() != NULL)
        //    {
        //        if (Config.GetPalPlayerCharacter()->CharacterParameterComponent != NULL)
        //        {
        //            Config.GetPalPlayerCharacter()->CharacterParameterComponent->ResetDyingHP();
        //        }
        //    }
        //}
 
    }
}