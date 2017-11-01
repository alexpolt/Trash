
import javax.xml.parsers.SAXParserFactory;
import javax.xml.parsers.ParserConfigurationException;
import org.xml.sax.XMLReader;
import org.xml.sax.ContentHandler;
import org.xml.sax.Attributes;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.XMLFilterImpl;

import java.io.*;

public class sax1 {

	public static void main(String args[]) {
		System.out.println("Heelo");
		SAXParserFactory f1 = SAXParserFactory.newInstance();
		f1.setNamespaceAware( true );
		try {
			XMLReader reader1 = f1.newSAXParser().getXMLReader();
			reader1.setContentHandler( new MyXMLHandler() );
			reader1.parse( new InputSource( new FileInputStream("sax1.xml") ) );
		} catch ( Exception e ) {
			e.printStackTrace();
		}
		System.out.println("Finish");
	}

	static class MyXMLHandler extends XMLFilterImpl {
		public void startElement( String ns, String localName, String globalName, Attributes attr ) {
			System.out.println( "Element: " + localName + ", global name: " + globalName );
			System.out.println( attr );
		}
		public void characters( char data[], int offset, int length ) {
			System.out.println( new String( data, offset, length) );
		}
	}

}

