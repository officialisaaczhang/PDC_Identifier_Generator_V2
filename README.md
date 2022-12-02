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
        Send the PDC clearance through private chat.
        
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
*For older version of ES: Click F on the Departure List and check the PDC box

To Use:

  - The tracker will change colors automatically according to the status of the PDC assigned.

        - Amber: A PDC assigned, awaiting pilot's readback.
        - Green: Pilot's readback is correct.
        - Red: A previously assigned PDC is deleted. This will stay to remind controller a previous assigned PDC is deleted.
        
   - The only time controllers need to click on the tracker to change status is when the readback from pilot is correct, thus, changing the color from Amber to Green.
        
  Any feedback is welcomed.
  
  WIP in future updates:
  
  User Customization;
  Pilot initiated PDC;
  Custom list of PDC assigned
