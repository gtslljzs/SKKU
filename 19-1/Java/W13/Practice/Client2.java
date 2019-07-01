package W13.Practice;
import java.net.*;
import java.io.*;
class Test extends Thread {
	public void run () {
		try {
			Socket soc = new Socket("localhost", 5000);
			DataInputStream dis = new DataInputStream (soc.getInputStream());

			System.out.println(dis.readUTF());

			dis.close();
			soc.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}

public class Client2 {
	public static void main (String[] args) {
		for (int i = 0; i < 100; i ++) {
			new Test ().start (); 
		}
	}
}
