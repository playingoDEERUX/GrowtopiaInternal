// messy code, I know, this internal is very experimental and was only released due to private reasons. After months with this, and constant updating, I hope all "GT Internals" wont be the big thing anymore, despite the fact what has been done 2 me too, GTv3 leak, several other leaks like growbrew, lose of channel. I mean if they want me to leak shit, I'll do it in this case.
// ~playingo
// IF YOU PAID FOR AN INTERNAL OR WILLING TO PAY; IT'S most likely not a great deal!! THIS INTERNAL HAS MORE THAN ENOUGH FEATURES, A CORE ONE BEING AAP BYPASS!!!
// DO NOT BUY AN INTERNAL, YOU WILL SAVE MUCH MORE MONEY BY JUST LEARNING THIS CODE BASE AND ADDING YOUR OWN FEATURES TO IT
// AND ABOUT THE ADDRESSES, I WILL BE RESPONSIBLE FOR UPDATING THEM AND ILL ENJOY DOING THAT :)
// yet another tool, that i will regret that i published it, but I do not tolerate people who cause massive issues, making children advertise their GT Internal very brazenly
// and if you think about it, in my opinion, trainers were more than enough to cheat in this game. *!#@tard community just couldnt satisfy themselves and even went into faking it.
// WITH THE RELEASE OF IT................................................. I dont care anymore
// and yes, it is command line only, its easy to port it to a gui base though. And yes I still appreciate all credits, even though I can smell them being left out already for some people.
// oh yes and not to forget, the odin internal that was released PROBABLY JUST CUZ OF ME AGAIN, cuz it was published instantly after I made a vid about it, was a worthless attempt into briging kids to code with odin.
// 23.06.2020

#include <Windows.h>
#include <detours.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include "proton/hash.hpp"
#include "proton/rtparam.hpp"
#include "proton/variant.hpp"
#include "utils.hpp"
// <d3d11.h>
//#pragma comment (lib, "d3d11.lib")

const char* windowName = "Growtopia";



using namespace std;
#pragma warning (disable : 4996)

uintptr_t* skinAddress;
uintptr_t mBase;
uintptr_t App;

int currentDropIt = 0;
int currentClothing = 0;

bool hasLogonAlready = false;

typedef uintptr_t func(...);
typedef void func2(int a1, string a2, uint64_t a3);

// sig scan soon, for now ill just constantly update these and perhaps bring out a full tutorial on how to find them, completely bringing out the internal out of gt children's mind existence.
#define Addr_LogToConsole 0x24FA00
#define Addr_SendPacket 0x2E20A0
#define Addr_SetFPSLimit 0x11060
#define Addr_GetApp 0x9E090
#define Addr_GetClient 0x9C6C0
#define Addr_SendPacketRaw 0x2E2120
#define Addr_LogWarningToConsole 0x26A3C0
#define Addr_RequestJoinWorldCommon 0x2B31C0
#define Addr_OnTextOverlay 0x1F3D40
#define Addr_OnChangeSkin 0x2D0340
#define Addr_OnConsoleMessage 0x1F7F90
#define Addr_ProcessTankUpdatePacket 0x1F5840

struct GameLogicComponent {

};

typedef uint64_t(__cdecl* _app)();
_app getapp2;
typedef void(__fastcall* _SendPackets)(int a1, string a2, uint64_t a3);
_SendPackets SendPacket;
typedef void(__fastcall* _SendPacketRaw)(int a1, void* data, int size, void* a4, uint64_t a5, int a6);
_SendPacketRaw SendPacketRaw;
typedef void(__fastcall* _ProcessTankUpdatePacket)(int a1, BYTE* data);
_ProcessTankUpdatePacket ProcessTankUpdatePacket;

func* LogToConsole;

typedef void(*Log)(char* text);
uintptr_t ltchook;
uintptr_t sphook;
uintptr_t sprhook;
uintptr_t ptuphook;

string gamever = "3.38";


uint64_t getpeer() {
	getapp2 = (_app)(mBase + Addr_GetApp);
	
	return *(uint64_t*)(*(uint64_t*)(getapp2() + 3984) + 200i64);
}
__int64 peery = 0;

//declares
#define NOP 0x90
//declares

void nop_(PVOID address, int bytes) {
	DWORD d, ds;
	VirtualProtect(address, bytes, PAGE_EXECUTE_READWRITE, &d);
	memset(address, NOP, bytes);
	VirtualProtect(address, bytes, d, &ds);
}

HANDLE pHandle; // just incase we would ever use external funcs;

void ProcessTankUpdatePacketHook(int a1, BYTE* data) 
{
	/*DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&(PVOID&)ptuphook, &ProcessTankUpdatePacketHook);
	DetourTransactionCommit();

	for (int i = 0; i < 56; i++) 
	{
		cout << (int)data[i] << endl;
	}

	//ProcessTankUpdatePacket(a1, data);*/
}

void SendPacketRawHook(int a1, void* data, int size, void* a4, uint64_t a5, int a6) 
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&(PVOID&)sprhook, &SendPacketRawHook);
	DetourTransactionCommit();

	byte* dataPtr = (byte*)data;
	bool doRecallSPR = true;

	switch (a1) 
	{
	case 4:
	{
		
		if (dataPtr[0] == 25) 
		{
			doRecallSPR = false;
			LogToConsole("[Internal] Blocked autoban packet of type 25!");
		}
		break;
	}
	}

	if (doRecallSPR)
		SendPacketRaw(a1, data, size, a4, a5, a6);

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)sprhook, &SendPacketRawHook);
	DetourTransactionCommit();
}

void SendPacketHook(int a1, string a2, uint64_t a3) {
	
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&(PVOID&)sphook, &SendPacketHook);
	DetourTransactionCommit();

	LogToConsole((char*)("msg type of sendpacket sent to server: " + to_string(a1)).c_str());
	
	std::string modifiedStringPacket = a2;
	
	if (a2.find("game_version") != -1) 
	{
		rtvar var = rtvar::parse(a2);
		const char* spoofedMac = "02:00:00:00:00:00";
		var.set("platformID", "4");
		var.set("mac", spoofedMac);
		var.set("rid", utils::CreateRandomIdentifier());
		var.set("wk", "NONE0");
		var.set("hash", utils::RTHash((unsigned char*)spoofedMac, 17));
		var.set("hash2", utils::RTHash((unsigned char*)spoofedMac, 17));
		var.set("country", "de");
		var.set("game_version", "4.20"); // version spoof, please use 3.37 and you can still continue playing with this :)
		var.remove("fz");
		var.remove("rid");
	    modifiedStringPacket = var.serialize();
	}

	SendPacket(a1, modifiedStringPacket, a3);
	

	cout << "msg: " << modifiedStringPacket << endl;//cout << str << endl;
	
	if (a2.find("action|join_request") != std::string::npos) 
	{
		currentDropIt = 0;
	}
	//size_t pos = a2.find("game_version|");
	
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)sphook, &SendPacketHook);
	DetourTransactionCommit();
}

struct TankPacketStruct 
{
#pragma pack (push,1)
	byte packetType;
	byte padding1;
	short padding2;
	int NetID;
	int secondaryNetID;
	int characterState;
	float padding3;
	int value;
	float x, y;
	int XSpeed, YSpeed;
	int padding4;
	int punchX, punchY;
	int extDataSize = 0;
#pragma pack (pop)
};

byte* packTankStruct(TankPacketStruct* tPckt) 
{
	byte* raw = (byte*)tPckt;
	return raw;
}

void LogToConsoleHook(char* message) {
	
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&(PVOID&)ltchook, &LogToConsoleHook);
	DetourTransactionCommit(); // detach so next time when we call it it will call original function in gt and not this custom one

	cout << message << endl;
	string msg(message);
	string x = "`4[hooktopia]`` " + msg;

	LogToConsole(x.c_str());

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)ltchook, &LogToConsoleHook); // re-attach for next time logtoconsole gets called
	DetourTransactionCommit();
}

DWORD WINAPI MainThread(HMODULE hModule)
{
	cout << "command line internal hooktopia - V3.37 - (C) 2020 playingo/DEERUX - Subscribe: " << endl;

	/*uintptr_t* addr1 = (uintptr_t*)(mBase + 0x574F08);
	uintptr_t* addr2 = (uintptr_t*)(*addr1 + 0x238);
	uintptr_t* addr3 = (uintptr_t*)(*addr2 + 0x80);
	uintptr_t* addr4 = (uintptr_t*)(*addr3 + 0x180);
	uintptr_t* addr5 = (uintptr_t*)(*addr4 + 0x0);
	uintptr_t* addr6 = (uintptr_t*)(*addr5 + 0x178);
	uintptr_t* addr7 = (uintptr_t*)(*addr6 + 0x1B0);
	skinAddress = (uintptr_t*)(*addr7 + 0x3D0);*/

	//cout << "Skin value is: " << *(int*)skinAddress << endl;

	/*ONLY CLIENT SIDED CALLS*/
	LogToConsole = (func*)(mBase + Addr_LogToConsole);
	func* SetFPSLimit = (func*)(mBase + Addr_SetFPSLimit);
	func* GetApp = (func*)(mBase + Addr_GetApp);

	func* LogWarningToConsole = (func*)(mBase + Addr_LogWarningToConsole);
	func* RequestJoinWorldCommon = (func*)(mBase + Addr_RequestJoinWorldCommon);
	func* OnTextOverlay = (func*)(mBase + Addr_OnTextOverlay);
	func* OnChangeSkin = (func*)(mBase + Addr_OnChangeSkin);
	func* OnConsoleMessage = (func*)(mBase + Addr_OnConsoleMessage);
	SendPacketRaw = (_SendPacketRaw)(mBase + Addr_SendPacketRaw);
	ProcessTankUpdatePacket = (_ProcessTankUpdatePacket)(mBase + Addr_ProcessTankUpdatePacket);

	/*ONLY CLIENT SIDED CALLS*/
	SendPacket = (_SendPackets)(mBase + Addr_SendPacket);
	App = GetApp();

	/*CALLS WHERE SERVER COMMUNICATION IS INVOLVED*/
	void* peer = (void*)(mBase + (App + 3984) + 200i64);
	
	/*CALLS WHERE SERVER COMMUNICATION IS INVOLVED*/

	cout << "Address of base: " << mBase << endl;
	cout << "Address of GetApp(): " << App << endl;

	while (!GetAsyncKeyState(VK_END))
	{
		Sleep(10);
		if (GetAsyncKeyState(VK_F1) & 1)
		{
			cout << "Calling LogToConsole..." << endl;
			LogToConsole("`4Hello from Internal!``");
			Sleep(100);
		}
		else if (GetAsyncKeyState(VK_F2) & 1)
		{
			string xf = "action|ping\n";
			SendPacket(0, xf, getpeer());
			SendPacket(0, xf, getpeer());
			string x = "action|ping";
			SendPacket(0, x, getpeer());
			SendPacket(0, x, getpeer());
			
		}
		else if (GetAsyncKeyState(VK_F3) & 1)
		{
			string xf = "action|input\n|text|/go";
			SendPacket(2, xf, getpeer());
			SendPacket(3, xf, getpeer());
			Sleep(100);
		}
		else if (GetAsyncKeyState(VK_F4) & 1) {
			string xf = "action|enter_game\n";
			SendPacket(2, xf, getpeer());
			SendPacket(3, xf, getpeer());
			Sleep(100);
		}
		else if (GetAsyncKeyState(VK_F5) & 1) {
			for (int i = 0; i < 10000; i++) {
				SendPacket(3, "action|quit_to_exit\n", getpeer());
			}
		}
		else if (GetAsyncKeyState(VK_F6) & 1) {
			string xf = "action|join_request\nname|VYTETEST\n";
			SendPacket(3, xf, getpeer());
			Sleep(100);
			// gonna spam it i removed sleep
		}
		else if (GetAsyncKeyState(VK_F7) & 1) 
		{
			TankPacketStruct pktStruct;
			pktStruct.packetType = 0;
			//pktStruct.characterState = -2;
			
			
			pktStruct.x = 64;
			pktStruct.y = 64;
			
			SendPacketRaw(4, ((byte*)&pktStruct), 56, 0, getpeer(), 1); // 1 = ENET_PACKET_FLAG_RELIABLE, a4 is ext data
			
			
		}
		else if (GetAsyncKeyState(VK_F8) & 1) 
		{
			TankPacketStruct pktStruct;
			pktStruct.value = currentDropIt++;
			pktStruct.packetType = 11;
			byte* raw = packTankStruct(&pktStruct);
			SendPacketRaw(4, raw, 56, 0, getpeer(), 1);
			delete[] raw;
		}
		else if (GetAsyncKeyState(VK_F9) & 1) 
		{
			SendPacket(2, "action|enter_game\n", getpeer());
			
		}
		else if (GetAsyncKeyState(VK_F10) & 1) 
		{
			if (currentClothing >= 9000) currentClothing = 0;
			TankPacketStruct pktStruct;
			pktStruct.value = currentClothing++;
			pktStruct.packetType = 10;
			byte* raw = packTankStruct(&pktStruct);
			SendPacketRaw(4, raw, 4000, 0, getpeer(), 1);
			delete[] raw;
		}
		else if (GetAsyncKeyState(VK_F11) & 1) 
		{
			SendPacket(2, "action|dialog_return\ndialog_name|\nbuttonClicked|yeslotto2", getpeer());
			OnTextOverlay(0, variant_t{ "Sending dialog packet..." });
		}
	}

	FreeConsole();
	FreeLibraryAndExitThread(hModule, 0);
	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		AllocConsole();
		freopen("CONOUT$", "w", stdout);

		mBase = (uintptr_t)GetModuleHandle(NULL);
		pHandle = GetCurrentProcess();

		DWORD oldprotect;
		VirtualProtectEx(pHandle, (LPVOID)(mBase + 0x1F4983), 2, PAGE_EXECUTE_READWRITE, &oldprotect);
		nop_((PVOID)(mBase + 0x1D30AE), 2);

		ltchook = (mBase + Addr_LogToConsole);
		sphook = (mBase + Addr_SendPacket);
		sprhook = (mBase + Addr_SendPacketRaw);
		ptuphook = (mBase + Addr_ProcessTankUpdatePacket);

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)ltchook, &LogToConsoleHook);
		DetourAttach(&(PVOID&)sphook, &SendPacketHook);
		DetourAttach(&(PVOID&)sprhook, &SendPacketRawHook);
		//DetourAttach(&(PVOID&)ptuphook, &ProcessTankUpdatePacketHook); //removed for several reasons
		DetourTransactionCommit();

		CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, nullptr);
		
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
