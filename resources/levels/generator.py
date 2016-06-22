#generate a blank level xyz dimensions
#gradually build up to more complex if desired

print("Welcome to the level generator.\n"
      "We are at version 0.1! To begin, please enter:")
w = input("The width:")
d = input("The depth:")
h = input("The height:")

print("Just one second!")

f = open("map", "w")

f.write(w + " " + d + " " + h + "\n")

for i in range(int(h)):
    f.write("\\\n")
    print("...")
    for j in range(int(d)):
        for k in range(int(w)):
            f.write("0  -")
        f.write("\n")

input("Should be done :)")

f.close()


