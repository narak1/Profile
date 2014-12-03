using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Net;
using System.Net.Sockets;

using System.Windows.Forms;

namespace ARAInst
{
	public class ServerInfo
	{
		public enum ServerStatus { Undef, Failed, Connected, };

		public string hostname;
		public int port_no;
		public string node_name;		// unique
		public string server_type;
		public int priority;

		public string opc_prog_id;		// server type: OPC
		public int tcp_port_no = 1234;	// server type: TCP

		public string avi_folder = "avi";
		public string log_folder = "log";
		public Logger.LogLevel log_mode = Logger.Default_LogLevel;

		public IPEndPoint end_point;	// unique
		public ServerStatus status = ServerStatus.Undef;
		public Socket sock = null;
		public Packet pack = null;
		public ListViewItem lvi = null;
	}
}
