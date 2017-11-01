import java.io.*;
import java.awt.event.*;
import java.awt.*;
import javax.swing.*;
import java.net.*;
import java.nio.*;
import java.nio.channels.*;

public class file2 implements KeyListener
{
	private static JTextArea text1;
	static public JFrame jframe1;

	public void keyPressed(KeyEvent e) { }
	public void keyReleased(KeyEvent e) {if(e.getKeyCode()==27 ) { save(); System.out.println("Bye!\n"); System.exit(0);} }
	public void keyTyped(KeyEvent e) { }

	public static MappedByteBuffer map1;

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
		text1.addKeyListener( new file2() );
		jframe1.setVisible(true);

		try
		{
			File file1 = new File("test.txt");
			if( ! file1.exists() )
						file1.createNewFile();
			int size = (int)file1.length();
			RandomAccessFile in1 = new RandomAccessFile( file1, "rw" );
			FileChannel ch1 = in1.getChannel();
			map1 = ch1.map( FileChannel.MapMode.READ_WRITE, 0, 1000 );
			ByteArrayOutputStream bout1 = new ByteArrayOutputStream();
			map1.limit( size );
			Channels.newChannel(bout1).write( map1 );
			System.out.println ( "Limit: " + map1.limit() + "\n" );
			text1.append( new String( bout1.toByteArray() ) );
			ch1.close();
			in1.close();
		}
		catch ( IOException e)
		{
			System.out.println ( e );
		}
	}
	public static void save() {
		System.out.println ( "Saving...\n" );
		String str1 = text1.getText();
		System.out.print ( str1.length() + "\n" );
		System.out.print ( str1 );
		map1.limit( (int)str1.length() );
		map1.put( str1.getBytes() );

	}
}

