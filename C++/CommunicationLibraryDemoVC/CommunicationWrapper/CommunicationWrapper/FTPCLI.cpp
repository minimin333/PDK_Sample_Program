#include "stdafx.h"
#include "Common.h"


namespace FTPWrapper {

	public ref class FTPCLI
	{
		static ODT::PowerPmacComLib::IFTPClientInterface^ ftp;
		static ODT::PowerPmacComLib::IFTPClientInterface^ communication;
		
		static bool licensed;
		deviceProperties^ currentDevProp = gcnew deviceProperties();
		static bool IsValidLicense = true;
		String^ m_defaultIPAddress = "192.168.0.200";
		String^ m_defaultPassword = "deltatau";
		String^ m_defaultPortNumber = "23";
		String^ m_defaultUser = "root";
		String^ downLoadFilePath = "";
		String^ FtpFilename = "/var/ftp/usrflash/";

		public: bool FTPDemoInit()
		{
			try
			{
				if (IsValidLicense)
				{
					communication = Connect::CreateFTPClient(CommunicationGlobals::FTPConnectionTypes::FTP, nullptr);
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
			downLoadFilePath = Path::GetTempPath();
			return true;
		}

		public: bool CheckCommunicationFlag()
		{
			if (communication == nullptr)
			{
				return false;
			}
			else
			{
				return true;
			}
		}

		public: bool ConnectFTPClient(String^ ServerTextBox)
		{
			ftp = Connect::CreateFTPClient(CommunicationGlobals::FTPConnectionTypes::FTP, nullptr);
			ftp->ConnectFTP(ServerTextBox, "root", "deltatau");

			if (!ftp->FTPConnected)
			{
				return false;
			}
			return true;
		}

		public: void RemoveFile(String^ m_FtpFilename, String^ uploadFileName)
		{
			ftp->RemoveFile(m_FtpFilename + uploadFileName);
		}

		public: int CheckFileSize(String^ m_FtpFilename, String^ m_FileName)
		{
			if (ftp->GetFileSize(m_FtpFilename + m_FileName) > 0)
			{
				return GREATERTHANZERO;
			}
			else if (ftp->GetFileSize(m_FtpFilename + m_FileName) <= 0)
			{
				return LESSTHANEQUALTOZERO;
			}
		}

		public: bool FTPDownloadFile(String^ uploadFileNameWithFolder, String^ m_FtpFilename)
		{
			auto ret = ftp->DownloadFile(uploadFileNameWithFolder, m_FtpFilename);
			return ret;
		}

		public: bool FTPUploadFile(String^ l_downloadFileNameWithFolder)
		{
			auto ret = ftp->UploadFile(l_downloadFileNameWithFolder, downLoadFilePath);
			return ret;
		}

		public: void DisconnectFTPConnection()
		{
			ftp->DisconnectFTP();
		}
	};
}

DllExport bool ICreateFTPClient(int type, const char* prevObject)
{
	FTPWrapper::FTPCLI m_Library;
	bool retVal = m_Library.FTPDemoInit();
	return retVal;
}

DllExport bool ICheckCommunicationFlag()
{
	FTPWrapper::FTPCLI m_Library;
	bool retVal = m_Library.CheckCommunicationFlag();
	return retVal;
}

DllExport bool IConnectFTPClient(const char* m_ServerTextBox)
{
	FTPWrapper::FTPCLI m_Library;
	String^ l_ServerTextBox = gcnew String(m_ServerTextBox);
	bool retVal = m_Library.ConnectFTPClient(l_ServerTextBox);
	return retVal;
}

DllExport void IRemoveFile(const char* m_FtpFilename, const char* uploadFileName)
{
	FTPWrapper::FTPCLI m_Library;
	String^ l_FtpFilename = gcnew String(m_FtpFilename);
	String^ l_uploadFileName = gcnew String(uploadFileName);
	m_Library.RemoveFile(l_FtpFilename, l_uploadFileName);
}

DllExport int ICheckFileSize(const char* m_FtpFilename, const char* uploadFileName)
{
	FTPWrapper::FTPCLI m_Library;
	String^ l_FtpFilename = gcnew String(m_FtpFilename);
	String^ l_uploadFileName = gcnew String(uploadFileName);
	int retVal = m_Library.CheckFileSize(l_FtpFilename, l_uploadFileName);
	return retVal;
}

DllExport bool IFTPDownloadFile(const char* uploadFileNameWithFolder, const char* m_FtpFilename)
{
	FTPWrapper::FTPCLI m_Library;
	String^ l_uploadFileNameWithFolder = gcnew String(uploadFileNameWithFolder);
	String^ l_FtpFilename = gcnew String(m_FtpFilename);	
	bool retVal = m_Library.FTPDownloadFile(l_uploadFileNameWithFolder, l_FtpFilename);
	return retVal;
}

DllExport bool IFTPUploadFile(const char* downloadFileNameWithFolder)
{
	FTPWrapper::FTPCLI m_Library;
	String^ l_downloadFileNameWithFolder = gcnew String(downloadFileNameWithFolder);
	bool retVal = m_Library.FTPUploadFile(l_downloadFileNameWithFolder);
	return retVal;
}

DllExport void IDisconnectFTPConnection()
{
	FTPWrapper::FTPCLI m_Library;	
	m_Library.DisconnectFTPConnection();
}