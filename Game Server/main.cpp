#include "pch.h"
#include "ServerNetworkManager.h"

ServerNetworkManager SNMgr;
GunLoader GunInfo;

int main()
{
	cout << "Running Test" << endl;
	cout << "Gun Test" << GunInfo.gun_table[0] << endl;

	SNMgr.Init();
	SNMgr.Accept();

}