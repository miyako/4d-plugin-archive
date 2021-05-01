//%attributes = {}

  //test symlink

$files:=New collection:C1472

If (Is macOS:C1572)
	$files.push("Macintosh HD:Applications:4D v17.5:4D.app:")
End if 

If (Is Windows:C1573)
	$files.push("C:\\Program Files\\4D\\4D v18.4\\4D")
End if 

$archive:=Folder:C1567(fk desktop folder:K87:19).file("4D.7z")

$options:=New object:C1471
$options.keepParent:=True:C214
$options.skipHidden:=True:C214
$options.format:=".7z"

$status:=archive write ($files;$archive.platformPath;$options)
