using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;

namespace map_generator
{
    class Core
    {

        private const int MARGIN = 5;

        private static int[] getLeftTop(HSV[,] colors, Dictionary<int, HSV[,]> colorDict)
        {
            double score = 1e9;
            int ax = 0, ay = 0;
            for (int offsetX = -MARGIN; offsetX < 2 * MARGIN; offsetX++)
            {
                for (int offsetY = -MARGIN; offsetY < 2 * MARGIN; offsetY++)
                {
                    double tmpScore = 1e9;
                    foreach (HSV[,] value in colorDict.Values)
                    {
                        double s = 0;
                        for (int x = MARGIN; x < 32 - MARGIN; x++)
                        {
                            for (int y = MARGIN; y < 32 - MARGIN; y++)
                            {
                                // s += Math.Abs(value[x, y] - grays[x + offsetX, y + offsetY]);
                                try
                                {
                                    s += HSV.getDistance(value[x, y], colors[x + offsetX, y + offsetY]);
                                }
                                catch (Exception)
                                {
                                }
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

        public static string calculate(Bitmap bitmap, Dictionary<int, HSV[,]> colorDict)
        {
            int width = (bitmap.Width + 16) / 32, height = (bitmap.Height + 16) / 32;
            // int[,] grays = bitmapToGray(bitmap);

            HSV[,] colors = new HSV[bitmap.Width, bitmap.Height];
            for (int i = 0; i < bitmap.Width; i++)
            {
                for (int j = 0; j < bitmap.Height; j++)
                {
                    colors[i, j] = new HSV(bitmap.GetPixel(i, j));
                }
            }

            int[,] ans = new int[width, height];

            int[] offset = getLeftTop(colors, colorDict);

            int offsetX = offset[0], offsetY = offset[1];

            // calculate
            for (int i = 0; i < width; i++)
            {
                for (int j = 0; j < height; j++)
                {
                    double t = 1e9;
                    int v = 0;
                    foreach (int id in colorDict.Keys)
                    {
                        // Bitmap value = dict[id];
                        HSV[,] value = colorDict[id];
                        double s = 0;
                        for (int x = MARGIN; x < 32 - MARGIN; x++)
                        {
                            for (int y = MARGIN; y < 32 - MARGIN; y++)
                            {
                                try
                                {
                                    s += HSV.getDistance(value[x, y], colors[32 * j + x + offsetX, 32 * i + y + offsetY]);
                                }
                                catch (Exception) { }
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
            // text = builder.ToString();

            // textBox1.Text = text;
            // button2_Click(null, null);
            return builder.ToString();
        }

    }
}