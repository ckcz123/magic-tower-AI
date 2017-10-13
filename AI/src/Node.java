import java.util.HashSet;

/**
 * Created by oc on 2017/10/12.
 */
public class Node {

    // 当前各种属性
    int hp, atk, def, mdef, yellow, blue, red, pickaxe, bomb;
    // 节点类型
    int type;

    // 坐标
    int f,x,y;

    HashSet<Node> linked;

    public Node(int _hp, int _atk, int _def, int _mdef, int _type, int _f, int _x, int _y) {
        hp=_hp; atk=_atk; def=_def; mdef=_mdef; type=_type; f=_f; x=_x; y=_y;
        yellow=0; blue=0; red=0; pickaxe=0; bomb=0;
        linked=new HashSet<>();
    }

    public void addNode(Node another) {
        linked.add(another);
    }

    public Node merge(Node another, boolean[][][] visited) {
        Node node;

        // 非怪物
        if (!another.isMonster()) {
            node = new Node(hp+another.hp, atk+another.atk, def+another.def,
                    mdef+another.mdef, type, another.f, another.x, another.y);
        }
        // 怪物
        else {
            int damage = Util.getDamage(atk, def, mdef, another.hp, another.atk, another.def, another.mdef);
            node = new Node(hp-damage, atk, def, mdef, type, another.f, another.x, another.y);
        }
        node.yellow = yellow+another.yellow;
        node.red = red+another.red;
        node.blue = blue+another.blue;
        node.pickaxe=pickaxe+another.pickaxe;
        node.bomb=bomb+another.bomb;

        // 加边
        for (Node to: linked) {
            if (!visited[to.f][to.x][to.y])
                node.addNode(to);
        }
        for (Node to: another.linked) {
            if (!visited[to.f][to.x][to.y])
                node.addNode(to);
        }

        return node;
    }

    public boolean equals(Object another) {
        return (another instanceof Node) && hashCode()==another.hashCode();
    }

    public int hashCode() {
        return 1000000*f+1000*x+y;
    }

    public boolean isDoor() {
        return type==Graph.DOOR_YELLOW || type==Graph.DOOR_BLUE || type==Graph.DOOR_RED;
    }

    public boolean isMonster() {
        return type>=Graph.MONSTER_BOUND;
    }

    public boolean isItem() {
        return !isDoor() && !isMonster();
    }

    public int getScore() {
        // return hp+1000*(atk+def)+60*mdef+300*yellow+450*blue+600*red;
        return hp;
    }

    public String toString() {
        return String.format("%s(%d,%d,%d) -- (%d,%d,%d,%d,%d,%d,%d) -- %d Linked",
                type==0?"":isDoor()?"DOOR ":isMonster()?"MONSTER ":"ITEM ",
                f, x, y, hp, atk, def, mdef, yellow, blue, red, linked.size());
    }

}
