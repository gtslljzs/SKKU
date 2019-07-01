package W13.Practice;
import java.io.Serializable;

public class Student implements Serializable {

    private int studnetAvg;
    private String studentName;

    Student(int avg, String name) {
        this.studnetAvg = avg;
        this.studentName = name;
    }

    public int getAvg() {
        return this.studnetAvg;
    }

    public void setAvg(int avg) {
        this.studnetAvg = avg;
    }

    public String getName() {
        return this.studentName;
    }

    public void setName(String name) {
        this.studentName = name;
    }


}