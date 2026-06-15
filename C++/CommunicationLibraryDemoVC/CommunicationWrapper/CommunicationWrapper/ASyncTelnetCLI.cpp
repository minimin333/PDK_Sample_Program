#include "stdafx.h"
#include "Common.h"

namespace ASyncTelnetWrapper {

	public ref class ASyncTelnetCLI
	{		
		static bool licensed;
		static deviceProperties^ currentDevProp = gcnew deviceProperties();
		static ManualResetEvent^ sync = gcnew ManualResetEvent(false);
		static IAsyncTerminalCommunicationInterface^ communication;
		static bool IsValidLicense = true;
		String^ m_defaultIPAddress = "192.168.0.200";
		String^ m_defaultPassword = "deltatau";
		String^ m_defaultPortNumber = "23";
		String^ m_defaultUser = "root";
		static String^ dataRecieved = "";
		int noOfCommandsSent;

		public: bool InitASync_Telnet()
		{
			try
			{
				if (IsValidLicense)
				{
					communication = Connect::CreateAsyncTerminal(CommunicationGlobals::ConnectionTypes::SSH, nullptr);
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

		public: bool ASync_TelnetInitDeviceEvent()
		{
			if (communication == nullptr)
			{
				return false;
			}
			auto devicePage = gcnew DevicePropertyPage(currentDevProp, communication->SocketConnected);
			devicePage->OnConnect += gcnew ODT::PowerPmacComLib::DevicePropertyPage::OnConnectFunction(this, &ASyncTelnetCLI::devicePage_OnConnect);
			devicePage->OnDisConnect += gcnew ODT::PowerPmacComLib::DevicePropertyPage::OnDisConnectFunction(this, &ASyncTelnetCLI::devicePage_OnDisConnect);
			devicePage->ShowDialog();
			return true;
		}

		public: bool devicePage_OnConnect(deviceProperties^ properties)
		{
			if (communication == nullptr)
			{
				return false;
			}

			communication = Connect::CreateAsyncTerminal(properties->Protocol, communication);
			auto bSuccess = communication->ConnectTerminal(properties->IPAddress, properties->PortNumber, properties->User, properties->Password);
			if (bSuccess)
			{
				currentDevProp->IPAddress = properties->IPAddress;
				currentDevProp->Password = properties->Password;
				currentDevProp->PortNumber = properties->PortNumber;
				currentDevProp->User = properties->User;
				communication->AsyncDataAvailable += gcnew ODT::PowerPmacComLib::AsyncDataReceiveEvent(this, &ASyncTelnetWrapper::ASyncTelnetCLI::OnAsyncDataAvailable);
			}
			return bSuccess;
		}

		public: void OnAsyncDataAvailable(System::Object ^sender, ODT::PowerPmacComLib::AsyncDataArgs ^e)
		{
			try
			{
				dataRecieved += e->Response;
				if (dataRecieved->Contains("#"))
					sync->Set();
			}
			catch (Exception^)
			{
				// ignored
			}
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
			communication->AsyncDataAvailable += gcnew ODT::PowerPmacComLib::AsyncDataReceiveEvent(this, &ASyncTelnetWrapper::ASyncTelnetCLI::OnAsyncDataAvailable);
			return bSuccess;
		}

		public: void AsyncCollectDevicePropertiesDataTelnet(String^ %response)
		{
			response = currentDevProp->IPAddress;
		}

		public: bool IsAsyncTelnetconnected()
		{
			if (communication->SocketConnected)
				return true;
			else
				return false;
		}

		public: int ASync_TelnetSendCommand(String^ commands, String^ %response)
		{
			if (communication == nullptr)
			{
				return INVALIDLICENSE;
			}

			if (communication->SocketConnected)
			{
				sync->Reset();
				noOfCommandsSent = 1;
				dataRecieved = "";

				auto communicationStatus = communication->AsyncSendCommand(commands);

				if (communicationStatus == Status::Ok)
				{
					if (sync->WaitOne(10000, false))
					{
						response = dataRecieved;
						return SUCCESS;
					}
					else
					{
						return FAILEDTOCONNECT;
					}
				}
				else
				{
					return FAILEDTOCONNECT;
				}
			}
			else
				return DEVICENOTCONNECT;
		}
	};
}

DllExport bool IInitASync_Telnet()
{
	ASyncTelnetWrapper::ASyncTelnetCLI m_Library;
	bool retval = m_Library.InitASync_Telnet();
	return retval;
}

DllExport bool IASync_TelnetInitDeviceEvent()
{
	ASyncTelnetWrapper::ASyncTelnetCLI m_Library;
	bool retval = m_Library.ASync_TelnetInitDeviceEvent();
	return retval;
}

DllExport bool IsAsyncTelnetconnected()
{
	ASyncTelnetWrapper::ASyncTelnetCLI m_Library;
	bool retval = m_Library.IsAsyncTelnetconnected();
	return retval;
}

DllExport void IAsyncCollectDevicePropertiesDataTelnet(const char** response)
{
	ASyncTelnetWrapper::ASyncTelnetCLI m_Library;
	String ^%str_1 = gcnew String(*response);
	m_Library.AsyncCollectDevicePropertiesDataTelnet(str_1);
	//*response = (const char*)(void*)Marshal::StringToHGlobalAnsi(str_1);
	IntPtr ptrTemp = Marshal::StringToHGlobalAnsi(str_1);
	*response = (const char*)(void*)ptrTemp;
	Marshal::FreeHGlobal(ptrTemp);
}

DllExport int IASync_TelnetSendCommand(const char* command, const char** response)
{
	ASyncTelnetWrapper::ASyncTelnetCLI m_Library;
	String^ l_command = gcnew String(command);
	String ^%str_1 = gcnew String(*response);
	int retval = m_Library.ASync_TelnetSendCommand(l_command, str_1);
	//*response = (const char*)(void*)Marshal::StringToHGlobalAnsi(str_1);
	IntPtr ptrTemp = Marshal::StringToHGlobalAnsi(str_1);
	*response = (const char*)(void*)ptrTemp;
	Marshal::FreeHGlobal(ptrTemp);
	return retval;
}