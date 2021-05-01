//%attributes = {}

  //test symlink

$files:=New collection:C1472

$files.push("Macintosh HD:Applications:4D v17.5:4D.app:")

$archive:=Folder:C1567(fk desktop folder:K87:19).file("4D.7z")

$options:=New object:C1471
$options.keepParent:=True:C214
$options.skipHidden:=True:C214
$options.format:=".7z"

$status:=archive write ($files;$archive.platformPath;$options)
