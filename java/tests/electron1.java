
import java.io.*;
import java.awt.*;
import java.util.*;

import java.awt.geom.*;
import java.awt.event.*;
import java.awt.image.*;

import javax.swing.*;
import javax.imageio.*;


public class electron1
{
	static int w = 400, h = 400;
	static boolean run = true;
	static double t = 0, u = 4, c = 8;
	static JFrame frame1;

	public static void main(String[] args) {

		p("Start!");

		frame1 = createFrame("Старое изображение", 50, 50, w, h );
		frame1.setDefaultCloseOperation( WindowConstants.EXIT_ON_CLOSE );
		Button b1 = new Button("Start");
		b1.addActionListener( new ActionListener() {
			public void actionPerformed( ActionEvent a ) {
				run = ! run;
			}
		} );
		frame1.add( b1, BorderLayout.SOUTH );

		try
		{
			(new Thread() {
				public void run() {
					while( run ) {
						t+=0.05;
						try { Thread.sleep( 100 ); } catch(Exception e){p(e);}
						frame1.repaint();
					}
				}
			}).run();
		}
		catch ( Exception e ) { p ( e ); }

		p("Finish!");

	} // main

	public static void p( Object arg1 ) { System.out.println( arg1 ); }

	static class JImgPanel extends JPanel {
		public JImgPanel( ) {
		}
		public void paintComponent( Graphics _g ){
			Graphics2D g = (Graphics2D) _g;
			g.setColor( Color.white );
			g.fillRect( 0, 0, w, h );
			g.setColor( Color.red );
			for( double a = 0; a < 2*Math.PI; a+=0.125 ) {
			double oldx=0, oldy=0;

			for( double i = 0; i < 150; i+=10 ) {
				double oX = Math.cos( a ) * i;
				double oY = Math.sin( a ) * i;

				double x = 200 + oX + u * ( t - Math.sqrt( i*i ) / c );
				double y = 200 + oY;
				g.setColor( new Color( (float)i/150, 0.0f, 0.0f) );
				if( oldx == 0 ) {oldx = x; oldy= y;}
				g.drawLine( (int) oldx, (int) oldy, (int) x, (int) y );

				oldx = x; oldy = y;

				if( x > w - 10 ) { t = 0; }

			}
			}
		}
	}

	public static JFrame createFrame( String title, int x, int y, int w, int h ) {
			JFrame frame1 = new JFrame( title );
			
			JImgPanel panel1 = new JImgPanel();
			//panel1.setSize( w, h );
			panel1.setPreferredSize( new Dimension( w, h ) );
			frame1.add( panel1, BorderLayout.CENTER );
			frame1.setLocation( x, y );
			frame1.setVisible( true );
			frame1.pack();
			return frame1;
	}


} // rast1

