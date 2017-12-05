
no utf8;

open $f1, '<mylog72patch1.bin';
open $f2, '+>mylog2patch.bin';

binmode $f1;
binmode $f2;

my $data1, data2;

#read $f1, $data1, 2;
my $c = 0;
while( $n=read $f1, $data1, 1024 ) {
$c += 1024;
	if($n == 1024 ) {
	for($i=0; $i < 512; $i++) {
		substr ($data2, $i*2, 2) =  substr ($data1, $i*2+1, 1)   .  (substr $data1, $i*2, 1);
	}}

	print $f2 $data2;
}
#for($i=$c+1;$i < 1<<22; $i++) {
#	print $f2 pack "C", 0xff;
#}
#print $f2 pack "C", 0xff;


close $f1;
close $f2;


