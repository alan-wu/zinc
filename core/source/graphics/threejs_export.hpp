/***************************************************************************//**
 * FILE : threejs_export.hpp
 *
 * File containing a class for exporting threejs vertices and script for rendering.
 */
/* OpenCMISS-Zinc Library
*
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "general/mystring.h"
#include "graphics/graphics.h"
#include "graphics/graphics_library.h"
#include "graphics/render_gl.h"
#include <string>
#include "jsoncpp/json.h"

struct GT_object;

/* generic threejs_export class with many basic methods to export
 * standard surfaces
 */
class Threejs_export
{
protected:

	bool morphVerticesExported, morphColoursExported, morphNormalsExported;
	int morphVertices, morphColours, morphNormals;
	int number_of_time_steps;
	char *groupName;
	std::string outputString;
	std::string facesString;
	std::string verticesMorphString;
	std::string normalMorphString;
	std::string colorsMorphString;
	std::string type;

	void writeVertexBuffer(const char *output_variable_name,
		GLfloat *vertex_buffer, unsigned int values_per_vertex,
		unsigned int vertex_count);

	void writeMorphVertexBuffer(const char *output_variable_name,
		std::string *output, GLfloat *vertex_buffer, unsigned int values_per_vertex,
		unsigned int vertex_count, int time_step);

	void writeIntegerBuffer(const char *output_variable_name,
		int *vertex_buffer, unsigned int values_per_vertex,
		unsigned int vertex_count);

	void writeMorphIntegerBuffer(const char *output_variable_name,
		std::string *output, int *vertex_buffer, unsigned int values_per_vertex,
		unsigned int vertex_count, int time_step);

	void writeIndexBuffer(struct GT_object *object, int typeMask,
		int number_of_points, unsigned int offset);

	void writeIndexBufferWithoutIndex(int typeMask, int number_of_points, unsigned int offset);

	void writeSpecialDataBuffer(struct GT_object *object, GLfloat *vertex_buffer,
		unsigned int values_per_vertex, unsigned int vertex_count);

	/* texture coordinates export*/
	void writeUVsBuffer(GLfloat *texture_buffer, unsigned int values_per_vertex,
		unsigned int vertex_count);

	int exportVertexBuffer(struct GT_object *object, int time_step);

	void exportColourBuffer(struct GT_object *object, int time_step, int *typebitmask);

	void exportNormalBuffer(struct GT_object *object, int time_step, int *typebitmask);

	void exportUVsBuffer(struct GT_object *object, int time_step, int *typebitmask);

private:
	char *filename;
	cmzn_streaminformation_scene_io_data_type mode;
	double textureSizes[3];

public:

	Threejs_export(const char *filename_in, int number_of_time_steps_in,
		cmzn_streaminformation_scene_io_data_type mode_in,
		int morphVerticesIn, int morphColoursIn, int morphNormalsIn, double *textureSizesIn, char *groupNameIn) :
			number_of_time_steps(number_of_time_steps_in), filename(duplicate_string(filename_in)),
		mode(mode_in), morphVertices(morphVerticesIn), morphColours(morphColoursIn),
		morphNormals(morphNormalsIn)
	{
		type = "Surfaces";
		if (textureSizesIn)
		{
			textureSizes[0] = textureSizesIn[0];
		   textureSizes[1] = textureSizesIn[1];
		   textureSizes[2] = textureSizesIn[2];
		}
		else
		{
			textureSizes[0] = 0.0;
		   textureSizes[1] = 0.0;
		   textureSizes[2] = 0.0;
		}
		groupName = groupNameIn ? duplicate_string(groupNameIn) : 0;
		verticesMorphString.clear();
		colorsMorphString.clear();
		normalMorphString.clear();
		facesString.clear();
		outputString.clear();
		morphVerticesExported = false;
		morphColoursExported = false;
		morphNormalsExported = false;
	}

	virtual ~Threejs_export();

	virtual int exportGraphics(cmzn_graphics *graphics, int time_step, double current_time);

	void exportMaterial(cmzn_material_id material);

	int beginExport();

	virtual int endExport();

	char *getGroupNameNonAccessed()
	{
		return groupName;
	}

	std::string *getExportString();

	bool getMorphVerticesExported()
	{
		return morphVerticesExported;
	}

	bool getMorphColoursExported()
	{
		return morphColoursExported;
	}

	bool getMorphNormalsExported()
	{
		return morphNormalsExported;
	}

	const char *getTypeString()
	{
		return type.c_str();
	}


};

/* class for export glyph into WebGL format, only
 * surface glyphs are supported.
 */
class Threejs_export_glyph : public Threejs_export
{
private:

	void exportStaticGlyphs(struct GT_object *object);

	void exportGlyphsTransformation(struct GT_object *glyph,  int time_step);

	void exportGlyphsLabel(struct GT_object *glyph);

	void writeGlyphIndexBuffer(struct GT_object *object, int typeMask);

	Json::Value metadata, positions_json, axis1_json, axis2_json, axis3_json,
		scale_json, color_json, label_json;

	std::string glyphTransformationString;

	char *glyphGeometriesURLName;

public:

	Threejs_export_glyph(const char *filename_in, int number_of_time_steps_in,
		cmzn_streaminformation_scene_io_data_type mode_in,
		int morphVerticesIn, int morphColoursIn, int morphNormalsIn,
		double *textureSizesIn, char *groupNameIn) :
		Threejs_export(filename_in, number_of_time_steps_in,
		mode_in, morphVerticesIn, morphColoursIn, morphNormalsIn, textureSizesIn, groupNameIn)
	{
		type = "Glyph";
		glyphTransformationString.clear();
		glyphGeometriesURLName = 0;
	}

	virtual ~Threejs_export_glyph();

	virtual int exportGraphics(cmzn_graphics *graphics, int time_step, double current_time);

	/* this return json format describing colours and transformation of the glyph */
	std::string *getGlyphTransformationExportString();

	void setGlyphGeometriesURLName(char *name);

};


/* class for export glyph into WebGL format, only
 * surface glyphs are supported.
 */
class Threejs_export_line : public Threejs_export
{
private:

	void exportLines(cmzn_graphics *graphics, int time_step);

	void exportCylinders(cmzn_graphics *graphics, int time_step, double current_time);

	void writeVerticesCountBuffer();

	void writeRadiusBuffer();

	void writeRadialSegmentsBuffer();

	void exportVerticesCount(struct GT_object *object);

	void exportRadius(struct GT_object *object);

	void exportRadialSegments(struct GT_object *object);

	std::string verticesCountString, radiiCountString, radialSegmentsString;

	Json::Value metadata, positions_json, radii_json, radial_segments_json, color_json, label_json, count_json;

public:

	Threejs_export_line(const char *filename_in, int number_of_time_steps_in,
		cmzn_streaminformation_scene_io_data_type mode_in,
		int morphVerticesIn, int morphColoursIn, int morphNormalsIn,
		double *textureSizesIn, char *groupNameIn) :
		Threejs_export(filename_in, number_of_time_steps_in,
		mode_in, morphVerticesIn, morphColoursIn, morphNormalsIn, textureSizesIn, groupNameIn)
	{
		type = "Lines";
	}

	virtual int exportGraphics(cmzn_graphics *graphics, int time_step, double current_time);

	virtual int endExport();
};
