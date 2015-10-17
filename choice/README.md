# choice
this is a simple command-line program which presents the user with a string (the text argument) and some options. it'll then return the index for the option chosen. the syntax currently is:

`choice [-c choices] [-n] [-C] text`

where:

 - -c/--choices defines the single-character options that the user has
 - -n/-nooptions does not print out the options after the text
 - text is the string that is presented to the user

-h can also be used for an overview of these arguments.

an example of it's usage could be:

    choice Do you want to continue?
    
    > Do you want to continue? [yn] y
    > (returns 1)

or

    choice Do you want to continue?

    > Do you want to continue? [yn] y
    > (returns 2)

the default options are yn, so basically yes/no. they can be changed with the -c/--choices argument, for example:
    
    choice -c abcd Enter an answer
    
    > Enter an answer [abcd] c
    > (returns 3)

# bugs
there are a couple of issues which I'll fix when I have time.

one bug is running it without specifing any text will result in it printing out what you ran, eg:

    choice -c aabcd
    
    > choice -c abcd [abcd] b
    > (returns 2)

that's all there really is to it. it's mostly designed to be used within shell scripts, but really can be used anywhere.


