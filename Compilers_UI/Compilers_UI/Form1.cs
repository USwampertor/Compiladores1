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
            errorTextBox.Clear();
            debugTextBox.Clear();
            debugTextBox.Text += ("Initiating compilation..." + '\r' + '\n');
            string toparse = inputTextBox.Text;
            String[] compDetails = compiler.Compile(toparse);
            debugTextBox.Text += ("Ended Compilation..." + '\r' + '\n');
            int j = 0;
            for(j = 0; j < compDetails.Length;++j)
            {
                if (compDetails[j] == "( ° w °)"|| compDetails[j] == "( u w u)")
                {
                    errorTextBox.Text += (compDetails[j] + '\r' + '\n');
                    break;
                }
                errorTextBox.Text += (compDetails[j] + '\r' + '\n');
            }
            for(int i = j+1; i<compDetails.Length;++i)
            {
                debugTextBox.Text += (compDetails[i] + '\r' + '\n');
            }
            if(j==0) errorTextBox.Text += "No errors found m8";
        }
    }
}
