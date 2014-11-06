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
			// 
			// tbDummySimPort
			// 
			this.tbDummySimPort.Location = new System.Drawing.Point(213, 13);
			this.tbDummySimPort.Name = "tbDummySimPort";
			this.tbDummySimPort.Size = new System.Drawing.Size(49, 25);
			this.tbDummySimPort.TabIndex = 2;
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
			// 
			// tbOutput
			// 
			this.tbOutput.Location = new System.Drawing.Point(15, 252);
			this.tbOutput.Multiline = true;
			this.tbOutput.Name = "tbOutput";
			this.tbOutput.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
			this.tbOutput.Size = new System.Drawing.Size(487, 154);
			this.tbOutput.TabIndex = 5;
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(514, 418);
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
	}
}

