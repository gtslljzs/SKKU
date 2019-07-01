import java.util.Scanner;
import java.io.IOException;
import java.io.*;

public class HelloWorld {

    public static void main (String[] args) throws IOException {

        OutputStream fos = new FileOutputStream("G:/SKKU/19-1/Java/test.txt");
        byte[] data = {'a', 'b', 'c'};
        fos.write(data);
        fos.write(data[0]);
        fos.flush();

        Writer osw = new OutputStreamWriter(fos);
        BufferedWriter bw = new BufferedWriter(osw);
        String data2 = "abc";
        bw.newLine();
        bw.write(data2);
        bw.write(data2, 0, 1);
        bw.flush();
        bw.close();
        osw.close();
        fos.close();

    }

}


class StopWatch {
    private String runTime;

    public long start() {
        return System.currentTimeMillis();
    }

    public long stop() {
        return System.currentTimeMillis();
    }

    public String getRunTime(long _msecs) {
        int msec = (int)_msecs % 1000;
        int sec = (int)(_msecs / 1000 % 60);
        int min = (int)(_msecs / 1000 / 60 % 60);

        this.runTime = String.format("%02d:%02d:%03d", min, sec, msec);

        return this.runTime;
    }
}