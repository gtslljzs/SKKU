package W13.Practice;
import java.net.*;
import java.io.*;
public class Server {
	public static void main(String[] args) throws IOException {
		ServerSocket ss = new ServerSocket(5000);
		while(true)
		{
			Socket soc = ss.accept();
			OutputStream out = soc.getOutputStream();
			DataOutputStream dos = new DataOutputStream (out);
			dos.writeUTF ("message from server");
			dos.close ();
			soc.close ();

		}
	}
}

