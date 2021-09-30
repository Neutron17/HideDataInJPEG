# Hide Data In JP(E)G with c

## How it works
Every JP(E)G has an End of Image [marker](https://en.wikipedia.org/wiki/JPEG#Syntax_and_structure) which is in hex `FF D9`, if we write after these bytes it won't effect the look of the image

## Example
```bash
./jpgh -f Tux.jpg -w Hello World
./jpgh -f Tux.jpg -r
```