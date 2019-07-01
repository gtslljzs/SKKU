package W13.Practice;
import java.net.*;
import java.text.*;
import java.util.*;
import java.io.*;

public class ServerThread extends Thread{
	private String name = null;
	private static SimpleDateFormat sdfDate = new SimpleDateFormat("yyy-MM-dd HH:mm:SSS");
	private static String getLog (String msg)
	{
		return "[" + sdfDate.format(new Date ()) + "] Server thread : " + msg;
	}
	public ServerThread()
	{
		this.name = "ServerThread";
	}
	public void run()
	{
		ServerSocket ss = null;
		try {
			ss = new ServerSocket(5000);
		}
		catch(IOException e)
		{
			e.printStackTrace();
		}
		while(true)
		{
			Socket soc = null;
			OutputStream out = null;
			try {
				soc = ss.accept();
				System.out.println(ServerThread.getLog("new connection arrived"));
				out = soc.getOutputStream();
				DataOutputStream dos = new DataOutputStream(out);
				dos.writeUTF("message from server");
				dos.close();
				soc.close();
			}
			catch(IOException e)
			{
				e.printStackTrace();
			}
			finally {
				try {
					soc.close();
				}
				catch(IOException e)
				{
					e.printStackTrace();
				}
			}
		}
	}

}
