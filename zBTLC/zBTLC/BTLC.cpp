//////////////////////////////////////////////////////////////////
////           Back to Liberty City - MAIN START              ////
//////////////////////////////////////////////////////////////////
//// Author: D4DJ                                             ////
//////////////////////////////////////////////////////////////////
//// Description:                                             ////
//// This file will initiate the BTLC modifications.          ////
//// SOME general modification are done in the main function. ////
//// Please keep the main function clean and use new functions////
//// instead.                                                 ////
//////////////////////////////////////////////////////////////////
//// CREDITS:                                                 ////
//// DK22PAC : SOME CLASSES/EVENTHANDLER/GPS                  ////
//// THIRTEENAG:WIDESCREENFIX; WINDOWMODE                     ////
//// AAP: SKYGFX                                              ////
//// SILENT: SILENTSPATCH                                     ////
//// LINK: MODLOADER                                          ////
//////////////////////////////////////////////////////////////////
////										                  ////
//////////////////////////////////////////////////////////////////
#define DEBUG 
float VERSION = 0.42f;

#include "BTLC_BASE\debug.h"
#include "BTLC_BASE\fixes.h"
#include "BTLC_BASE\weather.h"
#include "BTLC_BASE\visuals.h"
#include "BTLC_BASE\limits.h"
#include "BTLC_BASE\windowmode\windowmode.h"
#include "BTLC_BASE\windowmode\dxhandler.h"
#include "BTLC_BASE\fast_load.h"
#include "BTLC_BASE\CTrafficlights.h"
#include "BTLC_BASE\My_PlayerWeaponReload.h"
#include "BTLC_BASE\My_PlayerWallHitreaction.h"
#include "BTLC_BASE\My_GPS.h"
#include "BTLC_BASE\My_CCam.h"


#include "game_sa\CObject.h"
#include "game_sa\CPlayerPed.h"
#include "game_sa\CHud.h"
#include "game_sa\CRadar.h"
#include "game_sa\CVideomodemanager.h"
#include "game_sa\CAnimationManager.h"
#include "game_sa\CEntity.h"
#include "game_sa\List_c.h"
#include "game_sa\CProcObjectMan.h"
	
void btlc_init(); //BTLC INIT
void check_gameversion();
void ParseCommandlineArgument(int thing, char* pArg);
void Function_starter();



void Main()
{
#ifdef DEBUG
	debug::init();
	std::cout << std::endl;
	std::cout << "GTA:BACK TO LIBERTY CITY" << std::endl;
	std::cout << "developed by: D4DJ,  Majorapplepie" << std::endl << std::endl;
	std::cout << "Version: " << VERSION << " >DEBUG" << std::endl;
	std::cout << "COMPILED: " << __TIMESTAMP__ << std::endl << std::endl;
#endif // DEBUG

	//Windowmode INIT
	//CDxHandler::btogglereq_btlc = true;

	//COMMANDLINE READER
	MemoryVP::InjectHook(0x74879A, &ParseCommandlineArgument, PATCH_CALL);
	MemoryVP::Patch(0x74877D, 0);
	//check game version
	check_gameversion();

	//START BTLC STUFF
	Function_starter();
}

void Function_starter()
{
	btlc_init();			//INIT OF BTLC
	fastload::Init();		//Fast loader
	BUGFIX::init();			//Fixes of some small GTA Bugs
	weather::init();		//Changes the weather effects.
	limits::Init();
	visuals::init();		//VISUAL CHANGES init

	My_PlayerWeaponReload::init();	//Add Reload with "R" functions.
	My_PlayerWallhitreactions::init();
	My_GPS::init();
	My_CCam::INIT();		// IV Styled AIM CAM
	
	//Trafficlight changes
	CTrafficlights::Set_polygon_size(13);
	CTrafficlights::Set_Trafficlight_models();

	CHud::My_Init();		//New HuD
	CRadar::My_Init();		//New Radar
	CStreaming::My_Init();	//New COP stream functions - ALL cops&copcars in all cities 
	CAnimationStyleDescriptor::My_init(); //Armed running
	CEntity::My_Init();		//static shadows for all new traffic lights and Lamps
	CPed::My_Init();		//Armed Animations for Peds

	
	

	 //static Crosshair Hook
	MemoryVP::InjectHook(0x609CD0, &CPlayerPed::GetWeaponRadiusOnScreen, PATCH_JUMP);

	//New Masspoints for dynamic Objects
	MemoryVP::InjectHook(0x59F8A1, &CObject::SetObjectdata, PATCH_CALL);

	//set numMonitor to only give back one
	MemoryVP::InjectHook(0x7461AA, &CVideomodemanager::GetNumSubSystems);
	MemoryVP::InjectHook(0x74629C, &FIND_VIDEOMODES);
	MemoryVP::InjectHook(0x745D3B, &FIND_VIDEOMODES);
	MemoryVP::InjectHook(0x57A05A, &FIND_VIDEOMODES);
	MemoryVP::InjectHook(0x57CFA7, &FIND_VIDEOMODES);	


}



//BTLC INIT
void btlc_init()
{
	////DECLARATION OF GENERAL CHANGES
	static  char	AppName[18] = "GTA:BTLC";		//name of the GTA window.
	MemoryVP::Patch<void*>(0x619602 + 6, &AppName);
	static char  Userfiles[28] = "\\GTA BTLC User Files";//new USER folder
	MemoryVP::Patch<void*>(0x74503F, &Userfiles);
	static  char  settingsfile[13] = "gta_btlc.set";//new settings file
	MemoryVP::Patch<void*>(0x57C672, &settingsfile);
	MemoryVP::Patch<void*>(0x57C902, &settingsfile);
	MemoryVP::Patch<void*>(0x7489A0, &settingsfile);	
	
	//MemoryVP::Patch<BYTE>(0x5A3353, 0xFF);
	//MemoryVP::Nop(0x5A332A, 2);
	//MemoryVP::Nop(0x5A3355, 2);
	MemoryVP::Patch<int>(0x5A3327, 5000);
	MemoryVP::Patch<int>(0x5A335D + 1,8000);
	//MemoryVP::Patch<int>(0x5A3EB2  + 1, 2000);

}



void check_gameversion()
{
	// Find NT header
	uintptr_t          base = (uintptr_t)GetModuleHandleA(NULL);;
	IMAGE_DOS_HEADER*  dos = (IMAGE_DOS_HEADER*)(base);
	IMAGE_NT_HEADERS*  nt = (IMAGE_NT_HEADERS*)(base + dos->e_lfanew);
	int entryadress = base + nt->OptionalHeader.AddressOfEntryPoint;
	bool fail = 0;
	char* failtext  = "TestWindow";
	char* test = (char*)0x407610;

	switch (entryadress)
	{
	case 0x824570:  // GTA SA 1.0 US Compact //&and hoodlum?
		if (*test ==  (char)0xe9) 
		{
			failtext = "GTA SA 1.0 US Hoodlum not supported. \nPlease use GTA SA 1.0 US Compact";
			fail = 1;
			break;
		}
		std::cout << "EXE VERSION: COMPACT detected. -> start game" <<std::endl ;
		break;

	case 0x8245BC:  // GTA SA 1.0 EU Cracked (??????)
	case 0x8245B0:  // GTA SA 1.0 EU Cracked
		failtext = "GTA SA 1.0 EU Cracked not supported. \nPlease use GTA SA 1.0 US Compact";
		fail = 1;
		break;

	case 0x8252FC:  // GTA SA 1.1 US Cracked
		failtext = "GTA SA 1.0 US Cracked not supported. \nPlease use GTA SA 1.0 US Compact";
		fail = 1;
		break;

	case 0x82533C:  // GTA SA 1.1 EU Cracked
		failtext = "GTA SA 1.1 EU Cracked not supported. \nPlease use GTA SA 1.0 US Compact";
		fail = 1;
		break;

	case 0x85EC4A:  // GTA SA 3.0 (Cracked Steam Version)
		failtext = "GTA SA 3.0 (Cracked Steam Version) not supported. \nPlease use GTA SA 1.0 US Compact";
		fail = 1;
		break;

	case 0xD3C3DB:  // GTA SA 3.0 (Encrypted Steam Version)
		failtext = "GTA SA 3.0 (Encrypted Steam Version) not supported. \nPlease use GTA SA 1.0 US Compact";
		fail = 1;
		break;

	case 0x82457C:// GTA SA 1.0 US Cracked
		failtext = "GTA SA 1.0 US Cracked not supported. \nPlease use GTA SA 1.0 US Compact";
		fail = 1;
		break;

	default:
		failtext = "This Game EXE is not supported. \nPlease use GTA SA 1.0 US Compact";
		fail = 1;
		break;
	}

	if (fail == 1)
	{
		std::cout << "EXE VERSION: incompatible EXE detected. -> quit game" << std::endl;
		MessageBoxA(0, failtext, "GTA SA EXE not supported", MB_ICONWARNING);
		exit(0);
	}
}

void ParseCommandlineArgument(int thing, char* pArg)
{
	LPSTR TEST = GetCommandLine();
	//Close game if it isn't launched via launcher
	if (!strstr(TEST, "-launch"))
	{
		MessageBoxA(0, "Please use the GTA_BTLC Launcher to start this game.(GTA_BTLC.exe)", "BTLC - Launch", MB_ICONEXCLAMATION);
		exit(0);
		return;
	}

	if (pArg)
	{
		////DEV enables the debug_consoles and outputs
		//if (!_stricmp(pArg, "-launch"))
		//{
		//	debug_console();	//debug console
		//	return;
		//}
		//settings for windowed mode
		if (!_stricmp(pArg, "-windowed"))
		{	
			CDxHandler::WindowMode_Hook();
			std::cout << "windowmode" << std::endl;
			return;
		}
		////DEV enables the debug_consoles and outputs
		//if (!_stricmp(pArg, "-DEV"))
		//{
		//	debug::init();	//debug console
		//	return;
		//}
	}
}
