import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.FileInputStream;
import java.net.URL;
import java.util.HashMap;
import java.util.Map;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

/**
 * 解析xml文档，包括本地文档和url
 *
 */
public class WeatherUtil {
	
	InputStream inStream;
	Element root;

	public InputStream getInStream() {
		return inStream;
	}

	public void setInStream(InputStream inStream) {
		this.inStream = inStream;
	}

	public Element getRoot() {
		return root;
	}

	public void setRoot(Element root) {
		this.root = root;
	}

	public WeatherUtil() {
	}

	/**
	 * 通过输入流来获取新浪接口信息
	 * @param inStream
	 */
	public WeatherUtil(InputStream inStream) {
		
		if (inStream != null) {
			this.inStream = inStream;
			DocumentBuilderFactory domfac = DocumentBuilderFactory.newInstance();
			
			try {
				DocumentBuilder domBuilder = domfac.newDocumentBuilder();
				Document doc = domBuilder.parse(inStream);
				
				root = doc.getDocumentElement();
			} catch (ParserConfigurationException e) {
				e.printStackTrace();
			} catch (SAXException e) {
				e.printStackTrace();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

	public WeatherUtil(String path) {
		InputStream inStream = null;
		
		try {
			inStream = new FileInputStream(path);
		} catch (FileNotFoundException e1) {
			e1.printStackTrace();
		}
		
		if (inStream != null) {
			this.inStream = inStream;
			DocumentBuilderFactory domfac = DocumentBuilderFactory.newInstance();
			
			try {
				DocumentBuilder domBuilder = domfac.newDocumentBuilder();
				Document doc = domBuilder.parse(inStream);
				
				root = doc.getDocumentElement();
			} catch (ParserConfigurationException e) {
				e.printStackTrace();
			} catch (SAXException e) {
				e.printStackTrace();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

	public WeatherUtil(URL url) {
		InputStream inStream = null;
		
		try {
			inStream = url.openStream();
		} catch (IOException e1) {
			e1.printStackTrace();
		}
		
		if (inStream != null) {
			this.inStream = inStream;
			DocumentBuilderFactory domfac = DocumentBuilderFactory.newInstance();
			
			try {
				DocumentBuilder domBuilder = domfac.newDocumentBuilder();
				Document doc = domBuilder.parse(inStream);
				
				root = doc.getDocumentElement();
			} catch (ParserConfigurationException e) {
				e.printStackTrace();
			} catch (SAXException e) {
				e.printStackTrace();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

	/**
	 * 
	 * @param nodes
	 * @return 单个节点多个值以分号分隔
	 */
	public Map<String, String> getValue(String[] nodes) {
		if (inStream == null || root==null) {
			return null;
		}
		
		Map<String, String> map = new HashMap<String, String>();
		
		// 初始化每个节点的值为null
		for (int i = 0; i < nodes.length; i++) {
			map.put(nodes[i], null);
		}

		// 遍历第一节点
		NodeList topNodes = root.getChildNodes();
		if (topNodes == null) {
			return null;
		}
		
		for (int i = 0; i < topNodes.getLength(); i++) {		
			Node book = topNodes.item(i);
			
			if (book.getNodeType() == Node.ELEMENT_NODE) {
				for (int j = 0; j < nodes.length; j++) {
					for (Node node = book.getFirstChild();
							node != null; node = node.getNextSibling()) {
						
						if (node.getNodeType() == Node.ELEMENT_NODE) {	
							if (node.getNodeName().equals(nodes[j])) {
								String val = node.getTextContent();
								String temp = map.get(nodes[j]);
								
								if (temp != null && !temp.equals("")) {
									temp = temp + ";" + val;
								} else {
									temp = val;
								}
								
								map.put(nodes[j], temp);
							}
						}
					}
				}
			}
		}
		
		return map;
	}
}

