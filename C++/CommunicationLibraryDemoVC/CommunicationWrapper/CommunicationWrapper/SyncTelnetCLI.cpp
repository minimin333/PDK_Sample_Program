#include "stdafx.h"
#include "Common.h"

namespace SyncTelnetWrapper {

	public ref class SyncTelnetCLI
	{
		static ODT::PowerPmacComLib::ISyncTerminalCommunicationInterface^ syncGpascii;
		static bool licensed;
		static deviceProperties^ currentDevProp = gcnew deviceProperties();
		static ISyncTerminalCommunicationInterface^ communication;
		static bool IsValidLicense = true;
		String^ m_defaultIPAddress = "192.168.0.200";
		String^ m_defaultPassword = "deltatau";
		String^ m_defaultPortNumber = "23";
		String^ m_defaultUser = "root";

		public: bool InitSync_Telnet()
		{
			try
			{
				if (IsValidLicense)
				{
					communication = Connect::CreateSyncTerminal(CommunicationGlobals::ConnectionTypes::SSH, nullptr);
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

		public: bool Sync_TelnetInitDeviceEvent()
		{
			if (communication == nullptr)
			{
				return false;
			}
			auto devicePage = gcnew DevicePropertyPage(currentDevProp, communication->SocketConnected);
			devicePage->OnConnect += gcnew ODT::PowerPmacComLib::DevicePropertyPage::OnConnectFunction(this, &SyncTelnetCLI::devicePage_OnConnect);
			devicePage->OnDisConnect += gcnew ODT::PowerPmacComLib::DevicePropertyPage::OnDisConnectFunction(this, &SyncTelnetCLI::devicePage_OnDisConnect);
			devicePage->ShowDialog();
			return true;
		}

		public: bool devicePage_OnConnect(deviceProperties^ properties)
		{
			if (communication == nullptr)
			{
				return false;
			}

			communication = Connect::CreateSyncTerminal(properties->Protocol, communication);
			auto bSuccess = communication->ConnectTerminal(properties->IPAddress, properties->PortNumber, properties->User, properties->Password);
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
			if (communication->SocketConnected)
			{
				bSuccess = communication->DisconnectTerminal();

			}
			return bSuccess;
		}

		public: void CollectDevicePropertiesData(String^ %response)
		{
			response = currentDevProp->IPAddress;
		}

		public: bool IsTelnetconnected()
		{
			if (communication->SocketConnected)
				return true;
			else
				return false;
		}

		public: int Sync_TelnetSendCommand(String^ commands, String^ %response)
		{
			if (communication == nullptr)
			{
				return INVALIDLICENSE;
			}

			if (communication->SocketConnected)
			{
				auto communicationStatus = communication->SendCommand(commands, response);

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
		public:void SendCommand(String^ cmdparam, String^% response)
		{
			communication->SendCommand(cmdparam, response);
			response = gcnew System::String(response);
		}
	};
}

DllExport bool IInitSync_Telnet()
{
	SyncTelnetWrapper::SyncTelnetCLI m_Library;
	bool retval = m_Library.InitSync_Telnet();
	return retval;
}

DllExport bool IsTelnetconnected()
{
	SyncTelnetWrapper::SyncTelnetCLI m_Library;
	bool retval = m_Library.IsTelnetconnected();
	return retval;
}

DllExport bool ISync_TelnetInitDeviceEvent()
{
	SyncTelnetWrapper::SyncTelnetCLI m_Library;
	bool retval = m_Library.Sync_TelnetInitDeviceEvent();
	return retval;
}

DllExport void ICollectDevicePropertiesDataTelnet(const char** response)
{
	SyncTelnetWrapper::SyncTelnetCLI m_Library;
	String ^%str_1 = gcnew String(*response);
	m_Library.CollectDevicePropertiesData(str_1);
	//*response = (const char*)(void*)Marshal::StringToHGlobalAnsi(str_1);
	IntPtr ptrTemp = Marshal::StringToHGlobalAnsi(str_1);
	*response = (const char*)(void*)ptrTemp;
	Marshal::FreeHGlobal(ptrTemp);
}

DllExport int ISync_TelnetSendCommand(const char* command, const char** response)
{
	//SyncTelnetWrapper::SyncTelnetCLI m_Library;
	//String^ l_command = gcnew String(command);
	//String ^%str_1 = gcnew String(*response);
	//int retval = m_Library.Sync_TelnetSendCommand(l_command, str_1);
	//*response = (const char*)(void*)Marshal::StringToHGlobalAnsi(str_1);
	//Marshal::FreeHGlobal(IntPtr((void*)*response));
	SyncTelnetWrapper::SyncTelnetCLI m_Library;
	String^ l_cmdparam = gcnew String(command);
	String^% str_1 = gcnew String(*response);
	m_Library.SendCommand(l_cmdparam, str_1);
	*response = (char*)(void*)Marshal::StringToHGlobalAnsi(str_1);

	return 1;
}