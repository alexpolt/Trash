
import java.io.*;
import java.awt.*;
import java.util.*;
import java.awt.geom.*;
import java.awt.event.*;
import javax.swing.*;
import javax.imageio.*;
import java.awt.image.*;

public class cube1
{
	static JFrame frame1;
	static double TIME = 0;
	static int w = 500, h = 500, wh = w >> 1, hh = h >> 1, cubeW = 300, cubeH = 300, cubeWh = cubeW>>1, cubeHh = cubeH>>1;
	static Graphics2D g;
	static BufferedImage texture;
	static int texW, texH;

	static class vertex
	{
		public vertex( double x, double y, double z, double light) { this.x = x; this.y = y; this.z = z; this.light = light; }
		double x, y, z, u, v, light;
		public String toString() { return "Vertex x: " + x + ", y: " + y + ", z: " + z + ", light: " + light; }
	}
	static ArrayList list = new ArrayList( 50 );

	static void p( Object arg ) { System.out.println( arg ); }

	public static void main(String[] args) {
		p("Start!");

		try
		{
			texture = ImageIO.read( new File("nebula.jpg") );
			texW = texture.getWidth() - 1;
			texH = texture.getHeight() - 1;
		} catch ( Exception exc ) { p(exc); }

		frame1 = new JFrame("CUBE 1");
		frame1.setDefaultCloseOperation( WindowConstants.EXIT_ON_CLOSE );
		frame1.setSize( w, h );
		frame1.setLocation( 500, 300 );
		frame1.setIgnoreRepaint(true);
		frame1.setVisible(true);
		frame1.setBackground( null );
		frame1.createBufferStrategy(2);
		/*
		frame1.addMouseListener( new MouseAdapter() {
				int i = 0;
				public void mousePressed( MouseEvent e ) {
					( new Thread() {
						public void run() {
							while(true)
							try { 
								synchronized( texture ) {
								texture = ImageIO.read( new File("p:\\grandfuther"+(i%3+1)+".jpg") );
									texW = texture.getWidth() - 1;
									texH = texture.getHeight() - 1;
								}
								i++;
								sleep(3000);
							} catch ( Exception exc ) { p(exc); }
						}
					}).start();
				}
		});
		*/
		g = (Graphics2D) frame1.getBufferStrategy().getDrawGraphics();
		g.setStroke( new BasicStroke( 3 ) );
		
		for( int i = 0; i < 8; i ++ ) {
			vertex v = new vertex( (i & 1)*cubeW - cubeWh, ((i & 2) >> 1) * cubeH - cubeHh, ((i & 4) >> 2) * cubeW - cubeWh, 0 );
			list.add( v );
		}
		p( list );

		( new Thread() {
			public void run() {
				double angle = TIME = 0.00250, scalar;
				vertex _v, v[] = new vertex[4], normal, center;

				while( true ) {
					g.setPaint( Color.black );
					g.fillRect( 0, 0, w, h );
					//g.drawImage( texture, null, 0, 0);

					for( Object o: list ) {
						_v = (vertex) o;
						rotz( _v, angle );
						roty( _v, angle * 2 );
						rotx( _v, angle * 4 );
					}

					for( int bit = 0; bit < 3; bit++ ) {
						for( int bit1 = 0, i = 0; i < 2; i++, bit1 = 1 << bit ) {
						for( int t = 0; t < 4; t++ ) {
							int bit23 = t, index = 0;
							if( bit != 0 ) { index = bit1 | index | ( bit23 & 1 ) << 0; bit23 = bit23 >> 1; } 
							if( bit != 1 ) { index = bit1 | index | ( bit23 & 1 ) << 1; bit23 = bit23 >> 1; }
							if( bit != 2 ) { index = bit1 | index | ( bit23 & 1 ) << 2; bit23 = bit23 >> 1; }
							_v = (vertex) list.get( index );
							v[t] = new vertex( _v.x + wh, _v.y + hh, _v.z, 0 );
						}
						synchronized( texture ) {
							v[0].u = 0; v[0].v = 0;
							v[1].u = texW; v[1].v = 0;
							v[2].u = 0; v[2].v = texH;
							v[3].u = texW; v[3].v = texH;
						center = new vertex( v[0].x + (v[3].x-v[0].x)/2 - (wh), v[0].y + (v[3].y-v[0].y)/2 - (hh), v[0].z + (v[3].z-v[0].z)/2, 0 );
						//vertex center = new vertex( v[0].x - (wh), v[0].y - (hh), 0, 0 );
						if( center.z > 0 ) { 
							normal = normal( v[0], v[1], v[2] );
							scalar = center.x * normal.x + center.y * normal.y + center.z * normal.z;
							if( scalar < 0 ) { 
								_v = v[1]; v[1] = v[2]; v[2] = _v;
							}
							rasterize( g, v[0], v[1], v[2] );
						}

						if( center.z > 0 ) { 
							normal = normal( v[1], v[2], v[3] );
							scalar = center.x * normal.x + center.y * normal.y + center.z * normal.z;
							if( scalar < 0 ) { 
								_v = v[2]; v[2] = v[3]; v[3] = _v;
							}
							rasterize( g, v[1], v[2], v[3] );

							//Path2D.Double path = new Path2D.Double();
							//path.moveTo( v[0].x, v[0].y );
							//path.lineTo( v[1].x, v[1].y );
							//path.lineTo( v[2].x, v[2].y );
							//g.setColor( Color.black );
							//g.draw( path );
						}
						} //synchronized

						} // for bit1
					} // for bit

					g.setColor( Color.white );
					//for( int t = 0; t < 4; t++ ) {
					//	_v = v[t];
					//	g.fillRect( (int) ( _v.x ), (int) ( _v.y ), (int) ( (_v.z + 50)/ 8 + 1) , (int) ( (_v.z + 50)/ 8 + 1 ) );
					//}
					//p( v[0] + "\n" +  v[1] + "\n" + v[2] + "\n" + v[3] + "\n" );
					//drawCoords( g );
					frame1.getBufferStrategy().show();

					try { sleep(10); } catch ( Exception e ){ p(e); }
				}
			}
		}).start();
		
		p("Finish!");
	}

	static void rotz ( vertex v, double angle ) {
		double x = v.x, y = v.y, z = v.z;
		double cos = Math.cos( angle );
		double sin = Math.sin( angle );
		v.x = x * cos - y * sin;
		v.y = x * sin + y * cos;
	}
	static void rotx ( vertex v, double angle ) {
		double x = v.x, y = v.y, z = v.z;
		double cos = Math.cos( angle );
		double sin = Math.sin( angle );
		v.z = z * cos - y * sin;
		v.y = z * sin + y * cos;
	}
	static void roty ( vertex v, double angle ) {
		double x = v.x, y = v.y, z = v.z;
		double cos = Math.cos( angle );
		double sin = Math.sin( angle );
		v.x = x * cos - z * sin;
		v.z = x * sin + z * cos;
	}
	static vertex normal( vertex v1, vertex v2, vertex v3 ) {
		vertex v = new vertex( 0, 0, 0, 0 );
		v.x = (v3.y-v1.y)* (v2.z-v1.z) - (v3.z-v1.z) * (v2.y-v1.y);
		v.y = (v3.z-v1.z) * (v2.x-v1.x) - (v3.x-v1.x) * (v2.z-v1.z);
		v.z = (v3.x-v1.x) * (v2.y-v1.y) - (v3.y-v1.y) * (v2.x-v1.x);
		return v;
	}

	static void rasterize( Graphics2D g, vertex p1, vertex p2, vertex p3 ) {
		int minx = (int) ( Math.min( p1.x, Math.min( p2.x, p3.x ) ) ) - 1;
		int miny = (int) ( Math.min( p1.y, Math.min( p2.y, p3.y ) ) ) - 1;
		int maxx = (int) ( Math.max( p1.x, Math.max( p2.x, p3.x ) ) ) + 1;
		int maxy = (int) ( Math.max( p1.y, Math.max( p2.y, p3.y ) ) ) + 1;
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
/*
		double z0 = (int)(  ( cubeWh/2 + cubeWh + p1.z ) / (double) (cubeW) * 255  );
		double z1 = (int)(  ( cubeWh/2 + cubeWh + p2.z ) / (double) (cubeW) * 255  ); 
		double z2 = (int)(  ( cubeWh/2 + cubeWh + p3.z ) / (double) (cubeW) * 255  );
		z0 = z0 < 0 ? 0 : (z0 > 255 ? 255 : z0);
		z1 = z1 < 0 ? 0 : (z1 > 255 ? 255 : z1);
		z2 = z2 < 0 ? 0 : (z2 > 255 ? 255 : z2);
*/
		double u0 = p1.u;
		double u1 = p2.u - p1.u;
		double u2 = p3.u - p1.u;
		double v0 = p1.v;
		double v1 = p2.v - p1.v;
		double v2 = p3.v - p1.v;
		//System.out.printf("u0: %f, v0: %f, u1: %f, v1: %f, u2: %f, v2: %f\n\n", u0, v0, u1, v1, u2, v2 );
		//System.out.printf("dzdx: %f, dzdy: %f, dzdx: %f, dzdy: %f\nz0dx: %f, z0dy: %f\n\n", dzdx1, dzdy1, dzdx2, dzdy2, z0dx1, z0dy1 );

		double C1 = ( p2.y - p1.y ) * ( minx - p1.x ) - ( p2.x - p1.x ) * ( miny - p1.y );
		if( (p2.y - p1.y) < 0 ) C1++;
		else if( (p2.y - p1.y) == 0 && (p2.x - p1.x) < 0 ) C1++;

		double C2 = ( p3.y - p2.y ) * ( minx - p2.x ) - ( p3.x - p2.x ) * ( miny - p2.y );
		if( (p3.y - p2.y) < 0 ) C2++;
		else if( (p3.y - p2.y) == 0 && (p3.x - p2.x) < 0 ) C2++;

		double C3 = ( p1.y - p3.y ) * ( minx - p3.x ) - ( p1.x - p3.x ) * ( miny - p3.y );
		if( (p1.y - p3.y) < 0 ) C3++;
		else if( (p1.y - p3.y) == 0 && (p1.x - p3.x) < 0 ) C3++;

		double INC1X = p2.y - p1.y;
		double INC1Y = - ( p2.x - p1.x );

		double INC2X = p3.y - p2.y;
		double INC2Y = - ( p3.x - p2.x );

		double INC3X = p1.y - p3.y;
		double INC3Y = - ( p1.x - p3.x );

		//g.setPaint( Color.black );
		//g.drawRect( minx, miny, maxx-minx+1, maxy-miny+1 );

		for( int y = miny; y <= maxy; y++ ) {
			double C1X = C1, C2X = C2, C3X = C3;
			double _z0dx1 = z0dx1;	
			double _z0dx2 = z0dx2;
			for( int x = minx; x <= maxx; x++ ) {

				if( C1X > 0 && C2X > 0 && C3X > 0 ) {
							//int z1 = (int) ( (_z0dx1 + z0dy1) * 255 );
							//int z2 = (int) ( (_z0dx2 + z0dy2) * 255 );
							//int color = (int) ( z0 + (_z0dx1 + z0dy1) * z1 + (_z0dx2 + z0dy2) * z2 ) & 0xff;
							int u = (int) ( u0 + (_z0dx1 + z0dy1) * u1 + (_z0dx2 + z0dy2) * u2 );
							int v = (int)( v0 + (_z0dx1 + z0dy1) * v1 + (_z0dx2 + z0dy2) * v2 );
							u = u > texW ? texW : u < 0 ? 0 : u;
							v = v > texH ? texH : v < 0 ? 0 : v;
							int color = 0;
							color = texture.getRGB(  u, v  );
							//if( (_z0dx1 + z0dy1) > 1.00001 )
							//	System.out.printf("_z0dx1 + z0dy1: %f, z0dx1: %f, z0dy1: %f\n", _z0dx1 + z0dy1, _z0dx1, z0dy1 );
							//if( (_z0dx2 + z0dy2) > 1.00001 )
							//	System.out.printf("_z0dx2 + z0dy2: %f, z0dx2: %f, z0dy2: %f\n", _z0dx2 + z0dy2, _z0dx2, z0dy2 );
							//g.setPaint( new Color( color<<16 | color<<8 | color ) );
							g.setPaint( new Color( color ) );
							g.fillRect( x, y, 1, 1 );
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
	static public void drawCoords( Graphics2D g ) {
		//g.setXORMode( Color.white );
		//g.setColor( Color.white );
		g.setStroke( new BasicStroke( 2 ) );
		Path2D path = new Path2D.Double();
		path.moveTo( wh, 0 ); path.lineTo( wh, h-1 );
		path.moveTo( 0, hh ); path.lineTo( w-1, hh );
		g.draw( path );
		//g.setPaintMode();
	}

}

