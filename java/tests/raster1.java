
import java.io.*;
import java.awt.*;
import java.util.*;

import java.awt.geom.*;
import java.awt.event.*;
import java.awt.image.*;

import javax.swing.*;
import javax.imageio.*;


public class raster1
{

	static double[] param = { 1,0,0, 0.5 }; 
	static double TIME = 0;
	static boolean flagRun = true;

	public static void main(String[] args) {

		p("Start!");

		try
		{

				BufferedImage img1; 
				img1 = ImageIO.read( new File("nebula.jpg") );

				JFrame frame1 = createFrame("Старое изображение", 50, 50, img1.getWidth(), img1.getHeight(), img1 );
				//Graphics2D g = (Graphics2D) frame1.getGraphics();
				//g.drawImage( img1, null, 0, 0 );
				//g.dispose();

				JTextField edit1 = new JTextField();
				edit1.addKeyListener( new KeyAdapter() {
					public void keyPressed( KeyEvent e ) {
						if( e.getKeyCode() == KeyEvent.VK_ESCAPE ) {
							flagRun = false;
						}
						if( e.getKeyCode() == KeyEvent.VK_ENTER ) {
							String txt = ((JTextField) e.getSource() ).getText();
							p( "TEXT: " + txt );
							String[] params = txt.split(",");
							int i = 0;
							for(String _param: params){
								p( "PARAM: " + new Double( _param ) );
								param[i++] = new Double( _param );
							}
							try
							{
								process();
							}
							catch ( Exception exc ) { p(exc); }
						}
					}
				});
				edit1.setText( param[0]+","+param[1]+","+param[2]+","+param[3] );
				frame1.add( BorderLayout.SOUTH, edit1 );
				frame1.setDefaultCloseOperation( WindowConstants.EXIT_ON_CLOSE );
				frame1.pack();
				frame1.repaint();
				process();
		
		}
		catch ( Exception e ) { p(e); }

		p("Finish!");

	} // main

	public static void p( Object arg1 ) { System.out.println( arg1 ); }

	static class JImgPanel extends JPanel {
		BufferedImage img;
		public JImgPanel( BufferedImage img ) {
			this.img = img;
		}
		public void paintComponent( Graphics _g ){
			Graphics2D g = (Graphics2D) _g;
			g.drawImage( img, null, 0, 0 );
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

	public static JFrame createFrame( String title, int x, int y, int w, int h, BufferedImage img ) {
			JFrame frame1 = new JFrame( title );
			
			JImgPanel panel1 = new JImgPanel( img );
			panel1.setSize( w, h );
			panel1.setPreferredSize( new Dimension( w, h ) );
			frame1.add( panel1, BorderLayout.CENTER );

			frame1.setLocation( x, y );
			frame1.setVisible( true );
			frame1.pack();
			return frame1;
	}

	public static void process( ) throws Exception {

		BufferedImage img1 = ImageIO.read( new File("nebula.jpg") );
		p( img1 );

		byte[] imgData = ( (DataBufferByte) img1.getRaster().getDataBuffer() ).getData();
		int w = img1.getWidth(), h = img1.getHeight();
		int halfH = h >> 1, halfW = w >> 1;

		p( "Img data length: " + imgData.length + ", width: " + w + ", height: " + h );

		int xy[][] = {  {-halfW, -halfH}, {w-halfW-1, h-halfH-1}, {-halfW, h-halfH-1}, {w-halfW-1, -halfH}  };
		double maxx=0, minx=0, maxy=0, miny=0;
		TIME = Math.PI * 0.5;
		for( int i=0; i < xy.length; i++ ) {
				int x = xy[i][0], y = xy[i][1];
				p( "X: "+x+", Y: " +y);

				double newX = getNewX( x, y ), newY = getNewY( x, y );

				p( "NewX: "+newX+", NewY: " +newY);

				if( maxx < newX ) maxx = newX;
				if( maxy < newY ) maxy = newY;
				if( minx > newX ) minx = newX;
				if( miny > newY ) miny = newY;

			}
		TIME=0;
		maxx+=2; maxy+=2;
		p( "\nmaxx: "+maxx+", maxy: "+maxy+"\nminx: "+minx+", miny: "+miny);

		int newWidth = (int) (maxx - minx), newHeight = (int)(maxy - miny);
		p("newW: "+newWidth+", newH: "+newHeight);

		BufferedImage imgNew = new BufferedImage( newWidth, newHeight, BufferedImage.TYPE_3BYTE_BGR );
		JFrame frame1 = new JFrame( "Новое изображение" );			
		frame1.setSize( imgNew.getWidth(), imgNew.getHeight() );
		frame1.setLocation( 100, 400 );
		//frame1.setBackground( Color.black );
		//frame1.setUndecorated(true);
		frame1.setIgnoreRepaint( true );

		int maxOffset = newWidth * newHeight * 3;
		p( imgNew );

		byte[] newImgData = ( (DataBufferByte) imgNew.getRaster().getDataBuffer() ).getData();

		( new Thread() {
			public void run() {
				double step = 2;
				while( flagRun ) {
					if( TIME > 200 ) { step=-step; };
					if( TIME < -200 ) { step=-step; };
					TIME+=step;
					try { sleep(10); } catch ( Exception e ){ p(e); }
				}
			}
		}).start();

		frame1.setVisible( true );
		frame1.createBufferStrategy(2);
		BufferStrategy bufstr = frame1.getBufferStrategy();
		//Graphics2D gimg = (Graphics2D) imgNew.getGraphics();

		//while( flagRun ){
		//gimg.clearRect( 0, 0,  newWidth, newHeight );

		for( int y = -halfH; y < h-halfH; y++ )
			for( int x = -halfW; x < w-halfW; x++ ) {
				double newX = getNewX( x, y ), newY = getNewY( x, y );
				if( newX > maxx || newX < minx ) continue;
				if( newY > maxy || newY < miny ) continue;
				int newBytePos = (int)( newY - miny ) * newWidth * 3 + (int)( newX - minx ) * 3 ;
				int oldBytePos = (int)( y + halfH ) * w * 3 + (int)( x + halfW ) * 3 ;
				if( newBytePos >= maxOffset  || newBytePos < 0 ) continue;
				try
				{
					newImgData[ newBytePos ] = imgData[ oldBytePos ];
					newImgData[ newBytePos + 1 ] = imgData[ oldBytePos + 1 ];
					newImgData[ newBytePos + 2 ] = imgData[ oldBytePos + 2 ];
				}
				catch ( Exception e ) {
					p( "X: "+(x + ( w>>1 ))+", Y: " +(y + (h>>1)) );
					p( "NewX: "+( newX - minx )+", NewY: " +( newY - miny ));
					p( "NewBytePos: "+newBytePos+", oldBytePos: "+oldBytePos);
					throw e;
				}
			}
			Graphics2D g = (Graphics2D) bufstr.getDrawGraphics();
			g.drawImage( imgNew, null, 0, 0 );
			g.dispose();
			if( ! bufstr.contentsLost() ) bufstr.show();
		//}
		//frame1.dispose();
		//gimg.dispose();
	}

	public static double getNewX( int origX, int origY ) {
		//double dist = Math.PI * Math.sqrt( origX*origX + origY*origY ) / (300 + TIME);
		//return Math.cos( dist ) * origX - Math.sin( dist ) * origY;
		return param[0] * origX + param[1] * origY;
	}
	public static double getNewY( int origX, int origY ) {
		//double dist = Math.PI * Math.sqrt( origX*origX + origY*origY ) / (300 + TIME);
		//return Math.cos( dist ) * origY + Math.sin( dist ) * origX;
		return param[2] * origX + param[3] * origY;
	}

} // rast1

