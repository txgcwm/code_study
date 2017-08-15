import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Timer;


public class Broadcast {
    public void sendData()throws SocketException, UnknownHostException {
    	DatagramSocket ds = new DatagramSocket();
		String str = "1";
		DatagramPacket dp = new DatagramPacket(str.getBytes(),
				str.getBytes().length,
				InetAddress.getByName("192.168.1.255"), 9999);

		try {
			ds.send(dp);
		} catch (IOException e) {
			e.printStackTrace();
		}

		ds.close();
    }

    static class BroadcastTask extends java.util.TimerTask{ 
        @Override
        public void run() { 
        	Broadcast tt = new Broadcast();

            try {
                tt.sendData();
            } catch (SocketException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            } catch (UnknownHostException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        }
    }

    public static void main(String[] args) {
	    Timer timer = new Timer();
	    timer.schedule(new BroadcastTask(), 1000, 1000);

        return;
	}
}

