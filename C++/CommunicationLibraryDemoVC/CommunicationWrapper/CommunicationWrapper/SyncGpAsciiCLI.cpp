#include "stdafx.h"
#include "Common.h"
#include <string>
#include <vector>
//#include <msclr/marshal_cppstd.h>

namespace SyncGpAsciiWrapper {

	public ref class SyncGpAsciiCLI
	{
		static ODT::PowerPmacComLib::ISyncGpasciiCommunicationInterface^ syncGpascii;
		static bool licensed;
		static deviceProperties^ currentDevProp = gcnew deviceProperties();
		static ISyncGpasciiCommunicationInterface^ communication;
		static bool IsValidLicense = true;
		String^ m_defaultIPAddress = "192.168.0.200";
		String^ m_defaultPassword = "deltatau";
		String^ m_defaultPortNumber = "23";
		String^ m_defaultUser = "root";

		public: bool InitSync_GpAscii()
		{
			try
			{
				if (IsValidLicense)
				{
					communication = Connect::CreateSyncGpascii(CommunicationGlobals::ConnectionTypes::SSH, nullptr);
					IsValidLicense = true;
				}
			}
			catch (Exception^ ex)
			{
				IsValidLicense = false;
				return false;				
			}
			currentDevProp->IPAddress = m_defaultIPAddress;
			currentDevProp->Password = m_defaultPassword;
			currentDevProp->PortNumber = Int32::Parse(m_defaultPortNumber);
			currentDevProp->User = m_defaultUser;
			currentDevProp->Protocol = CommunicationGlobals::ConnectionTypes::SSH;
			return true;
		}

		public: bool Sync_GpAsciiInitDeviceEvent()
		{
			if (communication == nullptr)
			{
				return false;
			}
			auto devicePage = gcnew DevicePropertyPage(currentDevProp, communication->GpAsciiConnected);
			devicePage->OnConnect += gcnew ODT::PowerPmacComLib::DevicePropertyPage::OnConnectFunction(this, &SyncGpAsciiCLI::devicePage_OnConnect);
			devicePage->OnDisConnect += gcnew ODT::PowerPmacComLib::DevicePropertyPage::OnDisConnectFunction(this, &SyncGpAsciiCLI::devicePage_OnDisConnect);
			devicePage->ShowDialog();
			return true;
		}

		public: bool devicePage_OnConnect(deviceProperties^ properties)
		{
			if (communication == nullptr)
			{
				return false;
			}

			communication = Connect::CreateSyncGpascii(properties->Protocol, communication);
			auto bSuccess = communication->ConnectGpAscii(properties->IPAddress, properties->PortNumber, properties->User, properties->Password);
			if (bSuccess)
			{
				currentDevProp->IPAddress = properties->IPAddress;
				currentDevProp->Password = properties->Password;
				currentDevProp->PortNumber = properties->PortNumber;
				currentDevProp->User = properties->User;

			}
			return bSuccess;
		}

		public: bool devicePage_OnDisConnect()
		{
			if (communication == nullptr)
			{
				return false;
			}

			auto bSuccess = false;
			if (communication->GpAsciiConnected)
			{
				bSuccess = communication->DisconnectGpascii();

			}
			return bSuccess;
		}

		public: void CollectDevicePropertiesData(String^ %response)
		{
			response = currentDevProp->IPAddress;
		}

		public: bool IsGpAsciiconnected()
		{
			if (communication->GpAsciiConnected)
				return true;
			else
				return false;
		}

		public: int Sync_GpAsciiSendCommand(String^ commands, String^ %response)
		{
			if (communication == nullptr)
			{
				return INVALIDLICENSE;
			}

			if (communication->GpAsciiConnected)
			{
				auto communicationStatus = communication->GetResponse(commands, response);

				if (communicationStatus == Status::Ok)
				{
					return SUCCESS;
				}
				else
				{
					return FAILEDTOCONNECT;
				}
			}
			else
				return DEVICENOTCONNECT;
		}
		public: int Sync_GpAsciiSendCommand(String^ commands, String^% response, int echoMode)
		{
			if (communication == nullptr)
			{
				return INVALIDLICENSE;
			}

			if (communication->GpAsciiConnected)
			{
				auto communicationStatus = communication->GetResponse(commands, response, echoMode);

				if (communicationStatus == Status::Ok)
				{
					return SUCCESS;
				}
				else
				{
					return FAILEDTOCONNECT;
				}
			}
			else
				return DEVICENOTCONNECT;
		}
		public: int Sync_GpAsciiSendCommand(List<String^>^ commands, [OutAttribute] List<String^>^% response)
		{
			if (communication == nullptr)
			{
				return INVALIDLICENSE;
			}

			if (communication->GpAsciiConnected)
			{
				auto communicationStatus = communication->GetResponse(commands, response);

				if (communicationStatus == Status::Ok)
				{
					return SUCCESS;
				}
				else
				{
					return FAILEDTOCONNECT;
				}
			}
			else
				return DEVICENOTCONNECT;
		}
		public: int Sync_GpAsciiSendCommand(List<String^>^ commands, [OutAttribute] List<String^>^% response, int echoMode)
		{
			if (communication == nullptr)
			{
				return INVALIDLICENSE;
			}

			if (communication->GpAsciiConnected)
			{
				auto communicationStatus = communication->GetResponse(commands, response, echoMode);

				if (communicationStatus == Status::Ok)
				{
					return SUCCESS;
				}
				else
				{
					return FAILEDTOCONNECT;
				}
			}
			else
				return DEVICENOTCONNECT;
		}
		public: int Sync_GpAsciiSendCommand(List<String^>^ commands, bool parseData, [OutAttribute] List<String^>^% response)
		{
			if (communication == nullptr)
			{
				return INVALIDLICENSE;
			}

			if (communication->GpAsciiConnected)
			{
				auto communicationStatus = communication->GetResponse(commands, parseData, response);

				if (communicationStatus == Status::Ok)
				{
					return SUCCESS;
				}
				else
				{
					return FAILEDTOCONNECT;
				}
			}
			else
				return DEVICENOTCONNECT;
		}
		public: int Sync_GpAsciiSendCommand(List<String^>^ commands, bool parseData, [OutAttribute] List<String^>^% response, int echoMode)
		{
			if (communication == nullptr)
			{
				return INVALIDLICENSE;
			}

			if (communication->GpAsciiConnected)
			{
				auto communicationStatus = communication->GetResponse(commands, parseData, response, echoMode);

				if (communicationStatus == Status::Ok)
				{
					return SUCCESS;
				}
				else
				{
					return FAILEDTOCONNECT;
				}
			}
			else
				return DEVICENOTCONNECT;
		}
		public:	  std::string ConvertStringToString(String^ str)
		{
			using namespace System;
			using namespace Runtime::InteropServices;

			const char* chars = (const char*)(Marshal::StringToHGlobalAnsi(str)).ToPointer();
			std::string result(chars);
			Marshal::FreeHGlobal(IntPtr((void*)chars));


			return result;
		}
	};
}

DllExport bool ISyncCreateSyncGpascii(const char ConnectionType, const char* prevObject)
{
	SyncGpAsciiWrapper::SyncGpAsciiCLI m_Library;
	bool retval = m_Library.InitSync_GpAscii();
	return retval;
}

DllExport bool ISyncConnectGpAscii(const char* IPAddress, const char* PortNumber, const char* UserName, const char* Password)
{
	SyncGpAsciiWrapper::SyncGpAsciiCLI m_Library;
	deviceProperties^ tempDeviceProperties = gcnew deviceProperties();
	String^ m_defaultIPAddress = gcnew String(IPAddress);
	String^ m_defaultPortNumber = gcnew String(PortNumber);
	String^ m_defaultUser = gcnew String(UserName);
	String^ m_defaultPassword = gcnew String(Password);

	tempDeviceProperties->IPAddress = m_defaultIPAddress;
	tempDeviceProperties->Password = m_defaultPassword;
	tempDeviceProperties->PortNumber = Int32::Parse(m_defaultPortNumber);
	tempDeviceProperties->User = m_defaultUser;
	tempDeviceProperties->Protocol = CommunicationGlobals::ConnectionTypes::SSH;
	bool retval = m_Library.devicePage_OnConnect(tempDeviceProperties);
	return retval;
}

DllExport bool ISyncDisConnectGpAscii()
{
	SyncGpAsciiWrapper::SyncGpAsciiCLI m_Library;
	bool retval = m_Library.devicePage_OnDisConnect();
	return retval;
}

DllExport int ISyncGetResponse(const char* command, const char** response)
{
	static std::string s_responseStr;
	SyncGpAsciiWrapper::SyncGpAsciiCLI m_Library;
	System::String^ l_command = gcnew System::String(command);
	System::String^ str_1;

	int retval = m_Library.Sync_GpAsciiSendCommand(l_command, str_1, 3);
	System::IntPtr ptrTemp = System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(str_1);
	s_responseStr = static_cast<const char*>(ptrTemp.ToPointer());
	System::Runtime::InteropServices::Marshal::FreeHGlobal(ptrTemp);
	*response = s_responseStr.c_str();
	return retval;
}

DllExport int ISyncGetResponse(const char* command, const char** response, int echoMode)
{
	static std::string s_responseStr;  
	SyncGpAsciiWrapper::SyncGpAsciiCLI m_Library;
	System::String^ l_command = gcnew System::String(command);
	System::String^ str_1;

	int retval = m_Library.Sync_GpAsciiSendCommand(l_command, str_1,3);
	System::IntPtr ptrTemp = System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(str_1);
	s_responseStr = static_cast<const char*>(ptrTemp.ToPointer()); 
	*response = s_responseStr.c_str();
	System::Runtime::InteropServices::Marshal::FreeHGlobal(ptrTemp);
	return retval;
}

DllExport int ISyncGetResponse(std::vector<std::string>& command, std::vector<std::string>& response)
{
	SyncGpAsciiWrapper::SyncGpAsciiCLI m_Library;
	List<String^>^ l_command = gcnew List<String^>();
	for (const auto& cmd : command)
	{
		l_command->Add(gcnew String(cmd.c_str()));
	}
	List<String^>^ str_1 = gcnew List<String^>();
	for (const auto& res : response)
	{
		str_1->Add(gcnew String(res.c_str()));
	}
	int retval = m_Library.Sync_GpAsciiSendCommand(l_command, str_1);
	response.clear();
	for each (String ^ str in str_1)
	{
		response.push_back(m_Library.ConvertStringToString(str));
	}
	return retval;
}

DllExport int  ISyncGetResponse(const std::vector<std::string>& command, std::vector<std::string>& response, int echoMode)
{
	SyncGpAsciiWrapper::SyncGpAsciiCLI m_Library;
	List<String^>^ l_command = gcnew List<String^>();
	for (const auto& cmd : command)
	{
		l_command->Add(gcnew String(cmd.c_str()));
	}
	List<String^>^ str_1 = gcnew List<String^>();
	for (const auto& res : response)
	{
		str_1->Add(gcnew String(res.c_str()));
	}
	int retval = m_Library.Sync_GpAsciiSendCommand(l_command, str_1, echoMode);
	response.clear();
	for each (String ^ str in str_1)
	{
		response.push_back(m_Library.ConvertStringToString(str));
	}
	return retval;
}

DllExport int  ISyncGetResponse(const std::vector<std::string>& command, bool parseData, std::vector<std::string>& response)
{
	SyncGpAsciiWrapper::SyncGpAsciiCLI m_Library;
	List<String^>^ l_command = gcnew List<String^>();
	for (const auto& cmd : command)
	{
		l_command->Add(gcnew String(cmd.c_str()));
	}
	List<String^>^ str_1 = gcnew List<String^>();
	for (const auto& res : response)
	{
		str_1->Add(gcnew String(res.c_str()));
	}
	int retval = m_Library.Sync_GpAsciiSendCommand(l_command, parseData, str_1);
	response.clear();
	for each (String ^ str in str_1)
	{
		response.push_back(m_Library.ConvertStringToString(str));
	}
	return retval;
}

DllExport int  ISyncGetResponse(const std::vector<std::string>& command, bool parseData, std::vector<std::string>& response, int echoMode)
{
	SyncGpAsciiWrapper::SyncGpAsciiCLI m_Library;
	List<String^>^ l_command = gcnew List<String^>();
	for (const auto& cmd : command)
	{
		l_command->Add(gcnew String(cmd.c_str()));
	}
	List<String^>^ str_1 = gcnew List<String^>();
	for (const auto& res : response)
	{
		str_1->Add(gcnew String(res.c_str()));
	}
	int retval = m_Library.Sync_GpAsciiSendCommand(l_command, parseData, str_1, echoMode);
	response.clear();
	for each (String ^ str in str_1)
	{
		response.push_back(m_Library.ConvertStringToString(str));
	}
	return retval;
}

DllExport bool IsGpAsciiconnected()
{
	SyncGpAsciiWrapper::SyncGpAsciiCLI m_Library;
	bool retval = m_Library.IsGpAsciiconnected();
	return retval;
}

DllExport void ICollectDevicePropertiesData(const char** response)
{
	SyncGpAsciiWrapper::SyncGpAsciiCLI m_Library;
	String^% str_1 = gcnew String(*response);
	m_Library.CollectDevicePropertiesData(str_1);
	//*response = (const char*)(void*)Marshal::StringToHGlobalAnsi(str_1);
	IntPtr ptrTemp = Marshal::StringToHGlobalAnsi(str_1);
	*response = (const char*)(void*)ptrTemp;
	Marshal::FreeHGlobal(ptrTemp);
}

DllExport bool ISyncGpAsciiInitDeviceEvent()
{
	SyncGpAsciiWrapper::SyncGpAsciiCLI m_Library;
	return m_Library.Sync_GpAsciiInitDeviceEvent();
}