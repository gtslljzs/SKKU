package W13.Practice;
import java.text.*;
import java.util.*;
import java.io.*;
import java.net.*;

public class Server3 {
	private static ArrayList<Thread> arr = new ArrayList<Thread> ();
	private static SimpleDateFormat sdfDate = new SimpleDateFormat ("yyy-MM-dd HH:mm:SSS");

	public static String getLog (String msg) {
		return "[" + sdfDate.format(new Date ()) + "] Server thread: " + msg;
	}

	public static void main (String[] args) {
		ServerSocket ss = null;
		int id = 0;
		try {
			ss = new ServerSocket (5000);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		System.out.println("server is ready");
		while (true) {
			try {
				Socket soc = ss.accept ();
				System.out.println(Server3.getLog ("new connection arrived"));
				Thread t = new CommThread (soc, id ++);
				t.start ();
				arr.add(t);
				Iterator<Thread> iter = arr.iterator ();
				while (iter.hasNext ()) {
					t = iter.next ();
					if (!t.isAlive ()) {
						iter.remove ();
					}
				}
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}
}
