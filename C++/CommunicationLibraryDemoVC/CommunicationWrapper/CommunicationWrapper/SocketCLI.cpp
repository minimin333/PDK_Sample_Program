#include "stdafx.h"
#include "Common.h"

using namespace System::Threading;
using namespace System::Threading::Tasks;

namespace SocketWrapper {

	public ref class SocketDemo
	{
		//static ODT::PowerPmacComLib::ISyncGpasciiCommunicationInterface^ syncGpascii;
		static ODT::PowerPmacComLib::ITCPSocketCommunicationInterface^ socketCom;
		static bool licensed;
		//static deviceProperties^ currentDevProp = gcnew deviceProperties();
		//static ISyncGpasciiCommunicationInterface^ communication;
		static ITCPSocketCommunicationInterface^ communication;
		static bool IsValidLicense = true;
		String^ m_defaultIPAddress = "192.168.0.200";
		String^ m_defaultPassword = "deltatau";
		String^ m_defaultPortNumber = "1027";
		String^ m_defaultUser = "root";

	public: bool InitSocket()
	{
		try
		{
			if (IsValidLicense)
			{
				//communication = Connect::CreateSyncGpascii(CommunicationGlobals::ConnectionTypes::SSH, nullptr);
				socketCom = Connect::CreateSocketClient(socketCom);
				IsValidLicense = true;
			}
		}
		catch (Exception^ ex)
		{
			IsValidLicense = false;
			return false;
		}
		return true;
	}

	public: bool Connect_Socket(String^ ipaddress, int port, int timeoutMS)
	{
		socketCom = Connect::CreateSocketClient(socketCom);
		Task<int>^ connected = socketCom->connectToServer(ipaddress, port, timeoutMS);
		connected->Wait();
		return true;
	}

	public: bool DisConnect_Socket()
	{
		socketCom = Connect::CreateSocketClient(socketCom);
		int connected = socketCom->disconnectFromServer();
		return true;
	}


	public: bool DartSetUserMemChar(int address, int numVars, String^ strData)
	{
		String^ strDataNew = strData->Replace("\r\n", "+");
		array<String^>^ ValueToWrite = strDataNew->Split('+');
		int arrLength = ValueToWrite->Length;
		Task<bool>^ result = socketCom->DartSetUserMemChar(address, arrLength, ValueToWrite);
		result->Wait();
		return result->Result;
	}



	public: bool DartSetUserMemShort(int address, int numVars, String^ strData)
	{
		String^ strDataNew = strData->Replace("\r\n", "+");
		array<String^>^ ValueToWrite = strDataNew->Split('+');
		int arrLength = ValueToWrite->Length;
		Task<bool>^ result = socketCom->DartSetUserMemShort(address, arrLength, ValueToWrite);
		result->Wait();
		return result->Result;
	}

	public: bool DartSetUserMemInt(int address, int numVars, String^ strData)
	{
		String^ strDataNew = strData->Replace("\r\n", "+");
		array<String^>^ ValueToWrite = strDataNew->Split('+');
		int arrLength = ValueToWrite->Length;
		Task<bool>^ result = socketCom->DartSetUserMemInt(address, arrLength, ValueToWrite);
		result->Wait();
		return result->Result;
	}

	public: bool DartSetUserMemFloat(int address, int numVars, String^ strData)
	{
		String^ strDataNew = strData->Replace("\r\n", "+");
		array<String^>^ ValueToWrite = strDataNew->Split('+');
		int arrLength = ValueToWrite->Length;
		Task<bool>^ result = socketCom->DartSetUserMemFloat(address, arrLength, ValueToWrite);
		result->Wait();
		return result->Result;
	}

	public: bool DartSetUserMemDouble(int address, int numVars, String^ strData)
	{
		String^ strDataNew = strData->Replace("\r\n", "+");
		array<String^>^ ValueToWrite = strDataNew->Split('+');
		int arrLength = ValueToWrite->Length;
		Task<bool>^ result = socketCom->DartSetUserMemDouble(address, arrLength, ValueToWrite);
		result->Wait();
		return result->Result;
	}

	public: bool DartSetUserMemPvar(int address, int numVars, String^ strData)
	{
		String^ strDataNew = strData->Replace("\r\n", "+");
		array<String^>^ ValueToWrite = strDataNew->Split('+');
		int arrLength = ValueToWrite->Length;
		Task<bool>^ result = socketCom->DartSetUserMemPvar(address, arrLength, ValueToWrite);
		result->Wait();
		return result->Result;
	}

	public: array<String^>^ DartGetUserMemChar(int address, int numVars)
	{
		Task <array<String^>^>^ result = socketCom->DartGetUserMemChar(address, numVars);
		result->Wait();
		return result->Result;
	}


	public: array<short>^ DartGetUserMemShort(int address, int numVars)
	{
		Task <array<short>^>^ result = socketCom->DartGetUserMemShort(address, numVars);
		result->Wait();
		return result->Result;
	}

	public: array<int>^ DartGetUserMemInt(int address, int numVars)
	{
		Task <array<int>^>^ result = socketCom->DartGetUserMemInt(address, numVars);
		result->Wait();
		return result->Result;
	}

	public: array<float>^ DartGetUserMemFloat(int address, int numVars)
	{
		Task <array<float>^>^ result = socketCom->DartGetUserMemFloat(address, numVars);
		result->Wait();
		return result->Result;
	}

	public: array<double>^ DartGetUserMemDouble(int address, int numVars)
	{
		Task <array<double>^>^ result = socketCom->DartGetUserMemDouble(address, numVars);
		result->Wait();
		return result->Result;
	}

	public: array<String^>^ DartGetUserMemPvar(int address, int numVars)
	{
		Task <array<String^>^>^ result = socketCom->DartGetUserMemPvar(address, numVars);
		result->Wait();
		return result->Result;
	}


	};
}

DllExport bool IInitSocket()
{
	SocketWrapper::SocketDemo m_Library;
	bool retval = m_Library.InitSocket();
	return retval;
}

DllExport bool IConnect_Socket(const char* ipaddress, int port, int timeoutMS)
{
	SocketWrapper::SocketDemo m_Library;
	String^ l_ipaddress = gcnew String(ipaddress);
	bool retval = m_Library.Connect_Socket(l_ipaddress, port, timeoutMS);
	return retval;
}

DllExport bool IDisConnect_Socket()
{
	SocketWrapper::SocketDemo m_Library;
	bool retval = m_Library.DisConnect_Socket();
	return retval;
}


DllExport bool IDartSetUserMemChar(int address, int numVars, const char* strData)
{
	SocketWrapper::SocketDemo m_Library;
	String^ l_strData = gcnew String(strData);
	bool retval = m_Library.DartSetUserMemChar(address, numVars, l_strData);
	return retval;
}

DllExport bool IDartSetUserMemShort(int address, int numVars, const char* strData)
{
	SocketWrapper::SocketDemo m_Library;
	String^ l_strData = gcnew String(strData);
	bool retval = m_Library.DartSetUserMemShort(address, numVars, l_strData);
	return retval;
}

DllExport bool IDartSetUserMemInt(int address, int numVars, const char* strData)
{
	SocketWrapper::SocketDemo m_Library;
	String^ l_strData = gcnew String(strData);
	bool retval = m_Library.DartSetUserMemInt(address, numVars, l_strData);
	return retval;
}

DllExport bool IDartSetUserMemFloat(int address, int numVars, const char* strData)
{
	SocketWrapper::SocketDemo m_Library;
	String^ l_strData = gcnew String(strData);
	bool retval = m_Library.DartSetUserMemFloat(address, numVars, l_strData);
	return retval;
}

DllExport bool IDartSetUserMemDouble(int address, int numVars, const char* strData)
{
	SocketWrapper::SocketDemo m_Library;
	String^ l_strData = gcnew String(strData);
	bool retval = m_Library.DartSetUserMemDouble(address, numVars, l_strData);
	return retval;
}

DllExport bool IDartSetUserMemPvar(int address, int numVars, const char* strData)
{
	SocketWrapper::SocketDemo m_Library;
	String^ l_strData = gcnew String(strData);
	bool retval = m_Library.DartSetUserMemPvar(address, numVars, l_strData);
	return retval;
}

DllExport bool IDartGetUserMemChar(int address, int numVars, const char** response)
{
	SocketWrapper::SocketDemo m_Library;
	array<String^>^ readresponse = m_Library.DartGetUserMemChar(address, numVars);

	String^% str_1 = gcnew String(*response);
	for (int i = 0; i < readresponse->Length; i++)
	{
		str_1 += readresponse[i] + "\r\n";

	}

	*response = (const char*)(void*)Marshal::StringToHGlobalAnsi(str_1);

	return true;
}


DllExport bool IDartGetUserMemShort(int address, int numVars, const char** response)
{
	SocketWrapper::SocketDemo m_Library;
	array<short>^ readresponse = m_Library.DartGetUserMemShort(address, numVars);
	String^% str_1 = gcnew String(*response);

	for (int i = 0; i < readresponse->Length; i++)
	{
		str_1 += readresponse[i] + "\r\n";

	}

	*response = (const char*)(void*)Marshal::StringToHGlobalAnsi(str_1);
	return true;
}

DllExport bool IDartGetUserMemInt(int address, int numVars, const char** response)
{
	SocketWrapper::SocketDemo m_Library;
	array<int>^ readresponse = m_Library.DartGetUserMemInt(address, numVars);
	String^% str_1 = gcnew String(*response);

	for (int i = 0; i < readresponse->Length; i++)
	{
		str_1 += readresponse[i] + "\r\n";

	}

	*response = (const char*)(void*)Marshal::StringToHGlobalAnsi(str_1);
	return true;
}

DllExport bool IDartGetUserMemFloat(int address, int numVars, const char** response)
{
	SocketWrapper::SocketDemo m_Library;
	array<float>^ readresponse = m_Library.DartGetUserMemFloat(address, numVars);
	String^% str_1 = gcnew String(*response);

	for (int i = 0; i < readresponse->Length; i++)
	{
		str_1 += readresponse[i] + "\r\n";

	}

	*response = (const char*)(void*)Marshal::StringToHGlobalAnsi(str_1);
	return true;
}

DllExport bool IDartGetUserMemDouble(int address, int numVars, const char** response)
{
	SocketWrapper::SocketDemo m_Library;
	array<double>^ readresponse = m_Library.DartGetUserMemDouble(address, numVars);
	String^% str_1 = gcnew String(*response);

	for (int i = 0; i < readresponse->Length; i++)
	{
		str_1 += readresponse[i] + "\r\n";

	}

	*response = (const char*)(void*)Marshal::StringToHGlobalAnsi(str_1);
	return true;
}

DllExport bool IDartGetUserMemPvar(int address, int numVars, const char** response)
{
	SocketWrapper::SocketDemo m_Library;
	array<String^>^ readresponse = m_Library.DartGetUserMemPvar(address, numVars);
	String^% str_1 = gcnew String(*response);

	for (int i = 0; i < readresponse->Length; i++)
	{
		str_1 += readresponse[i] + "\r\n";

	}

	*response = (const char*)(void*)Marshal::StringToHGlobalAnsi(str_1);
	return true;
}

