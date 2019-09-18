'Example Console program 

'Let's clear the screen
Cls

'EmpytLine contains nothing, so we'll us it to print out nothing
Dim EmptyLine As String

'Print out a welcome
Print EmptyLine
Print EmptyLine
Print EmptyLine
Print EmptyLine
Print EmptyLine
Print EmptyLine
Print EmptyLine
Print EmptyLine
Print EmptyLine

Print "*******************************************************************************"
Print "*|---------------------------------------------------------------------------|*"
Print "*|                             Hello, World!                                 |*"
Print "*|             Welcome to the Open Source BASIC Compiler!                    |*"
Print "*|---------------------------------------------------------------------------|*"
Print "*******************************************************************************"
Sleep 4
Cls 

' Demonstrate how to work types and UDTs
Type PersonalInfo
  Age    As Integer
  Name   As String
  FavNum As Double
End Type

' Manually fill the stuff out
Dim MyInfo As PersonalInfo
MyInfo.Age = 99
MyInfo.Name = "Rocketman"
MyInfo.FavNum = 3.14^3.14

Print "                    Here's my life story in three sentences:                     "
Sleep 1
Print "My name is "; MyInfo.Name; " - Howdy!"
Sleep 1
Print "I am "; MyInfo.Age; " years old."
Sleep 1
Print "My favorite number is ", MyInfo.FavNum, " (yeah right)!"
Print EmptyLine
Sleep 3

' No control statements, so whatever you put gets discarded
Input "Are You Ready??? Yes or no?"; WorthlessVariable$
Print "Read or not, here comes the questionare!"
Print EmptyLine
Sleep 2

' Fill in the UDT via Input statements
Dim YourInfo As PersonalInfo

'Directions
Print EmptyLine
Print "       Now it's your turn to enter all the gory details of your life!          "
Sleep 3
Print "Please enter the following information:"
Sleep 2
Input "Name:            "; YourInfo.Name
Input "Age:             ";  YourInfo.Age
Input "Favorite Number: ", YourInfo.FavNum

Print EmptyLine
Print "Hello " + YourInfo.Name + ", how are you?"
Sleep 1
Print "In "; YourInfo.FavNum; " years, you will be "; YourInfo.FavNum + YourInfo.Age; "."
Sleep 2

' Now lets deal with arrays
Dim RandomNumbers(5) As Double

Print EmptyLine
Sleep 1
Print "Please type in random numbers:"
Input "Random Number 1: ", RandomNumbers(1)
Input "Random Number 2: ", RandomNumbers(2)
Input "Random Number 3: ", RandomNumbers(3)
Input "Random Number 4: ", RandomNumbers(4)
Input "Random Number 5: ", RandomNumbers(5)

Sum# = RandomNumbers(1) + RandomNumbers(2) + RandomNumbers(3) + RandomNumbers(4) + RandomNumbers(5)
Average# = Sum#/5
Print EmptyLine
Sleep 1
Print "Random Number Stats:"
Sleep 1
Print "Sum: "; Sum#
Sleep 1
Print "Average: "; Average#
Sleep 1
Print "Weird Calculation: "; Sum# * Average# / 2 ^ 4 +2

Sleep 2
Print EmptyLine
Print Emptyline
Print "                             Press ENTER to exit..."
Input "                                     ", A$

'Alternatively, you could use a sleep statement
'Pause for a while before quiting
'Sleep 4