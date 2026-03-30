# Command Line Packet Capture

## Features

1. **Capture of Packets**
	- Display
		- Time
		- Number
		- Source
		- Target
		- Type of Packet
		- Brief Description of Packet Captured
2. **Loading and Display of PCAP files**
3. **Export of PCAP Files**
4. **Extensive Packet Inspection**
5. **Special Hotkeys**
	- Filter
	- Search
	- Follow Packet
	- Display of a packet

## Dependencies

I intend for this program to rely on a couple of libraries

1. **ncurses**
	- TUI

2. **PcapPlusPlus**
	- Used for Packet Capture and Functionality


## Ideas

- Resize application with changes to terminal/window size
- Add help menu
- 

## NOTES ON ADDITIONAL FEATURES

We are able to use "newpad()" to extend a screen if our default is too large for the terminal that our application is running on.
    - Solves the issue where we are not able to show the help page when the terminal screen is too small
    - May have to be a future addition, where this will become a potential problem in the future


Managing Screen Switching
    - The best way we are going to do this is utilizing Panels, as we will be able to make them disappear easily.
    - This will also prevent having to redraw our screens constantly, but will leave more to clean up at the end!