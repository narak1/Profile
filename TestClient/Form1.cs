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

namespace TestClient
{
	public partial class Form1 : Form
	{
		private Socket m_socket = null;
		private Packet m_packet = new Packet();

		public Form1()
		{
			InitializeComponent();
		}

		public void PrintOut(string mesg)
		{
			this.tbOutput.AppendText(mesg + System.Environment.NewLine);
		}

		private void btnDummySimConn_Click(object sender, EventArgs e)
		{
			this.PrintOut("Connecting to " + this.tbDummySimHost.Text + ":" + this.tbDummySimPort.Text);

			try
			{
				string hostname = this.tbDummySimHost.Text;
				int portno = int.Parse(this.tbDummySimPort.Text);

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

				// Create a TCP/IP  socket.
				m_socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

				m_socket.Connect(remoteEP);
				this.PrintOut("Connected to " + ipAddress + ":" + portno);
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

		private void btnDummySimClose_Click(object sender, EventArgs e)
		{
			if (this.m_socket != null )
			{
				m_socket.Shutdown(SocketShutdown.Both);
				m_socket.Close();
				this.PrintOut("Closed socket");

				this.m_socket = null;
			}
		}

		private void btnAdd_Click(object sender, EventArgs e)
		{
			if ( String.IsNullOrEmpty(this.tbOption.Text) )
			{
				return;
			}

			try
			{
				this.m_packet.arg_clear();
				this.m_packet.set_cmd(Packet.Cmd.Add);
				this.m_packet.arg_add(this.tbOption.Text);
				byte[] sbuf = this.m_packet.encode();

				string tmp = sbuf.Length.ToString() + ":";
				for (int i = 0; i < sbuf.Length; i++)
				{
					tmp += " " + sbuf[i].ToString();
				}
				this.PrintOut(tmp);

				int nRet = this.m_socket.Send(sbuf);
				this.PrintOut("Sent Data: (" + nRet + ")");

				byte[] rbuf = new byte[1024];
				nRet = this.m_socket.Receive(rbuf);
				this.PrintOut("Receive Data: (" + nRet + ")");

				tmp = nRet.ToString() + ":";
				for (int i = 0; i < nRet; i++)
				{
					tmp += " " + rbuf[i].ToString();
				}
				this.PrintOut(tmp);
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
	}
}
