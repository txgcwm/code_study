import java.io.UnsupportedEncodingException;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLEncoder;
import java.text.ParseException;
import java.util.Map;

import org.json.JSONException;
import org.json.JSONObject;


public class AttributionQueryTest {
	public static void main(String[] args) throws JSONException, ParseException {
        String out_ip = AttributionQuery.getPublicNetworkIP();
        System.out.println(out_ip);

        String jsonString = AttributionQuery.GetAddressByIp(out_ip);
        
        System.out.println(jsonString);

        JSONObject jsonObj = new JSONObject(jsonString); 
        JSONObject dataInfo = jsonObj.getJSONObject("data");
        String city = dataInfo.getString("city"); 
        
        System.out.println("name: " + city);
       
		try {
			String link = "http://php.weather.sina.com.cn/xml.php?city=" + URLEncoder.encode(city.substring(0, city.length() - 1), "GBK")
							+ "&password=DJOYnieT8234jlsK&day=0";
			URL url = new URL(link);
			WeatherUtil parser = new WeatherUtil(url);
			String[] nodes = {"city", "status1", "temperature1", "status2", "temperature2"};
			Map<String, String> map = parser.getValue(nodes);
			System.out.println(map.get(nodes[0])+" 今天白天："+map.get(nodes[1])+" 最高温度："+map.get(nodes[2])+"℃ 今天夜间："
										+map.get(nodes[3])+" 最低温度："+map.get(nodes[4])+"℃ ");
		} catch (UnsupportedEncodingException e1) {
			e1.printStackTrace();
		} catch (MalformedURLException e) {
			e.printStackTrace();
		}
    }
}
