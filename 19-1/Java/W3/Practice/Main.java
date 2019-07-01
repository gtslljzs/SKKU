package W3.Practice;

public class Main {

    public static void main(String[] args) {
        Point a = new Point(30, 50);
        Point b = new Point(40, 60);
        Point c = new Point(35, 55);
        Rect r = new Rect(a, b);

        System.out.println(r.isPointIn(c));
        System.out.println(r.areaSize());

    }
}