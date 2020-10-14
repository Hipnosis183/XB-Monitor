using XBMonitor.Classes;
using System;
using System.Windows.Forms;
using System.Diagnostics;

namespace XBMonitor
{
    public partial class XBConfigForm : Form
    {
        public XBConfigForm()
        {
            InitializeComponent();

            LinkLabel_Romhack.TabStop = false;
            LinkLabel_Hipnosis.TabStop = false;
            Label_Middle.TabStop = false;

            XBConfig.CreateJoystickValues();
        }

        private void ConfigForm_Load(object sender, EventArgs e)
        {
            XBConfig.SetFeatureToAllControls(this.Controls);

            XBConfig.InitialSetup();
            LoadSettings();
        }

        private void LoadSettings()
        {
            XBConfig.LoadController(ComboBox_Device);

            LoadInputs();
            LoadMisc();
        }

        private void LoadInputs()
        {
            LoadInputs_P0();
            LoadInputs_P1();
            LoadInputs_P2();
        }

        private void LoadInputs_P0()
        {
            XBConfig.LoadInput(Button_P0_Test, 0x08);
            XBConfig.LoadInput(Button_P0_Exit, 0x6C);
        }

        private void LoadInputs_P1()
        {
            XBConfig.LoadInput(Button_P1_Up, 0x0C);
            XBConfig.LoadInput(Button_P1_Down, 0x10);
            XBConfig.LoadInput(Button_P1_Left, 0x14);
            XBConfig.LoadInput(Button_P1_Right, 0x18);
            XBConfig.LoadInput(Button_P1_Start, 0x00);
            XBConfig.LoadInput(Button_P1_Service, 0x68);
            XBConfig.LoadInput(Button_P1_ButtonA, 0x1C);
            XBConfig.LoadInput(Button_P1_ButtonB, 0x20);
            XBConfig.LoadInput(Button_P1_ButtonC, 0x24);
            XBConfig.LoadInput(Button_P1_ButtonD, 0x28);
            XBConfig.LoadInput(Button_P1_ButtonE, 0x2C);
            XBConfig.LoadInput(Button_P1_ButtonF, 0x30);
        }

        private void LoadInputs_P2()
        {
            XBConfig.LoadInput(Button_P2_Up, 0x40);
            XBConfig.LoadInput(Button_P2_Down, 0x44);
            XBConfig.LoadInput(Button_P2_Left, 0x48);
            XBConfig.LoadInput(Button_P2_Right, 0x4C);
            XBConfig.LoadInput(Button_P2_Start, 0x34);
            XBConfig.LoadInput(Button_P2_Service, 0x38);
            XBConfig.LoadInput(Button_P2_ButtonA, 0x50);
            XBConfig.LoadInput(Button_P2_ButtonB, 0x54);
            XBConfig.LoadInput(Button_P2_ButtonC, 0x58);
            XBConfig.LoadInput(Button_P2_ButtonD, 0x5C);
            XBConfig.LoadInput(Button_P2_ButtonE, 0x60);
            XBConfig.LoadInput(Button_P2_ButtonF, 0x64);
        }

        private void LoadMisc()
        {
            XBConfig.GetCheck(CheckBox_POV, 0x00);
        }

        private void ComboBox_Device_SelectedIndexChanged(object sender, EventArgs e)
        {
            XBConfig.SetDevice(ComboBox_Device);
        }

        private void Button_Refresh_Click(object sender, EventArgs e)
        {
            XBConfig.LoadController(ComboBox_Device);
        }

        private void Button_P1_Up_MouseDown(object sender, MouseEventArgs e)
        {
            XBConfig.Button_Click(Button_P1_Up, 0x0C, e);
        }

        private void Button_P1_Down_MouseDown(object sender, MouseEventArgs e)
        {
            XBConfig.Button_Click(Button_P1_Down, 0x10, e);
        }

        private void Button_P1_Left_MouseDown(object sender, MouseEventArgs e)
        {
            XBConfig.Button_Click(Button_P1_Left, 0x14, e);
        }

        private void Button_P1_Right_MouseDown(object sender, MouseEventArgs e)
        {
            XBConfig.Button_Click(Button_P1_Right, 0x18, e);
        }

        private void Button_P1_Start_MouseDown(object sender, MouseEventArgs e)
        {
            XBConfig.Button_Click(Button_P1_Start, 0x00, e);
        }

        private void Button_P1_Service_MouseDown(object sender, MouseEventArgs e)
        {
            XBConfig.Button_Click(Button_P1_Service, 0x68, e);
        }

        private void Button_P1_ButtonA_MouseDown(object sender, MouseEventArgs e)
        {
            XBConfig.Button_Click(Button_P1_ButtonA, 0x1C, e);
        }

        private void Button_P1_ButtonB_MouseDown(object sender, MouseEventArgs e)
        {
            XBConfig.Button_Click(Button_P1_ButtonB, 0x20, e);
        }

        private void Button_P1_ButtonC_MouseDown(object sender, MouseEventArgs e)
        {
            XBConfig.Button_Click(Button_P1_ButtonC, 0x24, e);
        }

        private void Button_P1_ButtonD_MouseDown(object sender, MouseEventArgs e)
        {
            XBConfig.Button_Click(Button_P1_ButtonD, 0x28, e);
        }

        private void Button_P1_ButtonE_MouseDown(object sender, MouseEventArgs e)
        {
            XBConfig.Button_Click(Button_P1_ButtonE, 0x2C, e);
        }

        private void Button_P1_ButtonF_MouseDown(object sender, MouseEventArgs e)
        {
            XBConfig.Button_Click(Button_P1_ButtonF, 0x30, e);
        }

        private void Button_P2_Up_MouseDown(object sender, MouseEventArgs e)
        {
            XBConfig.Button_Click(Button_P2_Up, 0x40, e);
        }

        private void Button_P2_Down_MouseDown(object sender, MouseEventArgs e)
        {
            XBConfig.Button_Click(Button_P2_Down, 0x44, e);
        }

        private void Button_P2_Left_MouseDown(object sender, MouseEventArgs e)
        {
            XBConfig.Button_Click(Button_P2_Left, 0x48, e);
        }

        private void Button_P2_Right_MouseDown(object sender, MouseEventArgs e)
        {
            XBConfig.Button_Click(Button_P2_Right, 0x4C, e);
        }

        private void Button_P2_Start_MouseDown(object sender, MouseEventArgs e)
        {
            XBConfig.Button_Click(Button_P2_Start, 0x34, e);
        }

        private void Button_P2_Service_MouseDown(object sender, MouseEventArgs e)
        {
            XBConfig.Button_Click(Button_P2_Service, 0x38, e);
        }

        private void Button_P2_ButtonA_MouseDown(object sender, MouseEventArgs e)
        {
            XBConfig.Button_Click(Button_P2_ButtonA, 0x50, e);
        }

        private void Button_P2_ButtonB_MouseDown(object sender, MouseEventArgs e)
        {
            XBConfig.Button_Click(Button_P2_ButtonB, 0x54, e);
        }

        private void Button_P2_ButtonC_MouseDown(object sender, MouseEventArgs e)
        {
            XBConfig.Button_Click(Button_P2_ButtonC, 0x58, e);
        }

        private void Button_P2_ButtonD_MouseDown(object sender, MouseEventArgs e)
        {
            XBConfig.Button_Click(Button_P2_ButtonD, 0x5C, e);
        }

        private void Button_P2_ButtonE_MouseDown(object sender, MouseEventArgs e)
        {
            XBConfig.Button_Click(Button_P2_ButtonE, 0x60, e);
        }

        private void Button_P2_ButtonF_MouseDown(object sender, MouseEventArgs e)
        {
            XBConfig.Button_Click(Button_P2_ButtonF, 0x64, e);
        }

        private void Button_P0_Test_MouseDown(object sender, MouseEventArgs e)
        {
            XBConfig.Button_Click(Button_P0_Test, 0x08, e);
        }

        private void Button_P0_Exit_MouseDown(object sender, MouseEventArgs e)
        {
            XBConfig.Button_Click(Button_P0_Exit, 0x6C, e);
        }

        private void CheckBox_POV_CheckedChanged(object sender, EventArgs e)
        {
            XBConfig.SetConfig_CheckBox(CheckBox_POV, 0x00);
        }

        private void Button_P1_SetAll_Click(object sender, EventArgs e)
        {
            XBConfig.SetInput(Button_P1_Up, 0x0C);
            XBConfig.SetInput(Button_P1_Down, 0x10);
            XBConfig.SetInput(Button_P1_Left, 0x14);
            XBConfig.SetInput(Button_P1_Right, 0x18);
            XBConfig.SetInput(Button_P1_Start, 0x00);
            XBConfig.SetInput(Button_P1_Service, 0x68);
            XBConfig.SetInput(Button_P1_ButtonA, 0x1C);
            XBConfig.SetInput(Button_P1_ButtonB, 0x20);
            XBConfig.SetInput(Button_P1_ButtonC, 0x24);
            XBConfig.SetInput(Button_P1_ButtonD, 0x28);
            XBConfig.SetInput(Button_P1_ButtonE, 0x2C);
            XBConfig.SetInput(Button_P1_ButtonF, 0x30);

            XBConfig.SetInput(Button_P0_Test, 0x08);
            XBConfig.SetInput(Button_P0_Exit, 0x6C);
        }

        private void Button_P2_SetAll_Click(object sender, EventArgs e)
        {
            XBConfig.SetInput(Button_P2_Up, 0x40);
            XBConfig.SetInput(Button_P2_Down, 0x44);
            XBConfig.SetInput(Button_P2_Left, 0x48);
            XBConfig.SetInput(Button_P2_Right, 0x4C);
            XBConfig.SetInput(Button_P2_Start, 0x34);
            XBConfig.SetInput(Button_P2_Service, 0x38);
            XBConfig.SetInput(Button_P2_ButtonA, 0x50);
            XBConfig.SetInput(Button_P2_ButtonB, 0x54);
            XBConfig.SetInput(Button_P2_ButtonC, 0x58);
            XBConfig.SetInput(Button_P2_ButtonD, 0x5C);
            XBConfig.SetInput(Button_P2_ButtonE, 0x60);
            XBConfig.SetInput(Button_P2_ButtonF, 0x64);
        }

        private void Button_P1_ClearAll_Click(object sender, EventArgs e)
        {
            XBConfig.ClearInput(Button_P1_Up, 0x0C);
            XBConfig.ClearInput(Button_P1_Down, 0x10);
            XBConfig.ClearInput(Button_P1_Left, 0x14);
            XBConfig.ClearInput(Button_P1_Right, 0x18);
            XBConfig.ClearInput(Button_P1_Start, 0x00);
            XBConfig.ClearInput(Button_P1_Service, 0x68);
            XBConfig.ClearInput(Button_P1_ButtonA, 0x1C);
            XBConfig.ClearInput(Button_P1_ButtonB, 0x20);
            XBConfig.ClearInput(Button_P1_ButtonC, 0x24);
            XBConfig.ClearInput(Button_P1_ButtonD, 0x28);
            XBConfig.ClearInput(Button_P1_ButtonE, 0x2C);
            XBConfig.ClearInput(Button_P1_ButtonF, 0x30);

            XBConfig.ClearInput(Button_P0_Test, 0x08);
            XBConfig.ClearInput(Button_P0_Exit, 0x6C);
        }

        private void Button_P2_ClearAll_Click(object sender, EventArgs e)
        {
            XBConfig.ClearInput(Button_P2_Up, 0x40);
            XBConfig.ClearInput(Button_P2_Down, 0x44);
            XBConfig.ClearInput(Button_P2_Left, 0x48);
            XBConfig.ClearInput(Button_P2_Right, 0x4C);
            XBConfig.ClearInput(Button_P2_Start, 0x34);
            XBConfig.ClearInput(Button_P2_Service, 0x38);
            XBConfig.ClearInput(Button_P2_ButtonA, 0x50);
            XBConfig.ClearInput(Button_P2_ButtonB, 0x54);
            XBConfig.ClearInput(Button_P2_ButtonC, 0x58);
            XBConfig.ClearInput(Button_P2_ButtonD, 0x5C);
            XBConfig.ClearInput(Button_P2_ButtonE, 0x60);
            XBConfig.ClearInput(Button_P2_ButtonF, 0x64);

            XBConfig.ClearInput(Button_P0_Test, 0x08);
            XBConfig.ClearInput(Button_P0_Exit, 0x6C);
        }

        private void LinkLabel_Romhack_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            Process.Start("https://github.com/zxmarcos");
        }

        private void LinkLabel_Hipnosis_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            Process.Start("https://hipnosis183.github.io/");
        }

        private void Button_Save_Click(object sender, EventArgs e)
        {
            XBConfig.WriteXBConfig();
            Application.ExitThread();
        }

        private void Button_Cancel_Click(object sender, EventArgs e)
        {
            Application.ExitThread();
        }
    }
}
