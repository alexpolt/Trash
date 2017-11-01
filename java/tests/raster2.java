
import java.io.*;
import java.awt.*;
import java.util.*;

import java.awt.geom.*;
import java.awt.event.*;
import java.awt.image.*;

import javax.swing.*;
import javax.imageio.*;


public class raster2
{
	static int w = 500, h = 500, w2 = w>>1, h2 = h>>1;
	static BufferedImage img1;
	static int[] imgdata;
	static JFrame frame1;
	static Graphics2D g;
	static boolean run = true;

	public static void main(String[] args) {
		p("Start!");
		img1 = new BufferedImage( w, h, BufferedImage.TYPE_INT_RGB );

		float[] kernData = { 0, -1, 0,	-1, 4, -1,	0, -1, 0 };
		Kernel img1Kern = new Kernel( 3, 3, kernData );
		BufferedImageOp img1Op=null;// = new ConvolveOp( img1Kern );

		frame1 = createFrame( "Raster2", 0, 0, w, h, img1, img1Op );
		frame1.setDefaultCloseOperation( WindowConstants.EXIT_ON_CLOSE );
		frame1.addMouseMotionListener( new MouseAdapter( ) {
			public void mouseMoved( MouseEvent e ) {
				Point p = e.getPoint();  p.x = p.x; p.y = p.y;
				g.setPaint( new GradientPaint( p, Color.white, new Point( p.x+60, p.y ), Color.red, true ) );
				g.fill( new Ellipse2D.Double( p.x-30, p.y-30, 50, 50 ) );
			}
		}
		);

		g = (Graphics2D) img1.createGraphics();

		imgdata = ( (DataBufferInt) img1.getRaster().getDataBuffer() ).getData();
		
		( new Thread() {
			public void run() {
				int TIME = 0;
				while( run ) {
					if( false && (TIME % 2) == 0 ) {
						int inc = TIME % 200;
						g.setPaint( new GradientPaint( new Point( 50+inc, 50+inc ), Color.white, new Point( 140+inc, 140+inc ), Color.red ) );
						g.fill( new Ellipse2D.Double( 50+inc, 50+inc, 100, 100 ) );
					}

					for( int y = 1; y < h-1; y++ )
					for( int x = 1; x < w-1; x++ )
					{
						imgdata[ off(x,y) ] = imgdata[ off(x,y) ] & 0xFFF8F8F8;
					}

					for( int y = 1; y < h-1; y++ )
					for( int x = 1; x < w-1; x++ )
					{
						for( int band = 0; band < 24; band+=8 )
						imgdata[ off(x,y) ] = 
							imgdata[ off(x,y) ] & ~( 0xFF << band )  |  
							(
								( (imgdata[ off(x-1,y-1) ] >> band & 0xFF) + (imgdata[ off(x+1,y-1) ] >> band & 0xFF) + 
									  (imgdata[ off(x+1,y+1) ] >> band & 0xFF) + (imgdata[ off(x-1,y+1) ] >> band & 0xFF) 
								) >> 2 & 0xFF 
							) << band;

					}
				frame1.repaint();
				TIME++;
				try { sleep(50); } catch ( Exception e ){ p(e); }
				}
			}
		}).start();

		//g.dispose();

		p("Finish!");
	} // main

	public static void p( Object arg1 ) { System.out.println( arg1 ); }

	public static int off( int x, int y ) { return w * y + x; }

	static class JImgPanel extends JPanel {
		BufferedImage img;
		BufferedImageOp op;
		public JImgPanel( BufferedImage img, BufferedImageOp op ) {
			this.img = img;
			this.op = op;
		}
		public void paintComponent( Graphics _g ){
			Graphics2D g = (Graphics2D) _g;
			g.drawImage( img, op!=null?op:null, 0, 0 );
			g.setXORMode( Color.white );
			g.setStroke( new BasicStroke( 2 ) );
			int x = img.getWidth(), y = img.getHeight();
			int hx = x >> 1, hy = y >> 1;
			Path2D path = new Path2D.Double();
			path.moveTo( hx, 0); path.lineTo( hx, y-1);
			path.moveTo( 0, hy); path.lineTo( x-1, hy);
			g.draw( path );
			g.setPaintMode();
		}
	}

	public static JFrame createFrame( String title, int x, int y, int w, int h, BufferedImage img, BufferedImageOp op ) {
			JFrame frame1 = new JFrame( title );
			
			JImgPanel panel1 = new JImgPanel( img, op );
			panel1.setSize( w, h );
			panel1.setPreferredSize( new Dimension( w, h ) );
			frame1.add( panel1, BorderLayout.CENTER );

			frame1.setLocation( x, y );
			frame1.setVisible( true );
			frame1.pack();

			return frame1;
	}


} // rast1

