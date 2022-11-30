# PDC_Identifier_Generator_V2
A Euroscope Plugin that generates PDC Identifier for PDC clearance in the format of ###X

PDC_identifier Assigner for Euroscope

Set-up:

Other SET -> Plug-ins -> Load -> Load PDC_identifier.dll
Choose Standard ES radar screen from the forbidden
Add it to the allowed to draw on types "<<"
Hit S on the Departrue list
Add Item -> Tag Item type "PDC_identifer/PDC" -> Header name "PDC"
Left Button "PDC/Grant PDC" -> Right button "PDC/Reset"
Click F on the Departure List and check the PDC box

To Use:

 - Initial assignment:
        Check everything is set on the flight strip.
        Left click on the column under PDC.
        A dialogue will pop up showing a preview of the PDC message.
        Once hit "Yes", the PDC clearance will be copied to the clipboard.
        Send the PDC clearance through private chat.
        
  - Re-assignment:
        When there is an existing identifier assigned, a pop up will confirm with you about reassignment of the identifier.
        Proceed like the initial assignment when preview window shows.
        
  - Delete:
        Simply right click to delete the Code.
        
  Any feedback is welcomed.
