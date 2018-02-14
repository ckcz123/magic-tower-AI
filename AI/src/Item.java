
/**
 * Created by oc on 2017/10/13.
 */
public class Item {

    int hp, atk, def, mdef, yellow, blue, red, green, special;

    public Item() {
        hp=atk=def=mdef=yellow=blue=red=green=special=0;
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
        if (green>0) builder.append("green+").append(green).append(';');
        return builder.toString();
    }

    public void merge(Item another) {
        if (another==null) return;
        hp+=another.hp; atk+=another.atk; def+=another.def; mdef+=another.mdef;
        yellow+=another.yellow; blue+=another.blue; red+=another.red; green+=another.green;
        special+=another.special;
    }

    public Item setHp(int hp) {
        this.hp = hp; return this;
    }

    public Item setAtk(int atk) {
        this.atk = atk; return this;
    }

    public Item setDef(int def) {
        this.def = def; return this;
    }

    public Item setMdef(int mdef) {
        this.mdef = mdef; return this;
    }

    public Item setYellow(int yellow) {
        this.yellow = yellow; return this;
    }

    public Item setBlue(int blue) {
        this.blue = blue; return this;
    }

    public Item setRed(int red) {
        this.red = red; return this;
    }

    public Item setGreen(int green) {
        this.green = green; return this;
    }

    public Item setSpecial(int special) {
        this.special = special; return this;
    }
}
