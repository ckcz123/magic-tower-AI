import java.util.*;

/**
 * Created by oc on 2017/10/12.
 */
public class Graph {

    public static final int ROAD = 0;
    public static final int WALL = 1;


    public static final int RED_JEWEL = 11;
    public static final int BLUE_JEWEL = 12;
    public static final int GREEN_JEWEL = 13;

    public static final int YELLOW_KEY = 15;
    public static final int BLUE_KEY = 16;
    public static final int RED_KEY = 17;

    public static final int RED_POTION = 18;
    public static final int BLUE_POTION = 19;
    public static final int YELLOW_POTION = 20;
    public static final int GREEN_POTION = 21;

    public static final int DOOR_YELLOW = 81;
    public static final int DOOR_BLUE = 82;
    public static final int DOOR_RED = 83;

    public static final int UPSTAIR = 87;
    public static final int DOWNSTAIR = 88;

    public static final int MONSTER_BOUND = 101;
    public static final int BOSS_INDEX = 199;


    int floor, row, col;
    int[][][] map;
    // up 0,1  down 2,3
    int[][] stair;

    int bossId=-1;

    int p_atk, p_def, p_mdef, p_red, p_blue, p_yellow, p_green;
    HashMap<Integer, Monster> monsterMap;

    ArrayList<Node> list;

    public Graph(Scanner scanner) {
        list=new ArrayList<>();
        floor=scanner.nextInt(); row=scanner.nextInt(); col=scanner.nextInt();
        map=new int[floor][row][col];
        stair=new int[floor][4];
        for (int i=0;i<floor;i++) stair[i]=new int[]{-1,-1,-1,-1};

        for (int i=0;i<floor;i++) {
            for (int j=0;j<row;j++) {
                for (int k=0;k<col;k++) {
                    map[i][j][k]=scanner.nextInt();
                }
            }
        }

        // points
        p_atk=scanner.nextInt();
        p_def=scanner.nextInt();
        p_mdef=scanner.nextInt();
        p_red=scanner.nextInt();
        p_blue=scanner.nextInt();
        p_yellow=scanner.nextInt();
        p_green=scanner.nextInt();

        // Monsters
        monsterMap=new HashMap<>();
        int num=scanner.nextInt();
        for (int i=0;i<num;i++) {
            int id=scanner.nextInt();
            monsterMap.put(id, new Monster(id, scanner.nextInt(), scanner.nextInt(), scanner.nextInt(), scanner.nextInt()));
        }

        // Initial Node
        int hp=scanner.nextInt(), atk=scanner.nextInt(), def=scanner.nextInt(), mdef=scanner.nextInt(),
                yellow=scanner.nextInt(), blue=scanner.nextInt(), red=scanner.nextInt(),
                floor=scanner.nextInt(), x=scanner.nextInt(), y=scanner.nextInt();

        Node node=new Node(0, new Hero(hp, atk, def, mdef, yellow, blue, red), null, null, 0,
                floor, x, y);
        list.add(node);

        buildMap();

        mergeNode();

        // set id
        for (int i=0;i<list.size();i++) {
            list.get(i).setId(i);
            if (list.get(i).type==BOSS_INDEX)
                bossId=i;
        }

    }

    public void buildMap() {
        for (int i=0;i<floor;i++) {
            for (int j=0;j<row;j++) {
                for (int k=0;k<col;k++) {
                    Node node=null;
                    if (map[i][j][k]==UPSTAIR) {
                        stair[i][0]=j; stair[i][1]=k;
                    }
                    if (map[i][j][k]==DOWNSTAIR) {
                        stair[i][2]=j; stair[i][3]=k;
                    }
                    if (map[i][j][k]==YELLOW_KEY)
                        node=new Node(map[i][j][k], null, null,
                                new Item(0,0,0,0,1,0,0),
                                0,i,j,k);
                    if (map[i][j][k]==BLUE_KEY)
                        node=new Node(map[i][j][k], null, null,
                                new Item(0,0,0,0,0,1,0),
                                0,i,j,k);
                    if (map[i][j][k]==RED_KEY)
                        node=new Node(map[i][j][k], null, null,
                                new Item(0,0,0,0,0,0,1),
                                0,i,j,k);
                    if (map[i][j][k]==RED_JEWEL)
                        node=new Node(map[i][j][k], null, null,
                                new Item(0,p_atk,0,0,0,0,0),
                                0,i,j,k);
                    if (map[i][j][k]==BLUE_JEWEL)
                        node=new Node(map[i][j][k], null, null,
                                new Item(0,0,p_def,0,0,0,0),
                                0,i,j,k);
                    if (map[i][j][k]==GREEN_JEWEL)
                        node=new Node(map[i][j][k], null, null,
                                new Item(0,0,0,p_mdef,0,0,0),
                                0,i,j,k);
                    if (map[i][j][k]==RED_POTION)
                        node=new Node(map[i][j][k], null, null,
                                new Item(p_red,0,0,0,0,0,0),
                                0,i,j,k);
                    if (map[i][j][k]==BLUE_POTION)
                        node=new Node(map[i][j][k], null, null,
                                new Item(p_blue,0,0,0,0,0,0),
                                0,i,j,k);
                    if (map[i][j][k]==YELLOW_POTION)
                        node=new Node(map[i][j][k], null, null,
                                new Item(p_yellow,0,0,0,0,0,0),
                                0,i,j,k);
                    if (map[i][j][k]==GREEN_POTION)
                        node=new Node(map[i][j][k], null, null,
                                new Item(p_green,0,0,0,0,0,0),
                                0,i,j,k);

                    if (map[i][j][k]==DOOR_YELLOW)
                        node=new Node(map[i][j][k], null, null, null, 1, i,j,k);
                    if (map[i][j][k]==DOOR_BLUE)
                        node=new Node(map[i][j][k], null, null, null, 2, i,j,k);
                    if (map[i][j][k]==DOOR_RED)
                        node=new Node(map[i][j][k], null, null, null, 3, i,j,k);
                    if (map[i][j][k]>=MONSTER_BOUND) {
                        Monster monster=monsterMap.get(map[i][j][k]);
                        if (monster==null) continue;
                        node=new Node(map[i][j][k], null, monster, null, 0, i, j, k);
                    }

                    if (node!=null)
                        list.add(node);
                }
            }
        }

        // build graph
        int len=list.size();
        for (int i=0;i<len;i++) {
            for (int j=i+1;j<len;j++) {
                Node n1=list.get(i), n2=list.get(j);
                if (isLinked(n1.f, n1.x, n1.y, n2.f, n2.x, n2.y)) {
                    n1.addNode(n2);
                    n2.addNode(n1);
                }
            }
        }
    }


    private void mergeNode() {
        for (int i=1;i<list.size();i++) {
            Node n1=list.get(i);
            for (int j=i+1;j<list.size();j++) {
                Node n2=list.get(j);
                if (shouldMerge(n1, n2)) {
                    n1.merge(n2);
                    list.remove(j);
                    mergeNode();
                    return;
                }
            }
        }
    }

    private boolean shouldMerge(Node n1, Node n2) {
        if (n1.item!=null || n2.item!=null) return false;
        if (n1.type==BOSS_INDEX || n2.type==BOSS_INDEX) return false;
        if (!n1.linked.contains(n2) || !n2.linked.contains(n1)) return false;
        for (Node node: n2.linked)
            if (n1.linked.contains(node))
                return false;
        for (Node u: new Node[] {n1, n2}) {
            Node v=u==n1?n2:n1;
            for (Node x: u.linked) {
                for (Node y: u.linked) {
                    if (x.equals(y) || x.equals(v) || y.equals(v)) continue;
                    if (!x.linked.contains(y) || !y.linked.contains(x)) return false;
                }
            }
        }
        return true;
    }

    private boolean isLinked(int f1, int x1, int y1, int f2, int x2, int y2) {
        // 多层
        if (f1<f2) return isLinked(f2,x2,y2,f1,x1,y1);
        if (f1!=f2)
            return isLinked(f1,x1,y1,f1,stair[f1][2],stair[f1][3])
                && isLinked(f1-1,stair[f1-1][0],stair[f1-1][1],f2,x2,y2);

        if (x1==x2 && y1==y2) return true;
        boolean[][] visited=new boolean[row][col];
        visited[x1][y1]=true;

        Queue<Integer> queue=new LinkedList<>();
        queue.offer(x1); queue.offer(y1);
        while (!queue.isEmpty()) {
            int x=queue.poll(), y=queue.poll();
            for (int[] dir: new int[][] {{-1,0},{0,1},{1,0},{0,-1}}) {
                int nx=x+dir[0], ny=y+dir[1];
                if (nx<0 || nx>=row || ny<0 || ny>=col) continue;
                if (nx==x2 && ny==y2) return true;
                if (visited[nx][ny] || (map[f1][nx][ny]!=ROAD && map[f1][nx][ny]!=UPSTAIR && map[f1][nx][ny]!=DOWNSTAIR)) continue;
                visited[nx][ny]=true;
                queue.offer(nx); queue.offer(ny);
            }
        }

        return false;
    }

    public void run() {
        System.out.println("\n------ Starting BFS ------");
        State state=new State(this, list.get(0));
        State answer=null;

        int index=0;

        HashSet<String> set=new HashSet<>();
        HashMap<String, Integer> map=new HashMap<>();

        // !!! start bfs !!!!!

        long start=System.currentTimeMillis();

        PriorityQueue<State> queue=new PriorityQueue<>((o1,o2)->{
            if (o1.cnt==o2.cnt) return o2.getScore()-o1.getScore();
            return o1.cnt-o2.cnt;
        });
        queue.offer(state);

        while (!queue.isEmpty()) {
            state=queue.poll();

            if (!set.add(state.hashString())) continue;

            if (state.shouldStop()) {
                if (answer==null || answer.getScore()<state.getScore())
                    answer=state;
                continue;
                //break;
            }

            // extend
            for (Node node: state.current.linked) {
                // visited
                if (state.visited[node.id]) continue;

                /*
                // should extend?
                boolean shouldExtend=false;
                for (Node x: node.linked) {
                    if (!x.equals(state.current) && !state.current.linked.contains(x)) {
                        shouldExtend=true;
                        break;
                    }
                }
                if (!shouldExtend) {
                    continue;
                }
                */

                // extend
                State another=new State(state).merge(node);
                if (another==null) continue;
                String hash=another.hashString();
                if (map.getOrDefault(hash, -1)>another.getScore()) continue;
                map.put(hash, another.getScore());
                queue.offer(another);

            }

            index++;
            if (index%1000==0) {
                System.out.println(String.format("Calculating... %d calculated, %d still in queue.", index, queue.size()));
            }

        }
        System.out.println("cnt: "+index);
        System.out.println("------ BFS FINISHED ------");

        if (answer==null) {
            System.out.println("No solution!");
        }
        else {
            for (String string: answer.route) {
                System.out.println(string);
            }
        }

        long end=System.currentTimeMillis();

        System.err.println(String.format("Time used: %.3fs", (end-start)/1000.0));
    }
}
