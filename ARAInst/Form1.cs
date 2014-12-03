using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using System.Net;
using System.Net.Sockets;

using System.Threading;
using System.IO;

namespace ARAInst
{
	public partial class Form1 : Form
	{
		public Logger m_log = new Logger();
		public ServerManager m_ServerMgr = new ServerManager();
		public enum SubItemName { SeqNo, ServerType, NodeName, HostName, PortNo, Priority, Status };

		public Form1()
		{
			InitializeComponent();
			Globals.m_form = this;
		}

		private void Form1_Load(object sender, EventArgs e)
		{
			try
			{
				this.m_log.init(Logger.Default_LogLevel);
				this.m_log.print_log(Logger.LogLevel.DebugLog, "Start....");
			}
			catch (Exception ex)
			{
				MessageBox.Show(ex.Message);
			}
		}

		private void Form1_FormClosed(object sender, FormClosedEventArgs e)
		{
			this.m_log.finish();
		}

		public void PrintOut(string mesg)
		{
			this.textBox_log.AppendText(mesg + System.Environment.NewLine);
			try
			{
				this.m_log.print_log(Logger.LogLevel.InfoLog, mesg);
			}
			catch (Exception ex)
			{
				this.textBox_log.AppendText(ex.Message + System.Environment.NewLine); ;
			}
		}

		public void PrintLog(string s)
		{
			try
			{
				this.m_log.print_log(Logger.LogLevel.DebugLog, s);
			}
			catch (Exception ex)
			{
				this.textBox_log.AppendText(ex.Message + System.Environment.NewLine); ;
			}
		}

		private void button_add_Click(object sender, EventArgs e)
		{
			try
			{
				string hostname = this.textBox_Hostname.Text;
				if (hostname.Length == 0)
				{
					this.PrintOut("Input Hostname!");
					return;
				}
				string strPortno = this.textBox_ServicePort.Text;
				if (strPortno.Length == 0)
				{
					this.PrintOut("Input Port No!");
					return;
				}

				int portno = int.Parse(strPortno);
				if (portno < 0 || portno > ushort.MaxValue)
				{
					this.PrintOut("Port No is between 0 to " + ushort.MaxValue + "!");
					return;
				}

				IPHostEntry ipHostInfo = Dns.GetHostEntry(hostname);
				IPAddress ipAddress = null;
				IPEndPoint remoteEP = null;

				foreach (IPAddress ip in ipHostInfo.AddressList)
				{
					if (ip.AddressFamily == AddressFamily.InterNetwork)
					{
						ipAddress = ip;
						remoteEP = new IPEndPoint(ip, portno);
						break;
					}
				}

				ServerInfo info = new ServerInfo();
				info.server_type = this.comboBox_ServerType.Text;
				info.node_name = this.textBox_NodeName.Text;
				info.hostname = hostname;
				info.port_no = portno;
				info.end_point = remoteEP;
				try
				{
					info.priority = int.Parse(this.textBox_ServerPriority.Text);
				}
				catch (Exception)
				{
					info.priority = 0;
				}

				// Dup Check
				if (this.m_ServerMgr.Add(info) == false)
				{
					this.PrintOut("Duplicated! Connection Info=" + info.end_point + ", Node Name=" + info.node_name);
					return;
				}

				this.PrintOut("Add SIMBridge " + hostname + ":" + strPortno);
				this.PrintLog("Main Thread ID = " + Thread.CurrentThread.ManagedThreadId );

				ListViewItem lvi = this.listViewEx1.Items.Add(this.m_ServerMgr.Num().ToString());
				lvi.SubItems.Add(info.server_type);
				lvi.SubItems.Add(info.node_name);
				lvi.SubItems.Add(hostname);
				lvi.SubItems.Add(portno.ToString());
				lvi.SubItems.Add(info.priority.ToString());
				lvi.SubItems.Add("");
				info.lvi = lvi;

				// Save System Config
				this.m_ServerMgr.save();

				// Create a TCP/IP  socket.
				Socket socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
				info.sock = socket;
				info.pack = new Packet();

				//Create connected callback
				AsyncCallback cbConnected = new AsyncCallback(Connected);

				//Connect to host
				socket.BeginConnect(remoteEP, cbConnected, info);
				this.PrintOut("Connecting to " + info.end_point);
			}
			catch (SocketException se)
			{
				this.PrintOut("Socket Exception: " + se.Message);
			}
			catch (Exception ex)
			{
				this.PrintOut("Exception: " + ex.Message);
			}
		}

		void Connected(IAsyncResult ar)
		{
			try
			{
				//Get socket reference
				ServerInfo info = (ServerInfo)ar.AsyncState;
				this.PrintOut("Connected to " + info.end_point + ", " + info.sock.Connected);
				this.PrintLog("Callback Thread ID = " + Thread.CurrentThread.ManagedThreadId);

				if (info.sock.Connected)
				{
					info.status = ServerInfo.ServerStatus.Connected;
				}
				else
				{
					info.status = ServerInfo.ServerStatus.Failed;
				}

				info.lvi.SubItems[(int)SubItemName.Status].Text = info.status.ToString();
				if (info.sock.Connected == false)
					return;

				// send Hello
				info.pack.arg_clear();
				info.pack.set_cmd(Packet.Cmd.Hello);
				info.pack.arg_add(info.node_name);
				info.pack.arg_add(info.server_type);
				info.pack.arg_add(info.priority);
				byte[] sbuf = info.pack.encode();

				string tmp = sbuf.Length.ToString() + ":";
				for (int i = 0; i < sbuf.Length; i++)
				{
					tmp += " " + sbuf[i].ToString();
				}
				this.PrintLog(tmp);

				int nRet = info.sock.Send(sbuf);
				this.PrintLog("Sent Data: (" + nRet + ")");

				byte[] rbuf = new byte[1024];
				nRet = info.sock.Receive(rbuf);
				this.PrintLog("Receive Data: (" + nRet + ")");

				tmp = nRet.ToString() + ":";
				for (int i = 0; i < nRet; i++)
				{
					tmp += " " + rbuf[i].ToString();
				}
				this.PrintLog(tmp);
			}
			catch (SocketException ex)
			{
				this.PrintOut("Socket Error: " + ex.Message);
			}
			catch (Exception ex)
			{
				this.PrintOut("Error: " + ex.Message);
			}
		}

		private void button_open_Click(object sender, EventArgs e)
		{
			string last_path = this.load_registry_value();
			OpenFileDialog lfd = new OpenFileDialog();
			lfd.Filter = "XML file (*.xml)|*.xml|All files (*.*)|*.*";
			lfd.FilterIndex = 1;
			lfd.Title = "Open config file";
			lfd.RestoreDirectory = true;
			lfd.InitialDirectory = last_path;
			lfd.Multiselect = false;
			if (lfd.ShowDialog() == DialogResult.OK)
			{
				last_path = Path.GetDirectoryName(lfd.FileName);
				this.save_registry_value(last_path);
				this.m_ServerMgr.load(lfd.FileName);
			}
		}

		public string adc_registry_pos = "Software\\ARA\\ARAinst";
		public string adc_registry_key = "LastPath";

		public string load_registry_value()
		{
			string keyvalue = null;
			try
			{
				Microsoft.Win32.RegistryKey regkey;
				regkey = Microsoft.Win32.Registry.CurrentUser.OpenSubKey(adc_registry_pos);
				if (regkey == null)
				{
					this.PrintLog("Registry SubKey [" + adc_registry_pos + "] not found.");
					return null;
				}
				keyvalue = (string)regkey.GetValue(adc_registry_key);
				regkey.Close();
			}
			catch (Exception)
			{
			}
			return keyvalue;
		}

		public void save_registry_value(string keyvalue)
		{
			try
			{
				Microsoft.Win32.RegistryKey regkey;
				regkey = Microsoft.Win32.Registry.CurrentUser.OpenSubKey(adc_registry_pos, true);
				if (regkey == null)
				{
					regkey = Microsoft.Win32.Registry.CurrentUser.CreateSubKey(adc_registry_pos);
					if (regkey == null)
					{
						this.PrintLog("Registry SubKey [" + adc_registry_pos + "] creation fail.");
						return;
					}
				}
				regkey.SetValue(adc_registry_key, keyvalue);
				regkey.Close();
			}
			catch (Exception)
			{
			}
		}

	}
}
