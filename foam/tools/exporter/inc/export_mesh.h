#pragma once

#include <foam/core/ext/io.h>
#include <foam/core/ext/sugar.h>
#include <foam/core/ext/vector.h>
#include <foam/core/ext/logger.h>
#include <conf.h>


//Notes when using Blender, dunno how this would be with other models from other software
//but I'd assume similar, because I think it's something to do with assimp more than the file
//format that is exported.

//Meshes must be aligned in edit mode, because all locations in 
//object mode get set to 0,0,0 at some point in the pipeline. 
//Also scales all must be the same size in object mode. 

//So basically just use edit mode for everything, when resizing or moving to align up parts 
//or whatever.


void fe_export_mesh_bundle(const char *input_path, const char *output_path, const char *config_path, const char *root_ent_name);




