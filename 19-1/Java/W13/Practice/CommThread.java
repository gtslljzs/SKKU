package W13.Practice;
import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;

public class CommThread extends Thread {
	private Socket soc;
	private int id;
	public CommThread (Socket soc, int id) {
		this.soc = soc;
		this.id = id;
	}

	public void run () {
		try {
			OutputStream os = soc.getOutputStream ();
			DataOutputStream dos = new DataOutputStream (os);

			dos.writeUTF ("message from server (" + id + ")");
			System.out.println (Server3.getLog ("message is sent (" + id + ")"));

			dos.close ();
			this.soc.close ();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
