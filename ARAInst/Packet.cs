using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ARAInst
{
	public class Packet
	{
		public enum Cmd { Error, Hello, Set, Info, SimInit, SimTime, TagInit, TagDown, Send, Run };
		static string[] strCmd = { "Error", "Hello", "Set", "Info", "SimInit", "SimTime", "TagInit", "TagDown", "Send", "Run" };
		const int header_stx = 4;
		const int header_cmd = 8;
		const int header_len = 4;
		const int header_size = 16;
		Cmd m_cmd;
		public List<Argument> m_arglist = new List<Argument>();

		const int buff_size = 256;

		// send buffer
		byte[] send_buff = new byte[buff_size];
		int send_total = 0;				// 보내야 할 데이타
		int send_num = 0;				// 보낸 데이타 (offset)

		// recv buffer
		byte[] recv_buff = new byte[buff_size];
		int recv_total = 0;				// 받은 데이타
		int parse_num = 0;				// 파싱한 메시지 데이타

		public void set_cmd(Cmd cmd)
		{
			this.m_cmd = cmd;
		}

		public Cmd get_cmd()
		{
			return this.m_cmd;
		}

		public void arg_clear()
		{
			this.m_arglist.Clear();
		}

		public void arg_add(string str)
		{
			this.m_arglist.Add(new Argument(str));
		}

		public void arg_add(int val)
		{
			this.m_arglist.Add(new Argument(val));
		}

		public void arg_add(double val)
		{
			this.m_arglist.Add(new Argument(val));
		}

		public byte[] encode(ref int total)
		{
			// calc total packet length
			//
			int length = Packet.header_size;
			length += 4;
			foreach (Argument arg in this.m_arglist)
			{
				length += arg.m_size;
			}

			// buffer reuse or new alloc
			//
			if (this.send_buff.Length < length)
			{
				Array.Resize<byte>(ref this.send_buff, length);
			}

			byte[] mesg = this.send_buff;
			int idx = 0;
			mesg[idx++] = Convert.ToByte('#');
			mesg[idx++] = Convert.ToByte('!');
			mesg[idx++] = Convert.ToByte('R');
			mesg[idx++] = Convert.ToByte('S');

			// command in header
			//
			byte[] strBytes = Encoding.ASCII.GetBytes(strCmd[(int)m_cmd]);
			Buffer.BlockCopy(strBytes, 0, mesg, idx, strBytes.Length);
			idx += 8;

			// total packet length in header
			//
			byte[] intBytes = BitConverter.GetBytes(length);
			Buffer.BlockCopy(intBytes, 0, mesg, idx, intBytes.Length);
			idx += intBytes.Length;

			// body encoding
			//	argument num
			//
			intBytes = BitConverter.GetBytes(this.m_arglist.Count);
			Buffer.BlockCopy(intBytes, 0, mesg, idx, intBytes.Length);
			idx += intBytes.Length;

			// type length value
			//
			foreach (Argument arg in this.m_arglist)
			{
				Buffer.BlockCopy(arg.encode(), 0, mesg, idx, arg.m_size);
				idx += arg.m_size;
			}

			this.send_total = idx;

			total = idx;
			return mesg;
		}

		public byte[] get_send(ref int offset, ref int total)
		{
			offset = this.send_num;
			total = this.send_total;
			return this.send_buff;
		}

		public void set_sent(int num)
		{
			this.send_num += num;
		}

		public bool is_send_done()
		{
			return this.send_total == this.send_num;
		}

		public void clear_send()
		{
			this.send_num = 0;
			this.send_total = 0;
		}

		public byte[] get_recv(ref int offset)
		{
			offset = this.recv_total;
			return this.recv_buff;
		}

		public void set_recv(int num)
		{
			// resize if need
			this.recv_total += num;
			if (this.recv_total >= this.recv_buff.Length)
			{
				Array.Resize<byte>(ref this.recv_buff, this.recv_buff.Length + Packet.buff_size);
			}
		}

		public void clear_recv()
		{
			int remain = this.recv_total - this.parse_num;
			Buffer.BlockCopy(this.recv_buff, this.parse_num, this.recv_buff, 0, remain);
			this.recv_total = remain;
			this.parse_num = 0;
		}

		public bool parse()
		{
			if (this.recv_total < Packet.header_size )
			{
				Globals.print_log("not completed header: recv num=" + this.recv_total);
				return false;
			}

			int idx = 0;

			if (this.find_stx()==false )
			{
				Globals.print_log("STX not found");
				return false;
			}
			idx += Packet.header_stx;

			string cmd = Encoding.ASCII.GetString(this.recv_buff, idx, Packet.header_cmd);
			idx += Packet.header_cmd;

			this.m_cmd = (Packet.Cmd)Enum.Parse(typeof(Packet.Cmd), cmd);

			this.parse_num = BitConverter.ToInt32(this.recv_buff, idx);
			idx += Packet.header_len;

			// Body Parse
			int num = BitConverter.ToInt32(this.recv_buff, idx);	// argument number (integer)
			idx += 4;

			this.m_arglist.Clear();
			for (int i = 0; i < num; i++)
			{
				Argument arg = new Argument();
				this.m_arglist.Add(arg);

				idx = arg.decode(this.recv_buff, idx);
			}

			return true;
		}

		public bool find_stx()
		{
			if (this.recv_buff[0] == '#' && this.recv_buff[1] == '!' && this.recv_buff[2] == 'R')
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
}
