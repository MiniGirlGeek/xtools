# bin2array
this program allows the user to find out the IP address tied to a specific hostname.

`usage: getip hosts ...`

it has no option arguments, but can take multiple hosts at once, for example:

    getip foo.bar bar.foo
    
    > 12.34.56.78
    > 42.42.42.42

# bugs
need some kind of regex checker to check whether the URL is valid, plus check whether the host itself is up. this program just gets whatever IP address it can find, and so it will return one even if the URL is a completely random sequence of characters.

