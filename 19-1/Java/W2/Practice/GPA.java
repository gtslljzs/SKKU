package W2.Practice;
import java.util.Scanner;

public class GPA {

    public static void main(String[] args) {

        Scanner s = new Scanner(System.in);

        System.out.println("Input 1-1's grade");
        double gpa = s.nextDouble();
        System.out.println("Input 1-2's grade");
        gpa += s.nextDouble();
        System.out.println("Input 2-1's grade");
        gpa += s.nextDouble();
        System.out.println("Input 2-2's grade");
        gpa += s.nextDouble();
        System.out.println("Input 3-1's grade");
        gpa += s.nextDouble();
        System.out.println("Input 3-2's grade");
        gpa += s.nextDouble();
        System.out.println("Input 4-1's grade");
        gpa += s.nextDouble();
        System.out.println("Input 4-2's grade");
        gpa += s.nextDouble();

        System.out.println("\nAverage GPA : " + gpa/8);

        s.close();
        
    }
}