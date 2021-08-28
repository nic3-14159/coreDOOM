# coreDOOM
coreDOOM is a port of doomgeneric to run as a coreboot payload.
It links with libpayload, and draws to the linear framebuffer provided by coreboot.
WAD files are loaded off CBFS, but as usual you will need to supply your own WAD file.

This project was inspired by a message on the coreboot Discord about how the 
[barebox](https://barebox.org/) project had a port of Doom, called 
[bareDOOM](https://github.com/a3f/bareDOOM/), but coreboot did not.
This project aims to rectify that.

Limitations
-----------

- WAD files: At this time only the default WAD file paths hard coded into
doomgeneric can actually be loaded. TODO: add command line argument support
- Input: Only PS/2 keyboards (including emulated PS/2 keyboards often present on laptops)
are currently supported. TODO: add USB keyboard support
- Sound: As with doomgeneric, there is no sound support. Libpayload also doesn't really
have sound support, so this likely requires a fair amount of work.
- Saving games: This runs from the system boot flash, which for the purposes of a 
payload should generally be treated as read only. However, I may implement saving to
a file in memory to allow loading up a save in the middle of gameplay.
- Video: Assumes coreboot will present an RGB888 framebuffer with bits 24:31 as padding.
TODO: add support for other formats later
- Exiting: Exiting the game currently freezes the system. TODO: add reboot support
- Config file: Not currently supported
