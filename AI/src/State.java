import java.util.ArrayList;

/**
 * Created by oc on 2017/10/12.
 */
public class State {

    public static final int STOP_COUNT = 50;

    Node current;
    Graph graph;
    boolean[][][] visited;
    ArrayList<String> route;
    int cnt;

    public State(Graph _graph, Node node) {
        graph=_graph;
        current=node;
        cnt=0;
        visited=new boolean[graph.floor][graph.row][graph.col];
        visited[current.f][current.x][current.y]=true;
        route=new ArrayList<>();
        route.add(current.toString());
        eatItem();
        //route.add(current.toString());
    }

    public State(State another) {
        graph=another.graph;
        current=another.current;
        visited=new boolean[graph.floor][graph.row][graph.col];
        for (int i=0;i<graph.floor;i++) {
            for (int j=0;j<graph.row;j++) {
                for (int k=0;k<graph.col;k++) {
                    visited[i][j][k]=another.visited[i][j][k];
                }
            }
        }
        route=new ArrayList<>(another.route);
        cnt=another.cnt;
    }

    public State merge(Node node) {
        if (visited[node.f][node.x][node.y] || !current.linked.contains(node))
            return null;

        Node another = current.merge(node, visited);
        if (another==null) return null;

        current=another;

        visited[current.f][current.x][current.y]=true;
        route.add(current.toString());
        eatItem();
        cnt+=node.monsters.size()+node.doors.size();
        return this;
    }

    public void eatItem() {
        // 尽可能吃掉周边宝物
        boolean has=true;
        while (has) {
            has=false;

            for (Node node: current.linked) {
                if (visited[node.f][node.x][node.y]) continue;

                // if (node.isDoor() || (node.isMonster() && Util.getDamage(current.atk, current.def, current.mdef, node.hp, node.atk, node.def, node.mdef)>0)) continue;
                // if (node.isDoor() || node.isMonster()) continue;
                if (!node.shouldEat(current.hero)) continue;

                has=true;
                current=current.merge(node, visited);
                visited[current.f][current.x][current.y]=true;

                break;
            }
        }
    }

    public boolean shouldStop() {
        if (cnt>STOP_COUNT) return true;

        // Boss被打死？
        if (graph.boss!=null && visited[graph.boss.f][graph.boss.x][graph.boss.y]) return true;
        // if (current.f>0) return true;

        return false;
    }

    public String getString() {
        StringBuilder builder=new StringBuilder();
        for (Node node: graph.list) {
            if (node.item!=null) continue;
            builder.append(visited[node.f][node.x][node.y]?'1':'0');
        }
        return builder.toString();
    }

    public int getScore() {
        return current.getScore();
    }

}
