package W2.JAVA_2013311689_1;
import java.util.Scanner;

public class JAVA_2013311689_1 {

    public static void main(String[] args) {

        Scanner s = new Scanner(System.in);

        System.out.println("Input x y");
        int x = s.nextInt();
        int y = s.nextInt();

        if ((x > 50 && x < 100) && (y > 50 && y < 100))
            System.out.println("Inside");
        else if ((x == 50 || x == 100) && (y >= 50 && y <= 100))
            System.out.println("On the line segement");
        else if ((y == 50 || y == 100) && (x >= 50 && x <= 100))
            System.out.println("On the line segement");
        else
            System.out.println("Outside");
        
        s.close();

    }
}