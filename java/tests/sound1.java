
import javax.sound.sampled.*;


class sound1
{

	public static void main( String args[] ) {
		DataLine.Info lineInfo = new DataLine.Info( SourceDataLine.class, null );
		try
		{
			SourceDataLine line = (SourceDataLine) AudioSystem.getLine( lineInfo );
			p( line );
			p( line.getFormat() );
			p( line.getBufferSize() );
			line.open(line.getFormat());
			line.start( );
			byte[] sound = new byte[88200];
			for( int t = 1; t<6; t++) {
				for( int i = 0; i < 88200; i+=4 ) {
					double sin = Math.sin( (2 * Math.PI / 22050)* i * 50 * t );
					int amp = (int)(0xFFF * sin);
					sound[i] = (byte) (amp & 0xFF );
					sound[i+1] = (byte) (amp >> 8 & 0xFF );
					sound[i+2] = (byte) (amp & 0xFF );
					sound[i+3] = (byte) (amp >> 8 & 0xFF );
				}
				line.write( sound, 0, sound.length );
			}
			line.drain();
			line.close();
		}
		catch ( Exception e )
		{
			p(e);
		}
	}

	public static void p( Object args ) { System.out.println( args ); }

}