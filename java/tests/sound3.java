
import java.awt.event.*;
import javax.swing.*;
import javax.swing.plaf.basic.*;
import javax.swing.event.*;
import java.awt.*;
import java.io.*;
import java.util.*;
import javax.sound.sampled.*;


class sound3
{
	static int W = 500, H = 400, SLIDER_MAX = 1000;
	static MyCanvas canvas = new MyCanvas();

	static class MyCanvas extends JPanel 
	{
		int slider = SLIDER_MAX / 2, samples = 100, selX = 0, selW = 0, sel = 0;

		public MyCanvas() {
			addMouseListener( new MouseAdapter() {
				public void mousePressed( MouseEvent e ) {
					if( e.getButton() == e.BUTTON3 ) {
						p("Clear Selection");
						selX = selW = 0; 
						repaint();
						return;
					}
					Point p = e.getPoint();
					p( "Pressed at " + p );
					selX = p.x; selW = 0;
				}
				public void mouseReleased( MouseEvent e ) {
				}
			});
			addMouseMotionListener( new MouseAdapter() {
				public void mouseDragged( MouseEvent e ) {
					Point p = e.getPoint();
					selW = p.x - selX;
					repaint(); 
				}
			});
		}
		public void paintComponent( Graphics _g ) {
			super.paintComponent ( _g );

			Graphics2D g = ( Graphics2D ) _g;

			int W = getWidth(), H = getHeight();

			g.setColor( Color.yellow );
			g.fillRect( selX, 0, selW, H - 1 );
			g.setColor( Color.black );
			g.drawLine( 0, H >> 1, W-1, H >> 1 );

			byte[] buffer = bytes.toByteArray();
			int buflen = buffer.length;
			int bufpos = (int)( (double) slider / SLIDER_MAX * buflen );
			//p( "buflen: " + buflen + ", bufpos: " + bufpos );

			for( int x = 0; x < W; x++ ) {
				int offset = (int)( (double) x / W * samples );
				if( (bufpos + offset) < buflen ) {
					int sample = (int) buffer[ bufpos + offset ];
					g.fillRect( x, (H >> 1) + sample, 1, 1 );
				}
			}

		}
		public void slider( int value ) {
			slider = value;
			repaint();
		}
		public void samples( int value ) {
			p( "Samples " + value );
			samples = value > 0 ? value : bytes.size();
			repaint();
		}
		public void startRecord() {
			p("\nStart RECORD");
			bytes.reset();
			record = true;
				( new Thread() {

					public void run() {
						p("Thread started");

						DataLine.Info lineInfo = new DataLine.Info( TargetDataLine.class, format );
						p( "LineInfo: " + lineInfo );

						TargetDataLine line;
						try {
							line = (TargetDataLine) AudioSystem.getLine( lineInfo );

							byte[] buffer = new byte[ line.getBufferSize() / 5 ];

							p( "Buffer size: " + line.getBufferSize() + ", byte[] = " + buffer.length );

							line.open( format );
							line.start();

							while( record ) {
								int read = line.read( buffer, 0, buffer.length );
								p( "Read: " + read );
								bytes.write( buffer );
							}
							line.flush();
							line.stop();
							line.close();
							p( "Thread stopped.\nSize of data: " + bytes.size() );
							repaint();
							PrintWriter file = new PrintWriter("sound3.txt");
							byte[] buf = bytes.toByteArray();
							for( int i = 0; i < buf.length; i++)
								file.println( (int) buf[i] );
							file.close();

						} catch ( Exception e ) { p (e); }
					}
				}).start();

		}
		public void stopRecord() {
				p("Stop RECORD\n");
				record = false;
		}
		public void stopPlay() {
				p("\nSTOP PLAY");
				play = false;
		}
		public void startPlay() {
				p("\nSTART PLAY");
				play = true;

				( new Thread() {

					public void run() {
						p("Thread started");

						p( "LineInfo: " + lineInfoPlay );

						
						try {
							if(clipLine==null) clipLine = (Clip) AudioSystem.getLine( lineInfoPlay );

							byte[] buffer = bytes.toByteArray();
							p( "Line BefferSize: " + clipLine.getBufferSize() );
							p( "Bytes size: " + buffer.length );


							int offset = 0, length = buffer.length;
							if( selW > 0 ) {
								int bufpos = (int)( (double) slider / SLIDER_MAX * buffer.length );
								offset = bufpos + (int)( (double) selX / W * samples );
								length = (int)( (double) selW / W * samples );
								byte[] buffer2 = new byte[ length ];
								for( int i=0; i<length; i++)
									buffer2[i] = buffer[ offset + i ];
								buffer = buffer2;
							}
							p( "BufSize: " + buffer.length + ", Offset: " + offset + ", length: " + length + ", selX: " + selX + ", selW: " + selW );
							//clipLine.open( format, buffer, offset, length );
							clipLine.open( format, buffer, 0, buffer.length );
							//clipLine.setLoopPoints( offset, offset + length  - 1);

							while( play ) {
								//line.loop(1);
								clipLine.start();
								clipLine.drain();
								clipLine.setFramePosition(0);
							}
							//line.drain();
							clipLine.stop();
							clipLine.close();

						} catch ( Exception e ) { p (e); }
					}

				}).start();

		}

		boolean record = false, play = false;
		ByteArrayOutputStream bytes = new ByteArrayOutputStream();
		AudioFormat format = new AudioFormat( 22050, 8, 1, true, false );
		DataLine.Info lineInfoPlay = new DataLine.Info( Clip.class, format );
		Clip clipLine;
		static int start = 0;


		public void find() {
			p("FIND, start = " + start);

			byte _buf[] = bytes.toByteArray();

			if( start == 0 )
			for( int i = 0; i < _buf.length; i++ ) {
				if( Math.abs( _buf[ i ] ) > 10 ) {
					start = i;
					break;
				};
			}
			if( start < 10 ) start = 10;
			p("search... from " + start);

			ArrayList buf1 = new ArrayList();
			ArrayList buf2 = new ArrayList();

			for( int i = start; i < _buf.length - 10; i++ ) {

				int dif1 = 0; 
				int dif2 = 0; 
				for( int t = -9; t <= 0; t++ ) dif1 += Math.signum( _buf[ i + t ] - _buf[ i + t - 1 ] );
				for( int t = 0; t < 10; t++ ) dif2 += Math.signum( _buf[ i + t + 1 ] - _buf[ i + t ] );

				p( "dif1: " + dif1 );
				p( "dif2: " + dif2 );

				//try { Thread.sleep(100); } catch ( Exception e ){ p(e); }

				if( Math.abs( dif1 - dif2 ) > 10 ) {
					//for( int t = -10; t <= 10; i++ ) p( t + ": " + _buf[ i + t ] );
					start = i + 10;
					slider = (int)( (double)  SLIDER_MAX / _buf.length * i / W );
					selX = i - (int) ( (double) _buf.length / SLIDER_MAX * slider );
					selX = selX < 0 ? 0 : selX;
					selW = 10;
					myslider.setValue( slider );
					p( "\nnew start: " + i + ", slider: " + slider );
					break;
				}
			}
			repaint();

		}
	}

	static class button extends JButton
	{
		public button( String label ) { super( label ); setUI( new BasicToggleButtonUI() {} ); }
	}

	static JSlider myslider;

	public static void main( String args[] ) {

			JFrame frame1 = new JFrame ( "Sound 3" );
			frame1.setSize( W, H );
			frame1.setLocation( 300, 300 );
			frame1.setDefaultCloseOperation( WindowConstants.EXIT_ON_CLOSE );

			frame1.add( BorderLayout.CENTER, canvas );

			myslider = new JSlider( 0, SLIDER_MAX );
			myslider.setPaintTicks(true);
			myslider.setMajorTickSpacing( 100 );
			myslider.setMinorTickSpacing( 50 );
			myslider.addChangeListener( new ChangeListener() {
				public void stateChanged( ChangeEvent e ) {
					JSlider o = ( JSlider ) e.getSource();
					canvas.slider( o.getValue() );
				}
			} );

			frame1.add( BorderLayout.NORTH, myslider );

			JPanel buttons = new JPanel();

			JButton button = new button( "Запись!" );
			button.addMouseListener( new MouseAdapter() {
				Color bk;
				public void mousePressed( MouseEvent e ) 
				{ 
					JComponent o = (JComponent) e.getSource();
					bk = o.getBackground();
					o.setBackground( Color.orange );
					canvas.startRecord();
				}
				public void mouseReleased( MouseEvent e ) 
				{ 
					JComponent o = (JComponent) e.getSource();
					o.setBackground( bk ); 
					canvas.stopRecord();
				}
			});

			buttons.add( button );

			button = new button( "Проиграть!" );
			button.addMouseListener( new MouseAdapter() {
				public void mousePressed( MouseEvent e ) 
				{ 
					canvas.startPlay();
				}
				public void mouseReleased( MouseEvent e ) 
				{ 
					canvas.stopPlay();
				}
			});

			buttons.add( button );

			button = new button( "Искать" );
			button.addMouseListener( new MouseAdapter() {
				public void mouseReleased( MouseEvent e ) 
				{ 
					canvas.find();
				}
			});

			buttons.add( button );


			JComboBox combobox = new JComboBox( new Object[] { "100", "200", "400", "800", "1000", "1500", "2000", "4000", "-1" } );
			combobox.addActionListener( new ActionListener() {
				public void actionPerformed( ActionEvent e ) {
					JComboBox o = (JComboBox) e.getSource();
					canvas.samples( Integer.parseInt( (String) o.getSelectedItem() ) );
				}
			});
			buttons.add( combobox );

			frame1.add( BorderLayout.SOUTH, buttons );

			frame1.addComponentListener( new ComponentAdapter() {
				public void componentResized(ComponentEvent e){
					Component o = (Component) e.getSource();
					W = o.getWidth(); H = o.getHeight();
					p( "New width = " + W + ", height = " + H );
				}
			});

			frame1.setVisible( true );
			frame1.repaint();
	}

	public static void p( Object args ) { System.out.println( args ); }

}




