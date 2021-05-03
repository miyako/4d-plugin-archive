# 4d-plugin-archive
Utility based on libarchive

#### About this project

4D `zip` commands are based on [nih-at/libzip](https://github.com/nih-at/libzip).

`.4darchive` is a proprietary format that works with a `cache.xml` file, but will soon be replaced by `.zip`.

For standard archives, the native `zip` commands are sufficient; **there is no need for a plugin**.

The purpose of this project is to support `.7z` and other archive formats to store away large but highly compressable files such as `.4BK`.

The operation is typically performed on the server, require significant computation power and may take a while to complete.

Nevertheless its **impact on server performance** should be minimal.

#### Design principle

In order not to slow down the server during compression, and in order to be performant even if the application is running in interpretative mode, the plugin should perform compression in a **background thread**.

Instead of a callback method, the plugin should run asynchronously; the command should return a reference immediately, which can be used in 4D to query status at any time.

The total size is calculated first in the calling process. After that, the archive will be elegated to a new thread. 

The preprocessing should be relatively fast for `archive write`. The command will add up the actual file size as well as the extended attributes on Mac. Creating a 7-zip format will take more time, since .7z requires intense usage of the CPU.

By contrast, the preprocessing could be relatively slow for `archive read`. The command will add up the `archive_entry_size` of each item in the archive. Expanding a 7-zip format should be quite fast.

#### Technology

[libarchive](https://www.libarchive.org)

* Definition of hidden item on Mac:
 
  * `NSURL` `-getResourceValue:forKey:NSURLIsHiddenKeyerror:`
  * `path.at(0) == '.'`
  * `path.find("/.") != std::string::npos`

* Options for genral purpose:
  * noDotDot (default=false) 
  * noAbsolutePaths (default=false)
  * noAutoDir (default=false) 
  * atomic (default=false)
  * noHFSCompression (default=false)
  * forceHFSCompression (default=false)
  * clearNoChangeFlags (default=false)
  * unlink (default=false)
  * sparse (default=false)
  * metadata (default=true)
  * owner (default=true)
  * time (default=true)
  * perm (default=true)
  * ACL (default=true)
  * fflags (default=true)
  * xattr (default=true)
  * noOverWrite (default=true)
  * noOverWriteNewer (default=true)
  * secureSymlinks (default=true)

* Options for archive:
  * skipHidden (default=false) 
  * keepParent (default=false) 
  * format (default=.7z) 

* Options for .zip archive:

  * passphrase
  * compression (store or deflate)

* Options for unarchive: none (automatic)

* Supported formats: 

  * .7z
  * .zip = .jar
  * .cpio
  * .iso
  * .a = .ar
  * .tar
  * .tgz = .tar.gz
  * .tar.bz2
  * .tar.xz

#### Compress

```4d
$file:=Folder(fk desktop folder).file("test.7z")
$folder:=Folder(fk desktop folder).folder(Generate UUID)
$folder.create()

$options:=New object
$options.passphrase:="1234d"  //this option is only applicable for .zip; it does not work for .7z

$status:=archive read ($file.platformPath;$folder.platformPath;$options)

If ($status.uuid#Null)
	
	$uuid:=$status.uuid
	
	Repeat 
		
		$status:=archive get progress ($uuid)
		
		$message:=New collection($status.progress;"/";$status.total).join()
		
		MESSAGE($message)
		
	Until ($status.complete)
	
	$status:=archive abort ($uuid)
	
End if 
```

#### Expand

```4d
$files:=New collection
If (Is macOS)
	$files.push("Macintosh HD:Applications:4D v17.5:4D.app:")
End if 

If (Is Windows)
	$files.push("C:\\Program Files\\4D\\4D v18.4\\4D")
End if 

$folder:=Folder(fk desktop folder).folder(Generate UUID)

$options:=New object
$options.keepParent:=True
$options.skipHidden:=True
$options.format:=".7z"

$status:=archive write ($files;$folder.platformPath;$options)

If ($status.uuid#Null)
	
	$uuid:=$status.uuid
	
	Repeat 
		
		$status:=archive get progress ($uuid)
		
		$message:=New collection($status.progress;"/";$status.total).join()
		
		MESSAGE($message)
		
		DELAY PROCESS(Current process;10)
		
	Until ($status.complete)
	
	$status:=archive abort ($uuid)
	
End if 

ALERT("done!")
```
