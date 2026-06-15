using ODT.PowerPmacComLib;
using ODT.PowerPmacComLib.Core;
using ODT.PowerPmacComLib.ExtensionMethods;
using System;
using System.Collections.Generic;
using System.Dynamic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using static System.Net.WebRequestMethods;
using static System.Windows.Forms.AxHost;

namespace Motion_Sample
{
    public class PMAC_PDK
    {
        const int BUFFER_SIZE = 256;

        //SystemLoger.CSystemLoger m_Loger = new SystemLoger.CSystemLoger();


        byte[] m_bzRecvBuffer = null;
        bool m_fConnected = false;
        ISyncGpasciiCommunicationInterface m_ODT_Connect;
        ISyncTerminalCommunicationInterface syncTerminal;
        IFTPClientInterface m_ODT_FTP_Connect;

        object m_Locker = new object();

        public PMAC_PDK()
        {
            m_bzRecvBuffer = new byte[BUFFER_SIZE];
        }
        public bool Connect(string IP)  //Connect to the PMAC of the entered IP address.
        {
            if (m_fConnected) return true;

            // 재접속 시 이전 찌꺼기 포트 강제 초기화 (데드락 방지)
            Disconnect();

            UInt32 uIPAddress;
            String[] strIP = new String[4];
            bool bRet = false;
            Status eStatus = Status.Failed;

            strIP = IP.Split('.');
            if (strIP.Length != 4) return false;
            uIPAddress = (Convert.ToUInt32(strIP[0]) << 24) | (Convert.ToUInt32(strIP[1]) << 16) | (Convert.ToUInt32(strIP[2]) << 8) | Convert.ToUInt32(strIP[3]);

            try
            {
                m_ODT_Connect = ODT.PowerPmacComLib.Connect.CreateSyncGpascii(CommunicationGlobals.ConnectionTypes.SSH, null);
                syncTerminal = ODT.PowerPmacComLib.Connect.CreateSyncTerminal(CommunicationGlobals.ConnectionTypes.SSH, null);
                
                bRet = m_ODT_Connect.ConnectGpAscii(uIPAddress.ToString(), 22, "root", "deltatau");
                if (bRet == true)
                {
                    eStatus = m_ODT_Connect.SendCommand("echo 3", EchoMode.ResponseOnly);
                    m_fConnected = true;
                }
                else
                {
                    Disconnect(); // 연결 실패 시 생성된 찌꺼기 객체 해제
                    m_fConnected = false;
                    return false;
                }

                bRet = syncTerminal.ConnectTerminal(uIPAddress.ToString(), 22, "root", "deltatau");
                if (bRet == true)
                {
                    return true;
                }
                else
                {
                    Disconnect();
                    m_fConnected = false;
                    return false;
                }
            }
            catch (Exception)
            {
                Disconnect(); // 예외 발생 시 포트 누수 차단
                m_fConnected = false;
                return false;
            }
        }

        public string FTPConnect(string IP)
        {
            try
            {
                m_ODT_FTP_Connect = ODT.PowerPmacComLib.Connect.CreateFTPClient(CommunicationGlobals.FTPConnectionTypes.FTP, null);
                if(m_ODT_FTP_Connect == null)
                {
                    return "Lisence invalid!\n";
                }
                else
                {
                    if(m_ODT_FTP_Connect.ConnectFTP(IP, "root", "deltatau") != true)
                    {
                        return "Connect Fail!\n";
                    }
                    else
                    {
                        return "OK\n";
                    }
                }
            }
            catch (Exception ex)
            {
                return "Exception Error\n";
            }
        }
        public bool IsFTPConnected()
        {
            if (m_ODT_FTP_Connect == null)
            {
                return false;
            }
            else
            {
                if (m_ODT_FTP_Connect.FTPConnected == true)
                    return true;
                else
                    return false;
            }
        }
        public bool FTPDownload(string strPCFilePathWithFolder, string strPMACFilePath, string strFileName)
        {
            if (m_ODT_FTP_Connect == null)
            {
                return false;
            }
            else
            {
                if (m_ODT_FTP_Connect.FTPConnected == true)
                {
                    m_ODT_FTP_Connect.RemoveFile(strPMACFilePath + strFileName);

                    if (m_ODT_FTP_Connect.GetFileSize(strPMACFilePath + strFileName) > 0)
                    {
                        return false;
                    }
                    else
                    {
                        var ret = m_ODT_FTP_Connect.DownloadFile(strPCFilePathWithFolder, strPMACFilePath + strFileName);
                        if (ret)
                        {
                            if (m_ODT_FTP_Connect.GetFileSize(strPMACFilePath + strFileName) > 0)
                            {
                                var cmdDownload = "gpascii -i"+strPMACFilePath + strFileName + " -e/var/ftp/usrflash/Project/Log/filednlderror.log";//load the file into PowerPMAC memory
                                string response;
                                this.syncTerminal.SendCommand(cmdDownload, out response);

                                if (response.Contains("EOF"))
                                {
                                    return true;
                                }
                                else
                                {
                                    return false;
                                }
                            }
                            else
                                return false;
                        }
                        else
                        {
                            return false;
                        }
                    }
                }
                else
                    return false;
            }
        }
        public string GetResponse(string cmd)   //Send a command to the PMAC and recieve a response from PMAC.
        {
            if (m_fConnected == false)
                return "0";

            //!컨트롤러의 지령을 보낸다.
            if (cmd.Contains("="))
            {
                SetSend(cmd);
                return "0";
            }
            string strResponse = "";

            lock (m_Locker)
            {
                Status eReturn = Status.Failed;
                eReturn = m_ODT_Connect.GetResponse(cmd, out strResponse);
                if (eReturn == Status.TimeOut)
                    strResponse = "Timeout";
            }

            return strResponse;
        }

        public string GetResponse_Test()   //Send a command to the PMAC and recieve a response from PMAC.
        {
            if (m_fConnected == false)
                return "0";

            
            string strResponse = "";

            lock (m_Locker)
            {
                DateTime start1 = DateTime.Now;
                Status eReturn = Status.Failed;
                for (int i = 0; i < 100; i++)
                {
                    string cmd = $"Motor[{i}].ActPos";
                    string response;

                    m_ODT_Connect.GetResponse(cmd, out response);

                    // Console.WriteLine($"Motor {i}: {response}");
                }
                TimeSpan duration1 = DateTime.Now - start1;
                Console.WriteLine($"Case 1 (Single) 소요 시간: {duration1.TotalMilliseconds} ms");

                if (eReturn == Status.TimeOut)
                    strResponse = "Timeout";


                // Case 2: 빠른 방식 (List<string> 오버로딩 사용) 
                            // =========================================================
                            // 설명: 명령어를 리스트에 모두 담은 뒤 단 한 번의 GetResponse 호출로 처리

                DateTime start2 = DateTime.Now;

                List<string> cmdList = new List<string>();
                List<string> responseList; // out 파라미터로 받아올 리스트

                // 명령어 리스트 생성
                for (int i = 0; i < 100; i++)
                {
                    cmdList.Add($"Motor[{i}].ActPos");
                }

                // 매뉴얼 Syntax: Status GetResponse(List<string> cmd, out List<string> response) 
                // 단 한 번의 통신으로 100개의 응답을 모두 받아옴
                m_ODT_Connect.GetResponse(cmdList, out responseList);

                TimeSpan duration2 = DateTime.Now - start2;
                Console.WriteLine($"Case 2 (List) 소요 시간: {duration2.TotalMilliseconds} ms");
            }

            return strResponse;
        }

        public bool SetSend(string cmd) //Send a command to the PMAC.
        {
            if (m_fConnected == false)
                return false;

            Status eStatus = Status.Failed;
            lock (m_Locker)
            {
                eStatus = m_ODT_Connect.SendCommand(cmd, EchoMode.ResponseOnly);
            }

            return eStatus == Status.Ok ? true : false;
        }


        public void Disconnect()  //Disconnect to the PMAC.
        {
            if (m_ODT_Connect != null)
            {
                try { m_ODT_Connect.DisconnectGpascii(); } catch { }
            }
            if (syncTerminal != null)
            {
                try { syncTerminal.DisconnectTerminal(); } catch { }
            }
            m_fConnected = false;
        }

        public int IsConnect() //Check for connection to the PMAC.
        {
            bool bRet = m_ODT_Connect.GpAsciiConnected;
            bool bRet2 = m_ODT_Connect.SocketConnected;
            return Convert.ToInt32(bRet);
        }
    }
    

}
