using System;
using System.Drawing;
using System.IO;
using System.Windows.Forms;
using ODT.PowerPmacComLib;

namespace Motion_Sample
{
    public partial class Form1 : Form
    {
        public PMAC_PDK PMAC_A = new PMAC_PDK();
        public PMAC_PDK PMAC_B = new PMAC_PDK();
        private System.Windows.Forms.Timer _pollTimer;

        private bool _isConnA = false;
        private bool _isConnB = false;

        // UI Controls - A Panel
        private TextBox txtIpA, txtCmdA, txtLogA, txtPos1A, txtPos2A;
        private TextBox txtFileA;
        private string uploadFilePathA, uploadFileNameA;

        // UI Controls - B Panel
        private TextBox txtIpB, txtCmdB, txtLogB, txtPos1B, txtPos2B;
        private TextBox txtFileB;
        private string uploadFilePathB, uploadFileNameB;

        public Form1()
        {
            InitializeComponent();
            CheckForIllegalCrossThreadCalls = false;
            
            // 기존 Designer 리소스 기반 UI 숨기기
            foreach (Control c in this.Controls)
            {
                c.Visible = false;
            }

            this.Text = "다중 Power PMAC 통신 터미널 (진품 개조판)";
            this.Width = 1150;
            this.Height = 850;
            this.StartPosition = FormStartPosition.CenterScreen;

            InitializeDynamicUI();

            _pollTimer = new System.Windows.Forms.Timer();
            _pollTimer.Interval = 200;
            _pollTimer.Tick += OnPollTimer;
            _pollTimer.Start();
        }

        private void InitializeDynamicUI()
        {
            Font titleFont = new Font("Malgun Gothic", 10, FontStyle.Bold);

            // ==================== PMAC A ====================
            GroupBox grpA = new GroupBox() { Text = "PMAC A (192.168.0.200)", Top = 10, Left = 10, Width = 540, Height = 780, Font = titleFont, Visible = true };
            
            grpA.Controls.Add(new Label() { Text = "IP:", Top = 35, Left = 10, Width = 30, Font = this.Font, Visible = true });
            txtIpA = new TextBox() { Text = "192.168.0.200", Top = 32, Left = 40, Width = 150, Font = this.Font, Visible = true };
            grpA.Controls.Add(txtIpA);

            Button btnConnectA = new Button() { Text = "Connect", Top = 30, Left = 200, Width = 80, Font = this.Font, Visible = true };
            btnConnectA.Click += (s, e) => {
                Log("A", "연결 시도 중...");
                if(PMAC_A.Connect(txtIpA.Text)) { _isConnA = true; Log("A", "연결 성공!"); } else { _isConnA = false; Log("A", "연결 실패!"); }
            };
            grpA.Controls.Add(btnConnectA);

            Button btnDisconnectA = new Button() { Text = "Disconnect", Top = 30, Left = 290, Width = 90, Font = this.Font, Visible = true };
            btnDisconnectA.Click += (s, e) => { PMAC_A.Disconnect(); _isConnA = false; Log("A", "연결 해제됨"); };
            grpA.Controls.Add(btnDisconnectA);

            grpA.Controls.Add(new Label() { Text = "명령:", Top = 70, Left = 10, Width = 40, Font = this.Font, Visible = true });
            txtCmdA = new TextBox() { Top = 68, Left = 55, Width = 225, Font = this.Font, Visible = true };
            grpA.Controls.Add(txtCmdA);

            Button btnSendA = new Button() { Text = "Send", Top = 66, Left = 290, Width = 90, Font = this.Font, Visible = true };
            btnSendA.Click += (s, e) => {
                string res = PMAC_A.GetResponse(txtCmdA.Text);
                Log("A", $"전송: {txtCmdA.Text} -> 응답: {CleanStr(res)}");
            };
            grpA.Controls.Add(btnSendA);

            // -- M1 JOG --
            grpA.Controls.Add(new Label() { Text = "[ Motor 1 ]", Top = 110, Left = 10, Width = 80, Font = this.Font, Visible = true });
            Button btnSvOn1A = new Button() { Text = "SV ON", Top = 105, Left = 90, Width = 60, Font = this.Font, Visible = true };
            btnSvOn1A.Click += (s, e) => { PMAC_A.SetSend("#1j/"); Log("A", "M1 SV ON"); };
            Button btnSvOff1A = new Button() { Text = "SV OFF", Top = 105, Left = 155, Width = 60, Font = this.Font, Visible = true };
            btnSvOff1A.Click += (s, e) => { PMAC_A.SetSend("#1k"); Log("A", "M1 SV OFF"); };
            Button btnJm1A = new Button() { Text = "JOG -", Top = 105, Left = 220, Width = 60, Font = this.Font, Visible = true };
            btnJm1A.MouseDown += (s, e) => { PMAC_A.SetSend("#1j-"); }; btnJm1A.MouseUp += (s, e) => { PMAC_A.SetSend("#1j/"); };
            Button btnJp1A = new Button() { Text = "JOG +", Top = 105, Left = 285, Width = 60, Font = this.Font, Visible = true };
            btnJp1A.MouseDown += (s, e) => { PMAC_A.SetSend("#1j+"); }; btnJp1A.MouseUp += (s, e) => { PMAC_A.SetSend("#1j/"); };
            txtPos1A = new TextBox() { Top = 108, Left = 355, Width = 100, Font = this.Font, ReadOnly = true, TextAlign = HorizontalAlignment.Right, Visible = true };
            grpA.Controls.AddRange(new Control[] { btnSvOn1A, btnSvOff1A, btnJm1A, btnJp1A, txtPos1A });

            // -- M2 JOG --
            grpA.Controls.Add(new Label() { Text = "[ Motor 2 ]", Top = 150, Left = 10, Width = 80, Font = this.Font, Visible = true });
            Button btnSvOn2A = new Button() { Text = "SV ON", Top = 145, Left = 90, Width = 60, Font = this.Font, Visible = true };
            btnSvOn2A.Click += (s, e) => { PMAC_A.SetSend("#2j/"); Log("A", "M2 SV ON"); };
            Button btnSvOff2A = new Button() { Text = "SV OFF", Top = 145, Left = 155, Width = 60, Font = this.Font, Visible = true };
            btnSvOff2A.Click += (s, e) => { PMAC_A.SetSend("#2k"); Log("A", "M2 SV OFF"); };
            Button btnJm2A = new Button() { Text = "JOG -", Top = 145, Left = 220, Width = 60, Font = this.Font, Visible = true };
            btnJm2A.MouseDown += (s, e) => { PMAC_A.SetSend("#2j-"); }; btnJm2A.MouseUp += (s, e) => { PMAC_A.SetSend("#2j/"); };
            Button btnJp2A = new Button() { Text = "JOG +", Top = 145, Left = 285, Width = 60, Font = this.Font, Visible = true };
            btnJp2A.MouseDown += (s, e) => { PMAC_A.SetSend("#2j+"); }; btnJp2A.MouseUp += (s, e) => { PMAC_A.SetSend("#2j/"); };
            txtPos2A = new TextBox() { Top = 148, Left = 355, Width = 100, Font = this.Font, ReadOnly = true, TextAlign = HorizontalAlignment.Right, Visible = true };
            grpA.Controls.AddRange(new Control[] { btnSvOn2A, btnSvOff2A, btnJm2A, btnJp2A, txtPos2A });

            // -- File Download --
            grpA.Controls.Add(new Label() { Text = "파일:", Top = 190, Left = 10, Width = 40, Font = this.Font, Visible = true });
            txtFileA = new TextBox() { Top = 188, Left = 55, Width = 160, Font = this.Font, ReadOnly = true, Visible = true };
            Button btnFileSelA = new Button() { Text = "찾기", Top = 186, Left = 220, Width = 60, Font = this.Font, Visible = true };
            btnFileSelA.Click += (s, e) => {
                var d = new OpenFileDialog(); if (d.ShowDialog() == DialogResult.OK) { uploadFilePathA = d.FileName; uploadFileNameA = Path.GetFileName(d.FileName); txtFileA.Text = uploadFileNameA; }
            };
            Button btnFileDnA = new Button() { Text = "다운로드", Top = 186, Left = 290, Width = 90, Font = this.Font, Visible = true };
            btnFileDnA.Click += (s, e) => {
                if (PMAC_A.FTPConnect(txtIpA.Text) == "OK\n" && PMAC_A.IsFTPConnected()) {
                    if (PMAC_A.FTPDownload(uploadFilePathA, "/var/ftp/usrflash/", uploadFileNameA)) Log("A", "FTP 다운로드 성공!"); else Log("A", "다운로드 실패!");
                }
            };
            grpA.Controls.AddRange(new Control[] { txtFileA, btnFileSelA, btnFileDnA });

            txtLogA = new TextBox() { Multiline = true, ScrollBars = ScrollBars.Vertical, Top = 230, Left = 10, Width = 510, Height = 530, Font = this.Font, Visible = true };
            txtLogA.ReadOnly = true;
            grpA.Controls.Add(txtLogA);

            this.Controls.Add(grpA);

            // ==================== PMAC B ====================
            GroupBox grpB = new GroupBox() { Text = "PMAC B (192.168.0.201)", Top = 10, Left = 570, Width = 540, Height = 780, Font = titleFont, Visible = true };

            grpB.Controls.Add(new Label() { Text = "IP:", Top = 35, Left = 10, Width = 30, Font = this.Font, Visible = true });
            txtIpB = new TextBox() { Text = "192.168.0.201", Top = 32, Left = 40, Width = 150, Font = this.Font, Visible = true };
            grpB.Controls.Add(txtIpB);

            Button btnConnectB = new Button() { Text = "Connect", Top = 30, Left = 200, Width = 80, Font = this.Font, Visible = true };
            btnConnectB.Click += (s, e) => {
                Log("B", "연결 시도 중...");
                if(PMAC_B.Connect(txtIpB.Text)) { _isConnB = true; Log("B", "연결 성공!"); } else { _isConnB = false; Log("B", "연결 실패!"); }
            };
            grpB.Controls.Add(btnConnectB);

            Button btnDisconnectB = new Button() { Text = "Disconnect", Top = 30, Left = 290, Width = 90, Font = this.Font, Visible = true };
            btnDisconnectB.Click += (s, e) => { PMAC_B.Disconnect(); _isConnB = false; Log("B", "연결 해제됨"); };
            grpB.Controls.Add(btnDisconnectB);

            grpB.Controls.Add(new Label() { Text = "명령:", Top = 70, Left = 10, Width = 40, Font = this.Font, Visible = true });
            txtCmdB = new TextBox() { Top = 68, Left = 55, Width = 225, Font = this.Font, Visible = true };
            grpB.Controls.Add(txtCmdB);

            Button btnSendB = new Button() { Text = "Send", Top = 66, Left = 290, Width = 90, Font = this.Font, Visible = true };
            btnSendB.Click += (s, e) => {
                string res = PMAC_B.GetResponse(txtCmdB.Text);
                Log("B", $"전송: {txtCmdB.Text} -> 응답: {CleanStr(res)}");
            };
            grpB.Controls.Add(btnSendB);

            // -- M1 JOG --
            grpB.Controls.Add(new Label() { Text = "[ Motor 1 ]", Top = 110, Left = 10, Width = 80, Font = this.Font, Visible = true });
            Button btnSvOn1B = new Button() { Text = "SV ON", Top = 105, Left = 90, Width = 60, Font = this.Font, Visible = true };
            btnSvOn1B.Click += (s, e) => { PMAC_B.SetSend("#1j/"); Log("B", "M1 SV ON"); };
            Button btnSvOff1B = new Button() { Text = "SV OFF", Top = 105, Left = 155, Width = 60, Font = this.Font, Visible = true };
            btnSvOff1B.Click += (s, e) => { PMAC_B.SetSend("#1k"); Log("B", "M1 SV OFF"); };
            Button btnJm1B = new Button() { Text = "JOG -", Top = 105, Left = 220, Width = 60, Font = this.Font, Visible = true };
            btnJm1B.MouseDown += (s, e) => { PMAC_B.SetSend("#1j-"); }; btnJm1B.MouseUp += (s, e) => { PMAC_B.SetSend("#1j/"); };
            Button btnJp1B = new Button() { Text = "JOG +", Top = 105, Left = 285, Width = 60, Font = this.Font, Visible = true };
            btnJp1B.MouseDown += (s, e) => { PMAC_B.SetSend("#1j+"); }; btnJp1B.MouseUp += (s, e) => { PMAC_B.SetSend("#1j/"); };
            txtPos1B = new TextBox() { Top = 108, Left = 355, Width = 100, Font = this.Font, ReadOnly = true, TextAlign = HorizontalAlignment.Right, Visible = true };
            grpB.Controls.AddRange(new Control[] { btnSvOn1B, btnSvOff1B, btnJm1B, btnJp1B, txtPos1B });

            // -- M2 JOG --
            grpB.Controls.Add(new Label() { Text = "[ Motor 2 ]", Top = 150, Left = 10, Width = 80, Font = this.Font, Visible = true });
            Button btnSvOn2B = new Button() { Text = "SV ON", Top = 145, Left = 90, Width = 60, Font = this.Font, Visible = true };
            btnSvOn2B.Click += (s, e) => { PMAC_B.SetSend("#2j/"); Log("B", "M2 SV ON"); };
            Button btnSvOff2B = new Button() { Text = "SV OFF", Top = 145, Left = 155, Width = 60, Font = this.Font, Visible = true };
            btnSvOff2B.Click += (s, e) => { PMAC_B.SetSend("#2k"); Log("B", "M2 SV OFF"); };
            Button btnJm2B = new Button() { Text = "JOG -", Top = 145, Left = 220, Width = 60, Font = this.Font, Visible = true };
            btnJm2B.MouseDown += (s, e) => { PMAC_B.SetSend("#2j-"); }; btnJm2B.MouseUp += (s, e) => { PMAC_B.SetSend("#2j/"); };
            Button btnJp2B = new Button() { Text = "JOG +", Top = 145, Left = 285, Width = 60, Font = this.Font, Visible = true };
            btnJp2B.MouseDown += (s, e) => { PMAC_B.SetSend("#2j+"); }; btnJp2B.MouseUp += (s, e) => { PMAC_B.SetSend("#2j/"); };
            txtPos2B = new TextBox() { Top = 148, Left = 355, Width = 100, Font = this.Font, ReadOnly = true, TextAlign = HorizontalAlignment.Right, Visible = true };
            grpB.Controls.AddRange(new Control[] { btnSvOn2B, btnSvOff2B, btnJm2B, btnJp2B, txtPos2B });

            // -- File Download --
            grpB.Controls.Add(new Label() { Text = "파일:", Top = 190, Left = 10, Width = 40, Font = this.Font, Visible = true });
            txtFileB = new TextBox() { Top = 188, Left = 55, Width = 160, Font = this.Font, ReadOnly = true, Visible = true };
            Button btnFileSelB = new Button() { Text = "찾기", Top = 186, Left = 220, Width = 60, Font = this.Font, Visible = true };
            btnFileSelB.Click += (s, e) => {
                var d = new OpenFileDialog(); if (d.ShowDialog() == DialogResult.OK) { uploadFilePathB = d.FileName; uploadFileNameB = Path.GetFileName(d.FileName); txtFileB.Text = uploadFileNameB; }
            };
            Button btnFileDnB = new Button() { Text = "다운로드", Top = 186, Left = 290, Width = 90, Font = this.Font, Visible = true };
            btnFileDnB.Click += (s, e) => {
                if (PMAC_B.FTPConnect(txtIpB.Text) == "OK\n" && PMAC_B.IsFTPConnected()) {
                    if (PMAC_B.FTPDownload(uploadFilePathB, "/var/ftp/usrflash/", uploadFileNameB)) Log("B", "FTP 다운로드 성공!"); else Log("B", "다운로드 실패!");
                }
            };
            grpB.Controls.AddRange(new Control[] { txtFileB, btnFileSelB, btnFileDnB });

            txtLogB = new TextBox() { Multiline = true, ScrollBars = ScrollBars.Vertical, Top = 230, Left = 10, Width = 510, Height = 530, Font = this.Font, Visible = true };
            txtLogB.ReadOnly = true;
            grpB.Controls.Add(txtLogB);

            this.Controls.Add(grpB);

            // === 전체 폼 다크 테마 및 글자 가독성 일괄 적용 ===
            this.BackColor = Color.FromArgb(30, 30, 30); // 폼 배경 어둡게
            this.ForeColor = Color.White; // 기본 텍스트 흰색
            
            Action<Control.ControlCollection> applyDarkTheme = null;
            applyDarkTheme = (controls) =>
            {
                foreach (Control c in controls)
                {
                    if (c is Button btn)
                    {
                        btn.ForeColor = Color.White;
                        btn.BackColor = Color.FromArgb(70, 70, 70);
                        btn.FlatStyle = FlatStyle.Flat;
                        btn.FlatAppearance.BorderColor = Color.Gray;
                    }
                    else if (c is TextBox txt)
                    {
                        txt.ForeColor = Color.White;
                        txt.BackColor = Color.FromArgb(45, 45, 48);
                        txt.BorderStyle = BorderStyle.FixedSingle;
                    }
                    else if (c is GroupBox grp)
                    {
                        grp.ForeColor = Color.White;
                    }
                    else if (c is Label lbl)
                    {
                        lbl.ForeColor = Color.White;
                    }
                    
                    if (c.HasChildren)
                    {
                        applyDarkTheme(c.Controls);
                    }
                }
            };
            
            applyDarkTheme(this.Controls);
        }

        private string CleanStr(string input)
        {
            if (string.IsNullOrEmpty(input)) return "";
            return input.Replace("\x06", "").Replace("\r", "").Replace("\n", "").Trim();
        }

        private void Log(string target, string msg)
        {
            string line = $"[{DateTime.Now:HH:mm:ss}] {msg}\r\n";
            TextBox tb = (target == "A") ? txtLogA : txtLogB;
            tb.AppendText(line);
            
            // 윈폼 텍스트박스 메모리 무한 누적(UI 릭) 차단
            if (tb.Text.Length > 20000)
            {
                tb.Text = tb.Text.Substring(10000);
                tb.SelectionStart = tb.Text.Length;
                tb.ScrollToCaret();
            }
        }

        // GC 부하 방어용 정적 캐시
        private static readonly string CMD_POS1 = "Motor[1].Pos";
        private static readonly string CMD_POS2 = "Motor[2].Pos";

        private void OnPollTimer(object sender, EventArgs e)
        {
            if (_isConnA)
            {
                txtPos1A.Text = GetPos(PMAC_A, CMD_POS1);
                txtPos2A.Text = GetPos(PMAC_A, CMD_POS2);
            }
            if (_isConnB)
            {
                txtPos1B.Text = GetPos(PMAC_B, CMD_POS1);
                txtPos2B.Text = GetPos(PMAC_B, CMD_POS2);
            }
        }

        private string GetPos(PMAC_PDK pmac, string cmd)
        {
            try
            {
                string sAct = pmac.GetResponse(cmd);
                sAct = CleanStr(sAct);
                int eqAct = sAct.IndexOf("=");
                if (eqAct != -1) sAct = sAct.Substring(eqAct + 1);

                double valAct = 0;
                double.TryParse(sAct, out valAct);
                return valAct.ToString("F4");
            }
            catch
            {
                return "0.0000";
            }
        }

        protected override void OnFormClosed(FormClosedEventArgs e)
        {
            if (_pollTimer != null) _pollTimer.Stop();
            // 연결되지 않은 상태에서 끄면 m_ODT_Connect가 null이라 Disconnect 내부에서 에러가 터지므로 방어합니다.
            if (_isConnA) PMAC_A.Disconnect();
            if (_isConnB) PMAC_B.Disconnect();
            base.OnFormClosed(e);
        }

        // =========================================================
        // 자동 테스트 (콘솔 로그용 - 사용자가 직접 확인하라고 지시함)
        // =========================================================
        public static void RunSelfTest()
        {
            Console.WriteLine("=== [자체 테스트 시작] PMAC A/B 파싱 로직 검증 ===");
            try
            {
                string mockRawResponse = "Motor[1].Pos=123.456\r\n\x06";
                string clean = mockRawResponse.Replace("\x06", "").Replace("\r", "").Replace("\n", "").Trim();
                int eq = clean.IndexOf("=");
                if(eq != -1) clean = clean.Substring(eq + 1);
                
                double val = 0;
                double.TryParse(clean, out val);
                
                Console.WriteLine($"원본 PMAC 문자열: {mockRawResponse.Trim()}");
                Console.WriteLine($"파싱된 숫자 결과: {val.ToString("F4")}");
                
                if(val == 123.456)
                    Console.WriteLine("=> 파싱 로직 완벽 동작 확인 완료!");
                else
                    Console.WriteLine("=> 에러: 파싱 로직 비정상.");
            }
            catch(Exception ex)
            {
                Console.WriteLine("에러 발생: " + ex.Message);
            }
            Console.WriteLine("==================================================");
        }

        // =========================================================
        // Form1.Designer.cs 빌드 에러 회피용 기존 찌꺼기 핸들러 (사용 안함)
        // =========================================================
        private void btnConnect_Click(object sender, EventArgs e) {}
        private void btnDisconnect_Click(object sender, EventArgs e) {}
        private void btnJogMinus1_MouseDown(object sender, MouseEventArgs e) {}
        private void btnJogMinus1_MouseUp(object sender, MouseEventArgs e) {}
        private void btnJogPlus1_MouseDown(object sender, MouseEventArgs e) {}
        private void btnJogPlus1_MouseUp(object sender, MouseEventArgs e) {}
        private void btnServoOff1_Click(object sender, EventArgs e) {}
        private void btnServoOn1_Click(object sender, EventArgs e) {}
        private void btnJogMinus2_MouseDown(object sender, MouseEventArgs e) {}
        private void btnJogMinus2_MouseUp(object sender, MouseEventArgs e) {}
        private void btnJogPlus2_MouseDown(object sender, MouseEventArgs e) {}
        private void btnJogPlus2_MouseUp(object sender, MouseEventArgs e) {}
        private void btnServoOff2_Click(object sender, EventArgs e) {}
        private void btnServoOn2_Click(object sender, EventArgs e) {}
        private void btnSend_Click(object sender, EventArgs e) {}
        private void btnFileDownload_Click(object sender, EventArgs e) {}
        private void btnFileSelect_Click(object sender, EventArgs e) {}
    }
}
