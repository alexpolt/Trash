import java.awt.*;
import java.util.*;
import java.awt.geom.*;
import java.awt.event.*;
import javax.swing.*;


class spline1 {
	static Point2D.Double[] vec = new Point2D.Double[2];
	static LinkedList list = new LinkedList();
	static JFrame j1;

public static void main(String argv[]) {

	p("Start!");

	vec[0] = new Point2D.Double( 0, 100 );
	vec[1] = new Point2D.Double( 100, 0 );
		
	j1 = new JFrame("Spline1" );
	JPanel p1 = new JPanel( ) {
		public void paintComponent( Graphics _g ) {
			if( list.size() == 0 ) return;
			p("paint\n" + list.toString() );
			Graphics2D g = ( Graphics2D ) _g;
			Path2D path = new Path2D.Double();

			Point p1 = (Point) list.get(0);
			path.moveTo( p1.x, p1.y );
			for( Object o: list ) {
				Point _p = (Point) o;
				path.lineTo( _p.x, _p.y );
			}
			g.setStroke( new BasicStroke( 2 ) );
			g.setColor( Color.black );
			g.draw( path );

			if( list.size() < 3 || ((list.size() - 1) % 2) != 0  ) return;
			p( list.size() + "" );
			path.reset();
			path.moveTo( p1.x, p1.y );

			for( int i = 0; i < list.size()-1; i+=2 ) {
				Point orig = (Point) list.get( i  );
				Point prev = (Point) list.get( i + 1 );
				Point cur = (Point) list.get( i + 2 );
				double curx = (double) (cur.x - prev.x);
				double cury = (double) (cur.y - prev.y);
				double prevx = (double) (prev.x - orig.x);
				double prevy = (double) (prev.y - orig.y);
				path.moveTo( orig.x, orig.y );
				Point2D.Double _p = new Point2D.Double();
				for( double t = 0; t<=1; t+=0.01 ) {
					double k1 = 1 - (1-t) * (1-t);
					double k2 = t * t * t;
					_p.x = (double)orig.x + prevx * k1 + curx * k2;
					_p.y = (double)orig.y + prevy * k1 + cury * k2;
					path.lineTo( _p.x, _p.y );
				}
			}
			g.setStroke( new BasicStroke( 1 ) );
			g.setColor( Color.red );
			g.draw( path );

		}
	};
	p1.addMouseListener( new MouseAdapter() {
		public void mousePressed( MouseEvent e ) {
			if( e.getButton() == e.BUTTON3 ) {
				list.clear();
				j1.repaint();
				return;
			}
			list.add( e.getPoint() );
			j1.repaint();
			//p( list.toString() );
		}
	});
	j1.add( p1 );
	j1.setSize( 400,400 );
	j1.setLocation( 100, 100 );
	j1.setDefaultCloseOperation( WindowConstants.EXIT_ON_CLOSE );
	j1.setVisible(true);
	//j1.setIgnoreRepaint(false);
}

static void p(String arg) {
	System.out.println(arg);
}

}