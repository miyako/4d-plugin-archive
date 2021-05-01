//%attributes = {}
$file:=Folder:C1567(fk desktop folder:K87:19).file("test.7z")

$folder:=Folder:C1567(fk desktop folder:K87:19)

$status:=archive read ($file.platformPath;$folder.platformPath)
