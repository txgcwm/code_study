import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.util.Arrays;
//import org.slf4j.Logger;
//import org.slf4j.LoggerFactory;
import org.apache.commons.codec.digest.HmacUtils;
import org.apache.commons.codec.digest.DigestUtils;


public class HmacSha256 {

	/**
	 * @param args
	 * @throws UnsupportedEncodingException 
	 */
	public static void main(String[] args) throws UnsupportedEncodingException {
		String token="123456";
		String message="hello";
		String yymmdd = "20170616";//DateUtil.getYYYYMMDD();

		//原始参数基本sha
		byte[] messageByte = DigestUtils.sha256(message);
		System.out.println(Arrays.toString(messageByte));
		
		//计算签名key
		//1，token加盐hmac开始，yyyymmdd
		byte[] kDate = HmacUtils.hmacSha256(token, yymmdd);
		System.out.println(Arrays.toString(kDate));
		
		//2,kDate加盐1 hmac
		byte[] kRegion = HmacUtils.hmacSha256(kDate, "china".getBytes("utf-8"));
		System.out.println(Arrays.toString(kRegion));
		
		//2,kRegion加盐2 xmpp
		byte[] kService = HmacUtils.hmacSha256(kRegion, "xmpp".getBytes("utf-8"));
		System.out.println(Arrays.toString(kService));
		
		//3,kService加盐3 closeli_request
		byte[] signing_key = HmacUtils.hmacSha256(kService, "closeli_request".getBytes("utf-8"));
		System.out.println(Arrays.toString(signing_key));
		
		//计算最后签名，signing_key 和 原始参数基本sha的值计算
		String sign_resultFF = HmacUtils.hmacSha256Hex(signing_key,messageByte);
		System.out.println(sign_resultFF);
	}
}



 
