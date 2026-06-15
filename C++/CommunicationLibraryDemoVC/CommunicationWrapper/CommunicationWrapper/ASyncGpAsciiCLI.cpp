#include "stdafx.h"
#include "Common.h"

namespace ASyncGpAsciiWrapper {

	public ref class ASyncGpAsciiCLI
	{
		static ODT::PowerPmacComLib::IAsyncGpasciiCommunicationInterface^ asyncGpascii;
		static bool licensed;
		static deviceProperties^ currentDevProp = gcnew deviceProperties();
		static ManualResetEvent^ sync = gcnew ManualResetEvent(false);
		static IAsyncGpasciiCommunicationInterface^ communication;
		static bool IsValidLicense = true;
		String^ m_defaultIPAddress = "192.168.0.200";
		String^ m_defaultPassword = "deltatau";
		String^ m_defaultPortNumber = "23";
		String^ m_defaultUser = "root";
		char Ack = '\x06';
		static String^ dataRecieved = "";
		int noOfCommandsSent;

		public: bool InitASync_GpAscii()
		{
			try
			{
				if (IsValidLicense)	
				{
					communication = Connect::CreateAsyncGpascii(CommunicationGlobals::ConnectionTypes::SSH, nullptr);
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

		public: bool ASync_GpAsciiInitDeviceEvent()
		{
			if (communication == nullptr)
			{
				return false;
			}
			auto devicePage = gcnew DevicePropertyPage(currentDevProp, communication->GpAsciiConnected);
			devicePage->OnConnect += gcnew ODT::PowerPmacComLib::DevicePropertyPage::OnConnectFunction(this, &ASyncGpAsciiCLI::devicePage_OnConnect);
			devicePage->OnDisConnect += gcnew ODT::PowerPmacComLib::DevicePropertyPage::OnDisConnectFunction(this, &ASyncGpAsciiCLI::devicePage_OnDisConnect);
			devicePage->ShowDialog();
			return true;
		}

		public: bool devicePage_OnConnect(deviceProperties^ properties)
		{
			if (communication == nullptr)
			{
				return false;
			}

			communication = Connect::CreateAsyncGpascii(properties->Protocol, communication);
			auto bSuccess = communication->ConnectGPAscii(properties->IPAddress, properties->PortNumber, properties->User, properties->Password);
			if (bSuccess)
			{
				currentDevProp->IPAddress = properties->IPAddress;
				currentDevProp->Password = properties->Password;
				currentDevProp->PortNumber = properties->PortNumber;
				currentDevProp->User = properties->User;
				communication->AsyncDataAvailable += gcnew ODT::PowerPmacComLib::AsyncDataReceiveEvent(this, &ASyncGpAsciiWrapper::ASyncGpAsciiCLI::OnAsyncDataAvailable);

			}
			return bSuccess;
		}

		public: void OnAsyncDataAvailable(System::Object ^sender, ODT::PowerPmacComLib::AsyncDataArgs ^e)
		{
			try
			{
				dataRecieved += e->Response;
				auto noOfCommandsRecieved = 2;

				if (noOfCommandsRecieved > noOfCommandsSent)
				{
					sync->Set();
				}
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
			if (communication->GpAsciiConnected)
			{
				bSuccess = communication->DisconnectGpascii();

			}
			communication->AsyncDataAvailable += gcnew ODT::PowerPmacComLib::AsyncDataReceiveEvent(this, &ASyncGpAsciiWrapper::ASyncGpAsciiCLI::OnAsyncDataAvailable);
			return bSuccess;
		}

		public: void AsyncCollectDevicePropertiesData(String^ %response)
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

		public: int ASync_GpAsciiSendCommand(String^ commands, String^ %response)
		{
			if (communication == nullptr)
			{
				return INVALIDLICENSE;
			}

			if (communication->GpAsciiConnected)
			{
				sync->Reset();
				noOfCommandsSent = 1;
				dataRecieved = "";

				auto communicationStatus = communication->AsyncGetResponse(commands);

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

		public: int ASync_GpAsciiSendCommand(String^ commands, String^% response, int echoMode)
		{
			if (communication == nullptr)
			{
				return INVALIDLICENSE;
			}

			if (communication->GpAsciiConnected)
			{
				sync->Reset();
				noOfCommandsSent = 1;
				dataRecieved = "";

				auto communicationStatus = communication->AsyncGetResponse(commands, echoMode);

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

DllExport bool IAsyncCreateAsyncGpascii(const char ConnectionType, const char* prevObject)
{
	ASyncGpAsciiWrapper::ASyncGpAsciiCLI m_Library;
	bool retval = m_Library.InitASync_GpAscii();
	return retval;
}

DllExport bool IAsyncConnectGpAscii(const char* IPAddress, const char* PortNumber, const char* UserName, const char* Password)
{
	ASyncGpAsciiWrapper::ASyncGpAsciiCLI m_Library;
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

DllExport int IAsyncGetResponse(const char* command, const char** response)
{
	ASyncGpAsciiWrapper::ASyncGpAsciiCLI m_Library;
	String^ l_command = gcnew String(command);
	String^% str_1 = gcnew String(*response);
	int retval = m_Library.ASync_GpAsciiSendCommand(l_command, str_1);
	//*response = (const char*)(void*)Marshal::StringToHGlobalAnsi(str_1);
	IntPtr ptrTemp = Marshal::StringToHGlobalAnsi(str_1);
	*response = (const char*)(void*)ptrTemp;
	Marshal::FreeHGlobal(ptrTemp);
	return retval;
}

DllExport int IAsyncGetResponse(const char* command, const char** response, int echoMode)
{
	ASyncGpAsciiWrapper::ASyncGpAsciiCLI m_Library;
	String^ l_command = gcnew String(command);
	String^% str_1 = gcnew String(*response);
	int retval = m_Library.ASync_GpAsciiSendCommand(l_command, str_1, echoMode);
	//*response = (const char*)(void*)Marshal::StringToHGlobalAnsi(str_1);
	IntPtr ptrTemp = Marshal::StringToHGlobalAnsi(str_1);
	*response = (const char*)(void*)ptrTemp;
	Marshal::FreeHGlobal(ptrTemp);
	return retval;
}

DllExport bool IsAsyncGpAsciiconnected()
{
	ASyncGpAsciiWrapper::ASyncGpAsciiCLI m_Library;
	bool retval = m_Library.IsGpAsciiconnected();
	return retval;
}

DllExport void IAsyncCollectDevicePropertiesData(const char** response)
{
	ASyncGpAsciiWrapper::ASyncGpAsciiCLI m_Library;
	String ^%str_1 = gcnew String(*response);
	m_Library.AsyncCollectDevicePropertiesData(str_1);
	//*response = (const char*)(void*)Marshal::StringToHGlobalAnsi(str_1);
	IntPtr ptrTemp = Marshal::StringToHGlobalAnsi(str_1);
	*response = (const char*)(void*)ptrTemp;
	Marshal::FreeHGlobal(ptrTemp);
}

DllExport bool IASync_GpAsciiInitDeviceEvent()
{
	ASyncGpAsciiWrapper::ASyncGpAsciiCLI m_Library;
	bool retval = m_Library.ASync_GpAsciiInitDeviceEvent();
	return retval;
}
