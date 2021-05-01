//%attributes = {}
$version:=archive version ()

  //SET TEXT TO PASTEBOARD(JSON Stringify($version;*))

/*

Windows:

{
"zstd": "1.4.5",
"bz": "1.0.6, 6-Sept-2010",
"lzma": "5.2.3",
"z": "1.2.11",
"archive": "libarchive 3.5.1",
"lz4": "1.9.2"
}

eay32,iconv,charset,bz2,ws2_32,lzma,xml2,zlib is included in the plugin

*/

/*

Mac:

{
"zstd": "1.4.9",
"bz": "1.0.6, 6-Sept-2010",
"lzma": "5.2.5",
"z": "1.2.11",
"archive": "libarchive 3.5.1",
"lz4": "1.9.3"
}

libz,libbz2 is linked to the system
lz4 is included in libarchive
lzma,iconv,xml2 is included in the plugin

*/