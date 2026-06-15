#include "stdafx.h"
#include "Common.h"


namespace FirmwareUpdateWrapper {

	public ref class FirmwareUpdateCLI
	{
		static ODT::PowerPmacComLib::ISyncGpasciiCommunicationInterface^ ppgpAscii;
		static bool licensed;
		public: bool InitFirmwareUpdate()
		{
			try
			{
				ppgpAscii = Connect::CreateSyncGpascii(CommunicationGlobals::ConnectionTypes::SSH, nullptr);
				licensed = true;
			}
			catch (Exception^ ex)
			{
				return false;
			}
			return true;
		}

		public: int CheckFirmwareVersion(String^ IPAddress, String^ Username, String^ Password, String^ %response)
		{
			if (!licensed)
			{
				return 1;
			}
			if (ppgpAscii == nullptr)
			{
				return 2;
			}
			ppgpAscii->DisconnectGpascii();
			bool b = ppgpAscii->ConnectGpAscii(IPAddress, 22, Username, Password);
			if (!b)
			{
				return 2;
			}
			ppgpAscii->GetResponse("vers", response);
			ppgpAscii->DisconnectGpascii();
			return 0;
		}

		public: bool Checkversion(String^ expectedverion, String^ deviceversion)
		{
			bool bversion = CommunicationGlobals::CheckVersion(expectedverion, deviceversion);
			if (bversion)
			{
				if (ppgpAscii)
					ppgpAscii->DisconnectGpascii();
			}
			return bversion;
		}

		public: void GetResponsefromDevice(String^ type, String^ %response)
		{
			if (ppgpAscii)
				ppgpAscii->GetResponse(type, response);
		}

		public: int ConnectGpasciiDevice(String^ IPAddress, String^ Username, String^ Password)
		{
			if (!licensed)
			{
				return 1;
			}
			ppgpAscii->DisconnectGpascii();
			bool b = ppgpAscii->ConnectGpAscii(IPAddress, 22, Username, Password);
			if (!b)
			{
				return 2;
			}
			return 0;
		}

		public: void DisconnectGpasciiDevice()
		{
			if (ppgpAscii)
				ppgpAscii->DisconnectGpascii();
		}

		public: bool FirmwareUpdateforDevice(String^ FileName, String^ IPAddress, String^ Username, String^ Password)
		{
			auto updater = gcnew FirmwareUpdater();
			bool result;
			try
			{
				result = updater->UpdateFirmware(FileName, IPAddress, Username, Password, 22);
			}
			catch (FirmwareUpdater::ConnectionFailedException^)
			{
				result = false;
			}
			catch (FirmwareUpdater::InvalidCPUArchitectureException^)
			{
				result = false;
			}
			catch (FirmwareUpdater::FirmwareLoadException^)
			{
				result = false;
			}
			catch (FirmwareUpdater::InvalidHypervisorException^)
			{
				result = false;
			}
			catch (FirmwareUpdater::InvalidVersionException^)
			{
				result = false;
			}
			catch (FirmwareUpdater::MissingNFSServiceException^)
			{
				result = false;
			}
			catch (Exception^)
			{
				result = false;
			}
			if (result)
			{
				ppgpAscii->DisconnectGpascii();
				return true;
			}
			else
			{
				ppgpAscii->DisconnectGpascii();
				return false;
			}
		}

	};
}

DllExport bool IInitFirmwareUpdate()
{
	FirmwareUpdateWrapper::FirmwareUpdateCLI m_Library;
	bool bVal = m_Library.InitFirmwareUpdate();
	return bVal;
}

DllExport int ICheckFirmwareVersion(const char* IPAddress, const char* Username, const char* Password, const char** response)
{
	FirmwareUpdateWrapper::FirmwareUpdateCLI m_Library;

	String^ l_IPAddress = gcnew String(IPAddress);
	String^ l_Username = gcnew String(Username);
	String^ l_Password = gcnew String(Password);

	String ^%str_1 = gcnew String(*response);
	int retval = m_Library.CheckFirmwareVersion(l_IPAddress, l_Username, l_Password, str_1);
	//*response = (const char*)(void*)Marshal::StringToHGlobalAnsi(str_1);
	IntPtr ptrTemp = Marshal::StringToHGlobalAnsi(str_1);
	*response = (const char*)(void*)ptrTemp;
	Marshal::FreeHGlobal(ptrTemp);
	return retval;
}

DllExport bool ICheckversion(const char* expectedverion, const char* deviceversion)
{
	FirmwareUpdateWrapper::FirmwareUpdateCLI m_Library;
	String^ l_expectedverion = gcnew String(expectedverion);
	String^ l_deviceversion = gcnew String(deviceversion);
	bool retVal = m_Library.Checkversion(l_expectedverion, l_deviceversion);
	return retVal;
}

DllExport void IGetResponsefromDevice(const char* type, const char** response)
{
	FirmwareUpdateWrapper::FirmwareUpdateCLI m_Library;
	String^ l_type = gcnew String(type);
	String ^%str_1 = gcnew String(*response);
	m_Library.GetResponsefromDevice(l_type, str_1);
	//*response = (const char*)(void*)Marshal::StringToHGlobalAnsi(str_1);
	IntPtr ptrTemp = Marshal::StringToHGlobalAnsi(str_1);
	*response = (const char*)(void*)ptrTemp;
	Marshal::FreeHGlobal(ptrTemp);
}
DllExport int IConnectGpasciiDevice(const char* IPAddress, const char* Username, const char* Password)
{
	FirmwareUpdateWrapper::FirmwareUpdateCLI m_Library;

	String^ l_IPAddress = gcnew String(IPAddress);
	String^ l_Username = gcnew String(Username);
	String^ l_Password = gcnew String(Password);
	int retval = m_Library.ConnectGpasciiDevice(l_IPAddress, l_Username, l_Password);
	return retval;
}
DllExport void IDisconnectGpasciiDevice()
{
	FirmwareUpdateWrapper::FirmwareUpdateCLI m_Library;
	m_Library.DisconnectGpasciiDevice();
}

DllExport bool IFirmwareUpdateforDevice(const char* FileName, const char* IPAddress, const char* Username, const char* Password)
{
	FirmwareUpdateWrapper::FirmwareUpdateCLI m_Library;

	String^ l_FileName = gcnew String(FileName);
	String^ l_IPAddress = gcnew String(IPAddress);
	String^ l_Username = gcnew String(Username);
	String^ l_Password = gcnew String(Password);
	bool retval = m_Library.FirmwareUpdateforDevice(l_FileName, l_IPAddress, l_Username, l_Password);
	return retval;
}