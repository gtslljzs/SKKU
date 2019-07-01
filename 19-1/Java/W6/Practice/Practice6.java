package W6.Practice;

public class Practice6 {

    public static void main(String[] args) {

        SimpleCalculator calc = new SimpleCalculator();

        try {

            calc.add(3, 0);
        
        } catch (AddZeroException e) {
        
            System.out.println(e.message);
        
        }

        try {
            
            calc.subtract(3, 0);
        
        } catch (SubtractZeroException e) {

            System.out.println(e.message);

        }

        try {
        
            calc.add(1000, 5);
            calc.print();
        
        } catch(AddZeroException e) {

            System.out.println(e.message);
        
        } catch(OutOfRangeException e) {

            System.out.println(e.message);

        }
    }
}