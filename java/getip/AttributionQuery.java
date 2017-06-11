import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.BufferedReader;
import java.io.*;
import java.util.regex.*;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;


public class AttributionQuery {
    public static String getPublicNetworkIP() {
        String ip = "";
        String chinaz = "http://ip.chinaz.com/getip.aspx";
        String inputLine = "";
        String read = "";

        try {
            URL url = new URL(chinaz);
            HttpURLConnection urlConnection = (HttpURLConnection)url.openConnection();
            BufferedReader in = new BufferedReader(new InputStreamReader(
            										urlConnection.getInputStream()));

            while ((read = in.readLine()) != null) {
                inputLine += read;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        Pattern p = Pattern.compile("\\d+\\.\\d+\\.\\d+\\.\\d+");
        Matcher m = p.matcher(inputLine);

        if(m.find()) {
            ip = m.group(0);
        }

        return ip;
    }

    public static String GetAddressByIp(String IP) {
        String resout = "";

        try {
            resout = getJsonContent("http://ip.taobao.com/service/getIpInfo.php?ip="+IP);
        } catch(Exception e) { 
            e.printStackTrace();
        }

        return resout;     
    }
    
    public static String getJsonContent(String urlStr) {
        try {
            URL url = new URL(urlStr);
            HttpURLConnection httpConn = (HttpURLConnection)url.openConnection();

            httpConn.setConnectTimeout(3000);
            httpConn.setDoInput(true);
            httpConn.setRequestMethod("GET");
            
            int respCode = httpConn.getResponseCode();
            if (respCode == 200) {
                return ConvertStream2Json(httpConn.getInputStream());
            }
        } catch (MalformedURLException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

        return "";
    }
 
    private static String ConvertStream2Json(InputStream inputStream) {
        String jsonStr = "";
        ByteArrayOutputStream out = new ByteArrayOutputStream();
        byte[] buffer = new byte[1024];
        int len = 0;

        try {
            while ((len = inputStream.read(buffer, 0, buffer.length)) != -1) {
                out.write(buffer, 0, len);
            }

            jsonStr = new String(out.toByteArray());
        } catch (IOException e) {
            e.printStackTrace();
        }

        return jsonStr;
    }
}

