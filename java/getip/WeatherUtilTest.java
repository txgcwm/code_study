import java.io.UnsupportedEncodingException;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLEncoder;
import java.util.Map;


// http://blog.csdn.net/l_ch_g/article/details/8205817

public class WeatherUtilTest {
	public static void main(String[] args) {
		String link = "http://php.weather.sina.com.cn/xml.php?city=%D6%D8%C7%EC&password=DJOYnieT8234jlsK&day=0";
		
		try {
			System.out.println(URLEncoder.encode("重庆", "GBK"));
		} catch (UnsupportedEncodingException e1) {
			e1.printStackTrace();
		}
		
		try {
			URL url = new URL(link);
			WeatherUtil parser = new WeatherUtil(url);
			String[] nodes = {"city", "status1", "temperature1", "status2", "temperature2"};
			Map<String, String> map = parser.getValue(nodes);
			System.out.println(map.get(nodes[0]) + " 今天白天：" + map.get(nodes[1]) + " 最高温度：" + map.get(nodes[2])
								+ "℃ 今天夜间：" + map.get(nodes[3]) + " 最低温度：" + map.get(nodes[4]) + "℃ ");
		} catch (MalformedURLException e) {
			e.printStackTrace();
		}
	}
}
