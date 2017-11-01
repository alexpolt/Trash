
import java.awt.event.*;
import javax.swing.*;
import java.awt.*;
import java.io.*;
import javax.sound.sampled.*;
import javax.sound.midi.*;


class midi1
{

	public static void main( String args[] ) {
		p("START");
		try
		{
			MidiDevice.Info[] devices = MidiSystem.getMidiDeviceInfo();
			MidiDevice device = null;
			for( MidiDevice.Info deviceInfo: devices ) {
				p( deviceInfo );
				MidiDevice _device = MidiSystem.getMidiDevice( deviceInfo );
				if( _device instanceof Synthesizer ) device = _device;
			}
			p( "\nChoosed: " + device );
			//if( ! device.isOpen() ) device.open();

			Synthesizer s = MidiSystem.getSynthesizer();
			p( s );
			if( ! s.isOpen() ) s.open();

			Soundbank bank = MidiSystem.getSoundbank( new File("p:\\java\\JavaSoundDemo\\soundbank-deluxe.gm") );
			p( "Sound bank: " + bank  );
			p( "Sound bank supported: " + s.isSoundbankSupported( bank ) );

			Instrument piano = bank.getInstrument( new Patch( 0, 0 ) );
			p("Instrument: " + piano );
			s.loadInstrument( piano );

			MidiChannel[] channels = s.getChannels();
			MidiChannel channel = channels[0];
			p( "Channel: " + channel );
			channel.programChange( 0, 0 );

			int[] seq = new int[] { 60, 200, 61, 150, 70, 200 };
			for( int t = 1; t < 4; t += 1 )
			for( int i = 0; i < seq.length; i +=2 ) {
				int note = seq[ i ];
				int vel = 100;
				int sleep = seq[ i+1 ];
				channel.noteOn( note, vel );
				Thread.sleep( sleep*t );
				channel.noteOff( note );
			}

			channel.allNotesOff();
			Thread.sleep( 2000 );


			s.close();
		}
		catch ( Exception e ) { p(e); }

		p("FINISH");
	}

	public static void p( Object args ) { System.out.println( args ); }

}

