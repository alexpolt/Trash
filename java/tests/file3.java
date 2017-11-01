import java.io.*;
import java.util.*;
import java.awt.event.*;
import java.awt.*;
import javax.swing.*;
import java.net.*;
import java.nio.*;
import java.nio.channels.*;

public class file3 implements KeyListener
{
	private static JTextArea text1;
	static public JFrame jframe1;

	public void keyPressed(KeyEvent e) { }
	public void keyReleased(KeyEvent e) {if(e.getKeyCode()==27 ) { save(); System.out.println("Bye!\n"); System.exit(0);} }
	public void keyTyped(KeyEvent e) { }

	public static void main(String args[]) {
		System.out.println("Hello World!");
		if( args.length < 1 ) {
			System.out.println("Need args.");
			System.exit(0);
		}
		jframe1 = new JFrame();
		jframe1.addWindowListener( new WindowAdapter() { public void windowClosing(WindowEvent e){ System.out.println("bye!\n"); System.exit(0);} } );
		jframe1.setSize(200,200);
		text1 = new JTextArea();
		jframe1.getContentPane().add( new JScrollPane(text1) );
		text1.addKeyListener( new file3() );
		jframe1.setVisible(true);

		try
		{
			InetSocketAddress ia1 = new InetSocketAddress("ya.ru", 80);
			SocketChannel sch1 = SocketChannel.open( ia1 );
			sch1.configureBlocking( false );
			sch1.write( ByteBuffer.wrap( "GET / HTTP/1.1\r\nHost: ya.ru\r\n\r\n".getBytes() ) );
			Selector sel1 = Selector.open();
			sch1.register( sel1, SelectionKey.OP_READ );
			System.out.println( "select..." );
			System.out.println( "Num=" + sel1.select() );
			Set set1 = sel1.selectedKeys();
			SelectionKey skey1 = (SelectionKey) set1.iterator().next();
			ByteBuffer buf1 = ByteBuffer.allocate( 1000 );
			((SocketChannel)skey1.channel()).read( buf1 );
			System.out.println( "Length: " + buf1.array().length );
			text1.setText( new String( buf1.array() ) );
			sch1.close();
			sel1.close();
		}
		catch ( IOException e)
		{
			System.out.println ( e );
		}
	}
	public static void save() {
		System.out.println ( "Saving...\n" );
	}
}

