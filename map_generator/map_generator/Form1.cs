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

namespace map_generator
{
    public partial class Form1 : Form
    {

        Dictionary<int, Bitmap> dict;
        Dictionary<int, int[,]> grayDict;


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
            dict.Add(81, clipImage(map, 0, 64, road));
            dict.Add(82, clipImage(map, 32, 64, road));
            dict.Add(83, clipImage(map, 64, 64, road));
            dict.Add(87, clipImage(map, 32, 192, road));
            dict.Add(88, clipImage(map, 0, 192, road));
            // dict.Add(40, clipImage(map, 32, 224, road));

            dict.Add(11, clipImage(icon, 96, 32, road));
            dict.Add(12, clipImage(icon, 64, 32, road));
            dict.Add(13, clipImage(icon, 0, 64, road));
            dict.Add(15, clipImage(icon, 0, 0, road));
            dict.Add(16, clipImage(icon, 32, 0, road));
            dict.Add(17, clipImage(icon, 64, 0, road));
            dict.Add(18, clipImage(icon, 64, 64, road));
            dict.Add(19, clipImage(icon, 96, 64, road));
            dict.Add(20, clipImage(icon, 96, 96, road));
            dict.Add(21, clipImage(icon, 96, 128, road));
            dict.Add(22, clipImage(icon, 0, 32, road));
            dict.Add(23, clipImage(icon, 32, 32, road));

            for (int i = 0; i < 30; i++)
            {
                dict.Add(101 + i, clipImage(monster, 0, 32 * i, road));
            }
            dict.Add(199, clipImage(monster, 0, 36 * 32, road));


            grayDict = new Dictionary<int, int[,]>();
            foreach (int id in dict.Keys)
            {
                Bitmap bitmap = dict[id];
                int[,] grays = new int[32, 32];
                for (int i = 0; i < 32; i++)
                {
                    for (int j = 0; j < 32; j++)
                    {
                        grays[i, j] = color2gray(bitmap.GetPixel(i, j));
                    }
                }
                grayDict.Add(id, grays);
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

        private int color2gray(Color color)
        {
            return (299 * color.R + 587 * color.G + 114 * color.B + 500) / 1000;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Bitmap bitmap = new Bitmap(416, 416);
            Graphics graphic = Graphics.FromImage(bitmap);
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

        private int[] getLeftTop(int[,] grays)
        {
            int score = 999999999;
            int ax = 0, ay = 0;
            for (int offsetX = 0; offsetX < 8; offsetX++)
            {
                for (int offsetY = 0; offsetY < 8; offsetY++)
                {
                    int tmpScore = 999999999;
                    foreach (int[,] value in grayDict.Values)
                    {
                        int s = 0;
                        for (int x = 0; x < 32; x++)
                        {
                            for (int y = 0; y < 32; y++)
                            {
                                s += Math.Abs(value[x, y] - grays[x + offsetX, y + offsetY]);
                            }
                        }
                        if (tmpScore > s)
                        {
                            tmpScore = s;
                        }
                    }
                    if (score > tmpScore)
                    {
                        score = tmpScore;
                        ax = offsetX; ay = offsetY;
                    }
                }
            }
            return new int[] { ax, ay };
        }

        private void setText(Bitmap bitmap)
        {
            int width = bitmap.Width / 32, height = bitmap.Height / 32;
            int[,] grays = bitmapToGray(bitmap);
            string text = "";
            int[,] ans = new int[width, height];

            int[] offset = getLeftTop(grays);
            int offsetX = offset[0], offsetY = offset[1];

            for (int i = 0; i < width; i++)
            {
                for (int j = 0; j < height; j++)
                {
                    int t = 999999999, v = 0;
                    foreach (int id in grayDict.Keys)
                    {
                        // Bitmap value = dict[id];
                        int[,] value = grayDict[id];
                        int s = 0;
                        for (int x = 0; x < 32; x++)
                        {
                            for (int y = 0; y < 32; y++)
                            {
                                s += Math.Abs(value[x, y] - grays[32 * j + x + offsetX, 32 * i + y + offsetY]);
                            }
                        }
                        if (t > s)
                        {
                            t = s;
                            v = id;
                        }
                    }
                    ans[i, j] = v;
                }
            }
            StringBuilder builder = new StringBuilder();
            for (int i = 0; i < width; i++)
            {
                for (int j = 0; j < height; j++)
                {
                    builder.Append(ans[i, j]);
                    if (j != height - 1) builder.Append('\t');
                    else builder.Append("\r\n");
                }
            }
            text = builder.ToString();

            textBox1.Text = text;
            button2_Click(null, null);
        }

        private int[,] bitmapToGray(Bitmap bitmap)
        {
            int width = bitmap.Width, height = bitmap.Height;
            int[,] grays = new int[width, height];
            for (int i = 0; i < width; i++)
            {
                for (int j = 0; j < height; j++)
                {
                    grays[i, j] = color2gray(bitmap.GetPixel(i, j));
                }
            }
            return grays;
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
            // pictureBox1.Image = bitmap;
            setText(bitmap);
        }

        private void button3_Click(object sender, EventArgs e)
        {
            try 
            {
                Clipboard.SetText(textBox1.Text);
                MessageBox.Show("地图已复制到剪切板！", "复制成功！");
            }
            catch (Exception)
            {
                MessageBox.Show("请全选并手动复制。", "复制失败！");
            }

        }


    }
}
