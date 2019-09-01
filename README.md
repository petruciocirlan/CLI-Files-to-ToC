# CLI Files to ToC

This tool was initially created to be used in my app for the digital skills certificate (senior year of highschool).

## What it does

Once opened up, the app compiles the paths to the folders and files (also the files from the subfolders etc.) 
from the 'data' folder (which needs to be in the same directory ass the program). The first thing you'll see 
is the Table of Contents which shows the 'chapters' (folders) and their 'sections' (files) from this 'book' that is
the 'data' folder. You can select one section at a time and read it (display its contents).

## How to use

This tool works just like any CLI. Use `HELP` to see a list of the available commands.  

|Commands|Usage|Role|
|---|---|---|
|`CLEAR`|`CLEAR`|Clears the console.|
|`CL`|`CL <section name>`|Changes the selected section. Use quotes when section the section name contains spaces ("<section name>"). You can also use just a prefix as long as it is not shared between multiple sections/chapters (ex: `CL Hel` will select section "Hello world", but not if there is another section with that prefix, such as "Hello Neighbour!").|
|`HELP`|`HELP`|Displays a list of the available commands.|
|`READ`|`READ`|Displays contents of selected section.|
|`CONTENT`|`CONTENT`|Displays the Table of Contents as seen when opening the app.|
|`EXIT`|`EXIT`|Closes the application.|

## How it works

The paths of the folders/files located the 'data' directory are parsed through using a trie.  
Every folder/file MUST be preceded by a 2 digits number and a space! That is the order of the chapters and their sections.  

## Example of 'data' directory

```
01 Chapter example
	01 Section example
	02 Section example
	03 Section Example
02 Chapter example
	01 Section example
	02 Section example
```