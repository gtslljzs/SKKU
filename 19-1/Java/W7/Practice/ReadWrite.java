package W7.Practice;
import java.io.*;

public class ReadWrite {

    public static void main(String[] args) throws IOException{

        String path = new String("G:\\SKKU\\2019-1\\Java\\W7\\Practice\\test.txt");
        // String path = new String("G:/SKKU/2019-1/Java/W7/Practice/test.txt");
        String opath = new String("G:\\SKKU\\2019-1\\Java\\W7\\Practice\\output.txt");
        
        try {

            FileInputStream fis = new FileInputStream(path);
            InputStreamReader isr = new InputStreamReader(fis);
            BufferedReader br = new BufferedReader(isr);

            Writer osw = new OutputStreamWriter(System.out); // console
            OutputStream fos = new FileOutputStream(opath);
            OutputStreamWriter fosw = new OutputStreamWriter(fos); // file
            BufferedWriter bw = new BufferedWriter(fosw);
            

            String data = "abc";
            String readData = "";
            // System.out.println(data);

            do {
                String data2 = br.readLine();
                if (data2 == null) break;

                readData += data2;
                readData += "\n";

                osw.write(data, 0, 3);
                // osw.write(data2, 0, 1);

                bw.write(data2, 0, 5);

            } while (true);

            // System.out.println(readData);

            br.close();
            bw.close();
            isr.close();
            fis.close();

            osw.close();
        
        } catch (IOException e) {
            e.printStackTrace();
        }

        // String[] datasets = fis.split(",");

    }
}
