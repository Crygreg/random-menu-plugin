/*using namespace Union;

void PatchAddress(void* where, void* what)
{
    ulong protection = PAGE_EXECUTE_READWRITE;
    VirtualProtect(where, 4, protection, &protection);
    *(void**)where = what;
    VirtualProtect(where, 4, protection, &protection);
}

extern "C" __declspec(dllexport) void Game_Entry()
{
    static StringANSI name = StringANSI::Format("GAMESTART", rand() % 18 + 1);

    for (int address : { 0x004DB7EE + 1, 0x004DB815 + 1 })
        PatchAddress(reinterpret_cast<void*>(address), name.ToChar());
}
*/
using namespace Union;

namespace GOTHIC_NAMESPACE
{
    void __fastcall Partial_CGameManager_InitScreen_Open_BackTex(Union::Registers& reg);
    auto Hook_CGameManager_InitScreen_Open_BackTex = Union::CreatePartialHook(reinterpret_cast<void*>(0x00426171), &Partial_CGameManager_InitScreen_Open_BackTex);
    void __fastcall Partial_CGameManager_InitScreen_Open_BackTex(Union::Registers& reg)
    {
        zSTRING* screenTexture = reinterpret_cast<zSTRING*>(reg.eax);

        // generate random number from 1 to 3
        // shitty method cuz it's using rand() which is dogshit and not that random
        int randy = rand() % 3 + 1;

        // create texture name with generated number
        *screenTexture = zSTRING{ "startscreen" } + zSTRING{ randy };
        //MessageBoxA(nullptr, screenTex->ToChar(), "debug", 0);

    }


 /*   void __fastcall Partial_zCMenu_Enter_PlaySound(Union::Registers& reg);
    auto Hook_zCMenu_Enter_PlaySound = Union::CreatePartialHook(reinterpret_cast<void*>(0x004DB844), &Partial_zCMenu_Enter_PlaySound);
    void __fastcall Partial_zCMenu_Enter_PlaySound(Union::Registers& reg)
  {
        zSTRING* soundFX = reinterpret_cast<zSTRING*>(reg.eax);
        int randy = rand() % 3 + 1;

        // create texture name with generated number
        *soundFX = zSTRING{ "GAMESTART" } + zSTRING{ randy };
        //MessageBoxA(nullptr, soundFX->ToChar(), "debug", 0);
    }
*/

    struct zCMenu_Enter : zCMenu { void operator()(); };
    auto Ivk_zCMenu_Enter = Union::CreateHook(reinterpret_cast<void*>(0x004DB780), &zCMenu_Enter::operator());
    void zCMenu_Enter::operator()()
    {
        void (*EnterCallback)() = *reinterpret_cast<void(**)()>(0x008D1E7C);
        zCMusicTheme*& enterMusic = *reinterpret_cast<zCMusicTheme**>(0x008D1E98);
        int& firstTime = *reinterpret_cast<int*>(0x0089A688);
        int randy = rand() % 3 + 1;

        if (EnterCallback)
            EnterCallback();

        if (!inGameMenu)
        {
            zmusic->PlayThemeByScript(musicThemeName, 0, nullptr);

            if (!enterMusic)
            {
                enterMusic = zmusic->LoadThemeByScript(zSTRING{ "GAMESTART" } + zSTRING{ randy });
            }

            if (firstTime)
            {
                zmusic->PlayTheme(enterMusic, zMUS_THEME_VOL_DEFAULT, zMUS_TR_ENDANDINTRO, zMUS_TRSUB_MEASURE);
                firstTime = false;
            }
        }


/*     struct zCMenu_Enter : zCMenu { void operator()(); };
    auto Ivk_zCMenu_Enter = Union::CreateHook(reinterpret_cast<void*>(0x004DB780), &zCMenu_Enter::operator());
    void zCMenu_Enter::operator()()
    {
        void (*EnterCallback)() = *reinterpret_cast<void(**)()>(0x008D1E7C);
        zCSoundFX*& enterSound = *reinterpret_cast<zCSoundFX**>(0x008D1E98);
        int& firstTime = *reinterpret_cast<int*>(0x0089A688);
        int randy = rand() % 3 + 1;

        if (EnterCallback)
            EnterCallback();

        if (!inGameMenu)
        {
            zmusic->PlayThemeByScript(musicThemeName, 0, nullptr);

            if (!enterSound)
            {
                enterSound = zsound->LoadSoundFXScript(zSTRING{ "GAMESTART" } + zSTRING{ randy });
                enterSound->SetIsFixed(true);
            }

            if (firstTime)
            {
                zsound->PlaySound(enterSound, zSND_SLOT_NONE);
                firstTime = false;
            }
        }

*/


        ScreenInit();

        if (m_mainSel != -1)
            HandleEnterItem(m_listItems[m_mainSel]);

        zinput->ClearKeyBuffer();
    }

}