package W5.Java_2013311689_week5;
import java.util.ArrayList;
import java.util.Scanner;

public class Main_2013311689 {
    
    public static void main(String[] args) {
        
        Scanner scanner = new Scanner(System.in);
        ArrayList<String> names = new ArrayList<String>();
        String longest_name;
    
        for (int i = 0; i < 4; i++) {

            System.out.println("Please enter a name: ");
            String name = scanner.next();
            names.add(name);
        
        }

        longest_name = names.get(0);
        for (int i = 0; i < 4; i++) {
            
            System.out.println(names.get(i));

            if (names.get(i).length() > longest_name.length())
                longest_name = names.get(i);
            
        }

        System.out.println("Longest name: " + longest_name);

        scanner.close();

    }

}