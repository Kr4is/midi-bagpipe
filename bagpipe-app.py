import pygame.midi
import time
import os
import fluidsynth

fs = fluidsynth.Synth()
fs.start(driver="alsa")
fs.setting('synth.gain', 0.2)
sfid = fs.sfload("galician-bagpipe.sf2")
fs.program_select(0, sfid, 0, 0)
last_note = None

def printMIDIDeviceList():
        for i in range(pygame.midi.get_count()):
            print(pygame.midi.get_device_info(i), i)

pygame.init()
pygame.midi.init()

printMIDIDeviceList()

print("The default input device number is "  + str(pygame.midi.get_default_input_id()))
input_id = int(pygame.midi.get_default_input_id())
input_id = 3

clock = pygame.time.Clock()
crashed = False

print("input:")
a = pygame.midi.Input(input_id)

while (crashed == False):
    readed = a.read(1)
    if readed != []:
        on = (readed[0][0][0] == 144)
        note = readed[0][0][1]
        # print(f"note {note} on {on}")
        if (on):
            fs.noteon(0, note, 60)
        else:
            fs.noteoff(0, note)
