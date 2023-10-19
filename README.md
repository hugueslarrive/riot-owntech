@page main Riot OwnTech repository

# Riot OwnTech repository

<!-- @cond COMMENT --><script><!-- @endcond -->

This chapter is auto-generated from the OwnTech gitlab repository's
*.md files.

As the gitlab's markdown render engine do not handle doxygen tags and
doxygen do not handle markdown, I am using markdown doxygen hacks to
comment out doxygen tags from gitlab redered md files and to comment out
markdown sections from doxygen pdf output.

<!-- @cond COMMENT --></script><!-- @endcond -->

As `@page` doxygen tag only work if it is on the first line of the
`*.md` file it can't be hidden. If you know a way to do it, be free to
share it !

     * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *       _                                             *
     *    _ | | _                _______        _          *
     *   / || || \              |__   __|      | |         *
     *  / /`|_| \ \_        ___ __ | | ___  ___| |__       *
     * ( (`  ``  )\ \  /\  / | '_ \| |' _ `/ __| '_ `      *
     *  \ \ ___ / /\ \/  \/ /| |`| | | '__( (__| |`| |     *
     *   \_______/` \__/\__/`|_| |_|_ \___,\___|_| |_|     *
     *    ````````   ``` ```  ``  ```` ```` ``````  ``     *
     * D i g i t a l i z i n g  P o w e r  T o g e t h e r *
     * ``````````````````````````````````````````````````` *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * *

This is the repository for the RIOT firmware implementation of the
OwnTech project. 


# OwnTech on Riot

OwnTech is a software defined power converter technology suit. It allows
seamless and intuitive reprogramming of power electronics converters
using a dedicated GUI. 

RIOT OS has been chosen by the OwnTech team to implement the firmware
abstraction layer of the project.


# Files organization

<!-- @cond COMMENT --><script><!--</script>

| `--riot-owntech`          |                                                   |
|:-                         |:-                                                 |
| `  +--`[boards](boards)   | Enhanced board configurations                     |
| `  +--`[doc](doc)         | OwnTech firmware documentation source code        |
| `  +--`[riot_contrib.md](riot_contrib.md) | OwnTech contributions to the Riot project |
| `  +--`[hacks](hacks)     | Experiments done during OwntTech's hackathons     |
| `  +--`[modules](modules) | OwnTech's firmware modules                        |
| `  +--`[tests](tests)     | Tests for OwnTech's firmware modules and hardware |
| `  +--`[tools](tools)     | An stm32f334 discovery board based feeder (5 to 10v boost) and a sine wave C array generator |

<script>--></script><script><!-- @endcond -->

| `riot-owntech`            |                       |
|:-                         |:-                     |
| `  +--`boards             | @subpage boards       |
| `  +--`doc                | @subpage doc          |
| `  +--`riot_contrib.md    | @subpage riot_contrib |
| `  +--`hacks              | @subpage hacks        |
| `  +--`modules            | @subpage modules      |
| `  +--`tests              | @subpage tests        |
| `  +--`tools              | @subpage tools        |

<!-- @cond COMMENT --></script><!-- @endcond -->

