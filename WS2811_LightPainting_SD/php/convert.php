<?php

if($argc<=1) die("Usage: php convert.php <filename>.png\n");
$file_export = fopen("data.hex", "w");

$image_size = getimagesize($argv[1]);

print_r($image_size);

fwrite( $file_export, chr($image_size[1]/256) );
fwrite( $file_export, chr($image_size[1]%256) );

fwrite( $file_export, chr($image_size[0]/256) );
fwrite( $file_export, chr($image_size[0]%256) );

$img_import = imagecreatefrompng($argv[1]);

for( $x = 0 ; $x < $image_size[0] ; $x++ ) {
	for( $y = $image_size[1]-1 ; $y >= 0 ; $y-- ) {
		$color = imagecolorat($img_import, $x, $y);
		$color_r = ($color >> 16) & 0xFF;
		$color_g = ($color >>  8) & 0xFF;
		$color_b = ($color      ) & 0xFF;

		fwrite( $file_export, chr($color_r) );
		fwrite( $file_export, chr($color_g) );
		fwrite( $file_export, chr($color_b) );
	}
}

fclose($file_export);
?>
