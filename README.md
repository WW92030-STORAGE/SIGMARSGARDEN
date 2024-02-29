# SIGMARSGARDEN
Sigmar's Garden rudimentary implementation in C++. Originally from Opus Magnum by Zachtronics.

# HOW TO PLAY???
Basically there are different types of marbles (elements) on the board, denoted by alchemical symbols. In total there are 55 elements:

- Eight pieces each of the four cardinals - Air(🜁), Earth(🜃), Fire(🜂), Water(🜄)
- Four pieces of Salt(🜔)
- Four pieces each of Mors(-) and Vitae(+) (sometimes denoted as 🜍 and its mirror image)
- Five pieces of Mercury(☿)
- Six metals, one of each type - Lead(♄), Tin(♃), Iron(♂), Copper(♀), Silver(☽), Gold(☉). Gold is always placed in the center.
br/

The objective is to clear all marbles/tiles off the board. All tiles must be cleared in paris of compatible tiles, excluding Gold which can be removed by itself. br/
Tiles can be removed if on the six spaces surrounding them are at least three contiguous empty spaces (including those lying outside the board but are not shown).br/

# COMPATIBILITY

Not all pairs of free tiles can be taken. Certain pairs are compatible and can be taken, all others cannot.

- All cardinals are compatible with identical copies of themselves.
- Salt is compatible with itself and all cardinals
- Vitae is only compatible with Mors and vice versa
- Mercury is only compatible with the other metals. However, in the order listed, a metal can only be taken if all preceding metals have been. So you must clear Lead first, then Tin, then Iron, and so on.

# IMPLEMENTATION

The RNG is rudimentary. It first tries to generate 55 connected tiles in 3-way rotational symmetry around the center. This is the layout of the puzzle. We generate the list of 27 non-Gold pairs and shuffle this list. Each iteration, we take a pair and attempt to place them in two unoccupied currently free locations. If the entire process fails, we repeat a few more times with different layouts and then simply generate without any layout as a failsafe.

Solving the puzzles is done using a simple backtracking algorithm.
