
#include "stdafx.h"
#include "Common.h"


namespace ErrorEventWrapper {

	public ref class ErrorMask
	{
		public:
			String^ GetErrMasks;
			bool UpdateMask;
			unsigned long Csmask;
			unsigned long Globalmask;
			unsigned long Macromask;
			unsigned long Mtrmask;
	};

	public ref class ErrorsEventCLI
	{
		static ODT::PowerPmacComLib::ISyncGpasciiCommunicationInterface^ gpAsciiClient;
		static bool licensed;
		static bool connected;
		static deviceProperties^ currentDevProp = gcnew deviceProperties();
		ErrorEventWrapper::ErrorMask^ myerrmask = gcnew ErrorEventWrapper::ErrorMask;
		static IGetErrorsEventInterface^ communication;
		static bool IsValidLicense = true;
		String^ m_defaultIPAddress = "192.168.0.200";
		String^ m_defaultPassword = "deltatau";
		String^ m_defaultPortNumber = "23";
		String^ m_defaultUser = "root";	
		static String^ dataRecieved = "";
		static array<String^> ^motorStatusItems;
		static array<String^> ^coordStatusItems;
		static array<String^> ^sysStatusItems;
		static array<String^> ^ringStatusItems;
		static unsigned long defCSMask = 2105340;
		static unsigned long defGlobalMask = 4294967295;
		static unsigned long defMACROMask = 30;
		static unsigned long defMotorMask = 12591101;
		static int defUpdatePeriod = 100;
		int length = 6;
		static array<String^> ^dataarray = gcnew array<String^>(6);
		static int index = 0;

		public: bool Init_ErrorEvents()
		{			
			try
			{
				if (IsValidLicense)
				{
					communication = Connect::CreateGetErrorsEvent(CommunicationGlobals::ConnectionTypes::SSH, nullptr);
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
		
		public: bool InitDeviceErrorsEvent()
		{
			if (communication == nullptr)
			{
				return false;
			}
			auto devicePage = gcnew DevicePropertyPage(currentDevProp, communication->SocketConnected);
			devicePage->OnConnect += gcnew ODT::PowerPmacComLib::DevicePropertyPage::OnConnectFunction(this, &ErrorEventWrapper::ErrorsEventCLI::devicePage_OnConnect);
			devicePage->OnDisConnect += gcnew ODT::PowerPmacComLib::DevicePropertyPage::OnDisConnectFunction(this, &ErrorsEventCLI::devicePage_OnDisConnect);
			devicePage->ShowDialog();
			return true;
		}

		private: bool devicePage_OnConnect(deviceProperties^ properties)
		{
			if (communication == nullptr)
			{
				return false;
			}

			communication = Connect::CreateGetErrorsEvent(properties->Protocol, communication);
			auto bSuccess = communication->ConnectEvent(properties->IPAddress, properties->PortNumber, properties->User, properties->Password);
			connected = bSuccess;
			if (bSuccess)
			{
				currentDevProp->IPAddress = properties->IPAddress;
				currentDevProp->Password = properties->Password;
				currentDevProp->PortNumber = properties->PortNumber;
				currentDevProp->User = properties->User;
				GetCurrentStatusWords();

			}
			return bSuccess;
		}

		private: bool devicePage_OnDisConnect()
		{
			if (communication == nullptr)
			{
				return false;
			}

			auto bSuccess = false;
			if (communication->SocketConnected)
			{
				communication->StopGetErrorsEvent();
				bSuccess = communication->DisconnectEvent();

			}
			return bSuccess;
		}

		public: void CollectDevicePropertiesData(String^ %response)
		{
			response = currentDevProp->IPAddress;						
		}

		public: bool Isconnected()
		{
			if (communication->SocketConnected)
				return true;
			else
				return false;
		}

		public: bool CheckConnectFlag()
		{
			if (connected)
				return true;
			else
				return false;
		}

		public: void GetCurrentStatusWords()
		{
			gpAsciiClient = Connect::CreateSyncGpascii(currentDevProp->Protocol, gpAsciiClient);
			if (connected)
			{
				if (gpAsciiClient != nullptr)
				{
					gpAsciiClient->ConnectGpAscii(currentDevProp->IPAddress, currentDevProp->PortNumber, currentDevProp->User,
						currentDevProp->Password);
					if (gpAsciiClient->GpAsciiConnected)
					{
						array<wchar_t> ^id = {'\n'};
						String^ command = "Backup Motor[1].Status";
						String^ response;
						gpAsciiClient->GetResponse(command, response);
						motorStatusItems = response->Split(id,StringSplitOptions::RemoveEmptyEntries);

						command = "Backup Coord[1].Status";
						gpAsciiClient->GetResponse(command, response);
						coordStatusItems = response->Split(id, StringSplitOptions::RemoveEmptyEntries);

						command = "Backup Sys.Status";
						gpAsciiClient->GetResponse(command, response);
						sysStatusItems = response->Split(id, StringSplitOptions::RemoveEmptyEntries);

						command = "Backup macro.status[0]";
						gpAsciiClient->GetResponse(command, response);
						ringStatusItems = response->Split(id, StringSplitOptions::RemoveEmptyEntries);

						gpAsciiClient->DisconnectGpascii();
					}
				}
			}
			
		}

		public: bool ConnectGpasciiDevice()
		{
			gpAsciiClient = Connect::CreateSyncGpascii(currentDevProp->Protocol, gpAsciiClient);
			if (connected)
			{
				if (gpAsciiClient != nullptr)
				{
					gpAsciiClient->ConnectGpAscii(currentDevProp->IPAddress, currentDevProp->PortNumber, currentDevProp->User,
						currentDevProp->Password);
					if (gpAsciiClient->GpAsciiConnected)
					{
						return true;
					}
				}
			}			
			return false;
		}

		public: void GetResponsefromDevice(String^ type, String^ %response)
		{
			if (gpAsciiClient)
				gpAsciiClient->GetResponse(type, response);
		}

		public: void DisconnectGpasciiDevice()
		{
			if (gpAsciiClient)
				gpAsciiClient->DisconnectGpascii();
		}

		public: int StartErrorsEvent()
		{			
			if (communication == nullptr)
			{
				return INVALIDLICENSE;
			}
			if (communication->SocketConnected)
			{	
				index = 0;
				dataarray = gcnew array<String^>(length);
				communication->ErrorsResponse += gcnew ODT::PowerPmacComLib::GetErrorsResponse(this, &ErrorEventWrapper::ErrorsEventCLI::OnErrorsResponse);
				communication->StartGetErrorsEvent();
				return ERRORSEVENTSTART;
			}
			else
			{
				return DEVICENOTCONNECT;
			}
		}

		public: int StopErrorsEvent()
		{
			if (communication == nullptr)
			{
				return INVALIDLICENSE;
			}
			if (communication->SocketConnected)
			{
				communication->ErrorsResponse -= gcnew ODT::PowerPmacComLib::GetErrorsResponse(this, &ErrorEventWrapper::ErrorsEventCLI::OnErrorsResponse);
				communication->ErrorsMask -= gcnew ODT::PowerPmacComLib::GetErrorsMask(this, &ErrorEventWrapper::ErrorsEventCLI::OnErrorsMask);
				communication->StopGetErrorsEvent();
				//dataarray = gcnew array<String^>(length);
				//index = 0;
				return ERRORSEVENTSTOP;
			}
			else
			{
				return DEVICENOTCONNECT;
			}
		}

		public: void GetErrorData(String^ %first, String^ %second, String^ %third, String^ %fourth, String^ %five, String^ %six)
		{
			first = dataarray[0];
			second = dataarray[1];
			third = dataarray[2];
			fourth = dataarray[3];
			five = dataarray[4];
			six = dataarray[5];
		}

		public: void GetErrorResponse(String^ %response)
		{
			response = dataRecieved;
		}

		public: void OnErrorsResponse(System::Object ^sender, ODT::PowerPmacComLib::GetErrorsArgs ^e)
		{			
			try
			{				
				//dataRecieved = e->ErrorMessage;	
				dataarray->SetValue(e->ErrorMessage,index);
				index++;
			}
			catch (Exception^)
			{
				// ignored
			}
		}

		public: void OnErrorsMask(System::Object ^sender, ODT::PowerPmacComLib::ErrorsMaskArgs ^e)
		{
			try
			{
				unsigned long mask;
				switch (e->ErrorType)
				{
				case 'm':
					mask = e->ErrorMask;
					if (myerrmask->Mtrmask != mask)
					{
						//this.AppendTextToOutPut(string.Format(this.motorMaskMismatchFormat, this.myerrmask.Mtrmask, mask));
						myerrmask->Mtrmask = mask;
					}
					break;
				case 'c':
					mask = e->ErrorMask;
					if (myerrmask->Csmask != mask)
					{
						//this.AppendTextToOutPut(string.Format(this.motorMaskMismatchFormat, this.myerrmask.Csmask, mask));
						myerrmask->Csmask = mask;
					}
					break;
				case 'g':
					mask = e->ErrorMask;
					if (myerrmask->Globalmask != mask)
					{
						//AppendTextToOutPut(string.Format(this.motorMaskMismatchFormat, this.myerrmask.Globalmask, mask));
						myerrmask->Globalmask = mask;
					}
					break;
				case 'r':
					mask = e->ErrorMask;
					if (myerrmask->Macromask != mask)
					{
						//AppendTextToOutPut(string.Format(this.motorMaskMismatchFormat, this.myerrmask.Macromask, mask));
						myerrmask->Macromask = mask;
					}
					break;
				}
			}
			catch (Exception^)
			{
				 
			}
		}

		public: int SetMask_CS()
		{			
			if (!connected)
			{
				return DEVICENOTCONNECT;
			}
			else
			{				
				MaskForm^ mskForm = gcnew MaskForm("CustomCSMask", coordStatusItems);
				mskForm->Show();
				return SUCCESS;
			}
		}

		public: int SetMask_Global()
		{
			if (!connected)
			{
				return DEVICENOTCONNECT;
			}
			else
			{
				MaskForm^ mskForm = gcnew MaskForm("CustomGlobalMask", coordStatusItems);
				mskForm->Show();
				return SUCCESS;
			}
		}

		public: int SetMask_Macro()
		{
			if (!connected)
			{
				return DEVICENOTCONNECT;
			}
			else
			{
				MaskForm^ mskForm = gcnew MaskForm("CustomMACROMask", coordStatusItems);
				mskForm->Show();
				return SUCCESS;
			}
		}

		public: int SetMask_Motor()
		{
			if (!connected)
			{
				return DEVICENOTCONNECT;
			}
			else
			{
				MaskForm^ mskForm = gcnew MaskForm("CustomMotorMask", coordStatusItems);
				mskForm->Show();
				return SUCCESS;
			}
		}

		public: void UpdateMaskValueDefault()
		{
			myerrmask->Csmask = defCSMask;
			myerrmask->Globalmask = defGlobalMask;
			myerrmask->Macromask = defMACROMask;
			myerrmask->Mtrmask = defMotorMask;
			UpdateValues(defUpdatePeriod);
		}

		public: void UpdateMaskValue(String^ %txt_CS, String^ %txt_Global, String^ %txt_Macro, String^ %txt_Motor)
		{			
			myerrmask->Csmask = Convert::ToUInt64(MaskForm::CustomCsMask, 16);
			myerrmask->Globalmask = Convert::ToUInt64(MaskForm::CustomGlobalMask, 16);
			myerrmask->Macromask = Convert::ToUInt64(MaskForm::CustomMacroMask, 16);
			myerrmask->Mtrmask = Convert::ToUInt64(MaskForm::CustomMotorMask, 16);
			myerrmask->UpdateMask = UpdateMaskWords(myerrmask->Mtrmask, myerrmask->Csmask, myerrmask->Globalmask,
				myerrmask->Macromask, myerrmask->GetErrMasks);
			UpdateValues(defUpdatePeriod);
			txt_Motor = "0x" + myerrmask->Mtrmask.ToString("X16");
			txt_CS = "0x" + myerrmask->Csmask.ToString("X16");
			txt_Global = "0x" + myerrmask->Globalmask.ToString("X16");
			txt_Macro = "0x" + myerrmask->Macromask.ToString("X16");
		}

		public: void UpdateValues(int updateRate)
		{
			if (communication != nullptr)
			{
				communication->UpdatePeriod = ((float)updateRate) / 1000;
				communication->ErrMaskWords[0] = "0x" + myerrmask->Mtrmask.ToString("X16");
				communication->ErrMaskWords[1] = "0x" + myerrmask->Csmask.ToString("X16");
				communication->ErrMaskWords[2] = "0x" + myerrmask->Globalmask.ToString("X16");
				communication->ErrMaskWords[3] = "0x" + myerrmask->Macromask.ToString("X16");

				communication->ErrorsMask += gcnew ODT::PowerPmacComLib::GetErrorsMask(this, &ErrorEventWrapper::ErrorsEventCLI::OnErrorsMask);
				communication->ErrorsResponse -= gcnew ODT::PowerPmacComLib::GetErrorsResponse(this, &ErrorEventWrapper::ErrorsEventCLI::OnErrorsResponse);
				communication->StopGetErrorsEvent();
				communication->ErrorsResponse += gcnew ODT::PowerPmacComLib::GetErrorsResponse(this, &ErrorEventWrapper::ErrorsEventCLI::OnErrorsResponse);
				communication->StartGetErrorsEvent();

				communication->ErrMaskWords[0] = "0x" + myerrmask->Mtrmask.ToString("X16");
				communication->ErrMaskWords[1] = "0x" + myerrmask->Csmask.ToString("X16");
				communication->ErrMaskWords[2] = "0x" + myerrmask->Globalmask.ToString("X16");
				communication->ErrMaskWords[3] = "0x" + myerrmask->Macromask.ToString("X16");

				try
				{
					myerrmask->Mtrmask = Convert::ToUInt64(communication->ErrMaskWords[0], 16);
					myerrmask->Csmask = Convert::ToUInt64(communication->ErrMaskWords[1], 16);
					myerrmask->Globalmask = Convert::ToUInt64(communication->ErrMaskWords[2], 16);
					myerrmask->Macromask = Convert::ToUInt64(communication->ErrMaskWords[3], 16);
				}
				catch (Exception^)
				{
					// ignored
				}
			}
		}

		private: bool UpdateMaskWords(unsigned long mtrM, unsigned long csM, unsigned long globalM, unsigned long macroM, String^ %newMask)
		{
			newMask = "";
			try
			{
				myerrmask->GetErrMasks = "-c0x" + csM.ToString("X") + " -m0x" + mtrM.ToString("X") + " -g0x" + globalM.ToString("X") + " -r0x" +
					macroM.ToString("X");
				return true;
			}
			catch (Exception^)
			{
				return false;
			}
		}

	};
}

DllExport bool ICheckConnectFlag()
{
	ErrorEventWrapper::ErrorsEventCLI m_Library;
	bool retval = m_Library.CheckConnectFlag();
	return retval;
}

DllExport bool IInit_ErrorEvents()
{
	ErrorEventWrapper::ErrorsEventCLI m_Library;
	bool retval = m_Library.Init_ErrorEvents();
	return retval;
}

DllExport bool IConnectGpasciiDeviceErrorsEvent()
{
	ErrorEventWrapper::ErrorsEventCLI m_Library;
	bool retval = m_Library.ConnectGpasciiDevice();
	return retval;
}

DllExport void IDisconnectGpasciiDeviceErrorsEvent()
{
	ErrorEventWrapper::ErrorsEventCLI m_Library;
	m_Library.DisconnectGpasciiDevice();	
}

DllExport bool IInitDeviceErrorsEvent()
{
	ErrorEventWrapper::ErrorsEventCLI m_Library;
	bool retval = m_Library.InitDeviceErrorsEvent();
	return retval;
}

DllExport bool IIsconnectedErrorsEvent()
{
	ErrorEventWrapper::ErrorsEventCLI m_Library;
	bool retval = m_Library.Isconnected();
	return retval;
}

DllExport void ICollectDevicePropertiesDataErrorsEvent(const char** response)
{
	ErrorEventWrapper::ErrorsEventCLI m_Library;
	String ^%str_1 = gcnew String(*response);
	m_Library.CollectDevicePropertiesData(str_1);
	//*response = (const char*)(void*)Marshal::StringToHGlobalAnsi(str_1);
	IntPtr ptrTemp = Marshal::StringToHGlobalAnsi(str_1);
	*response = (const char*)(void*)ptrTemp;
	Marshal::FreeHGlobal(ptrTemp);
}

DllExport void IGetResponsefromDeviceErrorsEvent(const char* command, const char** response)
{
	ErrorEventWrapper::ErrorsEventCLI m_Library;
	String^ l_command = gcnew String(command);
	String ^%str_1 = gcnew String(*response);
	m_Library.GetResponsefromDevice(l_command, str_1);
	//*response = (const char*)(void*)Marshal::StringToHGlobalAnsi(str_1);
	IntPtr ptrTemp = Marshal::StringToHGlobalAnsi(str_1);
	*response = (const char*)(void*)ptrTemp;
	Marshal::FreeHGlobal(ptrTemp);
}

DllExport int IStartErrorsEvent()
{
	ErrorEventWrapper::ErrorsEventCLI m_Library;
	int retval = m_Library.StartErrorsEvent();
	return retval;
}

DllExport int IStopErrorsEvent()
{
	ErrorEventWrapper::ErrorsEventCLI m_Library;
	int retval = m_Library.StopErrorsEvent();
	return retval;
}

DllExport void IGetErrorResponse(const char** response)
{
	ErrorEventWrapper::ErrorsEventCLI m_Library;
	String ^%str_1 = gcnew String(*response);
	m_Library.GetErrorResponse(str_1);
	//*response = (const char*)(void*)Marshal::StringToHGlobalAnsi(str_1);
	IntPtr ptrTemp = Marshal::StringToHGlobalAnsi(str_1);
	*response = (const char*)(void*)ptrTemp;
	Marshal::FreeHGlobal(ptrTemp);
}

DllExport int ISetMask_CS()
{
	ErrorEventWrapper::ErrorsEventCLI m_Library;
	int retval = m_Library.SetMask_CS();
	return retval;
}

DllExport int ISetMask_Global()
{
	ErrorEventWrapper::ErrorsEventCLI m_Library;
	int retval = m_Library.SetMask_Global();
	return retval;
}

DllExport int ISetMask_Macro()
{
	ErrorEventWrapper::ErrorsEventCLI m_Library;
	int retval = m_Library.SetMask_Macro();
	return retval;
}

DllExport int ISetMask_Motor()
{
	ErrorEventWrapper::ErrorsEventCLI m_Library;
	int retval = m_Library.SetMask_Motor();
	return retval;
}

DllExport void IUpdateMaskValueDefault()
{	
	ErrorEventWrapper::ErrorsEventCLI m_Library;
	m_Library.UpdateMaskValueDefault();
}

DllExport void IUpdateMaskValue(const char** m_CS, const char** m_Global, const char** m_Macro, const char** m_Motor)
{
	ErrorEventWrapper::ErrorsEventCLI m_Library;
	String ^%l_CS = gcnew String(*m_CS);
	String ^%l_Global = gcnew String(*m_Global);
	String ^%l_Macro = gcnew String(*m_Macro);
	String ^%l_Motor = gcnew String(*m_Motor);
	m_Library.UpdateMaskValue(l_CS, l_Global, l_Macro, l_Motor);
	*m_CS = (const char*)(void*)Marshal::StringToHGlobalAnsi(l_CS);
	*m_Global = (const char*)(void*)Marshal::StringToHGlobalAnsi(l_Global);
	*m_Macro = (const char*)(void*)Marshal::StringToHGlobalAnsi(l_Macro);
	*m_Motor = (const char*)(void*)Marshal::StringToHGlobalAnsi(l_Motor);
}

DllExport void GetErrorData(const char** m_first, const char** m_second, const char** m_third, const char** m_fourth, const char** m_five, const char** m_six)
{
	ErrorEventWrapper::ErrorsEventCLI m_Library;	

	//array<String^> ^m_dataarray = gcnew array<String^>(5);
	
	String ^%l_first = gcnew String(*m_first);
	String ^%l_second = gcnew String(*m_second);
	String ^%l_third = gcnew String(*m_third);
	String ^%l_fourth= gcnew String(*m_fourth);
	String ^%l_five = gcnew String(*m_five);
	String ^%l_six = gcnew String(*m_six);

	m_Library.GetErrorData(l_first, l_second, l_third, l_fourth, l_five, l_six);
	*m_first = (const char*)(void*)Marshal::StringToHGlobalAnsi(l_first);
	*m_second = (const char*)(void*)Marshal::StringToHGlobalAnsi(l_second);
	*m_third = (const char*)(void*)Marshal::StringToHGlobalAnsi(l_third);
	*m_fourth = (const char*)(void*)Marshal::StringToHGlobalAnsi(l_fourth);
	*m_five = (const char*)(void*)Marshal::StringToHGlobalAnsi(l_five);
	*m_six = (const char*)(void*)Marshal::StringToHGlobalAnsi(l_six);

}