#!/Applications/MAMP/bin/php5/bin/php
<?php
$image_info = getimagesize($argv[1]);

$image_info['x'] = $image_info[0];
$image_info['y'] = $image_info[1];

// print_r($image_info); die();

$image = imagecreatefrompng($argv[1]);



echo "byte matrix[".$image_info['x']."][".($image_info['y']/8)."] = {\n";

for( $x = 0 ; $x < $image_info['x'] ; $x++ ) {
	
	echo "	{\n";
	for( $y_byte = 0 ; $y_byte < $image_info['y']/8 ; $y_byte++ ) {
		$octet = 0;
		for( $y_bit = 0 ; $y_bit < 8 ; $y_bit++ ) {
			$color = imagecolorat($image, $x, ($y_byte*8)+$y_bit);
			$octet = $octet*2;
			$octet = $octet+$color;
		}
		echo "		0b";
		printf("%08b",$octet);
		echo ",\n";
	}
	echo "	},\n";
}

echo "}\n";
?>