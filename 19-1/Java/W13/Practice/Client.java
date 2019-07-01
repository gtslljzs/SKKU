package W13.Practice;
import java.net.*;
import java.io.*;
public class Client {
	public static void main(String[] args) throws IOException{
		String serverIP = "localhost";
		Socket soc = new Socket(serverIP, 5000);
		InputStream in = soc.getInputStream();
		DataInputStream dis = new DataInputStream(in);
		System.out.println(dis.readUTF());
		dis.close();
		soc.close();
		}
	}

