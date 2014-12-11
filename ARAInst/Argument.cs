using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ARAInst
{
	public class Argument
	{
		public enum Type { None, Char, UChar, Bool, Short, Integer, Long, Single, Double, Binary };
		static int[] type_size = { 0, 1, 1, 2, 2, 4, 4, 8, 1 };
		public Type m_type = Type.None;
		public short m_length = 0;
		public byte[] m_value = null;
		public int m_size = 4;				// encoding block size

		public Argument()
		{
		}

		public Argument(string str)
		{
			this.m_type = Type.Char;
			this.m_length = (short)str.Length;
			this.m_value = Encoding.ASCII.GetBytes(str);
			//Array.Resize(ref this.m_value, str.Length+1);
			this.m_size = 2 + 2 + (this.m_length / 4 + 1) * 4;
		}

		public Argument(short val)
		{
			this.m_type = Type.Short;
			this.m_length = 1;
			this.m_value = BitConverter.GetBytes(val);
			this.m_size = 2 + 2 + 4;		// multiple of 4
		}

		public Argument(int val)
		{
			this.m_type = Type.Integer;
			this.m_length = 1;
			this.m_value = BitConverter.GetBytes(val);
			this.m_size = 2 + 2 + 4;
		}

		public Argument(long val)
		{
			this.m_type = Type.Long;
			this.m_length = 1;
			this.m_value = BitConverter.GetBytes(val);
			this.m_size = 2 + 2 + 8;
		}

		public Argument(float val)
		{
			this.m_type = Type.Single;
			this.m_length = 1;
			this.m_value = BitConverter.GetBytes(val);
			this.m_size = 2 + 2 + 4;
		}

		public Argument(double val)
		{
			this.m_type = Type.Double;
			this.m_length = 1;
			this.m_value = BitConverter.GetBytes(val);
			this.m_size = 2 + 2 + 8;
		}

		public byte[] encode()
		{
			byte[] arr = new byte[this.m_size];
			int idx = 0;

			// Type (2 byte)
			//
			byte[] intBytes = BitConverter.GetBytes((short)this.m_type);
			Buffer.BlockCopy(intBytes, 0, arr, idx, intBytes.Length);
			idx += 2;

			// Length (2 byte)
			//
			intBytes = BitConverter.GetBytes(this.m_length);
			Buffer.BlockCopy(intBytes, 0, arr, idx, intBytes.Length);
			idx += 2;

			// Value
			//
			Buffer.BlockCopy(this.m_value, 0, arr, idx, this.m_value.Length);

			return arr;
		}

		public int decode(byte[] buff, int idx)
		{
			//type length value
			this.m_type = (Argument.Type)BitConverter.ToInt16(buff, idx);
			idx += 2;
			this.m_length = BitConverter.ToInt16(buff, idx);
			idx += 2;
			this.m_size = 2 + 2 + (this.m_length / 4 + 1) * 4;		// data type 

			int data_len = this.m_length * Argument.type_size[(int)this.m_type];
			this.m_value = new byte[data_len];
			Buffer.BlockCopy(buff, idx, this.m_value, 0, data_len);

			idx += this.m_size;
			return idx;
		}
	}
}
