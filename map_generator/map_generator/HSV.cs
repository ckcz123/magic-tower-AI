using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;

namespace map_generator
{
    class HSV
    {
        public double H, S, B;
        public double X, Y, Z;

        private static double R = 100;
        private static double angle = 30;
        private static double h = R * Math.Cos(angle / 180 * Math.PI);
        private static double r = R * Math.Sin(angle / 180 * Math.PI);

        public HSV(Color color)
            : this(color.GetHue(), color.GetSaturation(), color.GetBrightness())
        {
        }

        public HSV(float _H, float _S, float _B)
        {
            H = _H; S = _S; B = _B;
            X = r * B * S * Math.Cos(H / 180 * Math.PI);
            Y = r * B * S * Math.Sin(H / 180 * Math.PI);
            Z = h * (1 - B);
        }

        public static double getDistance(HSV hsv1, HSV hsv2)
        {
            double dx = hsv1.X - hsv2.X, dy = hsv1.Y - hsv2.Y, dz = hsv1.Z - hsv2.Z;
            return dx * dx + dy * dy + dz * dz;
        }


    }
}