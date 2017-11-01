
import java.io.*;
import java.awt.*;
import java.util.*;

import java.awt.geom.*;
import java.awt.event.*;
import java.awt.image.*;

import javax.swing.*;
import javax.imageio.*;

import com.sun.image.codec.jpeg.*;

import javax.media.*;
import javax.media.util.*;
import javax.media.control.*;
import javax.media.format.*;

public class swing2
{
	static JFrame frame1;
	static JPanel panelNorth;
	static swingdemo swingdemo1 = new swingdemo();
	static GraphicsEnvironment graphEnv;
	static GraphicsDevice graphDev;
	static GraphicsConfiguration graphConf;

	public static void main(String[] args) {

		frame1 = new JFrame("swingtest");

		frame1.add( swingdemo1, BorderLayout.CENTER );

		JPanel buttons = new JPanel();

		JButton button = new JButton("Рисовать!");
		button.addActionListener( new ActionListener() {
					public void actionPerformed( ActionEvent e ) {
						swingdemo1.action("nebula2.jpg");
					}
			} );
		buttons.add( button );

		button = new JButton("Весь экран");
		button.addActionListener( new ActionListener() {
					public void actionPerformed( ActionEvent e ) {
						if( graphDev.getFullScreenWindow() != null ) {
							graphDev.setFullScreenWindow( null );
							frame1.setResizable( true );
						} else {
									frame1.setResizable(false);
									graphDev.setFullScreenWindow( frame1 );
						}
					}
			} );
		buttons.add( button );

		final JComboBox combobox = new JComboBox( new Object[] { "nebula.jpg", "nebula2.jpg", "japanese_girl.gif" } );
		combobox.addActionListener( new ActionListener() {
					public void actionPerformed( ActionEvent e ) {
						System.out.println( e );
						swingdemo1.action( (String) combobox.getSelectedItem() );
					}
			} );
		buttons.add( combobox );

		frame1.add( BorderLayout.SOUTH, buttons );
		frame1.add( BorderLayout.NORTH, panelNorth = new JPanel() );

		frame1.setUndecorated( false );
		frame1.setResizable( true );
		frame1.setDefaultCloseOperation( WindowConstants.EXIT_ON_CLOSE );
		frame1.setSize(500,400);
		frame1.setLocation(500,300);
		frame1.setVisible(true);

		graphEnv = GraphicsEnvironment.getLocalGraphicsEnvironment();
		graphDev = graphEnv.getDefaultScreenDevice();
		graphConf = graphDev.getDefaultConfiguration();
/*
		frame1.createBufferStrategy(2);		
		try {
			frame1.createBufferStrategy(2, new BufferCapabilities( new ImageCapabilities(true), new ImageCapabilities(true), BufferCapabilities.FlipContents.BACKGROUND	) );
		} catch ( AWTException e ){ System.out.println(e); }
		//frame1.getRootPane().setDoubleBuffered( false );
		//RepaintManager.currentManager( frame1 ).setDoubleBufferingEnabled( false );
*/		
	}

	static private class swingdemo extends JPanel
	{
		BufferedImage img1;
		BufferedImageOp img1op;
		Point _prevMousePos;

		static MediaLocator ml = new MediaLocator("vfw://0");
		static Player play;
		static FrameGrabbingControl f;

		public swingdemo() {
			MouseAdapter mouseEvents = new MouseAdapter() {
				public void mousePressed( MouseEvent e ) {
					//System.out.println( e );
					Point p = _prevMousePos = e.getPoint();
					if( e.getButton() == MouseEvent.BUTTON3 ) {
						System.out.println( "\nExit!" );
						System.exit(0);
					}
					try
					{
						if( play == null ) {
							play = Manager.createRealizedPlayer( ml );
							System.out.println( ml );
						}
						play.start();
						System.out.println( play );

						if(f==null) {
							f = (FrameGrabbingControl) play.getControl("javax.media.control.FrameGrabbingControl");
							System.out.println( f );
							Thread.sleep(5000);
						}

						Buffer b = f.grabFrame();
						System.out.println( b );
						System.out.println( ((byte[])b.getData()).length );

						BufferToImage b2i = new BufferToImage( (VideoFormat) b.getFormat() );
						System.out.println( b2i );

						Image im = b2i.createImage( b );
						System.out.println( im );

						img1 = new BufferedImage( im.getWidth(frame1), im.getHeight(frame1), BufferedImage.TYPE_INT_RGB );
						System.out.println( img1 );

						img1.createGraphics().drawImage( im, null, null );
						System.out.println( img1 );

					}
					catch ( Exception ex ){System.out.println(ex);}
					repaint();
					return;

					/*
					ColorModel c = img1.getColorModel();
					PixelInterleavedSampleModel s = ( PixelInterleavedSampleModel )img1.getSampleModel();
					WritableRaster r = img1.getRaster();
					DataBufferByte b = (DataBufferByte) r.getDataBuffer();
					byte[] buf = b.getData();

					int col = img1.getRGB( p.x, p.y );
					System.out.printf( "COLOR: %x\n", col);

					System.out.printf( "Get sample: %x\n", s.getSample( p.x, p.y, 0, b )  );
					
					int[] f = s.getPixel( p.x, p.y, (int[]) null, b );
					if( f.length == 1)
						System.out.printf("Pixel Index: %d\n", f[0] );
					else 
						System.out.printf("Pixel: %d %d %d\n", f[0], f[1], f[2] );

					System.out.printf("Num data elem colormodel: %d\n", c.getNumComponents() );
					System.out.printf("Num data elem samplemodel: %d\n", s.getNumDataElements() );
					System.out.printf("Num pix stride: %d\n", s.getPixelStride() );
					System.out.printf("Num scan line stride: %d\n", s.getScanlineStride() );

					System.out.printf("Pixel Last: %x\n", c.getRGB( s.getDataElements( p.x, p.y, null, b ) )  );

					for( int i=0; i<img1.getHeight();i++) 
						for( int t=0; t<img1.getWidth() * s.getPixelStride(); t+=s.getPixelStride() ) 
							buf[ i * s.getScanlineStride() + t ] += 3;
					repaint();
					*/
				}
				public void mouseDragged( MouseEvent e ) {
					Point curLocation = frame1.getLocation();
					curLocation.translate( e.getPoint().x - _prevMousePos.x, e.getPoint().y - _prevMousePos.y );
					frame1.setLocation( curLocation );
				}
				public void mouseMoved( MouseEvent e ) {
					//Point p = e.getPoint();
					//panelNorth.setBackground( new Color( img1.getRGB( p.x, p.y ) )  );
					//panelNorth.repaint();
				}
			};
			addMouseListener( mouseEvents );
			addMouseMotionListener( mouseEvents );
			setBorder( BorderFactory.createLineBorder( Color.green ) );
			setDoubleBuffered( false );
			setOpaque(true);
			setBackground( Color.gray );
		}
		public void paintComponent( Graphics _g ) {
			super.paintComponent( _g );
			System.out.println( ">Component: " + _g.getClipBounds() );
			Graphics2D g = (Graphics2D) _g; 
			//Graphics2D g = (Graphics2D) frame1.getBufferStrategy().getDrawGraphics();
			if( img1 != null ) {
				g.drawImage( img1, img1op == null ? null : img1op, 0, 0 );
			}
			g.dispose();
			//frame1.getBufferStrategy().show();
		}
		public void action( String fileName ) {
			System.out.println( "\nAction!" );
			try
			{
				img1 = ImageIO.read( new File( "p:\\" + fileName ) );
				//System.out.println( img1 );

				this.setPreferredSize( new Dimension( img1.getWidth(), img1.getHeight() )  );
				frame1.pack();

				BufferedImage _img1 = new BufferedImage( img1.getWidth(), img1.getHeight(), BufferedImage.TYPE_INT_RGB );
				_img1.createGraphics().drawImage( img1, null, 0, 0 );
				img1 = _img1;

				float opKernel[] = {
					0.0f, -1.0f, 0.0f,
					-1.0f, 4.0f, -1.0f,
					0.0f, -1.0f, 0.0f
				};
				//img1op = new ConvolveOp( new Kernel( 3, 3, opKernel ) );
				//img1 = img1op.filter( img1, null  );
			}
			catch ( IOException e) { System.out.println( e ); }
			repaint();
		}


	} // swingdemo


} // main

