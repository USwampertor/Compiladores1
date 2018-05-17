using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Compilers_UI
{
    public partial class Form1 : Form
    {
        CompilersCore.Manager compiler = new CompilersCore.Manager();
        public Form1()
        {
            InitializeComponent();
            InitializeCore();
        }
        public void InitializeCore()
        {
        }
        private void debugToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void compilarToolStripMenuItem_Click(object sender, EventArgs e)
        {
            String[] compDetails = compiler.compile("");
            compileOutput.Text = compDetails[0];
        }
    }
}
