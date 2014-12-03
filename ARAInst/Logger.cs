using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.IO;

namespace ARAInst
{
	public class Logger
	{
		public enum LogLevel { NoLog, ErrorLog, InfoLog, DebugLog };
		public static LogLevel Default_LogLevel = LogLevel.DebugLog;
		public LogLevel m_LogLevel = Logger.Default_LogLevel;
		public string m_format = "'ARAInst_'yyyy'-'MM'-'dd'.log'";

		StreamWriter m_sw = null;
		int m_Date = 0;
		Object thisLock = new Object();

		public void init(LogLevel log_level=LogLevel.DebugLog)
		{
			this.m_LogLevel = log_level;
			if (log_level == LogLevel.NoLog)
			{
				return;
			}

			try
			{
				DateTime cur = DateTime.Now;
				this.m_Date = cur.DayOfYear;
				string strFile = cur.ToString(m_format);
				this.m_sw = new StreamWriter(strFile);
			}
			catch (Exception ex)
			{
				throw ex;
			}
		}

		public void finish()
		{
			try
			{
				this.m_sw.Close();
				this.m_sw = null;
			}
			catch (Exception ex)
			{
				throw ex;
			}
		}

		public void print_log(LogLevel level, string s)
		{
			if (level > this.m_LogLevel)
			{
				return;
			}
			if (this.m_sw == null)
			{
				return;
			}
			try
			{
				DateTime cur = DateTime.Now;
				lock (thisLock)
				{
					if (this.m_Date != cur.DayOfYear)
					{
						this.m_Date = cur.DayOfYear;
						string strFile = cur.ToString(m_format);
						this.m_sw = new StreamWriter(strFile);
					}
				}
				// http://msdn.microsoft.com/ko-kr/library/az4se3k1(v=vs.110).aspx
				this.m_sw.WriteLine(cur.ToString("s") + " " + s);	// SortableDateTimePattern
				this.m_sw.Flush();
			}
			catch (Exception ex)
			{
				throw ex;
			}
		}
	}
}
