#include "utils.h"

extern DuiLib::CApplicationUI* g_app;

void utils::TCharToChar(const TCHAR* tchar, char* _char)
{
	{
		int iLength;
		//获取字节长度   
		iLength = WideCharToMultiByte(CP_ACP, 0, tchar, -1, NULL, 0, NULL, NULL);
		//将tchar值赋给_char    
		WideCharToMultiByte(CP_ACP, 0, tchar, -1, _char, iLength, NULL, NULL);
	}
}
