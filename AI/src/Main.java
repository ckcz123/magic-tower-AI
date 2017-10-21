import java.util.Scanner;

/**
 * Created by oc on 2017/10/12.
 */
public class Main {
    
    public static void main(String[] args){
        boolean shouldMerge=true, shouldEat=true;
        for (String string: args) {
            if ("--nomerge".equals(string)) shouldMerge=false;
            if ("--noeat".equals(string)) shouldEat=false;
        }

        Scanner scanner=new Scanner(System.in);
        Graph graph=new Graph(scanner, shouldMerge, shouldEat);

        System.out.println("------ Printing Nodes ------");

        int valid=0;

        for (Node node: graph.list) {
            System.out.println(node);
            if (!node.doors.isEmpty() || !node.monsters.isEmpty())
                valid++;
        }


        System.out.println(valid+"/"+graph.list.size()+" nodes in total.");

        graph.run();

    }
    
}
