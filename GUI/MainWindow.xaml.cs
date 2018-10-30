using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace CompressGUI
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        public enum Code { CODE_HUFFMAN , CODE_ARITHMETIC , CODE_LZ };

        public Code CodingMethod;
        private string SrcFileName;
        private string DstFileName;

        private void Btn_open_Click(object sender, RoutedEventArgs e)
        {
            System.Windows.Forms.OpenFileDialog openFileDialog = new System.Windows.Forms.OpenFileDialog
            {
                Multiselect = false,
                Filter = "All Files(*.*)|*.*"
            };
            if (openFileDialog.ShowDialog()==System.Windows.Forms.DialogResult.OK)
            {
                SrcFileName = openFileDialog.FileName;
                Tb_dst.Text = SrcFileName;
                FileInfo fileInfo = new FileInfo(SrcFileName);
                FileVersionInfo info = FileVersionInfo.GetVersionInfo(SrcFileName);
                Tb_filename.Content = SrcFileName.Substring(SrcFileName.LastIndexOf('\\')+1);
                Tb_filesize.Content = System.Math.Ceiling(fileInfo.Length / 1024.0) + "KB";
                Tb_filedate.Content = fileInfo.LastWriteTime;
            }
        }

        private void Btn_save_Click(object sender, RoutedEventArgs e)
        {
            System.Windows.Forms.SaveFileDialog saveFileDialog = new System.Windows.Forms.SaveFileDialog
            {
                Filter = "All Files(*.*)|*.*"
            };
            if (saveFileDialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                DstFileName = saveFileDialog.FileName;
                Tb_src.Text = DstFileName;
            }
        }

        private void Rb_huffman_Checked(object sender, RoutedEventArgs e)
        {
            CodingMethod = Code.CODE_HUFFMAN;
        }

        private void Rb_arithmetic_Checked(object sender, RoutedEventArgs e)
        {
            CodingMethod = Code.CODE_ARITHMETIC;
        }

        private void Rb_lz_Checked(object sender, RoutedEventArgs e)
        {
            CodingMethod = Code.CODE_LZ;
        }

        private void Btn_do_Click(object sender, RoutedEventArgs e)
        {
            Thread thread = new Thread(RunCommand);
            thread.Start();
        }
        private void RunCommand()
        {
            using (Process p = new Process())
            {
                p.StartInfo.FileName = "cmd";
                p.StartInfo.UseShellExecute = false;
                p.StartInfo.RedirectStandardInput = true;
                p.StartInfo.RedirectStandardOutput = true;
                p.StartInfo.RedirectStandardError = true;
                p.StartInfo.CreateNoWindow = true;
                p.Start();
                p.StandardInput.WriteLine("ping baidu.com&exit");
                p.StandardInput.AutoFlush = true;
                StreamReader reader = p.StandardOutput;
                string line;
                while (!reader.EndOfStream)
                {
                    line = reader.ReadLine();
                    Dispatcher.Invoke(new dg_SetOutputLabel(SetOutputLabelText), line);
                }
                p.WaitForExit();
                p.Close();
            }
        }
        private void SetOutputLabelText(string text)
        {
            Lb_output.Content = text;
        }
        private delegate void dg_SetOutputLabel(string text);
    }
}
