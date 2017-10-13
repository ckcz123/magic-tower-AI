import java.util.ArrayList;

/**
 * Created by oc on 2017/10/13.
 */
public class Item {

    int hp, atk, def, mdef, yellow, blue, red;

    public Item(int _hp, int _atk, int _def, int _mdef, int _yellow, int _blue, int _red) {
        hp=_hp; atk=_atk; def=_def; mdef=_mdef; yellow=_yellow; blue=_blue; red=_red;
    }

    public String toString() {
        if (hp>0) return "hp+"+hp;
        if (atk>0) return "atk+"+atk;
        if (def>0) return "def+"+def;
        if (mdef>0) return "mdef+"+mdef;
        if (yellow>0) return "yellow+"+yellow;
        if (blue>0) return "blue+"+blue;
        if (red>0) return "red+"+red;
        return "";
    }

}
