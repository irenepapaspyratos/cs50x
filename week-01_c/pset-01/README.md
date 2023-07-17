# Hello

https://cs50.harvard.edu/x/2023/psets/1/hello/

Modify `./hello` in such a way that it first prompts the user for their name and then prints hello, so-and-so, where so-and-so is their actual name.

# Mario More

https://cs50.harvard.edu/x/2023/psets/1/mario/more/

Toward the beginning of World 1-1 in Nintendo’s Super Mario Brothers, Mario must hop over adjacent pyramids of blocks, per the below.   

Let’s recreate those pyramids in C, albeit in text, using hashes (#) for bricks, a la the below. Each hash is a bit taller than it is wide, so the pyramids themselves will also be taller than they are wide. The program we’ll write will be called mario. And let’s allow the user to decide just how tall the pyramids should be by first prompting them for a positive integer between, say, 1 and 8, inclusive.

Here’s how the program might work if the user inputs 8 when prompted:
```
$ ./mario
Height: 8
       #  #
      ##  ##
     ###  ###
    ####  ####
   #####  #####
  ######  ######
 #######  #######
########  ########
```

# Credit Cards

https://cs50.harvard.edu/x/2023/psets/1/credit/

Write a program that prompts the user for a credit card number and then reports (via printf) whether it is a valid American Express, MasterCard, or Visa card number, per the definitions of each’s format herein. 
