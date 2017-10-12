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

    public static int getDamage2(int h_atk, int h_def, int h_mdef, int m_hp, int m_atk, int m_def, int m_spe)
    {
        if (m_spe==1) // 魔攻
            h_def=0;
        if (m_spe==2) // 坚固
            m_def=h_atk-1;

        if (h_atk<=m_def) // 无法战斗
            return 999999999;
        if (h_def>=m_atk) // 无法造成伤害
            return 0;

        int damage=0;
        if (m_spe==3) // 先攻
            damage+=m_atk-h_def;

        int m_times=1;
        if (m_spe==4) // 2连击
            m_times=2;

        damage+=(m_hp-1)/(h_atk-m_def)*(m_atk-h_def)*m_times; // 怪物伤害计算公式
        damage-=h_mdef;

        return damage<=0?0:damage;
    }


}
