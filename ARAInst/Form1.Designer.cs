namespace ARAInst
{
	partial class Form1
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.components = new System.ComponentModel.Container();
			this.label1 = new System.Windows.Forms.Label();
			this.comboBox_ServerType = new System.Windows.Forms.ComboBox();
			this.label2 = new System.Windows.Forms.Label();
			this.textBox_NodeName = new System.Windows.Forms.TextBox();
			this.textBox_Hostname = new System.Windows.Forms.TextBox();
			this.textBox_ServicePort = new System.Windows.Forms.TextBox();
			this.textBox_ServerPriority = new System.Windows.Forms.TextBox();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.button_add = new System.Windows.Forms.Button();
			this.label5 = new System.Windows.Forms.Label();
			this.label4 = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.listViewEx1 = new ListViewEx.ListViewEx();
			this.columnHeader6 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHeader3 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHeader4 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHeader5 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHeader7 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.contextMenuStrip1 = new System.Windows.Forms.ContextMenuStrip(this.components);
			this.deleteToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.textBox_log = new System.Windows.Forms.TextBox();
			this.button_open = new System.Windows.Forms.Button();
			this.button_download = new System.Windows.Forms.Button();
			this.textBox1 = new System.Windows.Forms.TextBox();
			this.textBox2 = new System.Windows.Forms.TextBox();
			this.label6 = new System.Windows.Forms.Label();
			this.label7 = new System.Windows.Forms.Label();
			this.button_manual = new System.Windows.Forms.Button();
			this.label10 = new System.Windows.Forms.Label();
			this.comboBox1 = new System.Windows.Forms.ComboBox();
			this.button_auto = new System.Windows.Forms.Button();
			this.label9 = new System.Windows.Forms.Label();
			this.textBox3 = new System.Windows.Forms.TextBox();
			this.button_view = new System.Windows.Forms.Button();
			this.button1 = new System.Windows.Forms.Button();
			this.button2 = new System.Windows.Forms.Button();
			this.groupBox1.SuspendLayout();
			this.contextMenuStrip1.SuspendLayout();
			this.SuspendLayout();
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(209, 63);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(91, 15);
			this.label1.TabIndex = 8;
			this.label1.Text = "Server Type:";
			// 
			// comboBox_ServerType
			// 
			this.comboBox_ServerType.FormattingEnabled = true;
			this.comboBox_ServerType.Items.AddRange(new object[] {
            "HYSYS.DLL",
            "HYSYS.OPC",
            "DeltaV.OPC",
            "Dummy.TCP"});
			this.comboBox_ServerType.Location = new System.Drawing.Point(306, 60);
			this.comboBox_ServerType.Name = "comboBox_ServerType";
			this.comboBox_ServerType.Size = new System.Drawing.Size(103, 23);
			this.comboBox_ServerType.TabIndex = 9;
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(6, 63);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(87, 15);
			this.label2.TabIndex = 6;
			this.label2.Text = "Node Name:";
			// 
			// textBox_NodeName
			// 
			this.textBox_NodeName.Location = new System.Drawing.Point(99, 60);
			this.textBox_NodeName.Name = "textBox_NodeName";
			this.textBox_NodeName.Size = new System.Drawing.Size(104, 25);
			this.textBox_NodeName.TabIndex = 7;
			this.textBox_NodeName.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
			// 
			// textBox_Hostname
			// 
			this.textBox_Hostname.Location = new System.Drawing.Point(109, 30);
			this.textBox_Hostname.Name = "textBox_Hostname";
			this.textBox_Hostname.Size = new System.Drawing.Size(94, 25);
			this.textBox_Hostname.TabIndex = 1;
			this.textBox_Hostname.Text = "localhost";
			this.textBox_Hostname.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
			// 
			// textBox_ServicePort
			// 
			this.textBox_ServicePort.Location = new System.Drawing.Point(296, 30);
			this.textBox_ServicePort.MaxLength = 5;
			this.textBox_ServicePort.Name = "textBox_ServicePort";
			this.textBox_ServicePort.Size = new System.Drawing.Size(50, 25);
			this.textBox_ServicePort.TabIndex = 3;
			this.textBox_ServicePort.Text = "1234";
			this.textBox_ServicePort.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
			// 
			// textBox_ServerPriority
			// 
			this.textBox_ServerPriority.Location = new System.Drawing.Point(415, 30);
			this.textBox_ServerPriority.MaxLength = 5;
			this.textBox_ServerPriority.Name = "textBox_ServerPriority";
			this.textBox_ServerPriority.Size = new System.Drawing.Size(49, 25);
			this.textBox_ServerPriority.TabIndex = 5;
			this.textBox_ServerPriority.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
			// 
			// groupBox1
			// 
			this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.groupBox1.Controls.Add(this.button_add);
			this.groupBox1.Controls.Add(this.label5);
			this.groupBox1.Controls.Add(this.textBox_ServerPriority);
			this.groupBox1.Controls.Add(this.label4);
			this.groupBox1.Controls.Add(this.label3);
			this.groupBox1.Controls.Add(this.textBox_ServicePort);
			this.groupBox1.Controls.Add(this.label1);
			this.groupBox1.Controls.Add(this.comboBox_ServerType);
			this.groupBox1.Controls.Add(this.textBox_Hostname);
			this.groupBox1.Controls.Add(this.label2);
			this.groupBox1.Controls.Add(this.textBox_NodeName);
			this.groupBox1.Location = new System.Drawing.Point(13, 13);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(580, 92);
			this.groupBox1.TabIndex = 7;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Server Information";
			// 
			// button_add
			// 
			this.button_add.Location = new System.Drawing.Point(476, 24);
			this.button_add.Name = "button_add";
			this.button_add.Size = new System.Drawing.Size(98, 54);
			this.button_add.TabIndex = 10;
			this.button_add.Text = "Add";
			this.button_add.UseVisualStyleBackColor = true;
			this.button_add.Click += new System.EventHandler(this.button_add_Click);
			// 
			// label5
			// 
			this.label5.AutoSize = true;
			this.label5.Location = new System.Drawing.Point(352, 33);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(57, 15);
			this.label5.TabIndex = 4;
			this.label5.Text = "Priority:";
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Location = new System.Drawing.Point(209, 33);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(81, 15);
			this.label4.TabIndex = 2;
			this.label4.Text = "Port No(*):";
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(6, 33);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(97, 15);
			this.label3.TabIndex = 0;
			this.label3.Text = "Hostname(*):";
			// 
			// listViewEx1
			// 
			this.listViewEx1.AllowColumnReorder = true;
			this.listViewEx1.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader6,
            this.columnHeader1,
            this.columnHeader2,
            this.columnHeader3,
            this.columnHeader4,
            this.columnHeader5,
            this.columnHeader7});
			this.listViewEx1.ContextMenuStrip = this.contextMenuStrip1;
			this.listViewEx1.DoubleClickActivation = false;
			this.listViewEx1.FullRowSelect = true;
			this.listViewEx1.GridLines = true;
			this.listViewEx1.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
			this.listViewEx1.Location = new System.Drawing.Point(13, 112);
			this.listViewEx1.MultiSelect = false;
			this.listViewEx1.Name = "listViewEx1";
			this.listViewEx1.Size = new System.Drawing.Size(579, 172);
			this.listViewEx1.TabIndex = 8;
			this.listViewEx1.UseCompatibleStateImageBehavior = false;
			this.listViewEx1.View = System.Windows.Forms.View.Details;
			// 
			// columnHeader6
			// 
			this.columnHeader6.Text = "No.";
			this.columnHeader6.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
			this.columnHeader6.Width = 40;
			// 
			// columnHeader1
			// 
			this.columnHeader1.Text = "Server Type";
			this.columnHeader1.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
			this.columnHeader1.Width = 100;
			// 
			// columnHeader2
			// 
			this.columnHeader2.Text = "Node Name";
			this.columnHeader2.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
			this.columnHeader2.Width = 100;
			// 
			// columnHeader3
			// 
			this.columnHeader3.Text = "Hostname";
			this.columnHeader3.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
			this.columnHeader3.Width = 100;
			// 
			// columnHeader4
			// 
			this.columnHeader4.Text = "Portno";
			this.columnHeader4.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
			// 
			// columnHeader5
			// 
			this.columnHeader5.Text = "Priority";
			this.columnHeader5.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
			// 
			// columnHeader7
			// 
			this.columnHeader7.Text = "Status";
			this.columnHeader7.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
			this.columnHeader7.Width = 90;
			// 
			// contextMenuStrip1
			// 
			this.contextMenuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.deleteToolStripMenuItem});
			this.contextMenuStrip1.Name = "contextMenuStrip1";
			this.contextMenuStrip1.Size = new System.Drawing.Size(123, 28);
			// 
			// deleteToolStripMenuItem
			// 
			this.deleteToolStripMenuItem.Name = "deleteToolStripMenuItem";
			this.deleteToolStripMenuItem.Size = new System.Drawing.Size(122, 24);
			this.deleteToolStripMenuItem.Text = "Delete";
			// 
			// textBox_log
			// 
			this.textBox_log.Location = new System.Drawing.Point(13, 291);
			this.textBox_log.Multiline = true;
			this.textBox_log.Name = "textBox_log";
			this.textBox_log.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
			this.textBox_log.Size = new System.Drawing.Size(579, 130);
			this.textBox_log.TabIndex = 9;
			// 
			// button_open
			// 
			this.button_open.Location = new System.Drawing.Point(13, 458);
			this.button_open.Name = "button_open";
			this.button_open.Size = new System.Drawing.Size(75, 52);
			this.button_open.TabIndex = 0;
			this.button_open.Text = "Open...";
			this.button_open.UseVisualStyleBackColor = true;
			this.button_open.Click += new System.EventHandler(this.button_open_Click);
			// 
			// button_download
			// 
			this.button_download.Location = new System.Drawing.Point(94, 458);
			this.button_download.Name = "button_download";
			this.button_download.Size = new System.Drawing.Size(87, 52);
			this.button_download.TabIndex = 1;
			this.button_download.Text = "Downlolad";
			this.button_download.UseVisualStyleBackColor = true;
			// 
			// textBox1
			// 
			this.textBox1.Location = new System.Drawing.Point(136, 427);
			this.textBox1.Name = "textBox1";
			this.textBox1.Size = new System.Drawing.Size(39, 25);
			this.textBox1.TabIndex = 11;
			this.textBox1.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
			// 
			// textBox2
			// 
			this.textBox2.Location = new System.Drawing.Point(264, 427);
			this.textBox2.Name = "textBox2";
			this.textBox2.Size = new System.Drawing.Size(39, 25);
			this.textBox2.TabIndex = 13;
			this.textBox2.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
			// 
			// label6
			// 
			this.label6.AutoSize = true;
			this.label6.Location = new System.Drawing.Point(12, 430);
			this.label6.Name = "label6";
			this.label6.Size = new System.Drawing.Size(118, 15);
			this.label6.TabIndex = 10;
			this.label6.Text = "Time Step (sec):";
			// 
			// label7
			// 
			this.label7.AutoSize = true;
			this.label7.Location = new System.Drawing.Point(183, 428);
			this.label7.Name = "label7";
			this.label7.Size = new System.Drawing.Size(75, 15);
			this.label7.TabIndex = 12;
			this.label7.Text = "Step Num:";
			// 
			// button_manual
			// 
			this.button_manual.Location = new System.Drawing.Point(186, 458);
			this.button_manual.Name = "button_manual";
			this.button_manual.Size = new System.Drawing.Size(72, 52);
			this.button_manual.TabIndex = 2;
			this.button_manual.Text = "Manual Run";
			this.button_manual.UseVisualStyleBackColor = true;
			// 
			// label10
			// 
			this.label10.AutoSize = true;
			this.label10.Location = new System.Drawing.Point(312, 428);
			this.label10.Name = "label10";
			this.label10.Size = new System.Drawing.Size(54, 15);
			this.label10.TabIndex = 14;
			this.label10.Text = "Speed:";
			// 
			// comboBox1
			// 
			this.comboBox1.FormattingEnabled = true;
			this.comboBox1.Items.AddRange(new object[] {
            "0.5",
            "1",
            "2",
            "3",
            "4",
            "5",
            "6",
            "7",
            "8",
            "9",
            "10"});
			this.comboBox1.Location = new System.Drawing.Point(372, 427);
			this.comboBox1.Name = "comboBox1";
			this.comboBox1.Size = new System.Drawing.Size(62, 23);
			this.comboBox1.TabIndex = 15;
			// 
			// button_auto
			// 
			this.button_auto.Location = new System.Drawing.Point(264, 458);
			this.button_auto.Name = "button_auto";
			this.button_auto.Size = new System.Drawing.Size(69, 52);
			this.button_auto.TabIndex = 3;
			this.button_auto.Text = "Auto Run";
			this.button_auto.UseVisualStyleBackColor = true;
			// 
			// label9
			// 
			this.label9.AutoSize = true;
			this.label9.Location = new System.Drawing.Point(440, 430);
			this.label9.Name = "label9";
			this.label9.Size = new System.Drawing.Size(90, 15);
			this.label9.TabIndex = 16;
			this.label9.Text = "Limits (sec):";
			// 
			// textBox3
			// 
			this.textBox3.Location = new System.Drawing.Point(532, 425);
			this.textBox3.Name = "textBox3";
			this.textBox3.Size = new System.Drawing.Size(60, 25);
			this.textBox3.TabIndex = 17;
			this.textBox3.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
			// 
			// button_view
			// 
			this.button_view.Location = new System.Drawing.Point(339, 458);
			this.button_view.Name = "button_view";
			this.button_view.Size = new System.Drawing.Size(87, 52);
			this.button_view.TabIndex = 4;
			this.button_view.Text = "View Taglist...";
			this.button_view.UseVisualStyleBackColor = true;
			// 
			// button1
			// 
			this.button1.Location = new System.Drawing.Point(432, 458);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(78, 52);
			this.button1.TabIndex = 5;
			this.button1.Text = "Snapshot Save";
			this.button1.UseVisualStyleBackColor = true;
			// 
			// button2
			// 
			this.button2.Location = new System.Drawing.Point(516, 458);
			this.button2.Name = "button2";
			this.button2.Size = new System.Drawing.Size(78, 52);
			this.button2.TabIndex = 6;
			this.button2.Text = "Snapshot Restore";
			this.button2.UseVisualStyleBackColor = true;
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(605, 522);
			this.Controls.Add(this.button2);
			this.Controls.Add(this.button1);
			this.Controls.Add(this.button_view);
			this.Controls.Add(this.label9);
			this.Controls.Add(this.textBox3);
			this.Controls.Add(this.button_auto);
			this.Controls.Add(this.comboBox1);
			this.Controls.Add(this.label10);
			this.Controls.Add(this.button_manual);
			this.Controls.Add(this.label7);
			this.Controls.Add(this.label6);
			this.Controls.Add(this.textBox2);
			this.Controls.Add(this.textBox1);
			this.Controls.Add(this.button_download);
			this.Controls.Add(this.button_open);
			this.Controls.Add(this.textBox_log);
			this.Controls.Add(this.listViewEx1);
			this.Controls.Add(this.groupBox1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
			this.Name = "Form1";
			this.Text = "ARAInst V2 Profile";
			this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Form1_FormClosed);
			this.Load += new System.EventHandler(this.Form1_Load);
			this.groupBox1.ResumeLayout(false);
			this.groupBox1.PerformLayout();
			this.contextMenuStrip1.ResumeLayout(false);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.ComboBox comboBox_ServerType;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.TextBox textBox_NodeName;
		private System.Windows.Forms.TextBox textBox_Hostname;
		private System.Windows.Forms.TextBox textBox_ServicePort;
		private System.Windows.Forms.TextBox textBox_ServerPriority;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.Button button_add;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Label label3;
		private ListViewEx.ListViewEx listViewEx1;
		private System.Windows.Forms.ColumnHeader columnHeader6;
		private System.Windows.Forms.ColumnHeader columnHeader1;
		private System.Windows.Forms.ColumnHeader columnHeader2;
		private System.Windows.Forms.ColumnHeader columnHeader3;
		private System.Windows.Forms.ColumnHeader columnHeader4;
		private System.Windows.Forms.ColumnHeader columnHeader5;
		private System.Windows.Forms.ColumnHeader columnHeader7;
		private System.Windows.Forms.ContextMenuStrip contextMenuStrip1;
		private System.Windows.Forms.ToolStripMenuItem deleteToolStripMenuItem;
		private System.Windows.Forms.TextBox textBox_log;
		private System.Windows.Forms.Button button_open;
		private System.Windows.Forms.Button button_download;
		private System.Windows.Forms.TextBox textBox1;
		private System.Windows.Forms.TextBox textBox2;
		private System.Windows.Forms.Label label6;
		private System.Windows.Forms.Label label7;
		private System.Windows.Forms.Button button_manual;
		private System.Windows.Forms.Label label10;
		private System.Windows.Forms.ComboBox comboBox1;
		private System.Windows.Forms.Button button_auto;
		private System.Windows.Forms.Label label9;
		private System.Windows.Forms.TextBox textBox3;
		private System.Windows.Forms.Button button_view;
		private System.Windows.Forms.Button button1;
		private System.Windows.Forms.Button button2;
	}
}

