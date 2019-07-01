package W4.Java_2013311689_week4;

public class EquilateralTriangle_2013311689 extends Triangle_2013311689 {

    EquilateralTriangle_2013311689(double sides[]) {

        super.setSideLength(sides);

    }

    @Override
    public boolean isValidTriangle() {
    
        if (super.isValidTriangle()) {

            double sides[] = super.getSideLength();
            double a = sides[0];
            double b = sides[1];
            double c = sides[2];

            if (a == b && b == c) {

                System.out.println("Also, this is an equilateral triangle");

                return true;
            }

            System.out.println("However, this is not an equilateral triangle");

        }

        return false;
    }

}