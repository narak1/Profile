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
		const int header_size = 16;
		Cmd m_cmd;
		List<Argument> m_arglist = new List<Argument>();

		public void set_cmd(Cmd cmd)
		{
			this.m_cmd = cmd;
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

		public byte[] encode()
		{
			// calc total packet length
			//
			int length = Packet.header_size;
			length += 4;
			foreach (Argument arg in this.m_arglist)
			{
				length += arg.m_size;
			}

			byte[] mesg = new byte[length];
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

			return mesg;
		}
	}
}
