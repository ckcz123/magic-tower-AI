import java.io.PrintStream;
import java.util.*;

public class Main {

    public static void main(String[] args) throws Exception {
        Scanner scanner=new Scanner(System.in);
        System.setOut(new PrintStream("output.txt"));

        int n=scanner.nextInt();
        int r=scanner.nextInt(), c=scanner.nextInt();
        ArrayList<String> list=new ArrayList<>();
        for (int i=0;i<n;i++) {
            System.out.println(i+1);
            for (int x=0;x<r;x++) {
                list.clear();
                for (int y=0;y<c;y++) {
                    list.add(map(scanner.nextInt())+"");
                }
                System.out.println(String.join("\t", list));
            }

            System.out.println("\n\n\n");
        }
    }

    private static int map(int x) {
        if (x==0) return 0;
        if (x==1) return 1;
        if (x==2) return 3;
        if (x==3) return 5;
        if (x==4) return 4;
        if (x==5) return 86;
        if (x==6) return 2;

        if (x==11) return 27;
        if (x==12) return 28;
        if (x==13) return 29;
        if (x==14) return 26;
        if (x==15) return 21;
        if (x==16) return 22;
        if (x==17) return 23;
        if (x==18) return 31;
        if (x==19) return 32;
        if (x==20) return 35;
        if (x==21) return 36;
        if (x==23) return 50;
        if (x==25) return 37;
        if (x==26) return 38;
        if (x==27) return 46;
        if (x==31) return 43;
        if (x==32) return 44;

        if (x==41 || x==42 || x==43 || x==44 || x==47 || x==48 || x==49) return 121;
        if (x==46 || x==51 || x==52 || x==53) return 122;
        if (x==45 || x==50) return 131;
        if (x==54 || x==55 || x==56 || x==57) return 124;
        if (x==69) return 133;
        if (x==70) return 134;

        if (x==81) return 81;
        if (x==82) return 82;
        if (x==83) return 83;
        if (x==84) return 85;
        if (x==85) return 7;
        if (x==86) return 8;
        if (x==87) return 87;
        if (x==88) return 88;

        if (x>=101 && x<=108) return x+100;
        if (x>=109 && x<=112) return x+(217-109);
        if (x>=113 && x<=120) return x+(209-113);
        if (x>=121 && x<=128) return x+100;
        if (x==130) return 246;
        if (x==132) return 236;
        if (x==133) return 232;
        if (x==134) return 229;

        if (x==151) return 261;
        if (x==152) return 262;
        if (x==153) return 263;
        if (x==154) return 264;

        if (x==201) return 165;
        if (x==202) return 166;
        if (x==203) return 161;
        if (x==204) return 163;
        if (x==205) return 164;
        if (x==206) return 162;
        if (x==207) return 171;
        if (x==208) return 89;
        if (x==209) return 168;
        if (x==210) return 169;
        if (x==211) return 170;

        return x;

    }

}