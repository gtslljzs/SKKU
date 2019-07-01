package W4.Java_2013311689_week4;
import java.util.Scanner;

public class Main_2013311689 {

    public static void main(String[] args) {

        // TODO Auto-generated method stub
        Scanner scanner = new Scanner(System.in);
        Triangle_2013311689 t = new Triangle_2013311689();

        while (true) {

            System.out.println("----------Start Make Triangle----------");
            double s[] = new double[3];
            for (int i = 0; i < 3; i++) {

                System.out.println("Enter S"+ (int) (i + 1) + ": ");
                s[i] = scanner.nextDouble();

            }

            if (s[0] == 0 || s[1] == 0 || s[2] == 0) break;

            t.setSideLength(s);
            t.area();
            t.printTriangleInfo();

            IsoscelesTriangle_2013311689 it = new IsoscelesTriangle_2013311689(t.getSideLength());
            it.isValidTriangle();

            EquilateralTriangle_2013311689 et = new EquilateralTriangle_2013311689(t.getSideLength());
            et.isValidTriangle();

            ScaleneTriangle_2013311689 st = new ScaleneTriangle_2013311689(t.getSideLength());
            st.isValidTriangle();

        }

        scanner.close();
        System.out.println("----------End Make Triangle----------");

    }
}