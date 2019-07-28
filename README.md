# 텍스트 파서 클래스
## 📢 개요
 파싱이란 어떠한 형태의 파일을 읽어서 다른 프로그램이 사용할 수 있는 데이터로 변환하는 것이다.
 
 이를 이용하는 예시로 게임을 들 수 있는데 게임 내에서는 캐릭터의 능력치, 아이템 종류와 수치, 캐릭터 대사, 설정 등 많은 데이터가 사용된다. 
 
 이러한 데이터들은 하드코딩을 할 수도 있지만 이렇게 할 경우, 데이터를 추가하거나 수정 또는 제거할 때 매번 새로 컴파일을 해야한다. 프로젝트가 커지면 커질수록 컴파일하는데 소요되는 시간이 증가하기 마련인데 조작할 때마다 컴파일을 한다면 여간 불편한게 아닐 것이다.
 따라서, 자주 변경되는 데이터들은 별도의 외부 파일에 입력하여 이를 읽어 사용하는 것이 여러모로 편리하다.
 
 파서에서 가장 중요한 부분은 문자열을 1Byte씩 읽으면서 일일히 비교 검사하는 작업이다. 데이터의 항목을 찾아 그 값을 얻으려면 불필요한 문자를 제거하고 일일이 단어 단위로 분리하여 항목의 이름과 비교하는 과정이 필요하다.
 
 이를 구현할 때 C언어에 문자를 검색하는 strtok()이라는 함수가 있지만 사용하지 않는다. 불필요한 비교 과정을 생략하여 속도를 빠르게 하고 코드의 유연성을 확보하기 위함이다.
 
 

## 💻 Text Parser
  서버에서 계속 주시해야 할 주요 항목들을 1초 주기로 모니터링하는 예제

  ![capture](https://github.com/kbm0996/-Utility-CParser/blob/master/figure/text.png)
  
  **figure 1. RawData*
  
  ![capture](https://github.com/kbm0996/-Utility-CParser/blob/master/figure/run.png)
  
  **figure 2. Result*
 
 
## 🅿 사용법 및 예제 코드

- **원본 데이터**
 
 Config.ini(텍스트 파일)
 
 	 :: TEST_ZONE
	 {
		Version = 014
		ServerID = 2
		ServerBindIP = "192.168.11.29"
		ServerBindPort = 50003
		//ServerBindPort = 50001
		WorkerThread = 33
		MaxUser = 3003
	 }

- **코드**

```cpp
	int iVersion, iServerID, iServerBindPort, iWorkerThread, iMaxUser;
	char szServerBindIP[256];
	CParser g_Parser;
	if (g_Parser.LoadFile("Config.ini"))
	{
		if (g_Parser.SearchField("TEST_ZONE"))
		{
			g_Parser.GetValue("Version", &iVersion);
			g_Parser.GetValue("ServerID", &iServerID);
			g_Parser.GetValue("ServerBindIP", szServerBindIP, sizeof(szServerBindIP));
			g_Parser.GetValue("ServerBindPort", &iServerBindPort);
			g_Parser.GetValue("WorkerThread", &iWorkerThread);
			g_Parser.GetValue("MaxUser", &iMaxUser);
      
			printf("\nTEST_ZONE\n\n");
			printf("*Version	= %d\n", iVersion);
			printf("*ServerID	= %d\n", iServerID);
			printf("*ServerBindIP	= \"%s\" \n", szServerBindIP);
			printf("*ServerBindPort	= %d\n", iServerBindPort);
			printf("*WorkerThread	= %d\n", iWorkerThread);
			printf("*MaxUser	= %d\n", iMaxUser);
		}
	}
