using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ARAInst
{
	public static class Globals
	{
		public static Form1 m_form = null;

		public static void print_log(string s)
		{
			if (m_form != null)
				m_form.PrintLog(s);
		}

		public static void print_out(string s)
		{
			if (m_form != null)
				m_form.PrintOut(s);
		}
	}
}
