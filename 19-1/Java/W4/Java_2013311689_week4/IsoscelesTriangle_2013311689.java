package W4.Java_2013311689_week4;

public class IsoscelesTriangle_2013311689 extends Triangle_2013311689 {

    IsoscelesTriangle_2013311689(double sides[]) {

        super.setSideLength(sides);

    }

    @Override
    public boolean isValidTriangle() {
    
        if (super.isValidTriangle()) {

            double sides[] = super.getSideLength();
            double a = sides[0];
            double b = sides[1];
            double c = sides[2];

            if (a == b || b == c || c == a) {

                System.out.println("Also, this is an isosceles triangle");

                return true;
            }

            System.out.println("However, this is not an isosceles triangle");
        
        }

        return false;
    }
}