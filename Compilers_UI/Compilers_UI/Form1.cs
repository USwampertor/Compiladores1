using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using CompilerCore;
namespace Compilers_UI
{
    public partial class Form1 : Form
    {
        CompilerCore.Manager compiler;
        public Form1()
        {
            InitializeComponent();
            InitializeCompiler();
        }
        public void InitializeCompiler()
        {
            compiler = new CompilerCore.Manager();
        }
        private void compileToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            String[] compDetails = compiler.Compile("");
            errorTextBox.Text = compDetails[0];
        }
    }
}
