import java.awt.event.*;
import javax.swing.*;
import java.awt.*;
import javax.sound.sampled.*;


class sound2
{
	static boolean run = true;

	public static void main( String args[] ) {
		DataLine.Info lineInfo1 = new DataLine.Info( TargetDataLine.class, null );
		final byte[] sound = new byte[ 441000 ];

		try
		{
			//Port port1 = (Port) AudioSystem.getLine( Port.Info.MICROPHONE );
			//p(port1);
			//port1.open();

			TargetDataLine line1 = (TargetDataLine) AudioSystem.getLine( lineInfo1 );

			p( line1 );
			p( line1.getFormat() );
			p( line1.getBufferSize() );

			line1.open( line1.getFormat() );
			line1.start( );
			int read = line1.read( sound, 0, sound.length );
			p("read: " + read);
			line1.flush();
			line1.close();
	
			JFrame frame1 = new JFrame( "test"  ) {
				public void paint( Graphics g ) {
					p( "Repaint" );
					g.setColor( Color.black );
					for( int x = 0; x < 300; x ++ ){
						int offs = (int) ( (double) (110250 / 300) * x ) * 4;
						int val = sound[ offs ] | sound[ offs + 1 ] << 8;
						int y = (int) ( (double) val / 20000 * 150 );
						g.drawRect(x, 150, 0, 0);
						g.drawLine(x, 150, x, y + 150 );
					}
				}
			};
			frame1.setSize( 300, 300 );
			frame1.setDefaultCloseOperation( WindowConstants.EXIT_ON_CLOSE );
			frame1.setVisible( true );
			frame1.repaint();

			frame1.addMouseListener( new MouseAdapter() {
				public void mouseReleased (MouseEvent e) {
					byte[] sound2 = new byte[ 220500 ];
					for( int i=0; i< sound2.length; i+=4) {
						sound2[ i ] = sound [ i*2 ];
						sound2[ i+1 ] = sound [ i*2+1 ];
						sound2[ i+2 ] = sound [ i*2+2 ];
						sound2[ i+3 ] = sound [ i*2+3 ];
					}
		try
		{

					DataLine.Info lineInfo2 = new DataLine.Info( SourceDataLine.class, null );
					SourceDataLine line2 = (SourceDataLine) AudioSystem.getLine( lineInfo2 );
					p("");
					p( line2 );
					p( line2.getFormat() );
					p( line2.getBufferSize() );
					p("\nplay " + sound2.length);
					line2.open();
					line2.start();
					int read = line2.write( sound2, 0, sound2.length );
					p("written: " + read);
					line2.drain();
					line2.close();
		}catch ( Exception e2 )
		{
			p(e);
		}

				
				}
			});


		}
		catch ( Exception e )
		{
			p(e);
		}
	}

	public static void p( Object args ) { System.out.println( args ); }

}