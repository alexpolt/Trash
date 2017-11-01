
import java.io.*;
import java.awt.*;
import java.util.*;
import java.math.*;
import java.awt.geom.*;
import java.awt.event.*;
import javax.swing.*;
import javax.imageio.*;
import java.awt.image.*;


	class Unit {
		public Color color = Color.black;
		public int x = 0, y = 0, dirx = 0, diry = 0, step = 0;

		public Unit( double _x, double _y ) {
			x = (int) _x;
			y = (int) _y;
			newDir();
		}

		public void newDir(  ) {
			dirx = Math.random() > 0.5f ? 1 : -1;
			diry = Math.random() > 0.5f ? 1 : -1;
		}


		public Point step( Point p ) {
			if( ++step % 20 == 0 ) newDir();
			Point ret = new Point();
			ret.x = 4 * dirx;
			ret.y = 4 * diry;
		return ret;
		}
	}

	class Unit1 extends Unit {
		LinkedList hist = new LinkedList();

		public Unit1( double _x, double _y ) {
			super( _x, _y );
		}

		public Point step( Point p ) {
			hist.add( p );
			if( hist.size() < 2 ) return new Point( 0, 0 );
			if( hist.size() > 5 ) hist.removeFirst();

			Point ret = new Point();
			Point p1 = (Point) hist.getFirst();
			Point p2 = (Point) hist.getLast();

			double dist = Math.sqrt( (p2.x-p1.x)*(p2.x-p1.x) +  (p2.y-p1.y)*(p2.y-p1.y) );

			double vecx = (double) (p2.x - p1.x) / dist;
			double vecy = (double) (p2.y - p1.y) / dist;
			ret.x = (int) ( p.x + vecx * 50 - x );
			ret.y = (int) ( p.y + vecy * 50 - y );
		return ret;
		}

	}


class robot1 {
	static boolean stop = false;
	static canvas canvas;
	static int X = 500, Y = 400;
	static LinkedList units = new LinkedList();


	public static void main(String[] args) {

		JFrame frame1 = new JFrame("Robot 1");
		frame1.setDefaultCloseOperation( WindowConstants.EXIT_ON_CLOSE );
		canvas = new canvas();
		frame1.getContentPane().add( canvas, BorderLayout.CENTER );
		JPanel butpanel = new JPanel();
		JButton button1 = new JButton("Pause");
		button1.addActionListener(
			new ActionListener() {
				public void actionPerformed( ActionEvent e ) {
					stop = !stop;
				}
		} );
		butpanel.add( button1 );


		frame1.getContentPane().add( BorderLayout.SOUTH, butpanel );
		frame1.setSize( X, Y );
		frame1.setLocation(500,300);
		frame1.setIgnoreRepaint(false);
		frame1.setVisible(true);
		
		units.add ( new Unit1( 20+ Math.random() * (X-50), 20+ Math.random() * (Y-50) ) );
		units.add ( new Unit( 20+ Math.random() * (X-50), 20+ Math.random() * (Y-50) ) );
		
		Unit u1 = (Unit) units.get(0);
		Unit u2 = (Unit) units.get(1);

		u2.color = Color.red;

		while( true ) {
			while( ! stop ) {

				Point pos = u1.step( new Point( u2.x, u2.y ) );
				u1.x = u1.x + (int) Math.signum( pos.x );
				u1.y = u1.y + (int) Math.signum( pos.y );
				pos = u2.step( new Point( u1.x, u1.y ) );
				//u2.x = u2.x + (int) Math.signum( pos.x );
				//u2.y = u2.y + (int) Math.signum( pos.y );
				u2.x = u2.x + pos.x;
				u2.y = u2.y + pos.y;

				canvas.repaint();

				try { Thread.sleep(100); } 
				catch(Exception e) {}
			}
		}

	}





	static private class canvas extends JPanel
	{
		public canvas() {
			super();
		}
		public void paintComponent( Graphics _g ) {
			super.paintComponent( _g );
			Graphics2D g = (Graphics2D) _g; 
			for( Object _unit: units ) {
				//g.setStroke( new BasicStroke( 2.0f,  unit.color ) );
				Unit unit = (Unit) _unit;
				if( unit.x < 10 || unit.x > X - 50 || unit.y < 10 || unit.y > Y - 50 ) unit.newDir();
				if( unit.x < 10 ) unit.x = 10;
				if( unit.x > X - 50 ) unit.x = X - 50;
				if( unit.y < 10 ) unit.y = 10;
				if( unit.y > Y - 50 ) unit.y = Y - 50;
				g.setPaint( unit.color );
				g.setColor( unit.color );
				g.fill( new Rectangle2D.Double( unit.x, unit.y, 10, 10 ) );
			}
		}

	}

	}



