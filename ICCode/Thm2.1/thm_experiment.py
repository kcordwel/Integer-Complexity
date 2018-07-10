# This code was written for the purposes of the paper "On Algorithms to Calculate Integer Complexity" by Cordwell, Epstein, Hemmady, Miller, Palsson, Sharma, Steinerberger, and Yen Nhi Truong Vu.

# To obtain the complexities in integer_complexities.txt, we both run Fuller's code and manually access/input values from the database of integer complexities that Iraids et. al. developed (see their paper "Integer Complexity: Experimental and Analytical Results")

# with thanks to https://stackoverflow.com/questions/6583573/how-to-read-numbers-from-file-in-python
def main():
    fDict = open("integer_complexities.txt", "r")
    fDBR = open("dbr_to_check.txt", "r")

    fSecondPass = open("dbr_second_pass.txt", "w")

    # initialize the dictionary
    compl_dictionary = {}

    for line in fDict:
        number, compl = [int(x) for x in line.split()]
        compl_dictionary[number] = compl

    # do a first pass and figure out the bad numbers
    count = 0
    for line in fDBR:
        b, r = [int(x) for x in line.split()]

        b1 = b
        powOfTwo = 0
        powOfThree = 0
        while b1 % 2 == 0:
            powOfTwo += 1
            b1 = b1/2
        while b1 % 3 == 0:
            powOfThree += 1
            b1 = b1/3
        # We know that the complexity of b, or f(b), is 2*powOfTwo + 3*powOfThree
        if(compl_dictionary[b + r] != 2*powOfTwo + 3*powOfThree + 1):
            # in our second pass, we'll look at the complexities of 2*b + r
            fSecondPass.write(str(b) + ", " + str(r) + "\n")
            num_secondPass = b*2 + r
            if (compl_dictionary[2*b + r] != 2*powOfTwo + 3*powOfThree + 3):
                count = count + 1
    print(count)

if __name__ == '__main__':
    main()
