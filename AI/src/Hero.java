/**
 * Created by oc on 2017/10/13.
 */
public class Hero {

    int hp, atk, def, mdef, money, yellow, blue, red;

    public Hero(int _hp, int _atk, int _def, int _mdef, int _money, int _yellow, int _blue, int _red) {
        hp=_hp; atk=_atk; def=_def; mdef=_mdef; money=_money; yellow=_yellow; blue=_blue; red=_red;
    }

    public Hero(Hero another) {
        this(another.hp, another.atk, another.def, another.mdef, another.money, another.yellow, another.blue, another.red);
    }

    public void getItem(Item item) {
        if (item==null) return;
        hp+=item.hp; atk+=item.atk; def+=item.def; mdef+=item.mdef;
        yellow+=item.yellow; blue+=item.blue; red+=item.red;
    }

    public boolean isValid() {
        return hp>0 && yellow>=0 && blue>=0 && red>=0;
    }

    public int getScore() {
        // return hp+1000*(atk+def)+60*mdef+300*yellow+450*blue+600*red;
        return hp;
    }

    public String toString() {
        return String.format("(%d,%d,%d,%d,%d,%d,%d,%d)", hp, atk, def, mdef, money, yellow, blue, red);
    }

}
