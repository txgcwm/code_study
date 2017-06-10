import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.BufferedReader;
import java.io.*;
import java.util.regex.*;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.Inet4Address;
import java.net.InetAddress;
import java.net.NetworkInterface;
import java.util.Enumeration;

// import net.sf.json.JSONObject;



public class GetAddressByIp
{
    public static String getV4IP() {
        String ip = "";
        String chinaz = "http://ip.chinaz.com/getip.aspx";
        String inputLine = "";
        String read = "";

        try {
            URL url = new URL(chinaz);
            HttpURLConnection urlConnection = (HttpURLConnection)url.openConnection();
            BufferedReader in = new BufferedReader(new InputStreamReader(urlConnection.getInputStream()));

            while ((read = in.readLine()) != null) {
                inputLine += read;
            }

            // System.out.println(inputLine);
        } catch (Exception e) {
            e.printStackTrace();
        }

        Pattern p = Pattern.compile("\\d+\\.\\d+\\.\\d+\\.\\d+");
        Matcher m = p.matcher(inputLine);

        if(m.find()) {
            ip = m.group(0);
            // System.out.println(ipstr);
        }

        return ip;
    }

    public static String GetAddressByIp(String IP) {
        String resout = "";

        try {
            String str = getJsonContent("http://ip.taobao.com/service/getIpInfo.php?ip="+IP);
            System.out.println(str);
          
            // JSONObject obj = JSONObject.fromObject(str);
            // JSONObject obj2 =  (JSONObject)obj.get("data");
            // String code = (String)obj.get("code");
            // if(code.equals("0")) {  
            //     resout = obj2.get("country")+"--" +obj2.get("area")+"--" +obj2.get("city")+"--"+obj2.get("isp");
            // } else {
            //     resout =  "IP地址有误";
            // }
        } catch(Exception e) { 
            e.printStackTrace();
            resout = "获取IP地址异常：" + e.getMessage();
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

    private static String decodeUnicode(String theString) {
        char aChar;
        int len = theString.length();
        StringBuffer outBuffer = new StringBuffer(len);
        for (int x = 0; x < len;) {
            aChar = theString.charAt(x++);
            if (aChar == '\\') {
                aChar = theString.charAt(x++);
                if (aChar == 'u') {
                    // Read the xxxx
                    int value = 0;
                    for (int i = 0; i < 4; i++) {
                        aChar = theString.charAt(x++);
                        switch (aChar) {
                        case '0':
                        case '1':
                        case '2':
                        case '3':
                        case '4':
                        case '5':
                        case '6':
                        case '7':
                        case '8':
                        case '9':
                            value = (value << 4) + aChar - '0';
                            break;
                        case 'a':
                        case 'b':
                        case 'c':
                        case 'd':
                        case 'e':
                        case 'f':
                            value = (value << 4) + 10 + aChar - 'a';
                            break;
                        case 'A':
                        case 'B':
                        case 'C':
                        case 'D':
                        case 'E':
                        case 'F':
                            value = (value << 4) + 10 + aChar - 'A';
                            break;
                        default:
                            throw new IllegalArgumentException(
                                    "Malformed   \\uxxxx   encoding.");
                        }

                    }
                    outBuffer.append((char) value);
                } else {
                    if (aChar == 't')
                        aChar = '\t';
                    else if (aChar == 'r')
                        aChar = '\r';
                    else if (aChar == 'n')
                        aChar = '\n';
                    else if (aChar == 'f')
                        aChar = '\f';
                    outBuffer.append(aChar);
                }
            } else
                outBuffer.append(aChar);
        }
        return outBuffer.toString();
    }

    public static void main(String[] args) {
        String out_ip = getV4IP();
        System.out.println(out_ip);

        String infojson = GetAddressByIp.GetAddressByIp(out_ip);
        // try {
        //     System.out.println(new String(infojson.getBytes("utf-8"), "gbk"));
        // } catch (IOException e) {
        //     e.printStackTrace();
        // }
        
        // System.out.println(new String(infojson.getBytes(), "utf-8"));
        System.out.println(decodeUnicode(infojson));
    }
}

