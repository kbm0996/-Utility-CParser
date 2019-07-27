#include "CParser.h"

mylib::CParser::CParser()
{
	_pRawData = nullptr;
	_iFileSize = _iFieldBeginPos = _iFieldEndPos = _iCurPos = 0;
}

mylib::CParser::CParser(const char * szFileName)
{
	if (!LoadFile(szFileName))
	{
		_pRawData = nullptr;
		_iFileSize = _iFieldBeginPos = _iFieldEndPos = _iCurPos = 0;
	}
}

mylib::CParser::~CParser()
{
	if (_pRawData != nullptr)
		delete[] _pRawData;
}

bool mylib::CParser::LoadFile(const char * szFileName)
{
	if (_pRawData != nullptr)
		delete[] _pRawData;

	FILE* pFile;
	fopen_s(&pFile, szFileName, "rt");
	if (pFile == NULL)
		return false;

	// Get File Size
	fseek(pFile, 0, SEEK_END);
	_iFileSize = _iFieldEndPos = ftell(pFile);
	fseek(pFile, 0, SEEK_SET); 
	///rewind(pFile); // fseek()�� �޸� ���Ͻ�Ʈ���� ���� ǥ�ñ�(error indicators)�� ����� ������ �̵� ���� ���θ� �������� �ʿ�

	// Get File Contents
	_pRawData = new char[_iFileSize];
	fread(_pRawData, _iFileSize, 1, pFile); // pFile�κ��� �� ��(1)�� _iFileSizeũ�� ��ŭ �о� _pRawData�� �Է�

	fclose(pFile);
	return true;
}

bool mylib::CParser::SearchField(const char * szFieldName)
{
	char* chpBuff;
	char chWord[en_TOKEN_MAX_LEN];
	int	iLength;

	_iCurPos = _iFieldBeginPos = 0;
	_iFileSize = _iFieldEndPos;

	// �ʵ� �̸� ���Է½�
	if (szFieldName == nullptr)
		return true;

	iLength = 0;
	chpBuff = _pRawData + _iCurPos;
	
	// ��ū Ž��
	while (GetNextToken(&chpBuff, &iLength))
	{
		memset(chWord, 0, en_TOKEN_MAX_LEN);
		memcpy(chWord, chpBuff, iLength);
		chpBuff = _pRawData + _iCurPos;
		
		// ������("::") Ž��
		if (0 == strcmp(chWord, "::"))
		{
			if (GetNextToken(&chpBuff, &iLength))
			{
				memset(chWord, 0, en_TOKEN_MAX_LEN);
				memcpy(chWord, chpBuff, iLength);
				chpBuff = _pRawData + _iCurPos;

				// �ʵ�� Ž��
				if (0 == strcmp(szFieldName, chWord))
				{
					if (GetNextToken(&chpBuff, &iLength))
					{
						memset(chWord, 0, en_TOKEN_MAX_LEN);
						memcpy(chWord, chpBuff, iLength);
						chpBuff = _pRawData + _iCurPos;

						// �ʵ� ���� ���� Ž��
						if (chWord[0] == '{')
						{
							_iFieldBeginPos = _iCurPos;
							while (GetNextToken(&chpBuff, &iLength))
							{
								memset(chWord, 0, en_TOKEN_MAX_LEN);
								memcpy(chWord, chpBuff, iLength);
								chpBuff = _pRawData + _iCurPos;
								
								// �ʵ� ���� ���� Ž��
								if (chWord[0] == '}')
								{
									_iFieldEndPos = _iCurPos;
									return true;
								}
							}
						}
					}					
				}
			}
		}
	}
	_iFieldBeginPos = 0;
	_iFieldEndPos = _iFileSize;
	return false;
}

bool mylib::CParser::GetValue(const char* szKey, int* pOutBuf)
{
	char* chpBuff;
	char chWord[en_TOKEN_MAX_LEN];
	int	iLength = 0;
	_iCurPos = _iFieldBeginPos;
	chpBuff = _pRawData + _iCurPos;
	
	while (GetNextToken(&chpBuff, &iLength))
	{
		memset(chWord, 0, en_TOKEN_MAX_LEN);
		memcpy(chWord, chpBuff, iLength);
		chpBuff = _pRawData + _iCurPos;
		if (0 == strcmp(szKey, chWord))
		{
			if (GetNextToken(&chpBuff, &iLength))
			{
				memset(chWord, 0, en_TOKEN_MAX_LEN);
				memcpy(chWord, chpBuff, iLength);
				chpBuff = _pRawData + _iCurPos;
				if (0 == strcmp(chWord, "="))
				{
					if (GetNextToken(&chpBuff, &iLength))
					{
						memset(chWord, 0, en_TOKEN_MAX_LEN);
						memcpy(chWord, chpBuff, iLength);
						*pOutBuf = atoi(chWord);

						return true;
					}
				}
			}
			break;
		}
	}
	return false;
}

bool mylib::CParser::GetValue(const char* szKey, char* pOutBuf, int iOutBufSize)
{
	char* chpBuff;
	char chWord[en_TOKEN_MAX_LEN];
	int	iLength = 0;
	_iCurPos = _iFieldBeginPos;
	chpBuff = _pRawData + _iCurPos;

	while (GetNextToken(&chpBuff, &iLength))
	{
		memset(chWord, 0, en_TOKEN_MAX_LEN);
		memcpy(chWord, chpBuff, iLength);
		chpBuff = _pRawData + _iCurPos;
		if (0 == strcmp(szKey, chWord))
		{
			if (GetNextToken(&chpBuff, &iLength))
			{
				memset(chWord, 0, en_TOKEN_MAX_LEN);
				memcpy(chWord, chpBuff, iLength);
				chpBuff = _pRawData + _iCurPos;
				if (0 == strcmp(chWord, "="))
				{
					if (GetNextToken(&chpBuff, &iLength))
					{
						iLength = iOutBufSize < iLength ? iOutBufSize : iLength;
						memset(pOutBuf, 0, iOutBufSize);
						memcpy(pOutBuf, chpBuff, iLength);

						return true;
					}
				}
			}
			break;
		}
	}
	return false;
}

bool mylib::CParser::GetValue(const char * szKey, WCHAR * pOutBuf, int iOutBufSize)
{
	char* chpBuff;
	char chWord[en_TOKEN_MAX_LEN];
	int	iLength = 0;
	_iCurPos = _iFieldBeginPos;
	chpBuff = _pRawData + _iCurPos;

	while (GetNextToken(&chpBuff, &iLength))
	{
		memset(chWord, 0, en_TOKEN_MAX_LEN);
		memcpy(chWord, chpBuff, iLength);
		if (0 == strcmp(szKey, chWord))
		{
			if (GetNextToken(&chpBuff, &iLength))
			{
				memset(chWord, 0, en_TOKEN_MAX_LEN);
				memcpy(chWord, chpBuff, iLength);
				if (0 == strcmp(chWord, "="))
				{
					if (GetNextToken(&chpBuff, &iLength))
					{
						iLength = iOutBufSize < iLength ? iOutBufSize : iLength;
						memset(pOutBuf, 0, iOutBufSize);

						// UTF-8 -> Unicode
						if (MultiByteToWideChar(CP_UTF8, 0, chpBuff, iLength, pOutBuf, iLength) == 0)
							break;

						return true;
					}
				}
			}
			break;
		}
	}
	return false;
}

bool mylib::CParser::SkipNoneCommand()	//�����̽�, ��, �����ڵ�, �ּ� ó��.
{
	char* chpBuff = _pRawData + _iCurPos;

	while (1)
	{
		if (*chpBuff == ',' || *chpBuff == '"' ||
			*chpBuff == en_TOKEN_SPACE || *chpBuff == en_TOKEN_BACKSPACE ||
			*chpBuff == en_TOKEN_TAB || *chpBuff == en_TOKEN_LF ||
			*chpBuff == en_TOKEN_CR)
		{
			++_iCurPos;
			++chpBuff;
			continue;
		}

		if (*chpBuff == '/')
		{
			++_iCurPos;
			++chpBuff;
			while (1)
			{
				////////////////////////////////////////
				if (*chpBuff == '/')	// "//" ����
				{
					while (1)
					{
						++_iCurPos;
						++chpBuff;
						if (*chpBuff == en_TOKEN_LF || *chpBuff == en_TOKEN_CR)
						{
							++_iCurPos;
							++chpBuff;
							if (*chpBuff == '/')
								continue;
							break;
						}
					}
					++_iCurPos;
					++chpBuff;
				}
				////////////////////////////////////////
				if (*chpBuff == '*')	// "/*" ����
				{
					while (1)
					{
						++_iCurPos;
						++chpBuff;
						if (*chpBuff == '*')
						{
							++_iCurPos;
							++chpBuff;
							if (*chpBuff == '/')	// "*/" ����
							{
								++_iCurPos;
								++chpBuff;
								break;
							}
						}
					}
					++_iCurPos;
					++chpBuff;
				}
				else
					break;
			}
		}
		else
		{
			return true;
		}

		// ������ �� or �ʵ��� ��
		if (_iCurPos > _iFileSize || _iCurPos > _iFieldEndPos)
			break;
	}

	return false;
}

bool mylib::CParser::GetNextToken(char** chppBuffer, int* pOutSize)
{
	char* pToken;

	if (!SkipNoneCommand())
		return false;

	*chppBuffer = _pRawData + _iCurPos;
	pToken = *chppBuffer;
	*pOutSize = 0;

	while (1)
	{
		if (*pToken == ',' || *pToken == '"' ||
			*pToken == en_TOKEN_SPACE || *pToken == en_TOKEN_BACKSPACE ||
			*pToken == en_TOKEN_TAB || *pToken == en_TOKEN_LF ||
			*pToken == en_TOKEN_CR)
			break;

		++pToken;
		++_iCurPos;
		++(*pOutSize);

		// ������ �� or �ʵ��� ��
		if (_iCurPos > _iFileSize || _iCurPos > _iFieldEndPos)
			break;
	}
	return true;
}