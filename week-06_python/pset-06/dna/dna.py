'''
arg1 = CSV-file with Str-Counts
arg2 = txt-file with DNA-Strs
'''

import sys
import csv


# Main function
def main():

    # Check number of commandline arguments
    if not len(sys.argv) == 3:
        print("Usage: python dna.py data.csv sequence.txt")
        sys.exit(1)

    # Create Dict csv_dict, that assigns to each name x dicts that contain str-name: number
    csv_dict = {}
    longest = {}

    # Open and load CSV in a dict with list of str-numbers as arg2
    with open(sys.argv[1], 'r') as fcsv:
        reader = csv.reader(fcsv)

        # Get header of csv and create a list of strs
        row0 = next(reader)
        x = len(row0)

        definition = []
        for i in range(1, x):
            definition.append(row0[i])

        # Fill csv_dict
        for row in reader:
            name = row[0]
            str_fields = {}
            for i in range(1, x):
                str_fields[row0[i]] = row[i]
            csv_dict[name] = str_fields

    # Open and load dna-string[0] with it
    with open(sys.argv[2], 'r') as fdna:
        reader = csv.reader(fdna)
        dna_string = next(reader)

    # Calculate the longest run of consecutive repeats of each str and add results to longest-dict
    for line in definition:
        result = longest_repeat(line, dna_string[0])
        longest[line] = result

    # Find person whose str-number matches the longest occurence of this str in the dna_string
    person = ''
    for row in csv_dict:
        name = row
        match = 0
        for line in definition:
            str_seq = line
            if int(csv_dict[name][str_seq]) == int(longest[str_seq]):
                match += 1
        if match == len(definition):
            person = name
            break

    # Print result
    if person == '':
        print("No match")
    else:
        print(person)


def longest_repeat(line, dna_string):
    finds = []
    start = 0
    start_new = 0
    count = 0
    length_str = len(line)

    # Only keep counting beyond 1 if the following string has the same sequenz.
    # Else => append count-result to finds and start new counting
    while not dna_string.find(line, start) == -1:
        pos = dna_string.find(line, start)
        if count == 0:
            count += 1

        start_new = pos + 1
        if dna_string.find(line, start_new) == start_new + length_str - 1:
            count += 1
        else:
            finds.append(count)
            count = 0

        # Sort finds and adjust start for search
        finds.sort()
        start = start_new

    # Return highest number of repeats if existent
    if len(finds) > 0:
        result = finds[len(finds) - 1]
    else:
        result = 0
    return result


# Start
if __name__ == "__main__":
    main()
