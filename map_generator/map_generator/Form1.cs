using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Text.RegularExpressions;

namespace map_generator
{
    public partial class Form1 : Form
    {

        Dictionary<int, Bitmap> dict;
        Dictionary<int, HSV[,]> colorDict;

        public Form1()
        {
            InitializeComponent();
            loadResources();
        }

        private void loadResources()
        {
            dict = new Dictionary<int, Bitmap>();
            Bitmap icon = Properties.Resources.icon;
            Bitmap map = Properties.Resources.map;
            Bitmap monster = Properties.Resources.monster;
            Bitmap road = clipImage(map, 0, 0);

            dict.Add(0, road);
            dict.Add(1, clipImage(map, 32, 0));
            dict.Add(2, clipImage(map, 64, 0));
            dict.Add(3, clipImage(map, 32, 32));
            dict.Add(4, clipImage(map, 0, 32));
            dict.Add(6, clipImage(map, 96, 0));
            dict.Add(81, clipImage(map, 0, 64, road));
            dict.Add(82, clipImage(map, 32, 64, road));
            dict.Add(83, clipImage(map, 64, 64, road));
            dict.Add(84, clipImage(map, 96, 64, road));
            dict.Add(87, clipImage(map, 32, 192, road));
            dict.Add(88, clipImage(map, 0, 192, road));
            // dict.Add(40, clipImage(map, 32, 224, road));

            dict.Add(27, clipImage(icon, 96, 32, road));
            dict.Add(28, clipImage(icon, 64, 32, road));
            dict.Add(29, clipImage(icon, 0, 64, road));
            dict.Add(21, clipImage(icon, 0, 0, road));
            dict.Add(22, clipImage(icon, 32, 0, road));
            dict.Add(23, clipImage(icon, 64, 0, road));
            dict.Add(24, clipImage(icon, 96, 0, road));
            dict.Add(31, clipImage(icon, 64, 64, road));
            dict.Add(32, clipImage(icon, 96, 64, road));
            dict.Add(33, clipImage(icon, 96, 96, road));
            dict.Add(34, clipImage(icon, 96, 128, road));
            dict.Add(35, clipImage(icon, 0, 32, road));
            dict.Add(36, clipImage(icon, 32, 32, road));

            for (int i = 0; i < 40; i++)
            {
                dict.Add(201 + i, clipImage(monster, 0, 32 * i, road));
            }
            dict.Add(299, clipImage(monster, 0, 28 * 32, road));

            colorDict = new Dictionary<int, HSV[,]>();
            foreach (int id in dict.Keys)
            {
                Bitmap bitmap = dict[id];
                HSV[,] colors = new HSV[32, 32];
                for (int i = 0; i < 32; i++)
                {
                    for (int j = 0; j < 32; j++)
                    {
                        colors[i, j] = new HSV(bitmap.GetPixel(i, j));
                    }
                }
                colorDict.Add(id, colors);
            }

            SendMessage(textBox1.Handle, EM_SETTABSTOPS, 1, new int[] { 4 * 4 });

        }

        private const int EM_SETTABSTOPS = 0x00CB;

        [DllImport("User32.dll", CharSet = CharSet.Auto)]
        public static extern IntPtr SendMessage(IntPtr h, int msg, int wParam, int[] lParam);

        private Bitmap clipImage(Bitmap source, int offsetX, int offsetY, Bitmap road = null)
        {
            Bitmap bitmap = new Bitmap(32, 32);
            Graphics graphic = Graphics.FromImage(bitmap);
            if (road != null)
                graphic.DrawImage(road, 0, 0, new Rectangle(0, 0, 32, 32), GraphicsUnit.Pixel);
            graphic.DrawImage(source, 0, 0, new Rectangle(offsetX, offsetY, 32, 32), GraphicsUnit.Pixel);
            return bitmap;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Bitmap bitmap = new Bitmap(416, 416);
            Graphics graphic = Graphics.FromImage(bitmap);

            string text = textBox1.Text;
            text = new Regex("\\t+").Replace(text, "  ");
            text = text.Replace("[", "").Replace("]", "").Replace(",", "");
            text = new Regex(" +").Replace(text, "\t");
            textBox1.Text = text;

            string[] lines = textBox1.Text.Split('\n');
            for (int i = 0; i < lines.Length; i++)
            {
                string[] words = lines[i].Trim().Split('\t');
                for (int j = 0; j < words.Length; j++)
                {
                    try
                    {
                        int id = Convert.ToInt32(words[j]);
                        graphic.DrawImage(dict[id], new Rectangle(32 * j, 32 * i, 32, 32), new Rectangle(0, 0, 32, 32), GraphicsUnit.Pixel);
                    }
                    catch (Exception) { }
                }
            }
            pictureBox1.Image = bitmap;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Image image = null;

            if (Clipboard.ContainsImage())
            {
                image = Clipboard.GetImage();
            }
            else
            {
                OpenFileDialog fileDialog = new OpenFileDialog();
                fileDialog.Filter = "图片文件|*.bmp;*.jpg;*.jpeg;*.gif;*.png";
                fileDialog.FilterIndex = 1;
                if (fileDialog.ShowDialog() == DialogResult.OK)
                {
                    image = Image.FromFile(fileDialog.FileName);
                }
            }

            if (image == null) return;
            Bitmap bitmap = new Bitmap(image.Width, image.Height);
            Graphics graphic = Graphics.FromImage(bitmap);
            graphic.DrawImage(image, 0, 0);
            textBox1.Text = Core.calculate(bitmap, colorDict);
            button2_Click(null, null);
        }

        private void button3_Click(object sender, EventArgs e)
        {
            try
            {
                string text = textBox1.Text;
                string[] strings = text.Split(new char[] { '\n' });

                StringBuilder builder = new StringBuilder();
                foreach (string s in strings)
                {
                    if (s.Trim().Length == 0) continue;
                    builder.Append('[');
                    string[] ss = s.Trim().Split(new char[] { '\t' });


                    int l = 5;
                    foreach (string s1 in ss)
                    {
                        if (l < 5)
                        {
                            builder.Append(',');
                            builder.Append(' ', 5 - l);
                        }
                        builder.Append(s1);
                        l = s1.Length;
                    }
                    builder.Append(']').Append(',').Append('\n');
                }
                Clipboard.SetText(builder.ToString());
                MessageBox.Show("地图的JS格式已复制到剪切板！", "复制成功！");
            }
            catch (Exception)
            {
                MessageBox.Show("请全选并手动复制。", "复制失败！");
            }

        }

    }
}
