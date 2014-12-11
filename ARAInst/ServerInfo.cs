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
		public enum ServerStatus { Undef, Failed, Connected, Closed, };

		public string hostname;
		public int port_no;
		public string node_name;		// unique
		public string server_type;
		public int priority;

		public string opc_prog_id;		// server type: OPC
		public int tcp_port_no = 0;		// server type: TCP

		public string avi_folder = null;
		public string log_folder = "log";
		public Logger.LogLevel log_mode = Logger.Default_LogLevel;

		public IPEndPoint end_point;	// unique
		public ServerStatus status = ServerStatus.Undef;
		public Socket sock = null;
		public Packet pack = null;
		public ListViewItem lvi = null;

		public void set_node_name(string node_name)
		{
			this.node_name = node_name;
			this.lvi.SubItems[(int)Form1.SubItemName.NodeName].Text = node_name;
		}

		public void set_server_type(string server_type)
		{
			this.server_type = server_type;
			this.lvi.SubItems[(int)Form1.SubItemName.ServerType].Text = server_type;
		}

		public void set_priority(int priority)
		{
			this.priority = priority;
			this.lvi.SubItems[(int)Form1.SubItemName.Priority].Text = priority.ToString();
		}

		public void set_status(ServerInfo.ServerStatus stat)
		{
			this.status = stat;
			this.lvi.SubItems[(int)Form1.SubItemName.Status].Text = stat.ToString();
		}
	}
}
