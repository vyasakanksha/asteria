#############################################################################
#                                                                           #
# This file is part of the Asteria project.                                 #
# Copyright (C) 2011 Samuel C. Payson, Akanksha Vyas                        #
#                                                                           #
# Asteria is free software: you can redistribute it and/or modify it under  #
# the terms of the GNU General Public License as published by the Free      #
# Software Foundation, either version 3 of the License, or (at your         #
# option) any later version.                                                #
#                                                                           #
# Asteria is distributed in the hope that it will be useful, but WITHOUT    #
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or     #
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License     #
# for more details.                                                         #
#                                                                           #
# You should have received a copy of the GNU General Public License along   #
# with Asteria. If not, see <http://www.gnu.org/licenses/>.                 #
#                                                                           #
#############################################################################

CC      = gcc
CFLAGS  = -g -Wall -Werror -O2 -march=native -Iinclude
LD      = gcc
LDFLAGS = -lGL -lGLU -lSDL -lGLEW -lm

MODULES = main gfxInit vMath md5Mesh.tab lex.md5Mesh md5Operations gfxShader

OBJ = $(patsubst %,obj/%.o,$(MODULES))
DEP = $(patsubst %,dep/%.M,$(MODULES))

CLEANFILES = game obj/*.o dep/*.M

game: $(OBJ)
	@ echo "  [LD]       $@"
	@ $(LD) $(LDFLAGS) $^ -o $@

obj/%.o: %.c
	@ echo "  [CC]       $@"
	@ $(CC) $(CFLAGS) -c $< -o $@

# Include automatically generated dependency files. GNU Make will first look
# for rules to build these files and update them if they are out of date.
ifeq (,$(findstring clean,$(MAKECMDGOALS)))
  -include $(DEP)
endif


# Build dependency files using gcc's '-M*' flags. The sed and awk scripts
# modify the files to fit into our directory model.
dep/%.M: %.c
	@ $(CC) -MM -MG $< | sed 's#[^ ]\+\.h#include/&#g' \
	                   | sed 's#[^ ]\+\.o#obj/&#g' \
	                   > $@



######################### Begin Lex/Yacc Rules ##############################
# We use custom rules for the lex/yacc files because generating them would  #
# result in an unfavourable readability-lost:simplicity-gained ratio.       #

obj/md5MeshTest.o: 

md5Mesh: obj/md5Mesh.tab.o obj/lex.md5Mesh.o obj/md5MeshTest.o
	@ echo "  [LD]       $@"
	@ $(LD) $(LDFLAGS) $^ -o $@

include/md5Mesh.h: md5Mesh.tab.c

md5Mesh.tab.c: md5Mesh.y
	@ echo "  [BISON]    $@"
	@ bison -p md5mesh -b md5Mesh --defines=include/md5Mesh.h -o $@ $<

lex.md5Mesh.c: md5Mesh.lex
	@ echo "  [LEX]      $@"
	@ flex -P md5mesh --yylineno -o $@ $<

CLEANFILES += lex.md5Mesh.c md5Mesh.tab.c include/md5Mesh.h md5Mesh

########################## End Lex/Yacc Rules ###############################

clean:
	@ echo "  [CLEAN]"
	@ rm -f $(CLEANFILES)
