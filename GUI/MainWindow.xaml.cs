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

        public enum CODE { CODE_HUFFMAN , CODE_ARITHMETIC , CODE_LZ };
        public enum ACTION { ENCODE,DECODE};
        public CODE CodingMethod = CODE.CODE_HUFFMAN;
        public ACTION action = ACTION.ENCODE;
        private string SrcFileName = null;
        private string DstFileName = null;
        private string exe, arg;
        private string filter = "Huffman Coded File(*.hfm)|*.hfm";

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
                Tb_src.Text = SrcFileName;
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
                Filter = filter
            };
            if (saveFileDialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                DstFileName = saveFileDialog.FileName;
                Tb_dst.Text = DstFileName;
            }
        }

        private void Rb_huffman_Checked(object sender, RoutedEventArgs e)
        {
            CodingMethod = CODE.CODE_HUFFMAN;
            filter = "Huffman Coded File(*.hfm)|*.hfm";
        }

        private void Rb_arithmetic_Checked(object sender, RoutedEventArgs e)
        {
            CodingMethod = CODE.CODE_ARITHMETIC;
            filter = "Arithmetic Coded File(*.atm)|*.atm";
        }

        private void Rb_lz_Checked(object sender, RoutedEventArgs e)
        {
            CodingMethod = CODE.CODE_LZ;
            filter = "LZ Coded File(*.lz)|*.lz";
        }

        private void Btn_do_Click(object sender, RoutedEventArgs e)
        {
            if(SrcFileName==null||DstFileName==null)
            {
                MessageBox.Show("请设置需要处理的文件名");
                return;
            }
            switch (action)
            {
                case ACTION.ENCODE:
                    arg = "-encode";
                    break;
                case ACTION.DECODE:
                    arg = "-decode";
                    break;
                default:
                    arg = "-encode";
                    break;
            }
            switch (CodingMethod)
            {
                case CODE.CODE_HUFFMAN:
                    exe = "Huffman.exe";
                    break;
                case CODE.CODE_LZ:
                    exe = "LZ.exe";
                    break;
                default:
                    exe = "Huffman.exe";
                    break;
            }
            arg = arg + " " + Tb_src.Text + " " + Tb_dst.Text;
            Thread thread = new Thread(RunCommand);
            thread.Start();
            Pb_main.Value = 0;
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

                p.StandardInput.WriteLine(exe + " " + arg + " &exit");
                p.StandardInput.AutoFlush = true;
                StreamReader reader = p.StandardOutput;
                string line;
                while (!reader.EndOfStream)
                {
                    line = reader.ReadLine();
                    if (line.StartsWith("Progress:"))
                    {
                        Dispatcher.Invoke(new dg_SetProgressBar(SetProgressBar), Convert.ToDouble(line.Substring(9)));
                    }
                    else
                    {
                        if(line.Equals("写入完成"))
                        {
                            Dispatcher.Invoke(new dg_SetProgressBar(SetProgressBar), 1);
                        }
                        Dispatcher.Invoke(new dg_SetOutputLabel(SetOutputLabelText), line);
                    }
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

        private void SetProgressBar(double value)
        {
            Pb_main.Value = value;
        }
        private delegate void dg_SetProgressBar(double value);

        private void Rb_encode_Checked(object sender, RoutedEventArgs e)
        {
            action = ACTION.ENCODE;
        }

        private void Rb_decode_Checked(object sender, RoutedEventArgs e)
        {
            action = ACTION.DECODE;
        }
    }
}
