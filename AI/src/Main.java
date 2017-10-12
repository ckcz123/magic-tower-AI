import java.util.Scanner;

/**
 * Created by oc on 2017/10/12.
 */
public class Main {
    
    public static void main(String[] args){
        Scanner scanner=new Scanner(System.in);
        Graph graph=new Graph(scanner);

        System.out.println("------ Printing Nodes ------");

        for (Node node: graph.list) {
            System.out.println(node);
        }

        graph.run();



    }
    
}
