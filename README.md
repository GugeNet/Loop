# Loop
4-track Eurorack Looper

A 4 hp wide 3U Eurorack module designed to be as simple as possible, but not simpler.

The user interface is limited but based on an idea of creating a linguistic model.

The commands consists of verbs: record, clear and loop. There are buttons for record and clear, a trigger input for loop.
In most situations there is also an implicit verb, such as mute.
The time aspect in the commands are either immidiate or suspended until loop. A short press means now, a long press means after the next loop command.
The objects are the four tracks.
The module will ask the user to select track by blinking the four track-buttons, or simetimes only the applicable tracks if appropriate.

Examples:

While playing click a green button and the track is muted and the button goues off.
Press the button for half a second instead of cust clicking and the button will blink green and start playing after the next loop.

Hold down record and notice all tracks blinking red as the module wants to know where to record. You can press one or more tracks and they will record after loop.

Another foundational idea is that every button has a function in any situation. There is no illegal action and the module is never unresponsive. The user is boss and presumably always has some intention.

So if the button for a blank track is pressed, it is presumed that record is the correct operation, while play is implicit for a track with sound.

The connections are stereo inputs and stereo outputs and a trigger input for loop.

There are six buttons. One bi-color illuminated pushbutton for each of the four tracks, one for clearing and one for record.

The basic operation of the module is to record to or play from four sound buffers and then on trigger loop to the beginning.

The module will always pass incoming sound through. Recording is additive, that is to say it doesn't remove existing sound from the track before recording.
Recording takes just the incoming sound.

Playing also adds sound to the incoming sound and when several tracks are playing they are all added equally to the incoming sound.









On reception of the loop trig the following scenarios are performed:

Playing -> Playing
Recording -> Playing
Muted -> Muted

The user can cue playing, recording or muting to happen on loop.

Record -> Track (long)
Track (long)
Clear -> Track (long)
