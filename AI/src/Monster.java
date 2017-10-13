/**
 * Created by oc on 2017/10/12.
 */
public class Monster {

    int id, hp, atk, def, special;
    public Monster(int _id, int _hp, int _atk, int _def, int _special) {
        id=_id; hp=_hp; atk=_atk; def=_def; special=_special;
    }

    public String toString() {
        return String.format("(%d,%d,%d,%d,%d)", id, hp, atk, def, special);
    }
}
