#pragma once
#include "afxwin.h"
#include <vector>
#include <string>
#define SUCCESS 0
#define INVALIDLICENSE 1
#define FAILEDTOCONNECT 2
#define DEVICENOTCONNECT 3
#define ERRORSEVENTSTART 4
#define ERRORSEVENTSTOP 5
#define UNSOLICITEDTIMEOUT 6
#define PORTNOTAVAILABLE 7
#define BUFFERNOTDEFINED 8
#define SOMEEXCEPTION 9
#define EVENTNOTSTARTED 10

#define GREATERTHANZERO 1		
#define LESSTHANEQUALTOZERO 2

#define DllExport __declspec(dllexport)
//ASync Class Functions.
DllExport bool IAsyncCreateAsyncGpascii(int ConnectionType, const char* prevObject);//Creates an asynchronous Gpascii communication object.
DllExport bool IAsyncConnectGpAscii(const char* IPAddress, const char* PortNumber, const char* UserName, const char* Password);//Open the Telnet port and opens Gpascii port with Async for PMAC communication
DllExport bool IASyncDisConnectGpAscii();//Closes the Telnet port and opens Gpascii port with Sync for PMAC communication
DllExport int  IAsyncGetResponse(const char* command, const char** response);//Sends command to the Power PMAC over SSH Server with Async type.
DllExport int  IAsyncGetResponse(const char* command, const char** response, int echoMode);//Sends command to the Power PMAC over SSH Server
DllExport bool IsAsyncGpAsciiconnected();//Boolean value that indicates if the program is connected.
DllExport void IAsyncCollectDevicePropertiesData(const char** response);//IPAddress of PMAC connected.

//Sync Class Functions.
DllExport bool ISyncCreateSyncGpascii(const char ConnectionType, const char* prevObject);//Creates an synchronous Gpascii communication object.
DllExport bool ISyncConnectGpAscii(const char* IPAddress, const char* PortNumber, const char* UserName, const char* Password);//Open the Telnet port and opens Gpascii port with Sync for PMAC communication
DllExport bool ISyncDisConnectGpAscii();//Closes the Telnet port and opens Gpascii port with Sync for PMAC communication
DllExport int  ISyncGetResponse(const char* command, const char** response);//Sends command to the Power PMAC over SSH Server with Sync type.
DllExport int  ISyncGetResponse(const char* command, const char** response, int ecohMode);//Sends command to the Power PMAC over SSH Server with Sync type.
DllExport int  ISyncGetResponse(const std::vector<std::string>& command, std::vector<std::string>& response);//Sends command to the Power PMAC over SSH Server with Sync type.
DllExport int  ISyncGetResponse(const std::vector<std::string>& command, std::vector<std::string>& response, int echoMode);//Sends command to the Power PMAC over SSH Server with Sync type.
DllExport int  ISyncGetResponse(const std::vector<std::string>& command, bool parseData, std::vector<std::string>& response);//Sends command to the Power PMAC over SSH Server with Sync type.
DllExport int  ISyncGetResponse(const std::vector<std::string>& command, bool parseData, std::vector<std::string>& response, int echoMode);//Sends command to the Power PMAC over SSH Server with Sync type.
DllExport bool IsGpAsciiconnected();//Boolean value that indicates if the program is connected.
DllExport void ICollectDevicePropertiesData(const char** response);//IPAddress of PMAC connected.
DllExport bool ISyncGpAsciiInitDeviceEvent();


//FTP Class Functions.
DllExport bool ICreateFTPClient(int type, const char* prevObject);//Creates an FTP client object.
DllExport bool ICheckCommunicationFlag();//Boolean value that indicates if the program is connected.
DllExport bool IConnectFTPClient(const char* m_ServerTextBox);//Opens the FTP connection for PMAC communication
DllExport void IRemoveFile(const char* m_FtpFilename, const char* uploadFileName);//Deletes file from client.
DllExport int ICheckFileSize(const char* m_FtpFilename, const char* uploadFileName);//Compares the file size of a file path from the client.
DllExport bool IFTPDownloadFile(const char* uploadFileNameWithFolder, const char* m_FtpFilename);//Returns True if file is successfully downloaded.
DllExport bool IFTPUploadFile(const char* downloadFileNameWithFolder);//Adds file to targeted file path.
DllExport void IDisconnectFTPConnection();//Closes the FTP connection for PMAC communication.

//Socket Class Function
DllExport bool IInitSocket();
DllExport bool IConnect_Socket(const char* command, int port, int timeoutMS);
DllExport bool IDisConnect_Socket();

DllExport bool IDartSetUserMemChar(int address, int numVars, const char* strData);
DllExport bool IDartSetUserMemShort(int address, int numVars, const char* strData);
DllExport bool IDartSetUserMemInt(int address, int numVars, const char* strData);
DllExport bool IDartSetUserMemFloat(int address, int numVars, const char* strData);
DllExport bool IDartSetUserMemDouble(int address, int numVars, const char* strData);
DllExport bool IDartSetUserMemPvar(int address, int numVars, const char* strData);

DllExport bool IDartGetUserMemChar(int address, int numVars, const char** response);
DllExport bool IDartGetUserMemShort(int address, int numVars, const char** response);
DllExport bool IDartGetUserMemInt(int address, int numVars, const char** response);
DllExport bool IDartGetUserMemFloat(int address, int numVars, const char** response);
DllExport bool IDartGetUserMemDouble(int address, int numVars, const char** response);
DllExport bool IDartGetUserMemPvar(int address, int numVars, const char** response);

DllExport bool IInitSync_Telnet();
DllExport bool IsTelnetconnected();
DllExport bool ISync_TelnetInitDeviceEvent();
DllExport void ICollectDevicePropertiesDataTelnet(const char** response);
DllExport int ISync_TelnetSendCommand(const char* command, const char** response);
//========================================================================
// Opaque Pointer (Handle) Based Multi-Device Native C API
//========================================================================
typedef void* HPmacDevice;

DllExport HPmacDevice PmacCreateDevice();
DllExport void PmacDestroyDevice(HPmacDevice hDevice);
DllExport bool PmacConnect(HPmacDevice hDevice, const char* ip, const char* user, const char* pw);
DllExport void PmacDisconnect(HPmacDevice hDevice);
DllExport bool PmacIsConnected(HPmacDevice hDevice);
DllExport int PmacGetResponse(HPmacDevice hDevice, const char* command, const char** response);

