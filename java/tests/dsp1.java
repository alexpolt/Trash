
import java.io.*;
import java.awt.*;
import java.util.*;
import java.math.*;
import java.awt.geom.*;
import java.awt.event.*;
import javax.swing.*;
import javax.imageio.*;
import java.awt.image.*;

import java.math.*;


class dsp1 {
	static canvas canvas;
	static int X = 500, Y = 400;
	static LinkedList units = new LinkedList();
	static boolean startDrawing = false;
	static Vector<Integer> vec1 = new Vector();
	static Vector<Integer> vec2 = new Vector();
	static Integer freq = 1;

	public static void main(String[] args) {

		JFrame frame1 = new JFrame("DSP 1");
		frame1.setDefaultCloseOperation( WindowConstants.EXIT_ON_CLOSE );
		canvas = new canvas();
		frame1.getContentPane().add( canvas, BorderLayout.CENTER );
		JPanel butpanel = new JPanel();
		JButton button1 = new JButton("Calculate");
		button1.addActionListener(
			new ActionListener() {
				public void actionPerformed( ActionEvent e ) {
					int i = 0;
					vec2.clear();
					for(double a = 0; a <= 2 * Math.PI ; a+=2 * Math.PI/400)
					vec2.add( (int) (Math.sin( a )*100) );
/*
					if( vec1.get(0) > 0 ) {
						System.out.print( "Error 1!\n" );
						return;
					}
					//System.out.println( vec1 );
					while( i < vec1.size() && vec1.get( i ) <= 0 ) { 
						//System.out.print( i + "(" + vec1.get( i ) + "), " );
						i++;
					}
					if( i == vec1.size() ) {
						System.out.print( "Error 2!\n" );
						return;
					}
					vec2.add( 0 );
					while( i < vec1.size() && vec1.get( i ) > 0 ) {
						vec2.add( vec1.get( i ) );
						i++;
					}
					vec2.add( 0 );
					//for( i = vec2.size() - 2; i >= 0; i-- )
					//				vec2.add( -vec2.get( i ) );
					int size = vec2.size();
					for( i = 1; i < size-1	; i++ )
									vec2.add( -vec2.get( i ) );
*/
					canvas.repaint();

				}
		} );

		button1.setFocusable( false );
		butpanel.add( button1 );
		JComboBox select1 = new JComboBox();
		select1.setFocusable( false );
		select1.addItem( "1" );
		select1.addItem( "2" );
		select1.addItem( "3" );
		select1.addItem( "4" );
		select1.addActionListener(
			new ActionListener() {
				public void actionPerformed( ActionEvent e ) {
					JComboBox sel = (JComboBox) e.getSource();
					freq = Integer.parseInt( (String) sel.getSelectedItem() );
				}
			}
		);
		butpanel.add( select1 );

		button1 = new JButton("Fuorier");
		button1.setFocusable( false );
		button1.addActionListener(
			new ActionListener() {
				public void actionPerformed( ActionEvent e ) {
					int sum1 = 0, sum2= 0, sum3 = 0;
					int offset = dsp1.vec2.size() / 4;
					for( int i = 0; i < dsp1.vec2.size()-1; i++ ) {
						int i2 = i * dsp1.freq % dsp1.vec2.size();
						sum1 += dsp1.vec2.get( i ) * dsp1.vec2.get( i2 );
						sum3 += dsp1.vec2.get( i ) * dsp1.vec2.get( i2 );
						int i3 = ( offset + i * dsp1.freq ) % dsp1.vec2.size();
						sum2 += dsp1.vec2.get( i ) * dsp1.vec2.get( i3 );
						sum3 += dsp1.vec2.get( i ) * dsp1.vec2.get( i3 );
					}
					System.out.println( "SUM1 = " + sum1 );
					System.out.println( "SUM2 = " + sum2 );
					System.out.println( "SUM3 = " + sum3 );
				}
		} );
		butpanel.add( button1 );


		frame1.getContentPane().add( BorderLayout.SOUTH, butpanel );
		frame1.setSize( X, Y );
		frame1.setLocation(500,300);
		frame1.setIgnoreRepaint(false);
		frame1.setVisible(true);
		
		canvas.getGraphics();
		canvas.addMouseMotionListener( new MouseAdapter( ) {
				public void mouseMoved( MouseEvent e ) {
					if( ! startDrawing ) return;
					Point p = e.getPoint();
					vec1.add( vec1.size(), p.y - 100 );
					Graphics2D g = (Graphics2D) canvas.getGraphics();
					g.setPaint( Color.red );
					g.setStroke( new BasicStroke( 2.0f ) );
					g.draw( new Rectangle2D.Double( p.x, p.y, 1, 1 ) );
				}
			}
		);
		frame1.addKeyListener( new KeyAdapter( ) {
				public void keyPressed( KeyEvent e ) {
					System.out.println( e );
					if( ! startDrawing )
							vec1.clear();
					if( e.getKeyCode() == e.VK_ENTER )
							startDrawing = ! startDrawing;
				}
			}
		);
		frame1.requestFocus();

	}
}


class canvas extends JPanel
{
		private Path2D.Double path = new Path2D.Double();

		public canvas() {
			super();
			path.moveTo(0,100);
			path.lineTo(dsp1.X,100);
		}
		public void paintComponent( Graphics _g ) {
			super.paintComponent( _g );
			Graphics2D g = (Graphics2D) _g; 
			g.setPaint( Color.black );
			g.setStroke( new BasicStroke( 2.0f ) );
			g.draw( path );

			int offset = dsp1.vec2.size() / 4;

			for( int i = 0; i < dsp1.vec2.size()-1; i++ ) {
				g.setPaint( Color.blue );
				g.draw( new Rectangle2D.Double ( i, 100 + dsp1.vec2.get( i ), 1, 1 ) );
				g.setPaint( Color.green );
				int i2 = i * dsp1.freq % dsp1.vec2.size();
				g.draw( new Rectangle2D.Double ( i, 100 + dsp1.vec2.get( i2 ), 1, 1 ) );
				g.setPaint( Color.yellow );
				int i3 = ( offset + i * dsp1.freq ) % dsp1.vec2.size();
				g.draw( new Rectangle2D.Double ( i, 100 + dsp1.vec2.get( i3 ), 1, 1 ) );
			}
		}

}





