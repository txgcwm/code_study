import org.apache.commons.codec.binary.Base64;
import org.apache.commons.codec.binary.StringUtils;

import javax.crypto.Mac;
import javax.crypto.spec.SecretKeySpec;
import java.security.NoSuchAlgorithmException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;
import java.util.UUID;


/**
 * HMAC-SHA256 test
 */
public class HMACSHA256UtilTest {
	private static Mac hmacSHA256 = Mac.getInstance("HmacSHA256");

 
   /**
    * hmac-sha256 加密
    * @param signingKey
    * @param stringToSign
    * @return
    */
   public String hmacSHA256Encode(String signingKey, String stringToSign) {
      String hash = null;
      try {
         SecretKeySpec secret_key = new SecretKeySpec(stringToSign.getBytes(), "HmacSHA256");
         hmacSHA256.init(secret_key);
         hash = Base64.encodeBase64String(hmacSHA256.doFinal(signingKey.getBytes()));
      } catch (Exception e) {
      }
      
      return hash;
   }
   
   public static void main(String[] args) {
      //secret
      String secret = "ZTaBVtwj8tpSOhExEi3D";
      //params
      Map<String, String> params = new HashMap<>();
      params.put("productKey", "80beb199-dcf");
      params.put("unifiedId", "18681452232@closeli.phone.cn1467410034550");
      params.put("deviceId", "xxxxS_54c9df03346c");
      //guid=时间戳+三位自增码
      String guid = System.currentTimeMillis() + UUID.randomUUID().toString().substring(0, 3);//模拟
      //message处理
      List<String> list = new ArrayList<>();
      list.addAll(params.keySet());
      Collections.sort(list, String.CASE_INSENSITIVE_ORDER);
      Iterator<String> it = list.iterator();
      StringBuilder sb = new StringBuilder();
      while (it.hasNext()) {
         String key = it.next();
         String value = params.get(key);
 
         sb.append("&").append(key).append("=").append(value);
      }
      String message = sb.toString();
      if (StringUtils.isNotBlank(message)) {
         message = message.substring(1);
      }
      //HMAC-SHA256加密
      String signingKey = hmacSHA256Encode(message, guid);
      //secret base64编码
      String stringToSign = Base64.encodeBase64String(secret.getBytes());
      //生成signature
      String signature = hmacSHA256Encode(signingKey, stringToSign);
   }
}


