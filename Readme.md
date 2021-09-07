# MIDI Converter and Visualizer
![\image html](https://media.tenor.com/images/dee19b8c77e9f3d7bb8d79fd1fe254e6/tenor.gif) 

## Usage:
-j [input_files] - JSON mode: convert from json to midi  
-m [input_files] - MIDI mode: convert from midi to json  
-r [-j ^ -m] root_directory - RECURSIVE mode: convert json or midi files recursively  
-v midi_file - VISUAL mode: starts piano roll visualization of midi file  

-e extension - appends extension to output files  
-t tempo - specifies the tempo of the .mid output, only applied with -j  
-i instrument_number - specifies the instrument of the .mid output, only applied with -j  

Key order in json format must be:  
[  
{ "note": 63, "start": 0, "duration": 0.1, "velocity": 127 },  
                                ...  
{ "note": 64, "start": 54.2, "duration": 0.1, "velocity": 127 }  
]  

## Instrument options:
![\image html](https://i.ibb.co/k60fDg6/Screenshot-from-2021-05-24-17-08-46.png) 

## Notes:
This program uses the following standard for midi files:
http://www.music.mcgill.ca/~ich/classes/mumt306/StandardMIDIfileformat.html
