import java.math.BigInteger;

class test2 {

	public static void main(String args[]) {
		if( args.length < 3 ) {
			System.out.println( "Need arguments: power start end" );
			return;
		}
		int power = Integer.parseInt( args[0] );
		long start = Long.parseLong( args[1] );
		long end = Long.parseLong( args[2] );
		int mask = (1<<28)-1;
		BigInteger r = BigInteger.ZERO;
		O:
		for(int t=1; t<=1; t++) { 
			long n = (long) Math.floor( Math.pow( f( power, BigInteger.valueOf( start ), t ).doubleValue(), 1.0/power ) );
			BigInteger p = BigInteger.valueOf(n).pow( power );
			System.out.println( "Start, power = " + power + ", t = " + t + ", start = " + start + ", end = " + end + ", n = " + n );
			for(long i=start; i<end; i++) {
				if( ((int)i & mask) == 0 ) System.out.println( "i = " + i );
				r = f( power, BigInteger.valueOf( i ), t );
				while( r.compareTo( p ) > 0 ) {
					p = BigInteger.valueOf(++n).pow( power );
				}
				if( r.equals( p ) ) {
					System.out.println("i=" + i + ", n = " + n + ", r = " + r);
					//break O;
				}
			}
		}
		System.out.println( "Finish" );
	}

	static BigInteger f( int power, BigInteger x, int t ) {
		if( power == 2 ) return f4( x, t );
		if( power == 3 ) return f3___( x, t );
		if( power == 4 ) return f4_( x, t );
		throw new RuntimeException("no such power");
	}

	static BigInteger f4( BigInteger x, int t ) {
		BigInteger r = BigInteger.ZERO;
		BigInteger k0 = BigInteger.valueOf( 4 );
		BigInteger k1 = BigInteger.valueOf( 6 );
		for(int i=0; i<t; i++) { 
			BigInteger xx = x.add( BigInteger.valueOf(i) );
			r = r.add( BigInteger.ONE );
			r = r.add( xx.multiply( k0 ) );
			BigInteger xxx = xx.multiply( xx );
			r = r.add( xxx.multiply( k1 ) );
			r = r.add( xxx.multiply( xx ).multiply( k0 ) );
		}
		return r;
	}

	static BigInteger f4_( BigInteger x, int t ) {
		BigInteger r = BigInteger.ZERO;
		BigInteger k0 = BigInteger.valueOf( 1 );
		BigInteger k1 = BigInteger.valueOf( 1 );
		for(int i=0; i<t; i++) { 
			BigInteger xx = x.add( BigInteger.valueOf(i) );
			//r = r.add( BigInteger.ONE );
			r = r.add( xx.multiply( k0 ) );
			BigInteger xxx = xx.multiply( xx );
			r = r.add( xxx.multiply( k1 ) );
			r = r.add( xxx.multiply( xx ).multiply( k0 ) );
		}
		return r;
	}


	static BigInteger f3( BigInteger x, int t ) {
		BigInteger r = BigInteger.ZERO;
		BigInteger k = BigInteger.valueOf( 3 );
		for(int i=0; i<t; i++) { 
			BigInteger xx = x.add( BigInteger.valueOf(i) );
			r = r.add( BigInteger.ONE );
			r = r.add( xx.multiply( k ) );
			r = r.add( xx.multiply( xx ).multiply( k ) );
		}
		return r;
	}

	static BigInteger f3_( BigInteger x, int t ) {
		BigInteger r = BigInteger.ZERO;
		BigInteger k = BigInteger.valueOf( 3 );
		for(int i=0; i<t; i++) { 
			BigInteger xx = x.add( BigInteger.valueOf(i) );
			//r = r.add( BigInteger.ONE );
			r = r.add( xx.multiply( k ) );
			r = r.add( xx.multiply( xx ).multiply( k ) );
		}
		return r;
	}

	static BigInteger f3__( BigInteger x, int t ) {
		BigInteger r = BigInteger.ZERO;
		BigInteger k = BigInteger.valueOf( 3 );
		for(int i=0; i<t; i++) { 
			BigInteger xx = x.add( BigInteger.valueOf(i) );
			//r = r.add( BigInteger.ONE );
			r = r.add( xx.multiply( k ) );
			r = r.add( BigInteger.ONE );
		}
		return r;
	}

	static BigInteger f3___( BigInteger x, int t ) {
		BigInteger r = BigInteger.ZERO;
		BigInteger k = BigInteger.valueOf( 9 );
     	r = r.add( x.multiply( k ) );
		r = r.add( BigInteger.ONE );
		return r;
	}


	static BigInteger f2( BigInteger x, int t ) {
		BigInteger r = BigInteger.ZERO;
		BigInteger k = BigInteger.valueOf( 2 );
		for(int i=0; i<t; i++) { 
			BigInteger xx = x.add( BigInteger.valueOf(i) );
			r = r.add( BigInteger.ONE );
			r = r.add( xx.multiply( k ) );
		}
		return r;
	}

	static BigInteger f2_( BigInteger x, int t ) {
		BigInteger r = BigInteger.ZERO;
		BigInteger k0 = BigInteger.valueOf( 9 );
		BigInteger k1 = BigInteger.valueOf( 12 );
		for(int i=0; i<t; i++) { 
			BigInteger xx = x.multiply( x );
			xx = xx.multiply( x );
			r = r.add( k0 );
			r = r.add( xx.multiply( k1 ) );
		}
		return r;
	}
}
