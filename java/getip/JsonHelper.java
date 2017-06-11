import java.lang.reflect.Method;
import java.text.ParseException;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import org.json.JSONException;
import org.json.JSONObject;

/**
 * 
 * 1:将JavaBean转换成Map、JSONObject 
 * 2:将Map转换成Javabean 
 * 3:将JSONObject转换成Map、Javabean
 **/
public class JsonHelper {
	public static Map<String, String> toMap(Object javaBean) {
        Map<String, String> result = new HashMap<String, String>();
        Method[] methods = javaBean.getClass().getDeclaredMethods();

        for (Method method : methods) {
            try {
                if (method.getName().startsWith("get")) {
                    String field = method.getName();
                    field = field.substring(field.indexOf("get") + 3);
                    field = field.toLowerCase().charAt(0) + field.substring(1);

                    Object value = method.invoke(javaBean, (Object[]) null);
                    result.put(field, null == value ? "" : value.toString());
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        return result;
    }

	public static Map<String, String> toMap(String jsonString) throws JSONException {
		JSONObject jsonObject = new JSONObject(jsonString);
		Map<String, String> result = new HashMap<String, String>();
		Iterator<?> iterator = jsonObject.keys();
		String key = null;
		String value = null;

		while (iterator.hasNext()) {
			key = (String) iterator.next();
			value = jsonObject.getString(key);
			result.put(key, value);
		}
		
		return result;
	}

	public static JSONObject toJSON(Object bean) {
		return new JSONObject(toMap(bean));
	}

	public static Object toJavaBean(Object javabean, Map<String, String> data) {
        Method[] methods = javabean.getClass().getDeclaredMethods();
        
        for (Method method : methods) {
            try {
                if (method.getName().startsWith("set")) {
                    String field = method.getName();
                    field = field.substring(field.indexOf("set") + 3);
                    field = field.toLowerCase().charAt(0) + field.substring(1);
                    method.invoke(javabean, new Object[] {
                    		data.get(field)
                    });
                }
            } catch (Exception e) {
            }
        }

        return javabean;
    }

	public static void toJavaBean(Object javabean, String jsonString)
			throws ParseException, JSONException {
		JSONObject jsonObject = new JSONObject(jsonString);
		Map<String, String> map = toMap(jsonObject.toString());
		toJavaBean(javabean, map);
	}
}

