//%attributes = {}
$file:=Folder:C1567(fk desktop folder:K87:19).file("test.7z")
$folder:=Folder:C1567(fk desktop folder:K87:19).folder(Current method name:C684)
$folder.delete(Delete with contents:K24:24)
$folder.create()

$options:=New object:C1471
$options.passphrase:="1234d"  //this option is only applicable for .zip; it does not work for .7z

$status:=archive read ($file.platformPath;$folder.platformPath;$options)

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