#include "stdafx.h"
#include "Common.h"

namespace UnsolicitedEventWrapper {

	public ref class UnsolicitedEventCLI
	{
		static IUnsolicitedEventInterface^ communication;
		static deviceProperties^ currentDevProp = gcnew deviceProperties();
		static bool bStartErrorsEvent = true;
		static bool IsValidLicense = true;
		String^ m_defaultIPAddress = "192.168.0.200";
		String^ m_defaultPassword = "deltatau";
		String^ m_defaultPortNumber = "23";
		String^ m_defaultUser = "root";
		static String^ dataRecieved = "";

		public: bool InitUnsolicitedEvent()
		{
			try
			{
				if (IsValidLicense)
				{
					communication = Connect::CreateUnsolicitedEvent(CommunicationGlobals::ConnectionTypes::SSH, nullptr);
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

		public: bool UnsolicitedDeviceEvent()
		{
			if (communication == nullptr)
			{
				return false;
			}

			auto devicePage = gcnew DevicePropertyPage(currentDevProp, communication->SocketConnected);
			devicePage->OnConnect += gcnew ODT::PowerPmacComLib::DevicePropertyPage::OnConnectFunction(this, &UnsolicitedEventCLI::devicePage_OnConnect);
			devicePage->OnDisConnect += gcnew ODT::PowerPmacComLib::DevicePropertyPage::OnDisConnectFunction(this, &UnsolicitedEventCLI::devicePage_OnDisConnect);
			devicePage->ShowDialog();
			return true;
		}

		public: bool devicePage_OnConnect(deviceProperties^ properties)
		{
			if (communication == nullptr)
			{
				return false;
			}

			communication = Connect::CreateUnsolicitedEvent(properties->Protocol, communication);
			auto bSuccess = communication->ConnectEvent(properties->IPAddress, properties->PortNumber, properties->User, properties->Password);
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
				communication->StopUnsolicitedEvent();
				bSuccess = communication->DisconnectEvent();
			}
			
			communication->UResponse -= gcnew ODT::PowerPmacComLib::UnsolicitedResponse(this, &UnsolicitedEventWrapper::UnsolicitedEventCLI::communication_UResponse);
			
			return bSuccess;
		}
				
		private: void communication_UResponse(System::Object ^sender, ODT::PowerPmacComLib::UnsolicitedArgs^ e)
        {
            try
            {
				dataRecieved = e->UnsolicitedMessage;
            }
            catch (Exception^)
            {
                // ignored
            }
        }

		public: void CollectDevicePropertiesData(String^ %response)
		{
			response = currentDevProp->IPAddress;
		}

		public: void GetErrorResponse(String^ %response)
		{
			response = dataRecieved;
		}

		public: bool IsConnected()
		{
			if (communication->SocketConnected)
				return true;
			else
				return false;
		}

		public: int StartUnsolicitedEvent(bool p0, bool p1, bool p2, bool p3, bool p4)
		{
			int retVal = -1;
			if (communication == nullptr)
			{
				return INVALIDLICENSE;
			}
			if (communication->SocketConnected)
			{				
				communication->UResponse += gcnew ODT::PowerPmacComLib::UnsolicitedResponse(this, &UnsolicitedEventWrapper::UnsolicitedEventCLI::communication_UResponse);
				communication->Buffer0 = p0;
				communication->Buffer1 = p1;
				communication->Buffer2 = p2;
				communication->Buffer3 = p3;
				communication->Buffer4 = p4;

				auto st = communication->StartUnsolicitedEvent();
				switch (st)
				{
					case ODT::PowerPmacComLib::UnsolicitedStatus::Ok:
						retVal = SUCCESS;
						break;					
					case ODT::PowerPmacComLib::UnsolicitedStatus::TimeOut:
						retVal = UNSOLICITEDTIMEOUT;
						break;					
				}
				return retVal;
			}
			else
			{				
				return DEVICENOTCONNECT;
			}
		}

		public: int StopUnsolicitedEvent()
		{
			if (communication == nullptr)
			{
				return INVALIDLICENSE;
			}
			if (communication->SocketConnected)
			{
				communication->UResponse -= gcnew ODT::PowerPmacComLib::UnsolicitedResponse(this, &UnsolicitedEventWrapper::UnsolicitedEventCLI::communication_UResponse);
				communication->StopUnsolicitedEvent();				
			}
			else
			{
				return DEVICENOTCONNECT;
			}
		}

		public: int SendCommandUnsolicited(String^ portNumber, String^ command)
		{
			int retCode = -1;
			if (communication == nullptr)
			{
				return INVALIDLICENSE;				
			}
			if (communication->SocketConnected)
			{
				auto portnum = -1;

				try
				{
					portnum = Convert::ToInt16(portNumber);
				}
				catch(Exception^)
				{
					return PORTNOTAVAILABLE;					
				}

				if (portnum != -1)
				{
					auto st = communication->SendString(portnum, command);
					switch (st)
					{
						case UnsolicitedStatus::BufferNotDefined:
							retCode = BUFFERNOTDEFINED;							
							break;
						case UnsolicitedStatus::Exception:
							retCode = SOMEEXCEPTION;							
							break;
						case UnsolicitedStatus::Failed:
							break;
						case UnsolicitedStatus::False:
							break;
						case UnsolicitedStatus::NotConnected:
							retCode = DEVICENOTCONNECT;							
							break;
						case UnsolicitedStatus::TimeOut:
							retCode = UNSOLICITEDTIMEOUT;							
							break;
						case UnsolicitedStatus::UnsolicitedNotStarted:
							retCode = EVENTNOTSTARTED;
							break;
						case UnsolicitedStatus::Ok:
							retCode = SUCCESS;
							break;
					}
				}
				return retCode;
			}
			else
			{
				return DEVICENOTCONNECT;
			}

		}
	};
}

DllExport bool IInitUnsolicitedEvent()
{
	UnsolicitedEventWrapper::UnsolicitedEventCLI m_Library;
	bool retval = m_Library.InitUnsolicitedEvent();
	return retval;
}

DllExport bool IUnsolicitedDeviceEvent()
{
	UnsolicitedEventWrapper::UnsolicitedEventCLI m_Library;
	bool retval = m_Library.UnsolicitedDeviceEvent();
	return retval;
}

DllExport bool IsUnsolicitedEventConnected()
{
	UnsolicitedEventWrapper::UnsolicitedEventCLI m_Library;
	bool retval = m_Library.IsConnected();
	return retval;
}

DllExport void ICollectDevicePropertiesDataUnsolicited(const char** response)
{
	UnsolicitedEventWrapper::UnsolicitedEventCLI m_Library;
	String ^%str_1 = gcnew String(*response);
	m_Library.CollectDevicePropertiesData(str_1);
	//*response = (const char*)(void*)Marshal::StringToHGlobalAnsi(str_1);
	IntPtr ptrTemp = Marshal::StringToHGlobalAnsi(str_1);
	*response = (const char*)(void*)ptrTemp;
	Marshal::FreeHGlobal(ptrTemp);
}

DllExport int IStartUnsolicitedEvent(bool p0, bool p1, bool p2, bool p3, bool p4)
{
	UnsolicitedEventWrapper::UnsolicitedEventCLI m_Library;
	int retval = m_Library.StartUnsolicitedEvent(p0, p1, p2, p3, p4);
	return retval;
}

DllExport int IStopUnsolicitedEvent()
{
	UnsolicitedEventWrapper::UnsolicitedEventCLI m_Library;
	int retval = m_Library.StopUnsolicitedEvent();
	return retval;
}

DllExport int ISendCommandUnsolicited(const char* portnumber, const char* command)
{
	UnsolicitedEventWrapper::UnsolicitedEventCLI m_Library;
	String^ l_portnumber = gcnew String(portnumber);
	String^ l_command = gcnew String(command);	
	int retval = m_Library.SendCommandUnsolicited(l_portnumber, l_command);
	return retval;
}

DllExport void IGetUnsolicitedResponse(const char** response)
{
	UnsolicitedEventWrapper::UnsolicitedEventCLI m_Library;
	String ^%str_1 = gcnew String(*response);
	m_Library.GetErrorResponse(str_1);
	//*response = (const char*)(void*)Marshal::StringToHGlobalAnsi(str_1);
	IntPtr ptrTemp = Marshal::StringToHGlobalAnsi(str_1);
	*response = (const char*)(void*)ptrTemp;
	Marshal::FreeHGlobal(ptrTemp);
}
