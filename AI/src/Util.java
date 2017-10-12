/**
 * Created by oc on 2017/10/12.
 */
public class Util {

    /**
     * 计算伤害<br/>
     * <ol><li>先攻</li><li>魔攻</li><li>坚固</li><li>2连击</li><li>3连击</li>
     * <li>破甲</li><li>反击</li><li>净化</li></ol>
     * @return
     */
    public static int getDamage(int hero_atk, int hero_def, int hero_mdef,
                                int mon_hp, int mon_atk, int mon_def, int mon_special) {
        // 魔攻
        if (mon_special==2) hero_def=0;
        // 坚固
        if (mon_special==3 && mon_def<hero_atk-1) mon_def=hero_atk-1;
        if (hero_atk<=mon_def) return 999999999;

        int per_damage=Math.max(mon_atk-hero_def, 0);
        // 2连击 & 3连击
        if (mon_special==4) per_damage*=2;
        if (mon_special==5) per_damage*=3;
        // 反击
        if (mon_special==7) per_damage+=(int)(0.1*hero_atk);

        // 先攻
        int damage=mon_special==1?per_damage:0;
        // 破甲
        if (mon_special==6) damage=(int)(0.9*hero_def);
        // 净化
        if (mon_special==8) damage=3*hero_mdef;

        int ans=damage+(mon_hp-1)/(hero_atk-mon_def)*per_damage;
        ans -= hero_mdef;

        // 魔防回血
        // return ans;

        // 魔防不回血
        return Math.max(ans, 0);

    }


}
