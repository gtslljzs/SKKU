package W4.Practice;

class A {

    // public A() {
    //     System.out.println("Constructor A1");
    // }

    public A(int x) {
        System.out.println("Constructor A2");
    }
}

class B extends A {

    // public B() {
    //     System.out.println("Constructor B1");
    // }
    
    public B(int x) {
        super(x);
        System.out.println("Constructor B2");
    }
}

public class Test {

    public static void main(String[] args) {
        B b;
        b = new B(4);
    }

}