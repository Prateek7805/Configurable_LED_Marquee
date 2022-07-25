file = open("ASCII_Table1.h", 'r')
filedata = file.readlines();
file.close()
allArray = ""
twoDArray = ""
for i in range(len(filedata)):
    letter = filedata[i]
    array = letter[1 : letter.index('}')]
    allArray += "const uint8_t a"+str(i)+"[8] PROGMEM = {" +array+"};\n"
    twoDArray += f"a{i},"

twoDArray = "const uint8_t * const ASCIIArray[] PROGMEM = {" + twoDArray[0 : len(twoDArray)-1] + "};"
allArray += twoDArray
file = open("ASCII_PROGMEM_TABLE.h", 'w')
file.write(allArray)
file.close()
    
