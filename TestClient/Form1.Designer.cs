namespace TestClient
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
			this.label1 = new System.Windows.Forms.Label();
			this.tbDummySimHost = new System.Windows.Forms.TextBox();
			this.tbDummySimPort = new System.Windows.Forms.TextBox();
			this.btnDummySimConn = new System.Windows.Forms.Button();
			this.btnDummySimClose = new System.Windows.Forms.Button();
			this.tbOutput = new System.Windows.Forms.TextBox();
			this.tbOption = new System.Windows.Forms.TextBox();
			this.btnAdd = new System.Windows.Forms.Button();
			this.btnDel = new System.Windows.Forms.Button();
			this.btnRead = new System.Windows.Forms.Button();
			this.btnWrite = new System.Windows.Forms.Button();
			this.btnSave = new System.Windows.Forms.Button();
			this.btnLoad = new System.Windows.Forms.Button();
			this.btnRemove = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(12, 16);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(84, 15);
			this.label1.TabIndex = 0;
			this.label1.Text = "DummySim:";
			// 
			// tbDummySimHost
			// 
			this.tbDummySimHost.Location = new System.Drawing.Point(107, 13);
			this.tbDummySimHost.Name = "tbDummySimHost";
			this.tbDummySimHost.Size = new System.Drawing.Size(100, 25);
			this.tbDummySimHost.TabIndex = 1;
			this.tbDummySimHost.Text = "localhost";
			// 
			// tbDummySimPort
			// 
			this.tbDummySimPort.Location = new System.Drawing.Point(213, 13);
			this.tbDummySimPort.Name = "tbDummySimPort";
			this.tbDummySimPort.Size = new System.Drawing.Size(49, 25);
			this.tbDummySimPort.TabIndex = 2;
			this.tbDummySimPort.Text = "9999";
			// 
			// btnDummySimConn
			// 
			this.btnDummySimConn.Location = new System.Drawing.Point(268, 5);
			this.btnDummySimConn.Name = "btnDummySimConn";
			this.btnDummySimConn.Size = new System.Drawing.Size(75, 37);
			this.btnDummySimConn.TabIndex = 3;
			this.btnDummySimConn.Text = "Connect";
			this.btnDummySimConn.UseVisualStyleBackColor = true;
			this.btnDummySimConn.Click += new System.EventHandler(this.btnDummySimConn_Click);
			// 
			// btnDummySimClose
			// 
			this.btnDummySimClose.Location = new System.Drawing.Point(349, 5);
			this.btnDummySimClose.Name = "btnDummySimClose";
			this.btnDummySimClose.Size = new System.Drawing.Size(75, 37);
			this.btnDummySimClose.TabIndex = 4;
			this.btnDummySimClose.Text = "Close";
			this.btnDummySimClose.UseVisualStyleBackColor = true;
			this.btnDummySimClose.Click += new System.EventHandler(this.btnDummySimClose_Click);
			// 
			// tbOutput
			// 
			this.tbOutput.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.tbOutput.Location = new System.Drawing.Point(12, 168);
			this.tbOutput.Multiline = true;
			this.tbOutput.Name = "tbOutput";
			this.tbOutput.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
			this.tbOutput.Size = new System.Drawing.Size(428, 154);
			this.tbOutput.TabIndex = 5;
			// 
			// tbOption
			// 
			this.tbOption.Location = new System.Drawing.Point(15, 48);
			this.tbOption.Name = "tbOption";
			this.tbOption.Size = new System.Drawing.Size(409, 25);
			this.tbOption.TabIndex = 6;
			// 
			// btnAdd
			// 
			this.btnAdd.Location = new System.Drawing.Point(12, 79);
			this.btnAdd.Name = "btnAdd";
			this.btnAdd.Size = new System.Drawing.Size(89, 37);
			this.btnAdd.TabIndex = 7;
			this.btnAdd.Text = "Tag Add";
			this.btnAdd.UseVisualStyleBackColor = true;
			this.btnAdd.Click += new System.EventHandler(this.btnAdd_Click);
			// 
			// btnDel
			// 
			this.btnDel.Location = new System.Drawing.Point(107, 79);
			this.btnDel.Name = "btnDel";
			this.btnDel.Size = new System.Drawing.Size(89, 37);
			this.btnDel.TabIndex = 8;
			this.btnDel.Text = "Tag Del";
			this.btnDel.UseVisualStyleBackColor = true;
			// 
			// btnRead
			// 
			this.btnRead.Location = new System.Drawing.Point(202, 79);
			this.btnRead.Name = "btnRead";
			this.btnRead.Size = new System.Drawing.Size(89, 37);
			this.btnRead.TabIndex = 9;
			this.btnRead.Text = "Tag Read";
			this.btnRead.UseVisualStyleBackColor = true;
			// 
			// btnWrite
			// 
			this.btnWrite.Location = new System.Drawing.Point(297, 79);
			this.btnWrite.Name = "btnWrite";
			this.btnWrite.Size = new System.Drawing.Size(89, 37);
			this.btnWrite.TabIndex = 10;
			this.btnWrite.Text = "Tag Write";
			this.btnWrite.UseVisualStyleBackColor = true;
			// 
			// btnSave
			// 
			this.btnSave.Location = new System.Drawing.Point(12, 122);
			this.btnSave.Name = "btnSave";
			this.btnSave.Size = new System.Drawing.Size(139, 37);
			this.btnSave.TabIndex = 11;
			this.btnSave.Text = "Snapshot Save";
			this.btnSave.UseVisualStyleBackColor = true;
			// 
			// btnLoad
			// 
			this.btnLoad.Location = new System.Drawing.Point(157, 122);
			this.btnLoad.Name = "btnLoad";
			this.btnLoad.Size = new System.Drawing.Size(139, 37);
			this.btnLoad.TabIndex = 12;
			this.btnLoad.Text = "Snapshot Load";
			this.btnLoad.UseVisualStyleBackColor = true;
			// 
			// btnRemove
			// 
			this.btnRemove.Location = new System.Drawing.Point(302, 122);
			this.btnRemove.Name = "btnRemove";
			this.btnRemove.Size = new System.Drawing.Size(139, 37);
			this.btnRemove.TabIndex = 13;
			this.btnRemove.Text = "Snapshot Remove";
			this.btnRemove.UseVisualStyleBackColor = true;
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(452, 334);
			this.Controls.Add(this.btnRemove);
			this.Controls.Add(this.btnLoad);
			this.Controls.Add(this.btnSave);
			this.Controls.Add(this.btnWrite);
			this.Controls.Add(this.btnRead);
			this.Controls.Add(this.btnDel);
			this.Controls.Add(this.btnAdd);
			this.Controls.Add(this.tbOption);
			this.Controls.Add(this.tbOutput);
			this.Controls.Add(this.btnDummySimClose);
			this.Controls.Add(this.btnDummySimConn);
			this.Controls.Add(this.tbDummySimPort);
			this.Controls.Add(this.tbDummySimHost);
			this.Controls.Add(this.label1);
			this.Name = "Form1";
			this.Text = "Form1";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.TextBox tbDummySimHost;
		private System.Windows.Forms.TextBox tbDummySimPort;
		private System.Windows.Forms.Button btnDummySimConn;
		private System.Windows.Forms.Button btnDummySimClose;
		private System.Windows.Forms.TextBox tbOutput;
		private System.Windows.Forms.TextBox tbOption;
		private System.Windows.Forms.Button btnAdd;
		private System.Windows.Forms.Button btnDel;
		private System.Windows.Forms.Button btnRead;
		private System.Windows.Forms.Button btnWrite;
		private System.Windows.Forms.Button btnSave;
		private System.Windows.Forms.Button btnLoad;
		private System.Windows.Forms.Button btnRemove;
	}
}

