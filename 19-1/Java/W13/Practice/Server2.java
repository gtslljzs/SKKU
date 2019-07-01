package W13.Practice;
import java.text.*;
import java.util.*;
import java.io.*;
public class Server2 {
	private static SimpleDateFormat sdfDate = new SimpleDateFormat("yyy-MM-dd HH:mm:SSS");
	private static String getLog (String msg)
	{
		return "[" + sdfDate.format(new Date ()) + "] Main thread : " + msg;
	}
	public static void main(String[] args) throws IOException{
		Thread t = new ServerThread();
		t.start();
		System.out.println(getLog("server Thread started:"));
		boolean flag = true;
		while (flag) {
			try {
				Thread.sleep(1000);
			}
			catch(InterruptedException e)
			{
				e.printStackTrace();
			}
			System.out.println(getLog("server still alive"));
		}
	}
	
}
