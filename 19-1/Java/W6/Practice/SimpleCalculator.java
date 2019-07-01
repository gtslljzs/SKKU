package W6.Practice;

public class SimpleCalculator {
    
    private int result = 0;

    public void add(int a, int b) throws AddZeroException {
        
        if (a == 0 || b == 0) throw new AddZeroException();
        else this.setResult(a + b);

    }

    public void subtract(int a, int b) throws SubtractZeroException {

        if (a == 0 || b == 0) throw new SubtractZeroException();
        else this.setResult(a - b);
    
    }

    public void print() throws OutOfRangeException {

        if (this.result < 0 || this.result > 1000) throw new OutOfRangeException();
        else System.out.println(this.result);

    }

    public void reset() { this.setResult(0); }

    int getResult() { return this.result; }

    void setResult(int x) { this.result = x; }

}

class AddZeroException extends Exception {

    public String message;

    public AddZeroException() {

        this.message = new String("Add Zero Exception");
    
    }

    public AddZeroException(String message) {
    
        this.message = new String(message);
    
    }

}

class SubtractZeroException extends Exception {

    public String message;

    public SubtractZeroException() {

        this.message = new String("Subtract Zero Exception");

    }

    public SubtractZeroException(String message) {
        
        this.message = new String(message);

    }
}

class OutOfRangeException extends Exception {

    public String message;

    public OutOfRangeException() {

        this.message = new String("Out of Range Exception");

    }

    public OutOfRangeException(String message) {

        this.message = new String(message);

    }
}