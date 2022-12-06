# PDC_Identifier_Generator_V2
A Euroscope Plugin that generates PDC Identifier for PDC clearance in the format of ###X

PDC_identifier Assigner for Euroscope

Set-up:

Other SET -> Plug-ins -> Load -> Load PDC_identifier.dll
Choose Standard ES radar screen from the forbidden
Add it to the allowed to draw on types "<<"
Hit S on the Departrue list
Add Item -> Tag Item type "PDC_identifer / PDC" -> Header name "PDC"
Left Button "PDC_identifier / Grant PDC" -> Right button "PDC / Reset"

*For older version of ES: Click F on the Departure List and check the PDC box

To Use:

 - Initial assignment:
        Check everything is set on the flight strip.
        Left click on the column under PDC.
        A dialogue will pop up showing a preview of the PDC message.
        Once hit "Yes", the PDC clearance will be copied to the clipboard.
        The message copied will contain the ".msg Callsign" part automatically.
        Paste and send the PDC clearance through chat box.
        
  - Re-assignment:
        When there is an existing identifier assigned, a pop up will confirm with you about reassignment of the identifier.
        Proceed like the initial assignment when preview window shows.
        
  - Delete:
        Simply right click to delete the Code.
        
A Readback status tracker is also made to help controllers track the status of the PDC readback.

Set-up:

Hit S on the Departrue list
Add Item -> Tag Item type "PDC_identifer / PDC Flag" -> It is recommanded to use a single letter Header name and set Width to "1"
Left/Right Button "PDC_identifier / Set Status"

*For older version of ES: Click F on the Departure List and check the box named with Header name.

To Use:

  - The tracker will change colors automatically according to the status of the PDC assigned.

        - Amber: A PDC assigned, awaiting pilot's readback.
        - Green: Pilot's readback is correct.
        - Red: A previously assigned PDC is deleted. This will stay to remind controller a previous assigned PDC is deleted.
        
   - The only time controllers need to click on the tracker to change status is when the readback from pilot is correct, thus, changing the color from Amber to Green.
   
   - Due to limitation of ES, controller who logs on to a new position (especially a lower position) needs to sync codes and status manually. This can be done by sending a private message to another controller. The message should be as follows:
  
     "syncPDC"
        
  Any feedback is welcomed.
  
  WIP in future updates:
  
  User Customization;
  Pilot initiated PDC;
  Custom list of PDC assigned
