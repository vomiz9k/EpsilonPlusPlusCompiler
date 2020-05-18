print("Input file name: ", end = "")
finput = input()
finput = open(finput, 'r', encoding="utf-8")
foutput = open("output.txt", 'w', encoding="utf-8");
searching = ["→ console", "console →", "∀ε>0", "∃", "∀", "⇒", "⊥", "Читд", "→ ℝ", "→", "*", "/", "+", "-", "=", "^", ";", "⇔", "(", ")", ">", "<", ">  =", "<  =", "! =", ","]
replasing = ["print ", "scan","main", "double ", "while ", " do: { ", "else ", "end", "", "return ", " * ", " / ", " + ", " - ", " = ", " ^ ", " } ", " == "," ( ", " ) ", " > ", " < ", ">=", "<=", "!=", " , "]
for line in finput:
	curr_pos = 0
	new_line = str()	
	for i in range(len(searching)):
		if line.count(searching[i]) > 0:
			if (searching[i] == "→ console") :
				line = "\t" * line.count("\t") + replasing[i] + line[line.rfind("\t") + 1:line.find(searching[i])] + line[line.find(searching[i]) + len(searching[i]):]
			else:
				line = line.replace(searching[i], replasing[i])
	foutput.write(line)
foutput.write("\n");