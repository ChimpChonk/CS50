import csv
import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        sys.exit("Incorrect argument")

    # TODO: Read database file into a variable
    db = []
    with open(sys.argv[1]) as file:
        reader = csv.DictReader(file)
        for row in reader:
            db.append(row)

    # TODO: Read DNA sequence file into a variable
    with open(sys.argv[2], "r") as file:
        dna_seq = file.read()

    # TODO: Find longest match of each STR in DNA sequence
    dna_subse = list(db[0].keys())[1:]

    long_match = {}
    for subseq in dna_subse:
        long_match[subseq] = longest_match(dna_seq, subseq)

    # TODO: Check database for matching profiles
    for person in db:
        seq_count = 0
        for subseq in dna_subse:
            if int(person[subseq]) == long_match[subseq]:
                seq_count += 1
                # print(f"{subseq}: {match}")
        if seq_count == len(dna_subse):
            print(person["name"])
            sys.exit()
    print("No match")
    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
