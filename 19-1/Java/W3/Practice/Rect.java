package W3.Practice;
import java.lang.Math;

public class Rect {
    Point a, b;

    Rect (Point a, Point b) {
        this.a = a;
        this.b = b;
    }

    boolean isPointIn(Point c) {
        
        if ((c.x > Math.min(this.a.x, this.b.x) && c.x < Math.max(this.a.x, this.b.x)) && (c.y > Math.min(this.a.y, this.b.y) && c.y < Math.max(this.a.y, this.b.y)))
            return true;
        else
            return false;

    }

    double areaSize() {
        double area = Math.abs((this.a.x - this.b.x) * (this.a.y - this.b.y));

        return area;
    }
}