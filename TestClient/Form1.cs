using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace TestClient
{
	public partial class Form1 : Form
	{
		public Form1()
		{
			InitializeComponent();
		}

		public void PrintOut(string mesg)
		{
			this.tbOutput.Text += mesg + System.Environment.NewLine;
		}

		private void btnDummySimConn_Click(object sender, EventArgs e)
		{
			this.PrintOut("Connecting to " + this.tbDummySimHost.Text + ":" + this.tbDummySimPort.Text);
		}
	}
}
