# BspDowngradeTool (CS:GO -> Alien Swarm)

This executable hackily modifies maps compiled with CS:GO's VBSP to work within Alien Swarm.
This gives code modders access to the newest VRAD and tools, complete with AO and perf fixes!

## Usage

1. Symlink your game directories in the `Counter-Strike Global Offensive` folder. This is [identical to the setup required to get Hammer++ to work in non-CS:GO games](https://steamcommunity.com/sharedfiles/filedetails/?id=2742731712).
2. Add a stage to call the executable in Hammer's `Run Map [Expert]` dialogue, either before or after before copying to your game's `maps` folder. Pass your `.bsp` as the only argument.

## Technical
This is possible as BSPs [haven't changed all that much in CS:GO](https://developer.valvesoftware.com/wiki/.bsp_(Source)#Versions).

The most drastic difference is the [v11 static prop lump](https://developer.valvesoftware.com/wiki/.bsp_(Source)#Static_props); identical to Alien Swarm's v9, save for 8 extra bytes at the end of each entry. Discarding this and changing the version number is trivial!

Some BSP lumps (namely `22` and `23`) hold different contents in CS:GO, though appear to be redundant in Alien Swarm, permitting their omission.
However, Source is a complex engine with many obscure features. It's possible this data is vital in some immediately unapparent way.
Unfortunately, CS:GO's format is closed-source and not well-documented. Time (and rigorous testing) will likely reveal further issues.

This BSP format will be well-supported by tools in the Source Engine community - it's very close to Portal 2's. In the future, Portal 2 could also be supported, though it may require running VBSP twice and splicing lumps together.