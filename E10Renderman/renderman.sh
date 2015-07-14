#!/bin/sh
#
# This shell script registers all directories that are required for using
# RenderMan. Afterwards, all relevant RenderMan files will be compiled.
#
# created by Daniel Jungblut, IWR Heidelberg, May 2008 -- daniel.jungblut@iwr.uni-heidelberg.de
# refined by Bastian Rieck, IWR Heidelberg, May 2010 -- bastian.rieck@iwr.uni-heidelberg.de
# updated by Susanne Kroemker, IWR Heidelberg, May 2012 -- susanne.kroemker@iwr.uni-heidelberg.de
# updated by Susanne Kroemker, IWR Heidelberg, May 2014 -- susanne.kroemker@iwr.uni-heidelberg.de


PATH="${PATH}:/export/home/cg/cg001/software/3delight-11.0.57/Linux-x86_64/bin"
LDPATH="${LDPATH}:/export/home/cg/cg001/software/3delight-11.0.57/Linux-x86_64/lib"
export LD_LIBRARY_PATH=.:/export/home/cg/cg001/software/3delight-11.0.57/Linux-x86_64/lib
export DL_SHADERS_PATH=.:/export/home/cg/cg001/software/3delight-11.0.57/Linux-x86_64/shaders
export DL_DISPLAYS_PATH=.:/export/home/cg/cg001/software/3delight-11.0.57/Linux-x86_64/displays
export DELIGHT=$DELIGHT$HOME

#PATH="${PATH}:~/software/3delight-11.0.57/Linux-x86_64/bin"
#LDPATH="${LDPATH}:~/software/3delight-11.0.57/Linux-x86_64/lib"
#export LD_LIBRARY_PATH=.:~/software/3delight-11.0.57/Linux-x86_64/lib
#export DL_SHADERS_PATH=.:~/software/3delight-11.0.57/Linux-x86_64/shaders
#export DL_DISPLAYS_PATH=.:~/software/3delight-11.0.57/Linux-x86_64/displays
#export DELIGHT=$DELIGHT$HOME

# compile texture
tdlmake mars.tif marscompiled.tif
tdlmake phobos.tif phoboscompiled.tif
tdlmake deimos.jpg deimoscompiled.tif
tdlmake marsheight.tif marsheightcompiled.tif
tdlmake phobosheight.jpg phobosheightcompiled.tif
tdlmake deimosbump.jpg deimosbumpcompiled.tif
tdlmake tile.jpg tilecompiled.tif
tdlmake tilebump.jpg tilebumpcompiled.tif


# compile shader
shaderdl bump_mapping.sl

# compile geometry data
renderdl mars.rib

# display the rendered output
display output.tif
