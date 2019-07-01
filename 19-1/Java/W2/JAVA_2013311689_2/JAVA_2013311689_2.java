package W2.JAVA_2013311689_2;
import java.util.Scanner;

public class JAVA_2013311689_2 {

    public static void main(String[] args) {

        Scanner s = new Scanner(System.in);
        System.out.println("Input your grade: ");
        String tmp = s.next();
        char grade = tmp.charAt(0);

        if (tmp.length() > 1) {
            System.out.println("Wrong Input");
            s.close();  
            return;
        }

        switch (grade) {
            case ('A'):
                System.out.println("Excellent");
                break;
            case ('B'):
                System.out.println("Excellent");
                break;
            case ('C'):
                System.out.println("Good");
                break;
            case ('D'):
                System.out.println("Good");
                break;
            case ('F'):
                System.out.println("Bye");
                break;
            default:
                System.out.println("Wrong Input");
                break;
        }

        s.close();

    }
}