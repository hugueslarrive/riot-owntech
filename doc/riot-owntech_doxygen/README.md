@page doxygen riot-owntech doxygen documentation config and log files

I use the following command to generate `doc/riot-owntech_doxygen/latex/refman.pdf`:

`doxygen doc/riot-owntech_doxygen/riot-owntech.doxyfile 2>doc/riot-owntech_doxygen/doxygen.log && cd doc/riot-owntech_doxygen/latex/ && make && cd ../../..`

