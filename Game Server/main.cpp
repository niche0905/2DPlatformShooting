#include "pch.h"
#include "ServerNetworkManager.h"

ServerNetworkManager SNMgr;

int main()
{
	cout << "Running Test" << endl;

	SNMgr.Init();
	SNMgr.Accept();

}