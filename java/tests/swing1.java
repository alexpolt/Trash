
import java.io.*;
import java.awt.*;
import java.util.*;
import java.awt.geom.*;
import java.awt.event.*;
import javax.swing.*;

public class swing1
{
	static JFrame frame1;
	static swingdemo swingdemo1 = new swingdemo();
	static double TIME = 0;

	public static void main(String[] args) {

		frame1 = new JFrame("swingtest");
		frame1.setDefaultCloseOperation( WindowConstants.EXIT_ON_CLOSE );
		frame1.getContentPane().add( swingdemo1, BorderLayout.CENTER );
		JPanel butpanel = new JPanel();
		JButton button1 = new JButton("Рисовать!");
		button1.addActionListener(
			new ActionListener() {
				public void actionPerformed( ActionEvent e ) {
					//swingdemo1.drawFigure();
					//Point p1 = (Point) swingdemo1.points.get(0), p2 = (Point) swingdemo1.points.get(1), p3 = (Point) swingdemo1.points.get(2);
					//swingdemo1.rasterize(p1,p2,p3);
					frame1.repaint();
				}
		} );
		butpanel.add( button1 );
		button1 = new JButton("Стереть");
		button1.addActionListener(
			new ActionListener() {
				public void actionPerformed( ActionEvent e ) {
					swingdemo1.points.clear();
					swingdemo1.repaint();
				}
		} );
		butpanel.add( button1 );


		frame1.getContentPane().add( BorderLayout.SOUTH, butpanel );
		frame1.setSize(500,400);
		frame1.setLocation(500,300);
		frame1.setIgnoreRepaint(false);
		frame1.setVisible(true);

		( new Thread() {
				public void run() {
					while(true) {
						//TIME=0.01;
						swingdemo1.repaint();
						try	{ sleep(100); } catch ( Exception e ){ System.out.println ( e );}
					}
				}
			}
		).start();
	}

	static private class swingdemo extends JPanel
	{
		public LinkedList points = new LinkedList();
		private GeneralPath path1 = new GeneralPath();
		static Point old;

		public swingdemo() {
			super();
			addMouseListener( new MouseAdapter() {
				public void mouseReleased( MouseEvent e ) {
					points.add( e.getPoint() );
					repaint();
				}
			});
			addMouseMotionListener( new MouseAdapter() {
				public void mouseMove( MouseEvent e ) {
					Point p = e.getPoint();
					if( old == null ) old = p;
					int dx = p.x - old.x, dy = p.y - old.y;
					old = p;
					AffineTransform t = AffineTransform.getRotateInstance( 0.1d, frame1.getWidth()>>1, frame1.getHeight()>>1 );
					LinkedList points = swingdemo1.points;
					Point2D.Double newP = new Point2D.Double();
					for( Object _i: points ) {
						Point i = (Point) _i;
						t.transform( new Point2D.Double(i.x, i.y), newP );
						i.x = (int) newP.x; i.y = (int) newP.y;
					}
					swingdemo1.repaint();
				}
			});
			System.out.println("Поехали!");
		}
		public void paintComponent( Graphics _g ) {
			super.paintComponent( _g );
			Graphics2D g = (Graphics2D) _g; 

			AffineTransform t = AffineTransform.getRotateInstance( 0.02, frame1.getWidth()>>1, frame1.getHeight()>>1 );
			Point2D.Double newP = new Point2D.Double();
			Iterator it = points.iterator();
			while( it.hasNext() ) { 
					Point p1 = (Point) it.next(); 
					t.transform( new Point2D.Double( p1.x, p1.y ), newP );
					p1.x = (int) newP.x; p1.y = (int) newP.y;
					g.drawRect( p1.x, p1.y, 0, 0 );
			}
			int size = swingdemo1.points.size(), i = 0;
			while( size > 0 && size % 3 == 0 && size > i ) {
				Point p1 = (Point) swingdemo1.points.get(i++), p2 = (Point) swingdemo1.points.get(i++), p3 = (Point) swingdemo1.points.get(i++);
				swingdemo1.rasterize( g, p1, p2, p3);
			}

			drawCoords( g );
			g.dispose();
			//Shape rect1 = new RoundRectangle2D.Double(50, 50, 100, 100, 10, 10);
			//g.draw ( rect1 );
			//g.fill( rect1 );

			//SwingUtilities.invokeLater( new Runnable() { public void run() { frame1.repaint(); } } );
		}
		public void rasterize( Graphics2D g, Point p1, Point p2, Point p3 ) {
			int minx = Math.min( p1.x, Math.min( p2.x, p3.x ) );
			int miny = Math.min( p1.y, Math.min( p2.y, p3.y ) );
			int maxx = Math.max( p1.x, Math.max( p2.x, p3.x ) );
			int maxy = Math.max( p1.y, Math.max( p2.y, p3.y ) );
			//System.out.printf("minx: %d, miny : %d, maxx: %d, maxy: %d\n", minx,miny,maxx,maxy);

			double A1 = ( p2.y - p1.y ) * ( 0 - 0 ) - ( 1 - 0 ) * ( p3.y - p1.y );
			double B1 = ( 1 - 0 ) * ( p3.x - p1.x ) - ( p2.x - p1.x ) * ( 0 - 0 );

			double A2 = ( p2.y - p1.y ) * ( 1 - 0 ) - ( 0 - 0 ) * ( p3.y - p1.y );
			double B2 = ( 0 - 0 ) * ( p3.x - p1.x ) - ( p2.x - p1.x ) * ( 1 - 0 );

			double C = ( p2.x - p1.x ) * ( p3.y - p1.y ) - ( p2.y - p1.y ) * ( p3.x - p1.x );
			//System.out.printf("A1: %f, B1: %f, C: %f\nA2: %f, B2: %f, C: %f\n", A1, B1, C, A2, B2, C );

			double dzdx1 = - A1 / C;
			double dzdy1 = - B1 / C;

			double dzdx2 = - A2 / C;
			double dzdy2 = - B2 / C;

			double z0dx1 = dzdx1 * ( minx - p1.x );
			double z0dy1 = dzdy1 * ( miny - p1.y );

			double z0dx2 = dzdx2 * ( minx - p1.x );
			double z0dy2 = dzdy2 * ( miny - p1.y );
			//System.out.printf("dzdx: %f, dzdy: %f, dzdx: %f, dzdy: %f\nz0dx: %f, z0dy: %f\n\n", dzdx1, dzdy1, dzdx2, dzdy2, z0dx1, z0dy1 );

			int C1 = ( p2.y - p1.y ) * ( minx - p1.x ) - ( p2.x - p1.x ) * ( miny - p1.y );
			int C2 = ( p3.y - p2.y ) * ( minx - p2.x ) - ( p3.x - p2.x ) * ( miny - p2.y );
			int C3 = ( p1.y - p3.y ) * ( minx - p3.x ) - ( p1.x - p3.x ) * ( miny - p3.y );

			int INC1X = p2.y - p1.y;
			int INC1Y = - ( p2.x - p1.x );

			int INC2X = p3.y - p2.y;
			int INC2Y = - ( p3.x - p2.x );

			int INC3X = p1.y - p3.y;
			int INC3Y = - ( p1.x - p3.x );

			//g.setPaint( Color.black );
			//g.drawRect( minx, miny, maxx-minx+1, maxy-miny+1 );

			for( int y = miny; y <= maxy; y++ ) {
				int C1X = C1, C2X = C2, C3X = C3;
				double _z0dx1 = z0dx1;
				double _z0dx2 = z0dx2;
				for( int x = minx; x <= maxx; x++ ) {

					if( C1X > 0 && C2X > 0 && C3X > 0 ) {
								int z1 = (int) ( (_z0dx1 + z0dy1) * 255 );
								int z2 = (int) ( (_z0dx2 + z0dy2) * 255 );
								int z0 = (int) ( 255 - (_z0dx1 + z0dy1) * 255 - (_z0dx2 + z0dy2) * 255 );
								//if( (_z0dx1 + z0dy1) > 1.00001 )
								//	System.out.printf("_z0dx1 + z0dy1: %f, z0dx1: %f, z0dy1: %f\n", _z0dx1 + z0dy1, _z0dx1, z0dy1 );
								//if( (_z0dx2 + z0dy2) > 1.00001 )
								//	System.out.printf("_z0dx2 + z0dy2: %f, z0dx2: %f, z0dy2: %f\n", _z0dx2 + z0dy2, _z0dx2, z0dy2 );
								g.setPaint( new Color( z1<<16 | z2<<8 | z0 ) );
								g.drawRect( x, y, 0, 0 );
					}
					C1X+=INC1X;
					C2X+=INC2X;
					C3X+=INC3X;
					_z0dx1+=dzdx1;
					_z0dx2+=dzdx2;
				}
				C1+=INC1Y;
				C2+=INC2Y;
				C3+=INC3Y;
				z0dy1+=dzdy1;
				z0dy2+=dzdy2;
			}

		}
		public void drawCoords( Graphics2D g ) {
			g.setXORMode( Color.white );
			g.setStroke( new BasicStroke( 2 ) );
			int x = frame1.getWidth(), y = frame1.getHeight();
			int hx = x >> 1, hy = y >> 1;
			Path2D path = new Path2D.Double();
			path.moveTo( hx, 0); path.lineTo( hx, y-1);
			path.moveTo( 0, hy); path.lineTo( x-1, hy);
			g.draw( path );
			g.setPaintMode();
		}
		public void drawFigure() {
			if( points.size() < 2 ) return;
			System.out.println( "Drawing figure...\n" );

			Graphics2D g = (Graphics2D) getGraphics(); 
			g.setStroke( new BasicStroke( 2, BasicStroke.CAP_BUTT, BasicStroke.JOIN_MITER ) );

			path1.reset();

			Point p1 = (Point) points.getFirst();
			System.out.println( "First poing: " + p1 );
			path1.moveTo( p1.x, p1.y );

			Iterator it = points.iterator();
			it.next();
			while(it.hasNext()) { p1 = (Point)it.next(); System.out.println( p1 ); path1.lineTo( p1.x, p1.y ); };
			path1.closePath();
			points.clear();
			repaint();
		}
	}

}

