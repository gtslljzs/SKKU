package W5.Practice;
import java.util.*;

public class PhoneNumber {

    public static void main(String[] args) {

        Scanner scanner = new Scanner(System.in);
        HashMap<String, String> hashMap = new HashMap<String, String>();
        hashMap.put("HongGilDong", "010-3333-9999");
        hashMap.put("KimGilDong", "010-9999-3333");
        hashMap.put("ParkGilDong", "010-8888-2222");
        hashMap.put("ChoiGilDong", "010-2222-8888");
        hashMap.put("LeeGilDong", "010-5555-0000");

        while (true) {
            System.out.println("Please enter a name (Exit: 0): ");
            String name = scanner.next();
            if (name.equals("0")) break;

            String phone_number = hashMap.get(name);
            System.out.println(name + "'s phone number: " + phone_number);
    
        }

        scanner.close();
       
    }

}