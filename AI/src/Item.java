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
        StringBuilder builder=new StringBuilder();
        if (hp>0) builder.append("hp+").append(hp).append(';');
        if (atk>0) builder.append("atk+").append(atk).append(';');
        if (def>0) builder.append("def+").append(def).append(';');
        if (mdef>0) builder.append("mdef+").append(mdef).append(';');
        if (yellow>0) builder.append("yellow+").append(yellow).append(';');
        if (blue>0) builder.append("blue+").append(blue).append(';');
        if (red>0) builder.append("red+").append(red).append(';');
        return builder.toString();
    }

    public void merge(Item another) {
        if (another==null) return;
        hp+=another.hp; atk+=another.atk; def+=another.def; mdef+=another.mdef;
        yellow+=another.yellow; blue+=another.blue; red+=another.red;
    }

}
