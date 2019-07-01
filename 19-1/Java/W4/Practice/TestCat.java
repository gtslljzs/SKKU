package W4.Practice;

public class TestCat {

    public static void main(String[] args) {

        Cat c1 = new Cat();
        Cat c2 = new Cat("Bamtol", 5.2, 2);

        System.out.println("Call printKind() of c1!");
        c1.printKind();
        System.out.println("Call printCatInfo() of c1!");
        c1.printCatInfo();

        System.out.println("Call printKind() of c2!");
        c2.printKind();
        System.out.println("Call printCatInfo of c2!");
        c2.printCatInfo();

        return;
    }
}