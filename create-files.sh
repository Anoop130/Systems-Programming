# list of file names
files=("1.C++ vs C" "2.Reading and Writing Data 1" 
	"3.Reading and Writing Data 2" "4.Reading values at regular time intervals"
	"5.A Timed Quiz" "6.Handling multiple signals from the child"
	"7.The Hi-Lo Game implemented with Multiple Processes"
	"8.Using exit, exec, and wait")

# creating files for each file name
for file in "${files[@]}"; do
	mkdir "$file"
done


