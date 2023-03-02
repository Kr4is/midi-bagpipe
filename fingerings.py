#!/usr/bin/env python3

# midi-bagpipe fingerings generator
# This script creates the structures for use in midi-bagpipe based on
# fingering tables defined here in human readable form

# FINGER POSITION DEFINED BY A STRING LIKE "XX XXX XXXX"
# BLANKS ARE MANDATORY
# WHERE X IS:
#  C: FINGER IS CLOSED
#  O: FINGER IS OPEN
#  -: FINGER DOES NOT CARE
# FINGERS FROM LEFT TO RIGHT IN THE STRING (UP TO DOWN IN THE CHANTER):
# LEFT THUMB TOP
# LEFT THUMB BOTTOM
# LEFT INDEX
# LEFT MIDDLE
# LEFT RING
# RIGH INDEX
# RIGHT MIDDLE
# RIGHT RING
# RIGHT LITTLE

#http://www.phys.unsw.edu.au/jw/notes.html

#GAITA GALEGA
start_note_number = 59
galician_mastergaita=(
	("-C CCC CCCC",), # B4
	("-C CCC CCCO",), # C5
	("-C CCC CCOC",), # C#5
	("-C CCC CCOO",), # D5
	("-C CCC COCO",), # Eb5
	("-C CCC COO-", "-C CCC COCC"), # E5
	("-C CCC O---", "-C CCC OCC-"), # F5
	("-C CCO C-OO", "-C CCO CCCC"), # F#5
	("-C CCO O---", "-C CCO CCCO"), # G5
	("-C COC ----",), # Ab5
	("-C COO ----",), # A5
	("-C OCO ----",), # Bb5
	("-C OOO ----", "-O CCC CCCC", "-C OCC CCCC"), # B5
    ("-O OOO ----", "-O CCC CCCO", "-C OCC ---O"), # C6
	("-O CCC CCOC",), # C#6
	("-O CCC CCOO",), # D6
	("-O CCC COCO",), # Eb6
	("-O CCC COO-", "-O CCC COCC"), # E6
	("-O CCC O---",), # F6
	("-O CCO C-OO", "-O CCO CCCC"), # F#6
    ("-O CCO O---", "-O CCO CCCO"), # G6
    ("-O COC ----",), # Ab6
    ("-O COO ----",), # A6
    ("-O OCO ----",), # Bb6
    ("-O OCC CCCC",), # B6
    ("-O OCC CCCO",), # C7
)


def generate_possitions(possition):
	final_possitions = []
	changed = False
	for pos in (10,9,8,7,5,4,3,1):
		if possition[pos]=='-':
			changed = True
			new_possition = list(possition)
			new_possition[pos] = 'C'
			sring_possition = "".join(new_possition)
			for generted_possition in generate_possitions(sring_possition):
				final_possitions.append(generted_possition)
			new_possition = list(possition)
			new_possition[pos] = '0'
			sring_possition = "".join(new_possition)
			for generted_possition in generate_possitions(sring_possition):
				final_possitions.append(generted_possition)
	if changed == False:
		final_possitions.append(possition)
	return [*set(final_possitions)]

def get_value_from_possition(possition):
	possitions_values = []
	for full_possition in generate_possitions(possition):
		possitions_values.append(get_value_from_full_possition(full_possition))
	return possitions_values

values = [1, 2, 4, 8, 16, 32, 64, 128, 1024]

def get_value_from_full_possition(fingers_possitions):
		possition_value = 0
		for i, pos in enumerate((10,9,8,7,5,4,3,1)):
			if fingers_possitions[pos]=='C':
				possition_value+= values[i]
		possition_value+= values[8]
		return possition_value

def get_notes_and_possitions():
	notes_and_possitions = []
	start_note = start_note_number
	for note_and_possitions in galician_mastergaita:
		note = start_note
		possitions = note_and_possitions
		for possition in possitions:
			possition_values = get_value_from_possition(possition)
			for possition_value in possition_values:
				notes_and_possitions.append((note, possition_value))
		start_note+=1
	return notes_and_possitions

def main():
	notes_and_possitions = get_notes_and_possitions()
	notes = []
	possitions = []
	for note_and_possition in notes_and_possitions:
		notes.append(note_and_possition[0])
		possitions.append(note_and_possition[1])
	print(notes)
	print(possitions)
	print(len(notes))

if __name__ == "__main__":
	main()