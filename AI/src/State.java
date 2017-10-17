import java.util.ArrayList;

/**
 * Created by oc on 2017/10/12.
 */
public class State {

    Node current;
    Graph graph;
    boolean[] visited;
    ArrayList<String> route;
    int shopTime;
    int cnt;

    public static final int STOP_COUNT = 45;

    public State(Graph _graph, Node node) {
        graph=_graph;
        current=node;
        cnt=0;
        visited=new boolean[graph.list.size()];
        visited[current.id]=true;
        route=new ArrayList<>();
        route.add(current.toString());
        shopTime=-1;
        eatItem();
    }

    public State(State another) {
        graph=another.graph;
        current=another.current;
        visited=another.visited.clone();
        route=new ArrayList<>(another.route);
        shopTime=another.shopTime;
        cnt=another.cnt;
    }

    public State merge(Node node) {
        if (visited[node.id] || !current.linked.contains(node))
            return null;

        Node another = current.merge(node, visited);
        if (another==null) return null;

        current=another;
        // visited[node.id]=true;
        visit(node);
        route.add(current.toString());
        eatItem();
        cnt++;
        return this;
    }

    // 尽可能吃掉周边宝物
    public void eatItem() {
        boolean has=true;
        while (has) {
            has=false;
            for (Node node: current.linked) {
                if (visited[node.id]) continue;
                if (!node.shouldEat(current.hero)) continue;
                has=true;
                current=current.merge(node, visited);
                if (node.item!=null && (node.item.special&1)!=0)
                    shopTime=Math.max(shopTime, 0);
                visit(node);
                // visited[node.id]=true;
                break;
            }
        }
        // Use shop
        while (graph.shop.useShop(current.hero, shopTime))
            shopTime++;
    }

    public void visit(Node node) {
        if (!visited[node.id] && current.linked.remove(node)) {
            for (Monster monster: node.monsters)
                current.hero.money+=monster.money;
            visited[node.id] = true;
        }
    }

    public boolean shouldStop() {
        if (cnt>STOP_COUNT) return true;

        // Boss被打死？
        if (graph.bossId>=0 && visited[graph.bossId]) return true;

        return false;
    }

    public long hash() {
        long val=0;
        int i=0;
        for (Node node: graph.list) {
            if (node.doors.isEmpty() && node.monsters.isEmpty()) continue;
            if (visited[node.id]) val|=(1L<<i);
            i++;
        }
        return val;
    }

    public int getScore() {
        return current.getScore();
    }

}
