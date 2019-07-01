package W2.JAVA_2013311689_3;
import java.util.Scanner;

public class JAVA_2013311689_3 {

    public static void main(String[] args) {

        Scanner s = new Scanner(System.in);
        System.out.println("Input charge to convert :");

        int charge = s.nextInt();
        int cnt_50000 = 0;
        int cnt_10000 = 0;
        int cnt_5000 = 0;
        int cnt_1000 = 0;
        int cnt_500 = 0;
        int cnt_100 = 0;
        int cnt_50 = 0;
        int cnt_10 = 0;

        while (charge >= 10) {
            if (charge >= 50000) {
                charge -= 50000;
                cnt_50000++;
            } else if (charge < 50000 && charge >= 10000) {
                charge -= 10000;
                cnt_10000++;
            } else if (charge < 10000 && charge >= 5000) {
                charge -= 5000;
                cnt_5000++;
            } else if (charge < 5000 && charge >= 1000) {
                charge -= 1000;
                cnt_1000++;
            } else if (charge < 1000 && charge >= 500) {
                charge -= 500;
                cnt_500++;
            } else if (charge < 500 && charge >= 100) {
                charge -= 100;
                cnt_100++;
            } else if (charge < 100 && charge >= 50) {
                charge -= 50;
                cnt_50++;
            } else if (charge < 50 && charge >= 10) {
                charge -= 10;
                cnt_10++;
            }
        }

        System.out.println("50000 : " + cnt_50000);
        System.out.println("10000 : " + cnt_10000);
        System.out.println("5000 :" + cnt_5000);
        System.out.println("1000 :" + cnt_1000);
        System.out.println("500 :" + cnt_500);
        System.out.println("100 :" + cnt_100);
        System.out.println("50 :" + cnt_50);
        System.out.println("10 :" + cnt_10);
        System.out.println("left :" + charge);
        
        s.close();

    }
}