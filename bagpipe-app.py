import os
os.environ['PYGAME_HIDE_SUPPORT_PROMPT'] = "hide"

import pygame.midi
import time
import fluidsynth

fs = fluidsynth.Synth()
fs.start(driver="alsa")
fs.setting('synth.gain', 0.2)
# sfid = fs.sfload("galician-bagpipe-full-notes.sf2")
sfid = fs.sfload("represas_2.sf2")
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
drones_playing = False
last_play_state = False
last_read = time.time()
while (crashed == False):
    readed = a.read(1)
    if readed != []:
        on = (readed[0][0][0] == 144)
        last_play_state = on
        note = readed[0][0][1] + 12
        velocity = readed[0][0][2]
        print(f"note {note} on {on} at velocity {velocity}")
        if (on):
            if not drones_playing:
                # START DRONES
                fs.noteon(0, 48, 127)
                fs.noteon(0, 60, 127)
            fs.noteon(0, note, 127)
            drones_playing = True
        else:
            fs.noteoff(0, note)
        last_read = time.time()
    
    if (time.time() - last_read > 0.5) and (not last_play_state) and drones_playing:
        # STOP DRONES
        fs.noteoff(0, 48)
        fs.noteoff(0, 60)
        drones_playing = False
