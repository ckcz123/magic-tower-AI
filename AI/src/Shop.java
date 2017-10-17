/**
 * Created by oc on 2017/10/17.
 */
public class Shop {

    int start, delta, hp, atk, def, mdef;

    public Shop(int _start, int _delta, int _hp, int _atk, int _def, int _mdef) {
        start=_start; delta=_delta; hp=_hp; atk=_atk; def=_def; mdef=_mdef;
    }

    public int moneyNeeded(int visitTime) {
        return start+delta*visitTime;
    }

    public int getHpPoint() {
        return hp;
    }

    public int getAtkPoint() {
        return atk;
    }

    public int getDefPoint() {
        return def;
    }

    public int getMDefPoint() {
        return mdef;
    }

    public boolean useShop(Hero hero, int visitTime) {
        if (visitTime<0) return false;
        if (hero.money<moneyNeeded(visitTime)) return false;

        /**
         * Algorithm to choose HP/ATK/DEF/MDEF
         */

        // Add atk
        hero.money-=moneyNeeded(visitTime);
        hero.atk+=getAtkPoint();
        return true;
    }



}
