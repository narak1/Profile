using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.IO;
using System.Xml;

namespace ARAInst
{
	public class ServerManager
	{
		string m_config_file = "system.xml";

		string str_root = "Config";
		string str_arainst = "ARAInst";
		string str_simbridge = "SIMBridge";

		string str_autosnapshot = "AutoSnapshot";
		string str_save_interval = "interval";
		string str_save_filenum = "filenum";

		string str_server = "Server";
		enum ServerConfigKey { HostName, PortNo, NodeName, ServerType, Priority, OPCProgID, TCPPortNo, AviDir, LogDir, LogMode };
		string[] server_config_key = { "Hostname", "PortNo", "NodeName", "ServerType", "Priority",
									"OPC.ProgID", "TCP.PortNo", "AVI_Folder", "LogFolder", "LogMode", };

		public int auto_save_interval = 10 * 60;	// sec
		public int auto_save_filenum = 20;

		List<ServerInfo> m_server = new List<ServerInfo>();

		public bool Add(ServerInfo info)
		{
			try
			{
				foreach (ServerInfo si in this.m_server)
				{
					if (info.node_name.Length != 0 && si.node_name == info.node_name)
					{
						return false;
					}
					else if (si.end_point.ToString() == info.end_point.ToString())
					{
						return false;
					}
				}
				this.m_server.Add(info);
				return true;
			}
			catch (Exception ex)
			{
				throw ex;
			}
		}

		public int Num()
		{
			return this.m_server.Count;
		}

		public void save(string file_path = null)
		{
			try
			{
				if (file_path == null) file_path = this.m_config_file;
				else this.m_config_file = file_path;

				FileStream fs = new FileStream(file_path, FileMode.Create);
				StreamWriter output = new StreamWriter(fs);

				XmlWriterSettings settings = new XmlWriterSettings();
				settings.Indent = true;
				XmlWriter xml = XmlWriter.Create(output, settings);

				xml.WriteStartDocument();
				xml.WriteStartElement(this.str_root);

				xml.WriteStartElement(this.str_arainst);
				xml.WriteStartElement(str_autosnapshot);

				xml.WriteStartElement(str_save_interval);
				xml.WriteValue(this.auto_save_interval);
				xml.WriteEndElement();

				xml.WriteStartElement(str_save_filenum);
				xml.WriteValue(this.auto_save_filenum);
				xml.WriteEndElement();

				xml.WriteEndElement();	// autosnapshot
				xml.WriteEndElement();	// ARAInst

				xml.WriteStartElement(this.str_simbridge);
				foreach (ServerInfo si in this.m_server)
				{
					this.write_server_info(xml, si);
				}
				xml.WriteEndElement();	// SIMBridge

				xml.WriteEndElement();	// root
				xml.WriteEndDocument();
				xml.Close();

				output.Close();
				fs.Close();
			}
			catch (Exception ex)
			{
				throw ex;
			}
		}

		public void load(string file_path = null)
		{
			try
			{
				if (file_path == null) file_path = this.m_config_file;
				else this.m_config_file = file_path;

				FileStream fs = new FileStream(file_path, FileMode.Open);
				StreamReader input = new StreamReader(fs);

				XmlReaderSettings settings = new XmlReaderSettings();
				settings.IgnoreComments = true;
				settings.IgnoreWhitespace = true;
				XmlReader xml = XmlReader.Create(input, settings);

				xml.Read();
				xml.ReadStartElement(this.str_root);

				xml.ReadStartElement(this.str_arainst);
				xml.ReadStartElement(str_autosnapshot);

				xml.ReadStartElement(str_save_interval);
				this.auto_save_interval = int.Parse(xml.ReadString());
				xml.ReadEndElement();

				xml.ReadStartElement(str_save_filenum);
				this.auto_save_filenum = int.Parse(xml.ReadString());
				xml.ReadEndElement();

				xml.ReadEndElement();	// autosnapshot
				xml.ReadEndElement();	// ARAInst

				xml.ReadStartElement(this.str_simbridge);
				while (xml.IsStartElement(this.str_server))
				{
					this.m_server.Add(this.read_server_info(xml));
				}
				xml.ReadEndElement();	// SIMBridge

				xml.ReadEndElement();	// root
				xml.Close();
				input.Close();
				fs.Close();
			}
			catch (Exception ex)
			{
				throw ex;
			}
		}

		private string xml_option_read(XmlReader xml, string key)
		{
			try
			{
				string ret = null;
				if (xml.IsStartElement(key))
				{
					xml.ReadStartElement(key);
					ret = xml.ReadString();
					xml.ReadEndElement();
				}
				return ret;
			}
			catch (Exception ex)
			{
				throw ex;
			}
		}

		private int xml_option_read_int(XmlReader xml, string key)
		{
			try
			{
				string ret = xml_option_read(xml, key);
				return int.Parse(ret);
			}
			catch (Exception)
			{
				return 0;
			}
		}

		private void xml_option_write(XmlWriter xml, string key, string value)
		{
			try
			{
				Globals.print_out("[" + key + "]=[" + value + "]");
				xml.WriteStartElement(key);
				xml.WriteString(value);
				xml.WriteEndElement();
			}
			catch (Exception ex)
			{
				throw ex;
			}
		}

		private void write_server_info(XmlWriter xml, ServerInfo info)
		{
			try
			{
				Globals.print_out("write_server_info(,)");

				int idx = 0;
				xml.WriteStartElement(this.str_server);
				this.xml_option_write(xml, this.server_config_key[idx++], info.hostname);
				this.xml_option_write(xml, this.server_config_key[idx++], info.port_no.ToString());
				this.xml_option_write(xml, this.server_config_key[idx++], info.node_name);
				this.xml_option_write(xml, this.server_config_key[idx++], info.server_type);
				this.xml_option_write(xml, this.server_config_key[idx++], info.priority.ToString());

				if (info.opc_prog_id != null && info.opc_prog_id != "")
				{
					this.xml_option_write(xml, this.server_config_key[idx], info.opc_prog_id);
				}
				++idx;

				if (info.tcp_port_no > 0)
				{
					this.xml_option_write(xml, this.server_config_key[idx], info.tcp_port_no.ToString());
				}
				++idx;

				if (info.avi_folder != null && info.avi_folder != "")
				{
					this.xml_option_write(xml, this.server_config_key[idx], info.avi_folder);
				}
				++idx;

				this.xml_option_write(xml, this.server_config_key[idx++], info.log_folder);
				this.xml_option_write(xml, this.server_config_key[idx], info.log_mode.ToString());
				xml.WriteEndElement();	// server
			}
			catch (Exception ex)
			{
				throw ex;
			}
		}

		private ServerInfo read_server_info(XmlReader xml)
		{
			try
			{
				ServerInfo info = new ServerInfo();

				int idx = 0;
				xml.ReadStartElement(this.str_server);
				info.hostname = this.xml_option_read(xml, this.server_config_key[idx++]);
				info.port_no = this.xml_option_read_int(xml, this.server_config_key[idx++]);
				info.node_name = this.xml_option_read(xml, this.server_config_key[idx++]);
				info.server_type = this.xml_option_read(xml, this.server_config_key[idx++]);
				info.priority = this.xml_option_read_int(xml, this.server_config_key[idx++]);

				info.opc_prog_id = this.xml_option_read(xml, this.server_config_key[idx++]);
				info.tcp_port_no = this.xml_option_read_int(xml, this.server_config_key[idx++]);

				info.avi_folder = this.xml_option_read(xml, this.server_config_key[idx++]);
				info.log_folder = this.xml_option_read(xml, this.server_config_key[idx++]);
				string log_mode = this.xml_option_read(xml, this.server_config_key[idx++]);
				try
				{
					info.log_mode = (Logger.LogLevel)Enum.Parse(typeof(Logger.LogLevel), log_mode);
				}
				catch (Exception)
				{
					info.log_mode = Logger.Default_LogLevel;
				}
				xml.ReadEndElement();	// server

				return info;
			}
			catch (Exception ex)
			{
				throw ex;
			}
		}
	}
}
