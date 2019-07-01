package W7.HW_2013311689;
import java.io.*;

public class Main {

    public static void main(String[] args) throws IOException {

        try {

            CSVMatrixReader reader1 = new CSVMatrixReader("csv_input1.txt");
            CSVMatrixReader reader2 = new CSVMatrixReader("csv_input2.txt");
            CSVMatrixWriter writer = new CSVMatrixWriter();

            int[][] matrix1 = reader1.read();
            int[][] matrix2 = reader2.read();
            int[][] result = new int[5][5];
            
            System.out.println("----------Read Matrix1----------");
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
                    System.out.print(matrix1[i][j] + " ");
                }
                System.out.println("");
            }
            
            System.out.println("----------Read Matrix2----------");
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
                    System.out.print(matrix2[i][j] + " ");
                }
                System.out.println("");
            }
            
            System.out.println("----------Result Matrix---------");
            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
                    result[i][j] = matrix1[i][j] + matrix2[i][j];
                    System.out.print(result[i][j] + " ");
                }
                System.out.println("");
            }

            writer.write(result);

        } catch (IOException e ) {
            
            e.printStackTrace();
        }
    }
}

class CSVMatrixReader {

    private String path = new String("G:\\SKKU\\2019-1\\Java\\W7\\HW_2013311689\\");
    private String input;

    CSVMatrixReader() {}
    CSVMatrixReader(String input) {

        this.input = new String(input);
    
    }

    public int[][] read() throws IOException {
        
        int[][] matrix = new int[5][5];
        int row = 0;

        try {

            FileInputStream fis = new FileInputStream(this.path + this.input);
            InputStreamReader isr = new InputStreamReader(fis);
            BufferedReader br = new BufferedReader(isr);
            
            do {

                String read_data = br.readLine();
                if (read_data == null) break;

                String[] token = read_data.split(",", -1);
                for (int i = 0; i < 5; i++)
                    matrix[row][i] = Integer.parseInt(token[i]);
                
                // for (int i = 0; i < 5; i++)
                //     System.out.println(matrix[row][i] + ", ");
                
                row++;

            } while (true);

            br.close();
            isr.close();
            fis.close();

        } catch (IOException e) {
            
            e.printStackTrace();

        }

        return matrix;
    }
        
}

class CSVMatrixWriter {

    private String path;

    CSVMatrixWriter() {
        
        this.path = new String("G:\\SKKU\\2019-1\\Java\\W7\\HW_2013311689\\csv_output.txt");

    }

    CSVMatrixWriter(String path) {

        this.path = new String(path);
    
    }

    public void write(int[][] data) throws IOException {
        
        try {

            OutputStream fos = new FileOutputStream(path);
            OutputStreamWriter osw = new OutputStreamWriter(fos);
            BufferedWriter bw = new BufferedWriter(osw);

            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
                    String buf = Integer.toString(data[i][j]);
                    if (j == 4) { buf += "\r\n"; }
                    else { buf += ","; }
                    bw.write(buf);
                }
            }
        
            bw.close();
            osw.close();
            fos.close();

        } catch (IOException e ) {

            e.printStackTrace();

        }
    }
}