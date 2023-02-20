#!/usr/bin/env python

# OPENPIPE FINGERINGS
# This script creates <fingerings.h> for use in OpenPipe based on
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
galician_mastergaita=(
	(59,("-C CCC CCCC",)), # B3
	(60,("-C CCC CCCO",)), # C4
	(61,("-C CCC CCOC",)), # C#4
	(62,("-C CCC CCOO",)), # D4
	(63,("-C CCC COCO",)), # Eb4
	(64,("-C CCC COO-", "-C CCC COCC")), # E4
	(65,("-C CCC O---", "-C CCC OCC-")), # F4
	(66,("-C CCO C-OO", "-C CCO CCCC")), # F#4
	(67,("-C CCO O---", "-C CCO CCCO")), # G4
	(68,("-C COC ----",)), # Ab4
	(69,("-C COO ----",)), # A4
	(70,("-C OCO ----",)), # Bb4
	(71,("-C OOO ----", "-O CCC CCCC", "-C OCC CCCC")), # B4
    (72,("-O OOO ----", "-O CCC CCCO", "-C OCC ---O")), # C5
	(73,("-O CCC CCOC",)), # C#5
	(74,("-O CCC CCOO",)), # D5
	(75,("-O CCC COCO",)), # Eb5
	(76,("-O CCC COO-", "-O CCC COCC")), # E5
	(77,("-O CCC O---",)), # F5
	(78,("-O CCO C-OO", "-O CCO CCCC")), # F#5
    (79,("-O CCO O---", "-O CCO CCCO")), # G5
    (80,("-O COC ----",)), # Ab5
    (81,("-O COO ----",)), # A5
    (82,("-O OCO ----",)), # Bb5
    (83,("-O OCC CCCC",)), # B5
    (84,("-O OCC CCCO",)), # C6
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
	for note_and_possitions in galician_mastergaita:
		note = note_and_possitions[0]
		possitions = note_and_possitions[1]
		for possition in possitions:
			possition_values = get_value_from_possition(possition)
			for possition_value in possition_values:
				notes_and_possitions.append((note, possition_value))
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