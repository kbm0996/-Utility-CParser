#include <cstdio>
#include "CParser.h"


void main()
{
	int iVersion, iServerID, iServerBindPort, iWorkerThread, iMaxUser;
	char szServerBindIP[256];
	WCHAR wszServerBindIP[256];

	mylib::CParser Parser("test.ini");
	//if (Parser.LoadFile("test.ini"))
	{
		if (Parser.SearchField("TEST_ZONE"))
		{
			Parser.GetValue("Version", &iVersion);
			Parser.GetValue("ServerID", &iServerID);
			Parser.GetValue("ServerBindIP", szServerBindIP, sizeof(szServerBindIP));
			Parser.GetValue("ServerBindPort", &iServerBindPort);
			Parser.GetValue("WorkerThread", &iWorkerThread);
			Parser.GetValue("MaxUser", &iMaxUser);

			printf("\nTEST_ZONE\n\n");
			printf("*Version	= %d\n", iVersion);
			printf("*ServerID	= %d\n", iServerID);
			printf("*ServerBindIP	= \"%s\" \n", szServerBindIP);
			printf("*ServerBindPort	= %d\n", iServerBindPort);
			printf("*WorkerThread	= %d\n", iWorkerThread);
			printf("*MaxUser	= %d\n", iMaxUser);
		}


		if (Parser.SearchField("TEST_ZONE_2"))
		{
			Parser.GetValue("Version", &iVersion);
			Parser.GetValue("ServerID", &iServerID);
			Parser.GetValue("ServerBindIP", szServerBindIP, sizeof(szServerBindIP));
			Parser.GetValue("ServerBindPort", &iServerBindPort);
			Parser.GetValue("WorkerThread", &iWorkerThread);
			Parser.GetValue("MaxUser", &iMaxUser);

			printf("\nTEST_ZONE_2\n\n");
			printf("*Version	= %d\n", iVersion);
			printf("*ServerID	= %d\n", iServerID);
			printf("*ServerBindIP	= \"%s\" \n", szServerBindIP);
			printf("*ServerBindPort	= %d\n", iServerBindPort);
			printf("*WorkerThread	= %d\n", iWorkerThread);
			printf("*MaxUser	= %d\n", iMaxUser);
		}

		if (Parser.SearchField())
		{
			Parser.GetValue("Version", &iVersion);
			Parser.GetValue("ServerID", &iServerID);
			Parser.GetValue("ServerBindIP", wszServerBindIP, sizeof(szServerBindIP));
			Parser.GetValue("ServerBindPort", &iServerBindPort);
			Parser.GetValue("WorkerThread", &iWorkerThread);
			Parser.GetValue("MaxUser", &iMaxUser);

			printf("\nNO_NAME\n\n");
			printf("*Version	= %d\n", iVersion);
			printf("*ServerID	= %d\n", iServerID);
			wprintf(L"*ServerBindIP	= \"%s\" \n", wszServerBindIP);
			printf("*ServerBindPort	= %d\n", iServerBindPort);
			printf("*WorkerThread	= %d\n", iWorkerThread);
			printf("*MaxUser	= %d\n", iMaxUser);
		}

		return;
	}
}