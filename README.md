
# [Pokémon Homebrew Bank​](https://gbatemp.net/threads/398718/)

PHBank is a homebrew which allows to manage a local bank for XY and ORAS Pokémon games, just like the Pokébank, but as a free offline service.
This homebrew is not intended to cheat (not even cloning). It's just a storage solution.

The source code is available under the [GPLv3 license](https://github.com/gocario/PHBank/blob/master/LICENSE) on [github](https://github.com/gocario/PHBank), and the resources are available on [github](https://github.com/gocario/PKBrew) too.

## What this Homebrew can do:
* Work on o3ds and n3ds!
* Navigate through the PC boxes of a XY/ORAS save and the Bank boxes.
* Display a more specific resume per Pokémon (like IVs/EVs).
* Select one Pokémon (with stylus or buttons) and move it to another slot/box.
* Select some Pokémon (with buttons) and move them to another box.
* Swap an entire box content to another one (pc <-> bank).
* Autocomplete the Pokédex when importing Pokémon to a gamesave.
* Prompt a dialog to save/exit/backup during the homebrew execution.
* Export/Import the savedata directly to/from the game, without the need of external tools (/main).
* Load/Save the bankdata to/from the bank, located in the SD card (/pk/bank/bank).

## How to install it:
* Just extract the archive at (to?) the root of your SD card.
* It must contains:
* The homebrew files /3ds/PHBank/[PHBankFiles].
* The cia installer in /cia/PHBank.cia
* The data files /pk/[DataFiles].

## How to import from <= [v1.1.4-beta-a]:
* Install PHBank normally, see above.
* Copy the bank file from /pkbank/bank to /pk/bank/bank.
* You can delete the /pkbank/ folder.

## Controls

### Everywhere​
* DPad/CPad: Move the cursor inbox, change box, change pc/bank.
* L/R: Change the current box.
* LZ/RZ: Switch from PC to Bank and vice versa (can be done with Pad and/or stylus too).
* Start: Open the savexit menu, for exiting or backing up.
* Select: Switch the selection mode (Single -> Quick -> Multiple).

### Touchscreen
* Move the cursor inbox.
* Change the current box.
* Switch PC/Bank.
* Drag & Drop Pokémon.
* Change the selection mode.

### In Single selection mode (Red):
* A: Select Pokémon / Move Pokémon if one is already selected.
* B: Cancel selection.
### In Quick selection mdoe (Blue):
* A: Select Pokémon / Move Pokémon if one is already selected.
* B: Cancel selection.
* Y: Swap the current PC box content with the current Bank box content.

### In Multiple selection mdoe (Blue):
* Y: Activate the box selector (TODO: transform it as a button on screen).

## Todo List
* Wonder box ;)
* Display more specific information per Pokémon (Tabs for contest/met/etc).
* Enhance more the GUI.
* When moving Pokémon with the DPad/A, switch the held Pokémon.

## Nota Bene
I only tested this homebrew with Pokémon Alpha Sapphire on a cartridge on a n3ds (9.3+).
Not cheat, no kidding.

## Credits
Thanks to [suloku](https://github.com/suloku) for all his work on the project.

Thanks to [Kaphotics](https://github.com/kwsch) & [Asia81](https://github.com/Asia81) (and [PKHex](https://github.com/kwsch/PKHeX)'s contributors) for the save functions (offset, algorithms) and the data (personal, text).

Thanks to [Slashcash](https://github.com/Slashcash) & [Stracker](https://github.com/Strackeror) (and [PCHex](https://github.com/Strackeror/PCHex)/[PCHex++](https://github.com/Slashcash/PCHex-plusplus)'s contributors) for their help.

Thanks to @Xenosaiga and [ihaveamac](https://github.com/ihaveamac) for being the cia build.

Thanks to [xerpi](https://github.com/xerpi) (and [SF2D](https://github.com/xerpi/sf2dlib)/[SFTD](https://github.com/xerpi/sftdlib)/[SFIL](https://github.com/xerpi/sfillib)'s contributores) for the graphic library.

Thanks to [profi200](https://github.com/profi200) and other people on #3dsdev who helped me.

Thanks to [smealum](https://github.com/smealum) and [yellows8](https://github.com/yellows8) for the exploits and [ctrulib](https://github.com/smealum/ctrulib).

Merci !

__Disclaimer: I'm not responsible if you lose or corrupt your save by using this homebrew. It mays contain bugs even within the normal intended use. Use it at your own risk.__

****ALWAYS MAKE A BACKUP!**** __(Hold L while launching homebrew or/and use tvds/svdt/JKSM)__
