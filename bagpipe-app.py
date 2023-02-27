import os
os.environ['PYGAME_HIDE_SUPPORT_PROMPT'] = "hide"

import pygame.midi
import time
import fluidsynth

fs = fluidsynth.Synth()
fs.start(driver="alsa")
fs.setting('synth.gain', 0.2)
sfid = fs.sfload("galician-bagpipe.sf2")
fs.program_select(0, sfid, 0, 0)

def getBagpipeMidiDevice():
    pygame.midi.init()
    for device_index in range(pygame.midi.get_count()):
        (_, name, input, _, _) = pygame.midi.get_device_info(device_index)

        if ("Arduino Leonardo" in str(name)) and input:
            return device_index
    
    pygame.midi.quit()
    return None

pygame.init()

while True:
    input_id = getBagpipeMidiDevice()
    if input_id is None:
        print("Bagpipe MIDI device not found. waiting for device...")
        time.sleep(0.1)
    else:
        break

clock = pygame.time.Clock()
crashed = False

print("input:")
a = pygame.midi.Input(input_id)

while (crashed == False):
    readed = a.read(1)
    if readed != []:
        on = (readed[0][0][0] == 144)
        note = readed[0][0][1]
        velocity = readed[0][0][2]
        print(f"note {note} on {on} at velocity {velocity}")
        if (on):
            fs.noteon(0, note, 127)
        else:
            fs.noteoff(0, note)
