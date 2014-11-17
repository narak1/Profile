using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TestClient
{
	class Argument
	{
		public enum Type { None, Char, UChar, Bool, Short, Integer, Long, Single, Double, Binary };
		public Type m_type = Type.None;
		public int m_length = 0;			// encoding block length
		public byte[] m_value = null;

		public Argument()
		{
			this.m_type = Type.None;
			this.m_value = new byte [0];
			this.m_length = 2 + 2;
		}

		public Argument(string str)
		{
			this.m_type = Type.Char;
			this.m_value = Encoding.ASCII.GetBytes(str);
			Array.Resize(ref this.m_value, str.Length+1);
			this.m_length = 2 + 2 + ((this.m_value.Length + 3) / 4) * 4;
		}

		public Argument(double val)
		{
			this.m_type = Type.Double;
			this.m_value = BitConverter.GetBytes(val);
			this.m_length = 2 + 2 + this.m_value.Length;
		}

		public byte[] encode()
		{
			byte[] arr = new byte[this.m_length];
			int idx = 0;

			// Type (2 byte)
			//
			byte[] intBytes = BitConverter.GetBytes((short)this.m_type);
			Buffer.BlockCopy(intBytes, 0, arr, idx, intBytes.Length);
			idx += intBytes.Length;

			// Length (2 byte)
			//
			intBytes = BitConverter.GetBytes((short)this.m_value.Length);
			Buffer.BlockCopy(intBytes, 0, arr, idx, intBytes.Length);
			idx += intBytes.Length;

			// Value
			//
			Buffer.BlockCopy(this.m_value, 0, arr, idx, this.m_value.Length);

			return arr;
		}
	}
}
