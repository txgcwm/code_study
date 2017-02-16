import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;


public class Log4jTest {

	public static void main(String[] args) {		
//		Logger logger = LogManager.getLogger(Log4jTest.class.getName());
		Logger logger = LogManager.getLogger();
		
		logger.trace("tarce");
		logger.debug("debug");
		logger.info("info");
		logger.warn("warn");
		logger.error("error");
		logger.fatal("fatal");
	}
}

// http://www.cnblogs.com/elaron/archive/2013/01/15/2861785.html
// http://blog.csdn.net/memray/article/details/17488433