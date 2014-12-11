using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Net;
using System.Net.Sockets;

using System.Threading;

using System.Windows.Forms;

namespace ARAInst
{
	public class Protocol
	{
		public void connect_start(ServerInfo si)
		{
			//Create connected callback
			AsyncCallback cbConnected = new AsyncCallback(Connected);

			//Connect to host
			IAsyncResult ar = si.sock.BeginConnect(si.end_point, cbConnected, si);
			Globals.print_out("Connecting to " + si.end_point);
		}

		void Connected(IAsyncResult ar)
		{
			try
			{
				ServerInfo info = (ServerInfo)ar.AsyncState;
				info.sock.EndConnect(ar);	// it's blocking call. but it's other thread

				Globals.print_log("Connected(): ar.IsCompleted=" + ar.IsCompleted);
				Globals.print_out("Connected to " + info.end_point + ", " + info.sock.Connected);
				Globals.print_log("Callback Thread ID = " + Thread.CurrentThread.ManagedThreadId);

				if (info.sock.Connected)
				{
					info.set_status(ServerInfo.ServerStatus.Connected);

					// send hello
					this.send_hello(info);
				}
				else
				{
					info.set_status(ServerInfo.ServerStatus.Failed);
				}
			}
			catch (SocketException ex)
			{
				Globals.print_out("Socket Error: " + ex.Message);
			}
			catch (Exception ex)
			{
				Globals.print_out("Error: " + ex.Message);
			}
		}

		public void send_start(ServerInfo si)
		{
			int total = 0;
			byte[] sbuf = si.pack.encode(ref total);

			string tmp = sbuf.Length.ToString() + ":";
			for (int i = 0; i < total; i++)
			{
				tmp += " " + sbuf[i].ToString();
			}
			Globals.print_log(tmp);

			//Create connected callback
			AsyncCallback cbSented = new AsyncCallback(Sented);

			IAsyncResult ar = si.sock.BeginSend(sbuf, 0, total, SocketFlags.None, cbSented, si);

			Globals.print_out("Sending to " + si.end_point);
		}

		public void recv_start(ServerInfo si)
		{
			int total = 0;
			byte[] rbuf = si.pack.get_recv(ref total);

			//Create connected callback
			AsyncCallback cbRecved = new AsyncCallback(Recved);

			IAsyncResult ar = si.sock.BeginReceive(rbuf, 0, total, SocketFlags.None, cbRecved, si);

			Globals.print_out("Recving wait from " + si.end_point);
		}

		void Sented(IAsyncResult ar)	// This callback method executes in a separate thread
		{
			ServerInfo si = (ServerInfo)ar.AsyncState;
			int ret = si.sock.EndSend(ar);

			Globals.print_log("Sented(): ar.IsCompleted=" + ar.IsCompleted + ", ret=" + ret);

			si.pack.set_sent(ret);
			if (si.pack.is_send_done())
			{
				// recv wait
				this.recv_start(si);
			}
			else
			{
				// if not completed sent
				int offset = 0;
				int total = 0;
				byte[] sbuf = si.pack.get_send(ref offset, ref total);
				AsyncCallback cbSented = new AsyncCallback(Sented);

				Globals.print_log("not Completed. so, resend :: offset=" + offset + ", total=" + total);

				ar = si.sock.BeginSend(sbuf, offset, total, SocketFlags.None, cbSented, si);
			}
		}

		void Recved(IAsyncResult ar)	// This callback method executes in a separate thread
		{
			ServerInfo si = (ServerInfo)ar.AsyncState;
			int ret = si.sock.EndReceive(ar);

			Globals.print_log("Recved(): ar.IsCompleted=" + ar.IsCompleted + ", ret=" + ret);
			if (ret == 0)
			{
				// connection closed. so, clear & status change
				si.set_status(ServerInfo.ServerStatus.Closed);
				return;
			}

			si.pack.set_recv(ret);	// buffer expanding ???

			while (si.pack.parse())	// multiple mesg recv case
			{
				// print packet message info
				Globals.print_log("cmd = " + si.pack.get_cmd());
				Globals.print_log("arg num=" + si.pack.m_arglist.Count);
				foreach (Argument arg in si.pack.m_arglist)
				{
					Globals.print_log("arg: tlv=" + arg.m_type + "," + arg.m_length + "," + arg.m_value);
				}

				// packet processing
				// take a long time no problem???
				switch (si.pack.get_cmd())
				{
					case Packet.Cmd.Error: this.recv_error(si); break;
					case Packet.Cmd.Hello: this.recv_hello(si); break;
					default: break;
				}

				// clear processed mesg
				si.pack.clear_recv();
			}

			// not complete or new recv
			int total = 0;
			byte[] rbuf = si.pack.get_recv(ref total);
			AsyncCallback cbRecved = new AsyncCallback(Recved);

			ar = si.sock.BeginReceive(rbuf, total, rbuf.Length, SocketFlags.None, cbRecved, si);
		}

		public void send_error(ServerInfo si)
		{
		}

		public void recv_error(ServerInfo si)
		{
			Globals.print_log("recv_error(" + si + ")");
			foreach (Argument arg in si.pack.m_arglist)
			{
				Globals.print_log("Argument: " + arg.m_type + "," + arg.m_length + "," + arg.m_value);
			}
		}

		public void send_hello(ServerInfo si)
		{
			si.pack.arg_clear();
			si.pack.set_cmd(Packet.Cmd.Hello);
			si.pack.arg_add("ARAInst");
			this.send_start(si);
		}

		public void recv_hello(ServerInfo si)
		{
			Globals.print_log("recv_hello(" + si + ")");
			foreach (Argument arg in si.pack.m_arglist)
			{
				Globals.print_log("Argument: " + arg.m_type + "," + arg.m_length + "," + arg.m_value);
			}

			List<Argument> arglist = si.pack.m_arglist;
			string node_name = Encoding.ASCII.GetString( arglist[0].m_value);
			string svr_type = Encoding.ASCII.GetString(arglist[1].m_value);
			int priority = BitConverter.ToInt32(arglist[2].m_value, 0);
			Globals.print_log("node=" + node_name + ", type=" + svr_type + ", prity=" + priority);

			bool set_flag = false;
			if (si.node_name.Length == 0)
			{
				si.set_node_name(node_name);
			}
			else if (si.node_name != node_name)
			{
				set_flag = true;
			}
			if (si.server_type.Length == 0)
			{
				si.set_server_type(svr_type);
			}
			else if (si.server_type != svr_type)
			{
				set_flag = true;
			}
			if (si.priority == 0)
			{
				si.set_priority(priority);
			}
			else if (si.priority != priority)
			{
				set_flag = true;
			}

			if (set_flag)
			{
				this.send_set(si);
			}
		}

		public void send_set(ServerInfo info)
		{
			info.pack.arg_clear();
			info.pack.set_cmd(Packet.Cmd.Set);
			info.pack.arg_add(info.node_name);
			info.pack.arg_add(info.server_type);
			info.pack.arg_add(info.priority);
			this.send_start(info);
		}
	}
}
