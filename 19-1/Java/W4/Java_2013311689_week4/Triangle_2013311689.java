package W4.Java_2013311689_week4;
import java.lang.Math;

public class Triangle_2013311689 implements Shape_2013311689 {

    private double side_length[] = new double[3]; // 3 length of side
    protected double area;

    @Override
    public void area() {

        double a = this.side_length[0];
        double b = this.side_length[1];
        double c = this.side_length[2];
        double s = (a + b + c) / 2;
        double S = Math.sqrt(s * (s - a) * (s - b) * (s - c));

        if (a + b > c && b + c > a && c + a > b)
            this.area = S;
        else this.area = 0;

    }

    public void setSideLength(double side[]) {
        
        this.side_length = side;

    }
    
    public double[] getSideLength() {

        return this.side_length;
    }
    
    public boolean isValidTriangle() {

        double a = this.side_length[0];
        double b = this.side_length[1];
        double c = this.side_length[2];

        if ((a + b > c) && (b + c > a) && (c + a > b)) {
        
            System.out.println("This is a triangle");

            return true;
        }
                
        System.out.println("This is not a triangle");

        return false;
    }

    public void printTriangleInfo() {

        System.out.println("----------Triangle Info----------");
        System.out.println("S0: " + this.side_length[0]);
        System.out.println("S1: " + this.side_length[1]);
        System.out.println("S2: " + this.side_length[2]);
        System.out.println("Area: " + this.area);
        System.out.println("---------------------------------");

    }

}