//%attributes = {}
$files:=New collection:C1472

If (Is macOS:C1572)
	$files.push("Macintosh HD:Applications:4D v17.5:4D.app:")
	  //$files.push(Folder(fk desktop folder).file("706.svg.pdf").platformPath
End if 

If (Is Windows:C1573)
	$files.push("C:\\Program Files\\4D\\4D v18.4\\4D")
End if 

$archive:=Folder:C1567(fk desktop folder:K87:19).file("test.7z")

$options:=New object:C1471
$options.keepParent:=True:C214
$options.skipHidden:=True:C214
$options.format:=".7z"

$status:=archive write ($files;$archive.platformPath;$options)

If ($status.uuid#Null:C1517)
	
	$uuid:=$status.uuid
	
	Repeat 
		
		$status:=archive get progress ($uuid)
		
		$message:=New collection:C1472($status.progress;"/";$status.total).join()
		
		MESSAGE:C88($message)
		
		DELAY PROCESS:C323(Current process:C322;10)
		
	Until ($status.complete)
	
	$status:=archive abort ($uuid)
	
End if 

ALERT:C41("done!")